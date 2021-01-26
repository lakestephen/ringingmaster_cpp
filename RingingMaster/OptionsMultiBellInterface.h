#if !defined(AFX_OPTIONSMULTIBELLINTERFACE_H__0191015E_9D11_4CCC_B7D8_DDFBF21B8B08__INCLUDED_)
#define AFX_OPTIONSMULTIBELLINTERFACE_H__0191015E_9D11_4CCC_B7D8_DDFBF21B8B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsMultiBellInterface.h : header file
//


#include "CJFlatComboBox.h"
#include "SerialPortManager.h"
#include "MultiBellInterfaceManagerEventListener.h"
#include "afxwin.h"


/////////////////////////////////////////////////////////////////////////////
// OptionsMultiBellInterface dialog

class OptionsMultiBellInterface : public CBCGPPropertyPage, MultiBellInterfaceManagerEventListener
{
// Construction
public:
	void multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp);
	void multiBellInterfaceManager_notifyMBIStatusEvent(bool portOK, bool MBIOK);
	OptionsMultiBellInterface();   // standard constructor
	~OptionsMultiBellInterface();

// Dialog Data
	//{{AFX_DATA(OptionsMultiBellInterface)
	enum { IDD = IDD_OPTIONS_MULTI_BELL_INTERFACE };
	CStatic	_connectMsgCtrl;
	CBCGPButton	_connectIcon;
	CCJFlatComboBox	_comPortCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsMultiBellInterface)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void update();

	void multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap);

	bool _inited;

	// Generated message map functions
	//{{AFX_MSG(OptionsMultiBellInterface)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnable();
	afx_msg void OnUpdateEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeneral(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedTest();
	afx_msg void OnUpdateTest(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeComPort();
	afx_msg void OnChangeVal();
	//}}AFX_MSG
	afx_msg LRESULT OnMultiBellInterfaceEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CBCGPButton _test;
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	CBCGPButton _reconnect;
	afx_msg void OnBnClickedReconnect();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSMULTIBELLINTERFACE_H__0191015E_9D11_4CCC_B7D8_DDFBF21B8B08__INCLUDED_)
