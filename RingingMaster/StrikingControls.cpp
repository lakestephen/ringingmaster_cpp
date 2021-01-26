// StrikingControls.cpp: implementation of the StrikingControls class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "StrikingControls.h"
#include "MemDC.h"
#include "PlayManager.h"
#include "TowerManager.h"
#include "BCGPNoSizeMiniFrameWnd.h"
#include "Tower.h"
#include "SimulatorManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StrikingControls::StrikingControls() :
_tower(NULL), 
_bell(0), 
_updatingCtrls(false)
{  

}

StrikingControls::~StrikingControls()
{

}


IMPLEMENT_DYNAMIC(StrikingControls, CBCGPDockingControlBar)

BEGIN_MESSAGE_MAP(StrikingControls, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(StrikingControls)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_APPLY_ALL, OnBnClickedApplyAll)
	ON_BN_CLICKED(IDC_REMOVE_ALL, OnBnClickedRemoveAll)
	ON_BN_CLICKED(IDC_RESET, OnBnClickedReset)
	ON_BN_CLICKED(IDC_RESET_ALL, OnBnClickedResetAll)
	ON_BN_CLICKED(IDC_QUIET, OnBnClickedQuiet)
	ON_BN_CLICKED(IDC_PREV_ERROR, OnBnClickedPrevError)
	ON_BN_CLICKED(IDC_USE_STRIKING_ERRORS, OnBnClickedUseStrikingErrors)
	ON_BN_CLICKED(IDC_LOCK_HAND_BACK, OnBnClickedLockHandBack)

	ON_UPDATE_COMMAND_UI(IDC_BELL, OnUpdateOn)
	ON_UPDATE_COMMAND_UI(IDC_APPLY_ALL, OnUpdateOn)
	ON_UPDATE_COMMAND_UI(IDC_REMOVE_ALL, OnUpdateOn)
	ON_UPDATE_COMMAND_UI(IDC_USE_STRIKING_ERRORS, OnUpdateOn)
	ON_UPDATE_COMMAND_UI(IDC_RESET, OnUpdateOn)
	ON_UPDATE_COMMAND_UI(IDC_RESET_ALL, OnUpdateOn)
	ON_UPDATE_COMMAND_UI(IDC_QUIET, OnUpdateOn)
	ON_UPDATE_COMMAND_UI(IDC_PREV_ERROR, OnUpdateOn)
	ON_UPDATE_COMMAND_UI(IDC_LOCK_HAND_BACK, OnUpdateOn)
	
	ON_CBN_SELCHANGE(IDC_BELL, OnCbnSelchangeBell)
	ON_REGISTERED_MESSAGE(NEN_CHANGED,OnNumericEditChange)
	
END_MESSAGE_MAP()



void StrikingControls::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	MemDC memdc(&dc);

	CRect rectClient;
	GetClientRect (rectClient);

	memdc.FillSolidRect (rectClient, ::GetSysColor (COLOR_BTNFACE));

	CFont* pOldFont = (CFont*) memdc.SelectObject(&app()->_fontStd);

	memdc.SetBkMode (TRANSPARENT);

	memdc.TextOut (10, 10, "Lead:");

	memdc.SelectObject (pOldFont);	 
}


int StrikingControls::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	SetMiniFrameRTC(RUNTIME_CLASS(StrikingControlsNoSizeMiniFrameWnd));

	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	_bells.Create(WS_CHILD|WS_VSCROLL|WS_TABSTOP|WS_VISIBLE|WS_VSCROLL|CBS_HASSTRINGS|CBS_DROPDOWNLIST, 
		CRect(5,5,65,200), this, IDC_BELL); 
	_bells.SetFont(&app()->_fontStd);

	_useStrikingControls.Create("Use Striking Errors", BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
		CRect (75,8,180,23),this,IDC_USE_STRIKING_ERRORS);
	_useStrikingControls.SetFont(&app()->_fontStd);

	_applyAll.Create("", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(185, 5, 207, 27), this, IDC_APPLY_ALL);
	_applyAll.m_bDrawFocus = FALSE;
	_applyAll.SetTooltip("Turn on striking errors for all bells.");
	_applyAll.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_applyAll.SetImage(IDB_TICK);

	_removeAll.Create("", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(213, 5, 235, 27), this, IDC_REMOVE_ALL);
	_removeAll.m_bDrawFocus = FALSE;
	_removeAll.SetTooltip("Turn off striking errors for all bells.");
	_removeAll.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_removeAll.SetImage(IDB_DELETE);

	_quiet.Create("Mute", BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
		CRect (5,33,50,48),this,IDC_QUIET);
	_quiet.SetFont(&app()->_fontStd);

	_followPreviousBellsError.Create("Follow Previous Bells Error", BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
		CRect (75,33,225,48),this,IDC_PREV_ERROR);
	_followPreviousBellsError.SetFont(&app()->_fontStd);

	_reset.Create("", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(255, 5, 277, 27), this, IDC_RESET);
	_reset.m_bDrawFocus = FALSE;
	_reset.SetTooltip("Reset striking controls for this bell.");
	_reset.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_reset.SetImage(IDB_RESET);

	_resetAll.Create("", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(255, 30, 277, 52), this, IDC_RESET_ALL);
	_resetAll.m_bDrawFocus = FALSE;
	_resetAll.SetTooltip("Reset striking controls for all bells.");
	_resetAll.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_resetAll.SetImage(IDB_RESETALL);	 


	_lockHandAndBack.Create("Lock Hand and Back", BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
		CRect (5,52,235,67),this,IDC_LOCK_HAND_BACK);
	_lockHandAndBack.SetFont(&app()->_fontStd);
	

	//hand
	_handstroke.Create("Handstroke:", WS_VISIBLE,
		CRect(5,70, 150,90), this);
	_handstroke.SetFont(&app()->_fontStd);

	_handLateness.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,
		CRect(15, 85, 85, 105),this,IDC_HAND_LATENESS);
	_handLateness.SetRange(-100, 100);//0% through 100% 

	_handearlylate.Create("% Early/Late", WS_VISIBLE,
		CRect(90,87, 160,105), this);
	_handearlylate.SetFont(&app()->_fontStd);

	_handQuality.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,
		CRect(160, 85, 230, 105),this,IDC_HAND_QUALITY);
	_handQuality.SetRange(0, 100);//0% through 100% 

	_handerratic.Create("% Erratic", WS_VISIBLE,
		CRect(235,87, 280,105), this);
	_handerratic.SetFont(&app()->_fontStd);

	//back
	_backstroke.Create("Backstroke:", WS_VISIBLE,
		CRect(5,110, 150,130), this);
	_backstroke.SetFont(&app()->_fontStd);

	_backLateness.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,
		CRect(15, 125, 85, 145),this,IDC_BACK_LATENESS);
	_backLateness.SetRange(-100, 100);//0% through 100% 

	_backearlylate.Create("% Early/Late", WS_VISIBLE,
		CRect(90,127, 160,145), this);
	_backearlylate.SetFont(&app()->_fontStd);

	_backQuality.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,
		CRect(160, 125, 230, 145),this,IDC_BACK_QUALITY);
	_backQuality.SetRange(0, 100);//0% through 100% 

	_backerratic.Create("% Erratic", WS_VISIBLE,
		CRect(235,127, 280,145), this);
	_backerratic.SetFont(&app()->_fontStd);
	

	update();

	towerManager()->addEventListener(this);
	simulatorManager()->addEventListener(this);

	return 0;
}

void StrikingControls::OnDestroy() 
{
	CBCGPDockingControlBar::OnDestroy();
	
	simulatorManager()->removeEventListener(this);
	towerManager()->removeEventListener(this);	
}

	

BOOL StrikingControls::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
	
	//return CBCGPDockingControlBar::OnEraseBkgnd(pDC);
}


void StrikingControls::setTower(Tower* tower, int bell)
{

	_tower = tower;

	if (bell != -1)
		_bell = bell;	

	ASSERT(_bell >=0 && _bell < MAXBELLS);

	if ((_bell < 0) || (_bell >=MAXBELLS))
		_bell = 0;

	update();
}

void StrikingControls::OnBnClickedApplyAll()
{
	if (_tower)
	{
		_tower->setAllStrikingOn(TRUE);
		applyTowerChange();
	}
}

void StrikingControls::OnBnClickedRemoveAll()
{
	if (_tower)
	{
		_tower->setAllStrikingOn(FALSE);
		applyTowerChange();
	}
}

void StrikingControls::OnBnClickedReset()
{
	if (_tower)
	{
		StrikingErrors&  strikingErrors = _tower->getStrikingError(_bell);
		strikingErrors.reset();
		applyTowerChange();
		update();
	}
}

void StrikingControls::OnBnClickedResetAll()
{
	if (_tower)
	{
		_tower->resetAllStriking();	 
		applyTowerChange();
	}
}

void StrikingControls::OnBnClickedQuiet()
{
	if (_tower)
	{
		StrikingErrors&  strikingErrors = _tower->getStrikingError(_bell);
		strikingErrors.setQuiet(!strikingErrors.getQuiet());
		applyTowerChange();
	}
}

void StrikingControls::OnBnClickedPrevError()
{
	if (_tower)
	{
		StrikingErrors&  strikingErrors = _tower->getStrikingError(_bell);
		strikingErrors.setFollowPreviousBellError(!strikingErrors.getFollowPreviousBellError());
		applyTowerChange();
	}
}

void StrikingControls::OnBnClickedUseStrikingErrors()
{
	if (_tower)
	{
		StrikingErrors&  strikingErrors = _tower->getStrikingError(_bell);
		strikingErrors.setUseStrikingErrors(!strikingErrors.getUseStrikingErrors());
		applyTowerChange();
	}
}

void StrikingControls::OnBnClickedLockHandBack()
{
	if (_tower)
	{
		StrikingErrors&  strikingErrors = _tower->getStrikingError(_bell);
		strikingErrors.setLockHandAndBack(!strikingErrors.getLockHandAndBack());
		applyTowerChange();
	}
}
		 
void StrikingControls::OnCbnSelchangeBell()
{
	if (_tower)
	{
		int index = _bells.GetCurSel();
		ASSERT(index != LB_ERR);
		if (index != LB_ERR)
			_bell = index;
        update();
	} 
}

void StrikingControls::OnUpdateOn(CCmdUI* pCmdUI) 
{			
	pCmdUI->Enable(_tower != NULL);

	if (_tower)
	{
		StrikingErrors& error = _tower->getStrikingError(_bell);
		switch(pCmdUI->m_nID)
		{
		case IDC_USE_STRIKING_ERRORS:
			pCmdUI->SetCheck(error.getUseStrikingErrors());
			break;
		case IDC_PREV_ERROR:
			pCmdUI->SetCheck(error.getFollowPreviousBellError());
			break;
		case IDC_QUIET:
			pCmdUI->SetCheck(error.getQuiet());
			break;    	
		case IDC_LOCK_HAND_BACK:
			pCmdUI->SetCheck(error.getLockHandAndBack());
			break;    	
			
		}
	}
}  		

void StrikingControls::applyTowerChange() 
{
	towerManager()->fireUpdateTowerList();
}

LRESULT StrikingControls::OnNumericEditChange(WPARAM wParam,LPARAM /*lParam*/)
{
	NumericEdit* alteredCtrl = (NumericEdit*)wParam;

	if ((_tower) && (!_updatingCtrls))
	{
		StrikingErrors& error = _tower->getStrikingError(_bell);
		
		if (alteredCtrl== &_handQuality)
			error.setHandstrokeQuality(_handQuality.GetValue());
		
		if (alteredCtrl== &_handLateness)
			error.setHandstrokeLateness(_handLateness.GetValue());
		
		if (alteredCtrl== &_backQuality)
			error.setBackstrokeQuality(_backQuality.GetValue());
		
		if (alteredCtrl== &_backLateness)
			error.setBackstrokeLateness(_backLateness.GetValue());
		applyTowerChange();
	}

	return 0;
}

void StrikingControls::update() 
{
	//all the check boxes are updated with StrikingControls::OnUpdateOn

	_bells.ResetContent();

	int selectedBell = 0;
	for (int i=0;i<simulatorManager()->getNumber();i++)
	{
		int index = _bells.AddString(CString(GlobalFunctions::bellNumbersToChar(i+1)));
		_bells.SetItemData(index, i);
		if (i==0 || i==_bell)
		{
			_bells.SetCurSel(i);
			selectedBell = i;
		}
	}

	_bell = selectedBell;


	if (_tower)
	{
		StrikingErrors& error = _tower->getStrikingError(_bell);
		
		_updatingCtrls = true;

		_handQuality.EnableWindow();
		_handLateness.EnableWindow();
		_backQuality.EnableWindow();
		_backLateness.EnableWindow();

		_handQuality.SetValue((float)error.getHandstrokeQuality());
		_handLateness.SetValue((float)error.getHandstrokeLateness());
		_backQuality.SetValue((float)error.getBackstrokeQuality());
		_backLateness.SetValue((float)error.getBackstrokeLateness());
		
		_updatingCtrls = false;
	}
	else
	{
		_handQuality.EnableWindow(FALSE);
		_handLateness.EnableWindow(FALSE);
		_backQuality.EnableWindow(FALSE);
		_backLateness.EnableWindow(FALSE);
	}
}

void StrikingControls::towerManager_notifyUpdateTowerList()
{
	Tower* tower = towerManager()->getTower();

	if (_tower != tower)
		_tower = tower;

	if (_tower)
		update();
}


void StrikingControls::simulatorManager_notifyDocumentChanged()
{
	update();
}