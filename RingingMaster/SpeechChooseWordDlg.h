#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SpeechItem.h"


// SpeechChooseWordDlg dialog

class SpeechChooseWordDlg : public CDialog
{
	DECLARE_DYNAMIC(SpeechChooseWordDlg)

public:
	SpeechChooseWordDlg(CWnd* pParent, const SpeechItem& item);   // standard constructor
	virtual ~SpeechChooseWordDlg();

// Dialog Data
	enum { IDD = IDD_SPEECH_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	SpeechItem _speechItem;
	virtual BOOL OnInitDialog();
	void update();
protected:
	virtual void OnOK();
	void captureData();
	CSpinButtonCtrl _delaySpin;
	CComboBox _count;
public:
	int _delay;
	CBCGPButton _ok;
private:
	CString _word;
};
