// OptionsTTS.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsTTS.h"
#include "SpeechManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsTTS dialog


OptionsTTS::OptionsTTS()
	: CBCGPPropertyPage(OptionsTTS::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsTTS)
	//}}AFX_DATA_INIT
}


void OptionsTTS::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsTTS)
	DDX_Control(pDX, IDC_SYSTEM_SETUP, _systemSetupBtn);
	DDX_Control(pDX, IDC_VOICES, _voices);
	DDX_Control(pDX, IDC_SPEED, _speedCtrl);
	DDX_Control(pDX, IDC_SYSTEM_VOLUME, _systemVolumeBtn);
	DDX_Control(pDX, IDC_TEST, _testBtn);
	DDX_Control(pDX, IDC_VOLUME, _volumeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsTTS, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsTTS)
	ON_BN_CLICKED(IDC_TEST, OnBnClickedTest)
	ON_BN_CLICKED(IDC_SYSTEM_VOLUME, OnSystemVolume)
	ON_UPDATE_COMMAND_UI(IDC_TEST, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_ENABLE, OnUpdateEnable)		   	
	ON_UPDATE_COMMAND_UI(IDC_SYSTEM_VOLUME, OnUpdateSystemVolume)		   	
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_CBN_SELCHANGE(IDC_VOICES, OnSelchangeVoices)
	ON_UPDATE_COMMAND_UI(IDC_SPEED, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_SPEED, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_FAST, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_SLOW, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_QUITET, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_LOUD, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_VOLUME, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_VOLUME, OnUpdateGeneral)		
	ON_UPDATE_COMMAND_UI(IDC_STATIC_VOICES, OnUpdateGeneral)		
	ON_UPDATE_COMMAND_UI(IDC_STATIC_SETUP, OnUpdateEnable)		   
	ON_UPDATE_COMMAND_UI(IDC_VOICES, OnUpdateGeneral)		   
	ON_BN_CLICKED(IDC_SYSTEM_SETUP, OnSystemSetup)
	ON_UPDATE_COMMAND_UI(IDC_SYSTEM_SETUP, OnUpdateGeneral)		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsTTS message handlers



BOOL OptionsTTS::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_testBtn.SetImage(IDB_SPEECH_BUBBLE);
	_testBtn.m_bDrawFocus = FALSE;
	_testBtn.SetTooltip("Test the speech generation system");
	_testBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_systemVolumeBtn.SetImage(IDB_VOLUME);
	_systemVolumeBtn.m_bDrawFocus = FALSE;
	_systemVolumeBtn.SetTooltip("Launch the system volume dialog");
	_systemVolumeBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_systemSetupBtn.SetImage(IDB_SPEECH);
	_systemSetupBtn.m_bDrawFocus = FALSE;
	_systemSetupBtn.SetTooltip("Launch the speech system setup.");
	_systemSetupBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	_volumeCtrl.SetRange(0,100);
	_volumeCtrl.SetPos(speechManager()->getTTSVolume());

	_speedCtrl.SetRange(-10,10);
	_speedCtrl.SetPos(speechManager()->getTTSSpeed());

	SpeechVoices& speechVoices = speechManager()->getSpeechVoices();

	int index = _voices.AddString("[computers default voice]");
	_voices.SetCurSel(index);
	for (int i=0;i<speechVoices.GetSize();i++)
	{
		SpeechVoice* item = speechVoices.GetAt(i);
		index = _voices.AddString(item->getName());

		if (index >= 0)
			_voices.SetItemDataPtr(index, item);				

		if ((!speechManager()->getTTSVoiceID().IsEmpty())&&
			(speechManager()->getTTSVoiceID().CompareNoCase(item->getID())==0 ))
		{
			_voices.SetCurSel(index);
		}
	}					   						

	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsTTS::OnUpdateGeneral(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(speechManager()->isTTSAvailable());
}		  

void OptionsTTS::OnBnClickedTest() 
{
	CWaitCursor wait;
	
	CString str;
	str.Format("You have selected %s as Ringing Master's voice.", speechManager()->getTTSVoiceName());
	speechManager()->speak(str);
}

void OptionsTTS::OnUpdateSystemVolume(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(speechManager()->isTTSAvailable()&&
		 speechManager()->isUISupported(SPDUI_AudioVolume, false));
}

void OptionsTTS::OnSystemVolume() 
{
	speechManager()->displayUI(SPDUI_AudioVolume, false);	
}		

void OptionsTTS::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	speechManager()->setTTSVolume(_volumeCtrl.GetPos());
	speechManager()->setTTSSpeed(_speedCtrl.GetPos());

		
	CBCGPPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void OptionsTTS::OnUpdateEnable(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(speechManager()->isTTSDLLAvailable());
	pCmdUI->SetCheck(speechManager()->getUsingTTS()?1:0);
}		  


void OptionsTTS::OnEnable() 
{
	speechManager()->setUsingTTS(!speechManager()->getUsingTTS());
}

void OptionsTTS::OnSelchangeVoices() 
{
	SpeechVoice* item = static_cast<SpeechVoice*>(_voices.GetItemDataPtr(_voices.GetCurSel()));
	
	if (item)
		speechManager()->setTTSVoiceID(item->getID());
	else
		speechManager()->setTTSVoiceID(CString(""));

	OnBnClickedTest();
}

void OptionsTTS::OnSystemSetup() 
{
	WinExec("control.exe speech", SW_NORMAL);	
}
