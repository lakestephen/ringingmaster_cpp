#if !defined(AFX_PRINTANALYSISSETUPDLG_H__2A8793E0_FEBD_11D5_BB7F_F400AAF0164B__INCLUDED_)
#define AFX_PRINTANALYSISSETUPDLG_H__2A8793E0_FEBD_11D5_BB7F_F400AAF0164B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintAnalysisSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PrintAnalysisSetupDlg dialog

class PrintAnalysisSetupDlg : public CDialog
{
// Construction
public:
	void update();
	PrintAnalysisSetupDlg(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PrintAnalysisSetupDlg)
	enum { IDD = IDD_PRINT_ANALYSIS_SETUP };
	CButton	_ctrlSpliceTransition;
	CBCGPButton	_print;
	CBCGPButton	_preview;
	CButton	_ctrlMultiBlock;
	CButton	_ctrlSplice;
	BOOL	_bAllWork;
	BOOL	_bCourse;
	BOOL	_bMultiBlock;
	BOOL	_bMusic;
	BOOL	_bSplice;
	BOOL	_bStatus;
	BOOL	_bFalseness;
	BOOL	_bLeadEnds;
	BOOL	_bCallsMade;
	BOOL	_bSpliceTransition;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PrintAnalysisSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PrintAnalysisSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPreview();
	afx_msg void OnPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTANALYSISSETUPDLG_H__2A8793E0_FEBD_11D5_BB7F_F400AAF0164B__INCLUDED_)
