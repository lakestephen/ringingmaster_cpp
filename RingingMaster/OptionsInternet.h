#if !defined(AFX_OPTIONSINTERNET_H__E77DFBB6_95B1_11D7_B6CB_000255162CF0__INCLUDED_)
#define AFX_OPTIONSINTERNET_H__E77DFBB6_95B1_11D7_B6CB_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsInternet.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// OptionsInternet dialog

class OptionsInternet : public CBCGPPropertyPage
{
// Construction
public:
	OptionsInternet();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsInternet)
	enum { IDD = IDD_OPTIONS_INTERNET };
	CButton	_internalBrowserCtrl;
	CBCGPButton	_testConection;
	CStatic	_posrtStatic;
	CStatic	_ipStatic;
	CButton	_useProxyCtrl;
	CEdit	_portCtrl;
	CButton	_defaultConnectionCtrl;
	CIPAddressCtrl	_ipADdress;
	int		_connectionStyle;
	int		_port;
	int		_launchStyle;
	//}}AFX_DATA
			  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsInternet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsInternet)
	virtual BOOL OnInitDialog();
	afx_msg void OnTestConnection();
	afx_msg void OnUpdateTestConnection(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSINTERNET_H__E77DFBB6_95B1_11D7_B6CB_000255162CF0__INCLUDED_)
