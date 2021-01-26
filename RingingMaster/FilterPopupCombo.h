#if !defined(AFX_FILTERPOPUPCOMBO_H__7B306F1D_0E04_4BFB_8B8C_C1411611C029__INCLUDED_)
#define AFX_FILTERPOPUPCOMBO_H__7B306F1D_0E04_4BFB_8B8C_C1411611C029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterPopupCombo.h : header file
//

#include "FilterCombo.h"

/////////////////////////////////////////////////////////////////////////////
// FilterPopupCombo window

class FilterPopupCombo : public FilterCombo
{
// Construction
public:
	FilterPopupCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FilterPopupCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~FilterPopupCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(FilterPopupCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	CFont _font;
	CWnd* _pParent;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERPOPUPCOMBO_H__7B306F1D_0E04_4BFB_8B8C_C1411611C029__INCLUDED_)
