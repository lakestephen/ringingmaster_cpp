#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MultiBellItem.h"


// MultiBellInterfaceChooseChannelDlg dialog

class MultiBellInterfaceChooseChannelDlg : public CDialog
{
	DECLARE_DYNAMIC(MultiBellInterfaceChooseChannelDlg)

public:
	MultiBellInterfaceChooseChannelDlg(CWnd* pParent, const MultiBellItem& item);   // standard constructor
	virtual ~MultiBellInterfaceChooseChannelDlg();

// Dialog Data
	enum { IDD = IDD_MBI_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	MultiBellItem _multiBellItem;
	virtual BOOL OnInitDialog();
	void update();
protected:
	virtual void OnOK();
	void captureData();
	CBCGPButton _nextMBI;  
	CSpinButtonCtrl _delaySpin;
	CComboBox _count;
	CComboBox _channel;
public:
	afx_msg void OnBnClickedNextMbi();
	int _delay;
	CBCGPButton _ok;
};
