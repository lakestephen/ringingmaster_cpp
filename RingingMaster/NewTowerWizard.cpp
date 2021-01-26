// NewTowerWizard.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NewTowerWizard.h"
#include "Tower.h"
#include ".\newtowerwizard.h"


// NewTowerWizard dialog

IMPLEMENT_DYNAMIC(NewTowerWizard, CDialog)
NewTowerWizard::NewTowerWizard(CWnd* pParent /*=NULL*/)
	: CDialog(NewTowerWizard::IDD, pParent)
	, _staticPath(_T(""))
	, _usingSimulator(FALSE)
	, _simulatorUseType(0)
	, _reflectorNumber(1)
	, _comPort(0)
	, _addSpeechCalls(FALSE)
{
}

NewTowerWizard::~NewTowerWizard()
{
}

void NewTowerWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PATH, _staticPath);
	DDX_Control(pDX, IDC_USING_SIMULATOR, _usingSimulatorCtrl);
	DDX_Check(pDX, IDC_USING_SIMULATOR, _usingSimulator);
	DDX_Radio(pDX, IDC_HOME_USE, _simulatorUseType);
	DDX_Radio(pDX, IDC_ONE_REFLECTOR, _reflectorNumber);
	DDX_Radio(pDX, IDC_COM1, _comPort);
	DDX_Check(pDX, IDC_ADD_SPEECH_CALLS, _addSpeechCalls);
}


BEGIN_MESSAGE_MAP(NewTowerWizard, CDialog)
	ON_BN_CLICKED(IDC_TEMPLATE_BUTT, OnBnClickedTemplateButt)
	ON_BN_CLICKED(ID_CREATE_BLANK, OnBnClickedCreateBlank)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_UPDATE_COMMAND_UI(IDC_HOME_USE, OnUpdateUsingSimulator)
	ON_UPDATE_COMMAND_UI(IDC_TOWER_USE_SERIAL, OnUpdateUsingSimulator)
	ON_UPDATE_COMMAND_UI(IDC_ADD_SPEECH_CALLS, OnUpdateUsingSimulator)
	ON_UPDATE_COMMAND_UI(IDC_TOWER_USE_MBI, OnUpdateUsingSimulator)
	ON_UPDATE_COMMAND_UI(IDC_ONE_REFLECTOR, OnUpdateUsingSimulatorInTower)
	ON_UPDATE_COMMAND_UI(IDC_TWO_REFLECTOR, OnUpdateUsingSimulatorInTower)
	ON_UPDATE_COMMAND_UI(IDC_COM1, OnUpdateUsingSimulatorInTowerMBI)
	ON_UPDATE_COMMAND_UI(IDC_COM2, OnUpdateUsingSimulatorInTowerMBI)
	ON_UPDATE_COMMAND_UI(IDC_COM3, OnUpdateUsingSimulatorInTowerMBI)
	ON_UPDATE_COMMAND_UI(IDC_COM4, OnUpdateUsingSimulatorInTowerMBI)

	
END_MESSAGE_MAP()


LRESULT NewTowerWizard::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}

// NewTowerWizard message handlers

void NewTowerWizard::OnBnClickedTemplateButt()
{
	Tower tower;
	if (tower.createFromTemplate())
	{
		TowerBell towerBell = tower.getTowerBell(0, hand);
		_treblePath = towerBell.getFile();
		_staticPath = _treblePath;
		UpdateData(FALSE);
	}
}

void NewTowerWizard::OnOK()
{
	if (_treblePath.IsEmpty())
	{
		AfxMessageBox("Please select the treble .wav file path.");
		return;
	}

	UpdateData();

	CDialog::OnOK();
}

void NewTowerWizard::OnBnClickedCreateBlank()
{
	EndDialog(ID_CREATE_BLANK);		
}

void NewTowerWizard::OnUpdateUsingSimulator(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_usingSimulatorCtrl.GetCheck() == TRUE);
}

void NewTowerWizard::OnUpdateUsingSimulatorInTower(CCmdUI* pCmdUI) 
{
	UpdateData();
	pCmdUI->Enable(_usingSimulatorCtrl.GetCheck() == TRUE && (_simulatorUseType == 1 || _simulatorUseType == 2));
}

void NewTowerWizard::OnUpdateUsingSimulatorInTowerMBI(CCmdUI* pCmdUI) 
{
	UpdateData();
	pCmdUI->Enable(_usingSimulatorCtrl.GetCheck() == TRUE && (_simulatorUseType == 1));
}
