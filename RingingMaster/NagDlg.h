#if !defined(AFX_NAGDLG_H__3DF27734_F1E8_11D4_B570_009027BB3286__INCLUDED_)
#define AFX_NAGDLG_H__3DF27734_F1E8_11D4_B570_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NagDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NagDlg dialog

class NagDlg : public CDialog
{
// Construction
public:
	void commonSetTimer();
	NagDlg(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NagDlg)
	enum { IDD = IDD_NAG };
	CButton	_register;
	CButton	_ok;
	CProgressCtrl	_progress;
	CBCGPURLLinkButton	_url;
	CBCGPURLLinkButton	_mail;
	CString	_message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NagDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int _timingCount;
	static bool _firstTime;
	// Generated message map functions
	//{{AFX_MSG(NagDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRegister();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAGDLG_H__3DF27734_F1E8_11D4_B570_009027BB3286__INCLUDED_)
