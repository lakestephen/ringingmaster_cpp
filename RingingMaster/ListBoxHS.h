#if !defined(AFX_LISTBOXHS_H__D65C3997_C7A0_11D5_B50F_000255162CF0__INCLUDED_)
#define AFX_LISTBOXHS_H__D65C3997_C7A0_11D5_B50F_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListBoxHS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ListBoxHS window

class ListBoxHS : public CListBox
{
// Construction
public:
	ListBoxHS();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ListBoxHS)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ResetContentHS();
	int AddStringHS(CString string);
	virtual ~ListBoxHS();

	// Generated message map functions
protected:
	//{{AFX_MSG(ListBoxHS)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXHS_H__D65C3997_C7A0_11D5_B50F_000255162CF0__INCLUDED_)
