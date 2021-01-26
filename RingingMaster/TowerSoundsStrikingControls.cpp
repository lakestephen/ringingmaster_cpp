// TowerSoundsStrikingControls.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsStrikingControls.h"
#include "Tower.h"
#include ".\TowerSoundsStrikingControls.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// TowerSoundsStrikingControls dialog

IMPLEMENT_DYNAMIC(TowerSoundsStrikingControls, CBCGPPropertyPage)
TowerSoundsStrikingControls::TowerSoundsStrikingControls(Tower& internalTower)
	: CBCGPPropertyPage(TowerSoundsStrikingControls::IDD),
	_internalTower(internalTower), 
	_bell(0)
{
}

TowerSoundsStrikingControls::~TowerSoundsStrikingControls()
{
}

void TowerSoundsStrikingControls::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TowerSoundsStrikingControls, CBCGPPropertyPage)
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


// TowerSoundsStrikingControls message handlers


BOOL TowerSoundsStrikingControls::OnInitDialog()
{
	CBCGPPropertyPage::OnInitDialog();

	_bells.Create(WS_CHILD|WS_VSCROLL|WS_TABSTOP|WS_VISIBLE|WS_VSCROLL|CBS_HASSTRINGS|CBS_DROPDOWNLIST, 
		CRect(25,25,85,220), this, IDC_BELL); 
	_bells.SetFont(&app()->_fontStd);

	_useStrikingControls.Create("Use Striking Errors", BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
		CRect (95,28,200,43),this,IDC_USE_STRIKING_ERRORS);
	_useStrikingControls.SetFont(&app()->_fontStd);

	_applyAll.Create("", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(205, 25, 227, 47), this, IDC_APPLY_ALL);
	_applyAll.m_bDrawFocus = FALSE;
	_applyAll.SetTooltip("Turn on striking errors for all bells.");
	_applyAll.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_applyAll.SetImage(IDB_TICK);

	_removeAll.Create("", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(233, 25, 255, 47), this, IDC_REMOVE_ALL);
	_removeAll.m_bDrawFocus = FALSE;
	_removeAll.SetTooltip("Turn off striking errors for all bells.");
	_removeAll.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_removeAll.SetImage(IDB_DELETE);

	_quiet.Create("Mute", BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
		CRect (25,53,70,68),this,IDC_QUIET);
	_quiet.SetFont(&app()->_fontStd);

	_followPreviousBellsError.Create("Follow Previous Bells Error", BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
		CRect (95,53,245,68),this,IDC_PREV_ERROR);
	_followPreviousBellsError.SetFont(&app()->_fontStd);

	_reset.Create("Reset", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(275, 25, 357, 47), this, IDC_RESET);
	_reset.m_bDrawFocus = FALSE;
	_reset.SetTooltip("Reset striking controls for this bell.");
	_reset.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_reset.SetImage(IDB_RESET);

	_resetAll.Create("Reset All", WS_CHILD|WS_TABSTOP|WS_VISIBLE,
		CRect(275, 50, 357, 72), this, IDC_RESET_ALL);
	_resetAll.m_bDrawFocus = FALSE;
	_resetAll.SetTooltip("Reset striking controld for all bells.");
	_resetAll.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_resetAll.SetImage(IDB_RESETALL);	 


	_lockHandAndBack.Create("Lock Hand and Back", BS_CHECKBOX | WS_VISIBLE | WS_CHILD,
		CRect (25,72,255,87),this,IDC_LOCK_HAND_BACK);
	_lockHandAndBack.SetFont(&app()->_fontStd);
	

	//hand
	_handstroke.Create("Handstroke:", WS_VISIBLE,
		CRect(25,90, 170,220), this);
	_handstroke.SetFont(&app()->_fontStd);

	_handLateness.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,
		CRect(35, 105, 105, 125),this,IDC_HAND_LATENESS);
	_handLateness.SetRange(-100, 100);//0% through 100% 

	_handearlylate.Create("% Early/Late", WS_VISIBLE,
		CRect(110,107, 180,125), this);
	_handearlylate.SetFont(&app()->_fontStd);

	_handQuality.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,
		CRect(180, 105, 250, 125),this,IDC_HAND_QUALITY);
	_handQuality.SetRange(0, 100);//0% through 100% 

	_handerratic.Create("% Erratic", WS_VISIBLE,
		CRect(255,107, 300,125), this);
	_handerratic.SetFont(&app()->_fontStd);

	//back
	_backstroke.Create("Backstroke:", WS_VISIBLE,
		CRect(25,130, 170,150), this);
	_backstroke.SetFont(&app()->_fontStd);

	_backLateness.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,
		CRect(35, 145, 105, 165),this,IDC_BACK_LATENESS);
	_backLateness.SetRange(-100, 100);//0% through 100% 

	_backearlylate.Create("% Early/Late", WS_VISIBLE,
		CRect(110,147, 180,165), this);
	_backearlylate.SetFont(&app()->_fontStd);

	_backQuality.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,
		CRect(180, 145, 250, 165),this,IDC_BACK_QUALITY);
	_backQuality.SetRange(0, 100);//0% through 100% 

	_backerratic.Create("% Erratic", WS_VISIBLE,
		CRect(255,147, 300,165), this);
	_backerratic.SetFont(&app()->_fontStd);
	

	update();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void TowerSoundsStrikingControls::OnBnClickedApplyAll()
{
	_internalTower.setAllStrikingOn(TRUE);
	
}

void TowerSoundsStrikingControls::OnBnClickedRemoveAll()
{
	_internalTower.setAllStrikingOn(FALSE);
	
}

void TowerSoundsStrikingControls::OnBnClickedReset()
{
	StrikingErrors&  strikingErrors = _internalTower.getStrikingError(_bell);
	strikingErrors.reset();
	
	update();
}

void TowerSoundsStrikingControls::OnBnClickedResetAll()
{
	_internalTower.resetAllStriking();	 
	
}

void TowerSoundsStrikingControls::OnBnClickedQuiet()
{
	StrikingErrors&  strikingErrors = _internalTower.getStrikingError(_bell);
	strikingErrors.setQuiet(!strikingErrors.getQuiet());
	
}

void TowerSoundsStrikingControls::OnBnClickedPrevError()
{
	StrikingErrors&  strikingErrors = _internalTower.getStrikingError(_bell);
	strikingErrors.setFollowPreviousBellError(!strikingErrors.getFollowPreviousBellError());
	
}

void TowerSoundsStrikingControls::OnBnClickedUseStrikingErrors()
{
	StrikingErrors&  strikingErrors = _internalTower.getStrikingError(_bell);
	strikingErrors.setUseStrikingErrors(!strikingErrors.getUseStrikingErrors());
	
}

void TowerSoundsStrikingControls::OnBnClickedLockHandBack()
{
	StrikingErrors&  strikingErrors = _internalTower.getStrikingError(_bell);
	strikingErrors.setLockHandAndBack(!strikingErrors.getLockHandAndBack());
	
}
		 
void TowerSoundsStrikingControls::OnCbnSelchangeBell()
{
	int index = _bells.GetCurSel();
	ASSERT(index != LB_ERR);
	if (index != LB_ERR)
		_bell = index;
    update();
}

void TowerSoundsStrikingControls::OnUpdateOn(CCmdUI* pCmdUI) 
{			
	pCmdUI->Enable(TRUE);

	StrikingErrors& error = _internalTower.getStrikingError(_bell);
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


LRESULT TowerSoundsStrikingControls::OnNumericEditChange(WPARAM wParam,LPARAM /*lParam*/)
{
	NumericEdit* alteredCtrl = (NumericEdit*)wParam;

	if (!_updatingCtrls)
	{
		StrikingErrors& error = _internalTower.getStrikingError(_bell);
		
		if (alteredCtrl== &_handQuality)
			error.setHandstrokeQuality(_handQuality.GetValue());
		
		if (alteredCtrl== &_handLateness)
			error.setHandstrokeLateness(_handLateness.GetValue());
		
		if (alteredCtrl== &_backQuality)
			error.setBackstrokeQuality(_backQuality.GetValue());
		
		if (alteredCtrl== &_backLateness)
			error.setBackstrokeLateness(_backLateness.GetValue());

		update();		
	}

	return 0;
}

void TowerSoundsStrikingControls::update() 
{
	//all the check boxes are updated with TowerSoundsStrikingControls::OnUpdateOn

	_bells.ResetContent();

	int selectedBell = 0;
	for (int i=0;i<_internalTower.getNumber();i++)
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


	StrikingErrors& error = _internalTower.getStrikingError(_bell);
	
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


