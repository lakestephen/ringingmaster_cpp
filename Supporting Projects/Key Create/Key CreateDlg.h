// Key CreateDlg.h : header file
//

#if !defined(AFX_KEYCREATEDLG_H__D6385BDA_F35E_11D4_B571_009027BB3286__INCLUDED_)
#define AFX_KEYCREATEDLG_H__D6385BDA_F35E_11D4_B571_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeyCreateDlg dialog

#include "ComboCompletion.h"

class CKeyCreateDlg : public CDialog
{
// Construction
public:
	CKeyCreateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyCreateDlg)
	enum { IDD = IDD_KEYCREATE_DIALOG };
	CComboCompletion	_versionCtrl;
	CString	_key;
	CString	_user;
	int		_version;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyCreateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKeyCreateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGenerate();
	afx_msg void OnVerify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCREATEDLG_H__D6385BDA_F35E_11D4_B571_009027BB3286__INCLUDED_)
