#if !defined(AFX_AUTOSELEDIT_H__EFED528E_51A4_11D4_B4D3_009027BB3286__INCLUDED_)
#define AFX_AUTOSELEDIT_H__EFED528E_51A4_11D4_B4D3_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoSelEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AutoSelEdit window

class AutoSelEdit : public CEdit
{
// Construction
public:
	AutoSelEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AutoSelEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~AutoSelEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(AutoSelEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOSELEDIT_H__EFED528E_51A4_11D4_B4D3_009027BB3286__INCLUDED_)
