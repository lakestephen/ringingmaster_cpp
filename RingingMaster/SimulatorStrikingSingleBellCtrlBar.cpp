// SimulatorStrikingSingleBellCtrlBar.cpp: implementation of the SimulatorStrikingSingleBellCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorStrikingSingleBellCtrlBar.h"
#include "SimulatorComboBtn.h"
#include "SimulatorManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SimulatorStrikingSingleBellCtrlBar::SimulatorStrikingSingleBellCtrlBar(SimulatorStrikingWndType type) :
_strikingWnd(type)
{

}

SimulatorStrikingSingleBellCtrlBar::~SimulatorStrikingSingleBellCtrlBar()
{

}
   
IMPLEMENT_DYNAMIC(SimulatorStrikingSingleBellCtrlBar, CBCGPDockingControlBar)

BEGIN_MESSAGE_MAP(SimulatorStrikingSingleBellCtrlBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(SimulatorStrikingSingleBellCtrlBar)
	ON_WM_CREATE()
	ON_WM_SIZE() 
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP	  
	ON_UPDATE_COMMAND_UI(ID_SIMULATOR_SET_BELL1, OnUpdateSetBell)
	ON_CBN_SELCHANGE(ID_SIMULATOR_SET_BELL1, OnSetBell1)
	ON_UPDATE_COMMAND_UI(ID_SIMULATOR_SET_BELL2, OnUpdateSetBell)
	ON_CBN_SELCHANGE(ID_SIMULATOR_SET_BELL2, OnSetBell2)
					

END_MESSAGE_MAP()



int SimulatorStrikingSingleBellCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty ();    


	_toolBar.Create (this, dwDefaultToolbarStyle, IDR_PROP_DEFAULT);
	_toolBar.LoadToolBar (IDR_SIMULATOR_GENERAL, 0, 0, TRUE /* Is locked */);
	_toolBar.SetBarStyle(_toolBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY);
	_toolBar.SetBarStyle (_toolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	_toolBar.SetOwner (this);
	_toolBar.SetRouteCommandsViaFrame (FALSE);	

	_strikingWnd.Create(this, ID_SIMULATOR_STRIKING_SINGLE_BELL1);

	AdjustLayout ();
	return 0;
}

void SimulatorStrikingSingleBellCtrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void SimulatorStrikingSingleBellCtrlBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}       

	CRect rectClient;
	GetClientRect (rectClient);
	
	int cyTlb = _toolBar.CalcFixedLayout (FALSE, TRUE).cy;

	_toolBar.SetWindowPos (NULL,
						rectClient.left, 
						rectClient.top , 
						rectClient.Width (),
						cyTlb,
						SWP_NOACTIVATE | SWP_NOZORDER);

	_strikingWnd.SetWindowPos (NULL,
									rectClient.left + 1, 
									rectClient.top + cyTlb + 1, 
									rectClient.Width () - 2,
									rectClient.Height () - (cyTlb) - 2,
									SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL SimulatorStrikingSingleBellCtrlBar::DestroyWindow() 
{

	return CBCGPDockingControlBar::DestroyWindow();

}

BOOL SimulatorStrikingSingleBellCtrlBar::OnEraseBkgnd(CDC* /*pDC*/) 
{			  	
	return TRUE;
}


void SimulatorStrikingSingleBellCtrlBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		   
	if (::IsWindow(_strikingWnd.GetSafeHwnd()))
	{
		CRect rectList;
		_strikingWnd.GetWindowRect (rectList);
		ScreenToClient (rectList);

		rectList.InflateRect (1, 1);
		dc.Draw3dRect (rectList, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
	}  
}    

void SimulatorStrikingSingleBellCtrlBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	
	_strikingWnd.SetFocus ();	
}

void SimulatorStrikingSingleBellCtrlBar::toolbarReset() 
{
	if (_strikingWnd.getType() == sswt_single_1)
		_toolBar.ReplaceButton (ID_DUMMY_SIMULATOR_SET_BELL, SimulatorComboBtn1());
	else if (_strikingWnd.getType() == sswt_single_2)
		_toolBar.ReplaceButton (ID_DUMMY_SIMULATOR_SET_BELL, SimulatorComboBtn2());
	else
		ASSERT(FALSE);
}

void SimulatorStrikingSingleBellCtrlBar::OnUpdateSetBell(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void SimulatorStrikingSingleBellCtrlBar::OnSetBell1() 
{	
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	CString towerName("");

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_SIMULATOR_SET_BELL1, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); 
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo = 
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL)
			{
				if (simulatorManager()->setSimulatorStrikingSngleBell(sswt_single_1, pCombo->GetCurSel()))
					break;
			}
		}
	}	
}  

void SimulatorStrikingSingleBellCtrlBar::OnSetBell2() 
{	
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	CString towerName("");

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_SIMULATOR_SET_BELL2, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); 
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo = 
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL)
			{
				if (simulatorManager()->setSimulatorStrikingSngleBell(sswt_single_2, pCombo->GetCurSel()))
					break;
			}
		}
	}	
}  
