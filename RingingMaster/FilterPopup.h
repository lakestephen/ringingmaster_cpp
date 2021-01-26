#if !defined(AFX_FILTERPOPUP_H__2F77A713_952A_4F0A_9816_A6323FC46B19__INCLUDED_)
#define AFX_FILTERPOPUP_H__2F77A713_952A_4F0A_9816_A6323FC46B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterPopup.h : header file
//
#include "AutoPopup.h"
#include "FilterPopupCombo.h"
#include "RecordManagerEventListener.h"

/////////////////////////////////////////////////////////////////////////////
// FilterPopup window

class FilterPopup  : public AutoPopup	, public RecordManagerEventListener
{
// Construction
public:
	FilterPopup();

// Attributes
public:
	virtual BOOL Create(CWnd* pParentWnd);
    void Set(CPoint point);


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FilterPopup)
	//}}AFX_VIRTUAL

// Implementation
public:

	void recordManager_updateFilterList();

	virtual ~FilterPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(FilterPopup)
	afx_msg void OnSelchangeOptionsFilterCombo();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	FilterPopupCombo _comboFilter;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERPOPUP_H__2F77A713_952A_4F0A_9816_A6323FC46B19__INCLUDED_)
