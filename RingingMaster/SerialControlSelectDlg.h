
#pragma once
#include "afxwin.h"

#include "SerialControlItem.h"
#include "afxcmn.h"
// SerialControlSelectDlg dialog

class SerialControlSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(SerialControlSelectDlg)

public:
	SerialControlSelectDlg(CWnd* pParent, const SerialControlItem& item);   // standard constructor
	virtual ~SerialControlSelectDlg();

// Dialog Data
	enum { IDD = IDD_SERIAL_PORT_CONTROL_SELECT };
	
	SerialControlItem _serialControlItem;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void update();




	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox _port;
	CComboBox _signal;
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
};
