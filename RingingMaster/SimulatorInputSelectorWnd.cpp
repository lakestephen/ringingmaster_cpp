// SimulatorInputSelectorWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorInputSelectorWnd.h"
#include "simulatorinputselectorwnd.h"
#include "MultiBellInterfaceManager.h"
#include "SerialControlManager.h"
#include "SpeechManager.h"
#include "KeyboardManager.h"
#include "SimulatorManager.h"
#include "SimulatorInput.h"  
#include "MemDC.h"
#include "UserMessages.h"

// SimulatorInputSelectorWnd

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(SimulatorInputSelectorWnd, CWnd)
SimulatorInputSelectorWnd::SimulatorInputSelectorWnd() : 
_input(NULL), 
_showAddBtn(false), 
_id(0), 
_textColor(RGB(0,0,0)), 
_drawLine(false)
{
}

SimulatorInputSelectorWnd::~SimulatorInputSelectorWnd()
{
}


BEGIN_MESSAGE_MAP(SimulatorInputSelectorWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(ID_SIMULATOR_IP_SELECT_COMBO, OnSelchangeCmbBox)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_SIMULATOR_OPTION_BTN, OnPressOptionBtn)	
	ON_BN_CLICKED(ID_SIMULATOR_ADD_BTN, OnPressAddBtn)	
	ON_WM_TIMER()
END_MESSAGE_MAP()




BOOL SimulatorInputSelectorWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	//no erasing is necessary as the paint function is using memDC, and is 
	//painting the whole background
	return TRUE; 
}


const int width  = 370;
const int height = 21;

BOOL SimulatorInputSelectorWnd::Create(const CPoint topLeft, CWnd* pParentWnd, UINT nID)
{
	DWORD dwExStyle = NULL;

	CRect rect(topLeft, CSize(width,height));

	BOOL ret = CWnd::CreateEx(dwExStyle, AfxRegisterWndClass(0, 0, 0, 0), 
		"SimulatorInputSelectorWnd", WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, rect, pParentWnd, nID, NULL);

	//add buttton
	_addButton.Create("",BS_PUSHBUTTON | WS_CHILD,
					CRect (75, 0,95, 20),this,ID_SIMULATOR_ADD_BTN);
	_addButton.m_bDrawFocus = FALSE;
	_addButton.m_nAlignStyle = CBCGPButton::ALIGN_LEFT;
	_addButton.SetImage(IDB_PLUS);
	_addButton.SetTooltip("Add an additional input");

	//combo	
	_combo.Create(WS_CHILD|WS_VSCROLL|WS_TABSTOP|WS_VISIBLE|WS_VSCROLL|CBS_HASSTRINGS|CBS_DROPDOWNLIST,
				  CRect(100,0,205,120), 
				  this, 
				  ID_SIMULATOR_IP_SELECT_COMBO);

	_combo.SetFont(&app()->_fontStd);

	_combo.SetItemData(_combo.AddString("No Input"), sit_none);

	_combo.SetItemData(_combo.AddString("Keyboard"), sit_keyboard);

	_combo.SetItemData(_combo.AddString("Multi Bell Interface"), sit_multiBellInterface);

	_combo.SetItemData(_combo.AddString("Microphone"), sit_speechRecognition);

	_combo.SetItemData(_combo.AddString("Serial Port Control"), sit_serial);

	//button
	_optionButton.Create("",BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
					CRect (210, 0,width, 20),this,ID_SIMULATOR_OPTION_BTN);
	_optionButton.m_bDrawFocus = FALSE;
	_optionButton.m_nAlignStyle = CBCGPButton::ALIGN_LEFT;

	update();
	
	
	return ret;
}


void SimulatorInputSelectorWnd::OnSelchangeCmbBox()
{
	if ((IsWindow(_combo.m_hWnd))&&
		(_combo.GetCurSel() != CB_ERR)&&
		(_input != NULL))
	{
		_input->setSimulatorInputType(static_cast<SimulatorInputType>(_combo.GetItemData(_combo.GetCurSel())));
		//if nothing already selected, then edit
		if (!_input->isValid())
			_input->edit(this);
	}

	::PostMessage(GetParent()->GetSafeHwnd(), ID_SIMULATOR_UPDATE_INPUT, _id, 0);

	update();
}

void SimulatorInputSelectorWnd::OnPressAddBtn() 
{
	::PostMessage(GetParent()->GetSafeHwnd(), ID_SIMULATOR_ADD_BTN, _id, 0);
}

void SimulatorInputSelectorWnd::OnPressOptionBtn() 
{
	if (_input)
	{
		_input->edit(this);

		::PostMessage(GetParent()->GetSafeHwnd(), ID_SIMULATOR_UPDATE_INPUT, _id, 0);

		update();
	}
}

void SimulatorInputSelectorWnd::update(void)
{
	if (::IsWindow(_combo.m_hWnd))
	{
		//add Btn
		_addButton.ShowWindow((_showAddBtn)?SW_SHOW:SW_HIDE);
		_optionButton.SetFaceColor((COLORREF)-1);

		//do we have an action, or any inputs?
		if ((!_input) ||
			(!simulatorManager()->anyInputsAvailable())||
			(!simulatorManager()->isSimulatorAvailable()))
		{
			_combo.EnableWindow(FALSE);
			_combo.SetCurSel(-1);

			_optionButton.EnableWindow(FALSE);
			_optionButton.SetWindowText("");
			_optionButton.SetImage((UINT)0);
			_optionButton.SetTooltip("");
			_optionButton.Invalidate();

		}
		else //we have an action
		{
			//combo item
			_combo.EnableWindow();												 
			for (int i=0;i<_combo.GetCount();i++)
			{
				if (_input->getSimulatorInputType() == 
					static_cast<SimulatorInputType>(_combo.GetItemData(i)))
				{
					_combo.SetCurSel(i);
				}
			}       


			//button
			_optionButton.EnableWindow(_input->isInput());
			_optionButton.SetWindowText(_input->getDescription());
			_optionButton.SetTextColor(_input->isValid()?RGB(0,0,0):RGB(255,0,0));

			switch (_input->getSimulatorInputType())
			{
			case sit_none:
				_optionButton.SetImage((UINT)0);
				_optionButton.SetTooltip("");
				break;
			case sit_keyboard:
				_optionButton.SetImage(IDB_KEYBOARD);
				_optionButton.SetTooltip("Set the key press (" + _input->getDescription() + ")");
				break;
			case sit_speechRecognition:
				_optionButton.SetImage(IDB_MIKE);
				_optionButton.SetTooltip("Set the microphone word (" + _input->getDescription() + ")");
				break;
			case sit_multiBellInterface:
				_optionButton.SetImage(IDB_MBI);
				_optionButton.SetTooltip("Set the multi bell interface channel (" + _input->getDescription() + ")");
				break;
			case sit_serial:
				_optionButton.SetImage(IDB_PORT);
				_optionButton.SetTooltip("Set the serial control details (" + _input->getDescription() + ")");
				break;
			default:
				_optionButton.EnableWindow(FALSE);
				ASSERT(FALSE);
			}
			_optionButton.Invalidate();

		}
	}
	Invalidate();
}

void SimulatorInputSelectorWnd::setSimulatorInput(int id, SimulatorInput* input, CString text, COLORREF textColor, bool showAddBtn, bool drawLine)
{
	_input = input;
	_text = text;
	_showAddBtn = showAddBtn;        
	_id = id;
	_textColor = textColor;
	_drawLine = drawLine;

	update();
}

void SimulatorInputSelectorWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	MemDC memdc(&dc);
	memdc.FillSolidRect (CRect(0,0,width,height), ::GetSysColor (COLOR_BTNFACE));
	memdc.SetBkMode (TRANSPARENT);

	CFont* oldFont = memdc.SelectObject(&app()->_fontStd);
	COLORREF oldColor = memdc.SetTextColor(_textColor);
	CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen* oldPen = memdc.SelectObject(&pen);

	memdc.DrawText(_text, CRect(0,0,75,20),DT_LEFT|DT_SINGLELINE|DT_VCENTER );
	if (_drawLine)
	{
		memdc->MoveTo(0,height-1);
		memdc->LineTo(width,height-1);
	}

	memdc.SelectObject(oldPen);
	memdc.SetTextColor(oldColor);
	memdc.SelectObject(oldFont);
}


void SimulatorInputSelectorWnd::setActiveIcon()
{
	//set the background of the button to red.
	_optionButton.SetFaceColor(RGB(255,0,0));
	SetTimer(SIMULATOR_INPUT_SEL_EVENT_TIMER, 750, NULL);
}

void SimulatorInputSelectorWnd::OnTimer(UINT nIDEvent)
{
	KillTimer(SIMULATOR_INPUT_SEL_EVENT_TIMER);
	//set the background back to clear
	_optionButton.SetFaceColor((COLORREF)-1);
	CWnd::OnTimer(nIDEvent);
}
