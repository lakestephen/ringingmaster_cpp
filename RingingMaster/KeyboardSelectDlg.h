
#pragma once
#include "afxwin.h"

#include "KeyboardItem.h"
#include "afxcmn.h"
// KeyboardSelectDlg dialog

class KeyboardSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(KeyboardSelectDlg)

public:
	KeyboardSelectDlg(CWnd* pParent, const KeyboardItem& item);   // standard constructor
	virtual ~KeyboardSelectDlg();

// Dialog Data
	enum { IDD = IDD_KEYBOARD_SELECT };
	
	KeyboardItem _keyboardItem;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void update();




	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox _type;
	CComboBox _count;
protected:
	virtual void OnOK();
	void captureData();
public:
	afx_msg void OnBnClickedNextPortControl();
	CBCGPButton _nextPortControl;
	CSpinButtonCtrl _delaySpin;
	int _delay;
	CBCGPButton _ok;
	CStatic _keyStatic;
};
