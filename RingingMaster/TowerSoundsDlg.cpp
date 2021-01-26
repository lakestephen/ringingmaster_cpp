#include "StdAfx.h"

#include "TowerSoundsDlg.h"
#include "TowerSoundsGeneral.h"
#include "TowerSoundsFiles.h"
#include "TowerSoundsFilesAdvanced.h"
#include "TowerSoundsOptions.h"
#include "TowerSoundsTTS.h"
#include "TowerSoundsBellInputs.h"
#include "TowerSoundsCommandInputs.h"
#include "TowerSoundsMethodInputs.h"
#include "TowerSoundsSimulatorDisplay.h"
#include "TowerSoundsStrikingControls.h"
#include "TowerSoundsCallInputs.h"

#include "PlayManager.h"
#include "TowerManager.h"
#include "WinPos.h"

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsDlg



IMPLEMENT_DYNAMIC(TowerSoundsDlg, PropertySheetBase)

TowerSoundsDlg::TowerSoundsDlg(CWnd* pParent, Tower& tower, bool editing)
	 : PropertySheetBase("Tower Profile", pParent),
 	_origTower(tower), //keep a reference
	_internalTower(tower), //keep a copy
	_editing(editing)
{

	towerManager()->setEditingProfile(true);
	//set up the dialog stuff	
	m_psh.dwFlags |= PSH_NOAPPLYNOW | PSH_PROPTITLE;
	m_psh.dwFlags &= ~PSH_HASHELP;


	_towerSoundsGeneral	= new TowerSoundsGeneral(_internalTower);                             
	_towerSoundsFiles = new TowerSoundsFiles(_internalTower);	 
	_towerSoundsFilesAdvanced = new TowerSoundsFilesAdvanced(_internalTower);	 
	_towerSoundsOptions = new TowerSoundsOptions(_internalTower);
	_towerSoundsTTS = new TowerSoundsTTS(_internalTower);
	_towerSoundsBellInputs = new TowerSoundsBellInputs(_internalTower);
	_towerSoundsCommandInputs = new TowerSoundsCommandInputs(_internalTower);
	_towerSoundsMethodInputs = new TowerSoundsMethodInputs(_internalTower);
	_towerSoundsCallInputs = new TowerSoundsCallInputs(_internalTower);
	_towerSoundsSimulatorDisplay = new TowerSoundsSimulatorDisplay(_internalTower);
	_towerSoundsStrikingControls = new TowerSoundsStrikingControls(_internalTower);

	CBCGPPropSheetCategory* pGeneral = AddTreeCategory (_T("General"), 0, 1);
	AddPageToTree (pGeneral, _towerSoundsGeneral, -1, 2);
	AddPageToTree (pGeneral, _towerSoundsOptions, -1, 2);

	CBCGPPropSheetCategory* pSounds = AddTreeCategory (_T("Sounds"), 0, 1);
	AddPageToTree (pSounds, _towerSoundsFiles, -1, 2);
	AddPageToTree (pSounds, _towerSoundsFilesAdvanced, -1, 2);
	AddPageToTree (pSounds, _towerSoundsTTS, -1, 2);

	CBCGPPropSheetCategory* pSimulator = AddTreeCategory (_T("Simulator"), 0, 1);
	AddPageToTree (pSimulator, _towerSoundsBellInputs, -1, 2);
	AddPageToTree (pSimulator, _towerSoundsCallInputs, -1, 2);	
	AddPageToTree (pSimulator, _towerSoundsMethodInputs, -1, 2);
	AddPageToTree (pSimulator, _towerSoundsCommandInputs, -1, 2);
	AddPageToTree (pSimulator, _towerSoundsStrikingControls, -1, 2);
	AddPageToTree (pSimulator, _towerSoundsSimulatorDisplay, -1, 2);
	

	SetActivePage(_towerSoundsGeneral);					  

}

TowerSoundsDlg::~TowerSoundsDlg()
{

	delete _towerSoundsGeneral;
	delete _towerSoundsFiles;
	delete _towerSoundsFilesAdvanced;
	delete _towerSoundsOptions;
	delete _towerSoundsTTS;
	delete _towerSoundsBellInputs;
	delete _towerSoundsCommandInputs;
	delete _towerSoundsMethodInputs;
	delete _towerSoundsCallInputs;
	delete _towerSoundsSimulatorDisplay;
	delete _towerSoundsStrikingControls;
	
	towerManager()->setEditingProfile(false);
}


BEGIN_MESSAGE_MAP(TowerSoundsDlg, PropertySheetBase)
	//{{AFX_MSG_MAP(TowerSoundsDlg)
	ON_BN_CLICKED(IDTEST, OnTest)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_UPDATE_COMMAND_UI(IDC_STOP, OnUpdateStop)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL TowerSoundsDlg::OnInitDialog()
{
	BOOL bResult = PropertySheetBase::OnInitDialog();

	WinPos::Load(this, "TowerSoundsDlg", false);
	
	setConstantText(_internalTower.getName());

	CWnd* okBtn = GetDlgItem(IDOK);
	if (okBtn && ::IsWindow(okBtn->m_hWnd))
	{
		//get the rect of the OK btn
  		CRect okBtnRect;	
		okBtn->GetWindowRect (okBtnRect);
		ScreenToClient (okBtnRect);

		//add in the extra buttons
		CRect stopBtnRect = (okBtnRect - CPoint(60,0));
		stopBtnRect.right = stopBtnRect.left + 32;
		_stopBtn.Create("", WS_CHILD|BS_CHECKBOX|WS_VISIBLE, stopBtnRect , this, IDC_STOP);
		_stopBtn.SetImage(IDB_STOP);
		_stopBtn.m_bDrawFocus = FALSE;
		_stopBtn.SetTooltip("Stop the playing method.");
		_stopBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		CRect testBtnRect = (okBtnRect - CPoint(160,0));
		testBtnRect.right += 20;
		_testBtn.Create("Apply && Test", WS_CHILD|BS_CHECKBOX|WS_VISIBLE, testBtnRect , this, IDTEST);
		_testBtn.SetImage(IDB_BELL);
		_testBtn.m_bDrawFocus = FALSE;
		_testBtn.SetTooltip("Test the profile sounds.");
		_testBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	}

	return bResult;
}

void TowerSoundsDlg::OnStop() 
{
	playManager()->OnPlayStop();
}

void TowerSoundsDlg::OnUpdateStop(CCmdUI* pCmdUI) 
{
	playManager()->OnUpdateStopMethod(pCmdUI);
}          

void TowerSoundsDlg::OnTest() 
{
	if (apply())
	{
		//stop if currently playing a single bell.
		if (::IsWindow(_towerSoundsFiles->m_hWnd))
			_towerSoundsFiles->OnTest(-1,hand,true);

		playManager()->test(false);	 

		_internalTower.setupForEditing();
	}
}

bool TowerSoundsDlg::apply() 
{
	//force all open tabs to save there data
	PressButton(PSBTN_APPLYNOW);
	
	if ((towerManager()->isTowerNameUnique(_internalTower.getName()))||
		((_editing)&&(_internalTower.getName() == _origTower.getName())))
	{
		CString msg;

		TowerValidity towerValidity = _internalTower.isValid(msg);
		switch (towerValidity)
		{
		case tv_ok:
			{
				_internalTower.cleanupAfterEditing();
				_origTower = _internalTower;
				towerManager()->fireUpdateTowerList();
				return true;
			}
			break;
		case tv_wavProblem:
			SetActivePage(_towerSoundsFiles);   
			AfxMessageBox(msg);
			break;
		case tv_bellInputProblem:
			SetActivePage(_towerSoundsBellInputs);
			AfxMessageBox(msg);
			break;
		case tv_commandInputProblem:
			SetActivePage(_towerSoundsCommandInputs);
			AfxMessageBox(msg);
			break;
		case tv_callInputProblem:
			SetActivePage(_towerSoundsCallInputs);
			AfxMessageBox(msg);
			break;
		case tv_methodInputProblem:
			SetActivePage(_towerSoundsMethodInputs);
			AfxMessageBox(msg);
			break;
		case tv_simulatorDisplay:
			SetActivePage(_towerSoundsSimulatorDisplay);
			AfxMessageBox(msg);
			break; 			
		}
	}
	else
	{
		SetActivePage(_towerSoundsGeneral);	   
		AfxMessageBox("Please choose a unique tower name");
	}


	return false;
}	
void TowerSoundsDlg::OnDestroy()
{
	WinPos::Save(this, "TowerSoundsDlg");

	PropertySheetBase::OnDestroy();
}

void TowerSoundsDlg::OnOK() 
{
	if (apply())
		PressButton(PSBTN_OK);
}

void TowerSoundsDlg::showTowerSoundsFilesPage()
{
	SetActivePage(_towerSoundsFiles);
}

