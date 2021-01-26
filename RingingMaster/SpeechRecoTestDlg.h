#if !defined(AFX_SPEECHRECOTESTDLG_H__C56EB6FD_7BBF_4856_B6C8_680A9C491E60__INCLUDED_)
#define AFX_SPEECHRECOTESTDLG_H__C56EB6FD_7BBF_4856_B6C8_680A9C491E60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpeechRecoTestDlg.h : header file
//
#include "ResizeDialog.h"
#include "SpeechManagerEventlistener.h"
#include "CJListBox.h"

/////////////////////////////////////////////////////////////////////////////
// SpeechRecoTestDlg dialog

class SpeechRecoTestDlg : public ResizeDialog, SpeechManagerEventListener
{
// Construction
public:
	SpeechRecoTestDlg(CWnd* pParent, const CStringArray& words);   // standard constructor
	virtual ~SpeechRecoTestDlg();

// Dialog Data
	//{{AFX_DATA(SpeechRecoTestDlg)
	enum { IDD = IDD_SPEECH_RECO_TEST };
	CCJListBox	_detectedWordsCtrl;
	CString	_testWords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SpeechRecoTestDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	const CStringArray& _words;

	virtual void speechManager_notifySpeechRecognisedFromSeperateThread(const SpeechResponse& response, DWORD timestamp) ;
	virtual void speechManager_notifyGetSpeechList(SpeechActionEventDataMap& speechActionEventDataMap);

	int _timestampOrigin;

	afx_msg LRESULT OnSpeechEvent(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(SpeechRecoTestDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPEECHRECOTESTDLG_H__C56EB6FD_7BBF_4856_B6C8_680A9C491E60__INCLUDED_)
