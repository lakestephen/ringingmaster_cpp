// SpeechRecoTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SpeechRecoTestDlg.h"
#include "SpeechManager.h"
#include "WinPos.h"
#include "UserMessages.h"
#include "PLayManager.h"
#include "SpeechResponse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SpeechRecoTestDlg dialog


SpeechRecoTestDlg::SpeechRecoTestDlg(CWnd* pParent, const CStringArray& words)
: ResizeDialog(SpeechRecoTestDlg::IDD, pParent),
_words(words), 
_timestampOrigin(0)
{
	//{{AFX_DATA_INIT(SpeechRecoTestDlg)
	_testWords = _T("");
	//}}AFX_DATA_INIT

	for (int i=0;i<_words.GetSize();i++)
	{
		if (i>0)
			_testWords += ", ";

		_testWords += _words.GetAt(i);
	}

	speechManager()->addEventListener(this);
}


SpeechRecoTestDlg::~SpeechRecoTestDlg()
{
	speechManager()->removeEventListener(this);
}

void SpeechRecoTestDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SpeechRecoTestDlg)
	DDX_Control(pDX, IDC_DETECTED_WORDS, _detectedWordsCtrl);
	DDX_Text(pDX, IDC_STATIC_TEST_WORDS, _testWords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SpeechRecoTestDlg, ResizeDialog)
	ON_MESSAGE(SPEECH_EVENT, OnSpeechEvent)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SpeechRecoTestDlg message handlers

BOOL SpeechRecoTestDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
	
//	AddControl(IDOK, repos_center, repos);
//	AddControl(IDCANCEL, repos_center, repos);	
	AddControl(IDC_STATIC_TEST_WORDS, resize, none);
	AddControl(IDC_DETECTED_WORDS, resize, resize);

	WinPos::Load(this, "SpeechRecoTestDlg", true);
		
	_detectedWordsCtrl.SetTabStops(50);	

	return TRUE;  // return TRUE unless you set the focus to a control

}



void SpeechRecoTestDlg::speechManager_notifySpeechRecognisedFromSeperateThread(const SpeechResponse& response, DWORD timestamp) 
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, SPEECH_EVENT ,(WPARAM)(&response), timestamp); 
  
}

LRESULT SpeechRecoTestDlg::OnSpeechEvent(WPARAM wParam, LPARAM lParam)	
{				 
	SpeechResponse* item = (SpeechResponse*)wParam;

	int time = lParam - _timestampOrigin;

	time = min(time, 99999);
	
	CString str;
	str.Format("%05d ms\t'%s'",time , item->getDescription());

	_detectedWordsCtrl.AddString(str);

	int nCount = _detectedWordsCtrl.GetCount();
	if (nCount > 0)
		_detectedWordsCtrl.SetCurSel(nCount-1);

	playManager()->playDefaultBell();

	_timestampOrigin = lParam; 

	return 0;
}

void SpeechRecoTestDlg::speechManager_notifyGetSpeechList(SpeechActionEventDataMap& speechActionEventDataMap)
{
	for (int i=0;i<_words.GetSize();i++)
		speechActionEventDataMap.SetAt(_words.GetAt(i),new SimulatorEventData(esigt_both));	
} 

BOOL SpeechRecoTestDlg::DestroyWindow() 
{
	WinPos::Save(this, "SpeechRecoTestDlg");
	
	return ResizeDialog::DestroyWindow();
}
