// MethodEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "ResizeDialog.h"
#include "memdc.h"
#include "MethodEditDlg.h"
#include "UserMessages.h"
#include "WinPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MethodEditDlg dialog


MethodEditDlg::MethodEditDlg(CWnd* pParent,int number, CString notation, int blankSize)
: ResizeDialog(MethodEditDlg::IDD, pParent),
_number(number),
_notation(notation)
{
	_methodEditWnd.setBlankStartSize(blankSize);
	//{{AFX_DATA_INIT(MethodEditDlg)
	_bell = _T("");
	_place = _T("");
	_row = _T("");
	_message = _T("");
	//}}AFX_DATA_INIT
}


void MethodEditDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MethodEditDlg)
	DDX_Control(pDX, IDC_METHOD_DRAW_UNDO, _undoBtn);
	DDX_Control(pDX, IDC_METHOD_DRAW_STYLE, _style);
	DDX_Control(pDX, IDC_METHOD_DRAW_CLEAR_ALL, _clearAll);
	DDX_Control(pDX, IDCANCEL, _cancleBtn);
	DDX_Control(pDX, IDOK, _okBtn);
	DDX_Control(pDX, IDC_METHOD_DRAW_REMOVE, _remove);
	DDX_Control(pDX, IDC_METHOD_DRAW_ADD, _add);
	DDX_Text(pDX, IDC_BELL_STATIC, _bell);
	DDX_Text(pDX, IDC_PLACE_STATIC, _place);
	DDX_Text(pDX, IDC_ROW_STATIC, _row);
	DDX_Text(pDX, IDC_MES, _message);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(MethodEditDlg, ResizeDialog)

BEGIN_MESSAGE_MAP(MethodEditDlg, ResizeDialog)
	//{{AFX_MSG_MAP(MethodEditDlg)
	ON_BN_CLICKED(IDC_METHOD_DRAW_ADD, OnMethodEditAdd)
	ON_BN_CLICKED(IDC_METHOD_DRAW_REMOVE, OnMethodEditRemove)
	ON_BN_CLICKED(IDC_METHOD_DRAW_STYLE, OnMethodEditStyle)
	ON_BN_CLICKED(IDC_METHOD_DRAW_CLEAR_ALL, OnMethodEditClearAll)
	ON_BN_CLICKED(IDC_METHOD_DRAW_UNDO, OnMethodEditUndo)
	ON_COMMAND(IDC_METHOD_DRAW_STYLE, OnMethodEditStyle)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_UPDATE_COMMAND_UI(IDOK, OnUpdateOk)
	ON_UPDATE_COMMAND_UI(IDC_METHOD_DRAW_REMOVE, OnUpdateRemove)
	ON_UPDATE_COMMAND_UI(IDC_METHOD_DRAW_CLEAR_ALL, OnUpdateClearAll)
	ON_UPDATE_COMMAND_UI(IDC_METHOD_DRAW_UNDO, OnUpdateUndo) 
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_MESSAGE(WM_SET_METHOD_POS,OnSetMethodPos)	
END_MESSAGE_MAP()  

LRESULT MethodEditDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}
		
/////////////////////////////////////////////////////////////////////////////
// MethodEditDlg message handlers

BOOL MethodEditDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();

	EnableToolTips();

	//get the position of the place holder static item
	CRect drawRect;
	GetDlgItem(IDC_METHOD_DRAW_WND_HOLD)->GetWindowRect(&drawRect);
	ScreenToClient(&drawRect);

	//create the scroll window, and reposition in the Z order
	_methodEditWnd.Create(NULL, "Draw Wnd", WS_TABSTOP, drawRect, 
								this, AAA_METHOD_DRAW_WND);
	_methodEditWnd.SetWindowPos(NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	AddControl(AAA_METHOD_DRAW_WND, resize, resize);
	AddControl(IDOK, repos, none);
	AddControl(IDCANCEL, repos, none);
	AddControl(IDC_PLACE_STATIC, repos, repos);	
	AddControl(IDC_ROW_STATIC, repos, repos);	
	AddControl(IDC_BELL_STATIC, repos, repos);	
	AddControl(IDC_MES, resize, repos);	


	//get the number of bells from registry

	_add.SetImage(IDB_PLUS);
	_add.m_bDrawFocus = FALSE;
	_add.SetTooltip("Add Row");
	_add.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_remove.SetImage(IDB_MINUS);
	_remove.m_bDrawFocus = FALSE;
	_remove.SetTooltip("Remove Row");
	_remove.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_undoBtn.SetImage(IDB_UNDO);
	_undoBtn.m_bDrawFocus = FALSE;
	_undoBtn.SetTooltip("Undo");
	_undoBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_style.SetImage(IDB_STYLE);
	_style.m_bDrawFocus = FALSE;
	_style.SetTooltip("Edit Style");
	_style.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_clearAll.SetImage(IDB_DELETE);
	_clearAll.m_bDrawFocus = FALSE;
	_clearAll.SetTooltip("Delete All Lines");
	_clearAll.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_okBtn.m_bDrawFocus = FALSE;
	_okBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_cancleBtn.m_bDrawFocus = FALSE;
	_cancleBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	WinPos::Load(this, "MethodEditDlg", true);

	_methodEditWnd.createMethod(_number, _notation);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void MethodEditDlg::OnMethodEditAdd() 
{
	_methodEditWnd.addRow();
	_methodEditWnd.updateDimentions();	 
}

void MethodEditDlg::OnMethodEditRemove() 
{
	_methodEditWnd.removeRow();
}

void MethodEditDlg::OnUpdateOk(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_methodEditWnd.isValid());
}

void MethodEditDlg::OnUpdateClearAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_methodEditWnd.getLineCount() > 0);
}

void MethodEditDlg::OnUpdateUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_methodEditWnd._undo.GetSize() > 0);
}

void MethodEditDlg::OnUpdateRemove(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_methodEditWnd.getHeight() > 2);
}

BOOL MethodEditDlg::DestroyWindow() 
{
	WinPos::Save(this, "MethodEditDlg");

	return ResizeDialog::DestroyWindow();
}

void MethodEditDlg::OnMethodEditStyle() 
{
	_methodEditWnd.OnMethodDrawStyle();
}

void MethodEditDlg::OnMethodEditClearAll() 
{
	_methodEditWnd.OnMethodDrawClearAll();
}

void MethodEditDlg::OnMethodEditUndo() 
{
	_methodEditWnd.OnMethodDrawUndo();
}

void MethodEditDlg::OnOK() 
{
	_notation = _methodEditWnd.calcNotation();
	ResizeDialog::OnOK();
}

LRESULT MethodEditDlg::OnSetMessageString(WPARAM wParam, LPARAM /*lParam*/)
{	
	
	UINT    nIDMsg = (UINT)wParam;
	CString strMsg;

	if (nIDMsg)
	{
		if (strMsg.LoadString(nIDMsg) != 0)
		{
			_message = strMsg;
			UpdateData(FALSE);
		}
		else
			TRACE1("Warning: no message line prompt for ID %x%04X\n", nIDMsg);
	}
	return 0;
}
   
LRESULT MethodEditDlg::OnSetMethodPos(WPARAM wParam, LPARAM /*lParam*/)
{
	MethodEditPos pos = _methodEditWnd.getMethodEditPos();
	if (wParam == TRUE)
	{
		_place.Format("Place %d", pos._place);	   
		_row.Format("Row %d", (pos._row -globalDataManager()->isStartAtRowZero()?1:0));
		if (pos._bell == -1)
			_bell.Empty();
		else
			_bell.Format("Bell %d", pos._bell);
	}
	else 
	{
		_place.Empty();
		_row.Empty();
		_bell.Empty();
	}

	UpdateData(FALSE);

	return 0;
}


