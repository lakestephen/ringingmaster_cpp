#if !defined(AFX_DIRECTIONDLG_H__EF674028_4592_11D7_B67F_000255162CF0__INCLUDED_)
#define AFX_DIRECTIONDLG_H__EF674028_4592_11D7_B67F_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DirectionDlg dialog

class DirectionDlg : public CDialog
{
// Construction
public:
	DirectionDlg( BOOL& ascending, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DirectionDlg)
	enum { IDD = IDD_DIRECTION };
	int		_ascending;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DirectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	BOOL& _originalAscending;

	// Generated message map functions
	//{{AFX_MSG(DirectionDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTIONDLG_H__EF674028_4592_11D7_B67F_000255162CF0__INCLUDED_)
