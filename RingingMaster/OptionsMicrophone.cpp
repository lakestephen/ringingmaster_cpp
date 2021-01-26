// OptionsMicrophone.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsMicrophone.h"
#include "SpeechManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsMicrophone dialog


OptionsMicrophone::OptionsMicrophone()
	: CBCGPPropertyPage(OptionsMicrophone::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsMicrophone)
	//}}AFX_DATA_INIT
}				  


void OptionsMicrophone::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsMicrophone)
	DDX_Control(pDX, IDC_SYSTEM_SETUP, _systemSetupBtn);
	DDX_Control(pDX, IDC_SETTINGS, _settingsBtn);
	DDX_Control(pDX, IDC_CONFIG_MIC, _configMikeBtn);
	DDX_Control(pDX, IDC_SYSTEM_VOLUME, _systemVolumeBtn);
	DDX_Control(pDX, IDC_TRAIN, _trainBtn);
	DDX_Control(pDX, IDC_TEST, _testBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsMicrophone, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsMicrophone)
	ON_BN_CLICKED(IDC_SETUP, OnSetup)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_UPDATE_COMMAND_UI(IDC_PROFILE_COMBO, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_ENABLE, OnUpdateEnable)		   	
	ON_UPDATE_COMMAND_UI(IDC_SYSTEM_VOLUME, OnUpdateSystemVolume)
	ON_BN_CLICKED(IDC_SYSTEM_VOLUME, OnSystemVolume)
	ON_BN_CLICKED(IDC_CONFIG_MIC, OnConfigMic)
	ON_BN_CLICKED(IDC_SYSTEM_SETUP, OnSystemSetup)
	ON_UPDATE_COMMAND_UI(IDC_STATIC_PROFILE, OnUpdateEnable)		   	
	ON_UPDATE_COMMAND_UI(IDC_SYSTEM_SETUP, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_NEW_PROFILE, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_DELETE_PROFILE, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_SETTINGS, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDD_OPTIONS_MICROPHONE, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_TEST, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_LANGUAGE, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_LANGUAGE, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_MIC_LEVEL, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_MICROPHONE, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_CONFIG_MIC, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_TRAIN, OnUpdateGeneral)		   	
	ON_BN_CLICKED(IDC_SETTINGS, OnSettings)
	ON_BN_CLICKED(IDC_TRAIN, OnTrain)
	ON_BN_CLICKED(IDC_TEST, OnBnClickedTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsMicrophone message handlers

void OptionsMicrophone::OnSetup() 
{
	speechManager()->displayUI(SPDUI_UserTraining, true);
	speechManager()->displayUI(SPDUI_MicTraining, true);
	speechManager()->displayUI(SPDUI_RecoProfileProperties, true);
	speechManager()->displayUI(SPDUI_AudioVolume, false);
}




void OptionsMicrophone::OnEnable() 
{
	speechManager()->setUsingSpeechRecognition(!speechManager()->getUsingSpeechRecognition());	
}

void OptionsMicrophone::OnUpdateGeneral(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(speechManager()->isSpeechRecognitionAvailable());
}		  

void OptionsMicrophone::OnUpdateEnable(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(speechManager()->isSpeechRecognitionDLLAvailable());
	pCmdUI->SetCheck(speechManager()->getUsingSpeechRecognition()?1:0);
}		  

void OptionsMicrophone::OnUpdateSystemVolume(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(speechManager()->isSpeechRecognitionAvailable()&&
		 speechManager()->isUISupported(SPDUI_AudioVolume, false));
}

void OptionsMicrophone::OnSystemVolume() 
{
	speechManager()->displayUI(SPDUI_AudioVolume, false);	
}		

BOOL OptionsMicrophone::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_systemVolumeBtn.SetImage(IDB_VOLUME);
	_systemVolumeBtn.m_bDrawFocus = FALSE;
	_systemVolumeBtn.SetTooltip("Launch the system volume dialog.");
	_systemVolumeBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_configMikeBtn.SetImage(IDB_MIKE);
	_configMikeBtn.m_bDrawFocus = FALSE;
	_configMikeBtn.SetTooltip("Launch the microphone setup wizard.");
	_configMikeBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_systemSetupBtn.SetImage(IDB_SPEECH);
	_systemSetupBtn.m_bDrawFocus = FALSE;
	_systemSetupBtn.SetTooltip("Launch the speech system setup.");
	_systemSetupBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_settingsBtn.SetImage(IDB_SOUND_SETTINGS);
	_settingsBtn.m_bDrawFocus = FALSE;
	_settingsBtn.SetTooltip("Launch the speech settings dialog.");
	_settingsBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_trainBtn.SetImage(IDB_TRAIN);
	_trainBtn.m_bDrawFocus = FALSE;
	_trainBtn.SetTooltip("Launch the speech training dialog.");
	_trainBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_testBtn.SetImage(IDB_MIKE);
	_testBtn.m_bDrawFocus = FALSE;
	_testBtn.SetTooltip("Launch the microphone test dialog.");
	_testBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsMicrophone::OnConfigMic() 
{
	speechManager()->displayUI(SPDUI_MicTraining, true);	
}


void OptionsMicrophone::OnSystemSetup() 
{
	WinExec("control.exe speech", SW_NORMAL);	
}

void OptionsMicrophone::OnSettings() 
{
	speechManager()->displayUI(SPDUI_RecoProfileProperties, true);	
}

void OptionsMicrophone::OnTrain() 
{
	speechManager()->displayUI(SPDUI_UserTraining, true);
}

void OptionsMicrophone::OnBnClickedTest() 
{
	speechManager()->testSpeechReco(this);
}
