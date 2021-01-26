#pragma once
#include "afxcmn.h"
#include "SerialControlManagerEventListener.h"

/////////////////////////////////////////////////////////////////////////////
// OptionsSerialControl dialog

class OptionsSerialControl : public CBCGPPropertyPage, SerialControlManagerEventListener
{
// Construction
public:
	OptionsSerialControl();   // standard constructor
	~OptionsSerialControl();
	
// Dialog Data
	//{{AFX_DATA(OptionsSerialControl)
	enum { IDD = IDD_OPTIONS_SERIAL_COMMAND};
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsSerialControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBCGPButton _test;
	CCheckListBox _ports;
	void update();

	// Generated message map functions
	//{{AFX_MSG(OptionsSerialControl)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	//}}AFX_MSG
	afx_msg void OnBnClickedTest();
	afx_msg void OnUpdateTest(CCmdUI* pCmdUI);
	afx_msg void OnEnable();
	afx_msg void OnUpdateEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeneral(CCmdUI* pCmdUI);
	afx_msg void OnClbnChkChangePorts();

	void serialControlManager_notifySerialControlStatusEvent();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnLbnSelchangePorts();
	int _debounce;
	CSpinButtonCtrl _debounceSpin;
	int _interval;
	CSpinButtonCtrl _intervalSpin;
	CBCGPButton _reconnect;
	afx_msg void OnBnClickedReconnect();
};
