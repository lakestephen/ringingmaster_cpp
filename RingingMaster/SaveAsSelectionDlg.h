#if !defined(AFX_SAVEASSELECTIONDLG_H__896014DB_E771_11D7_B701_000255162CF0__INCLUDED_)
#define AFX_SAVEASSELECTIONDLG_H__896014DB_E771_11D7_B701_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveAsSelectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SaveAsSelectionDlg dialog

class SaveAsSelectionDlg : public CDialog
{
// Construction
public:
	bool isTouchWindow();
	SaveAsSelectionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SaveAsSelectionDlg)
	enum { IDD = IDD_SAVE_AS_SELECTION };
	int		_touchWindow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SaveAsSelectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SaveAsSelectionDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEASSELECTIONDLG_H__896014DB_E771_11D7_B701_000255162CF0__INCLUDED_)
