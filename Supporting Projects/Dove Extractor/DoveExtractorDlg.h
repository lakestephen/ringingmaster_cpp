// DoveExtractorDlg.h : header file
//

#if !defined(AFX_DOVEEXTRACTORDLG_H__60150F52_0514_4DA6_A283_EFA45CDCB4A1__INCLUDED_)
#define AFX_DOVEEXTRACTORDLG_H__60150F52_0514_4DA6_A283_EFA45CDCB4A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDoveExtractorDlg dialog

class CDoveExtractorDlg : public CDialog
{
// Construction
public:
	CDoveExtractorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDoveExtractorDlg)
	enum { IDD = IDD_DOVEEXTRACTOR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoveExtractorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDoveExtractorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnExtract();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOVEEXTRACTORDLG_H__60150F52_0514_4DA6_A283_EFA45CDCB4A1__INCLUDED_)
