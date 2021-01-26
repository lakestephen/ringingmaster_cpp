#if !defined(AFX_FILTERCOMBO_H__F6B4CE49_7A63_41B5_8B4F_2B9AA1ECC657__INCLUDED_)
#define AFX_FILTERCOMBO_H__F6B4CE49_7A63_41B5_8B4F_2B9AA1ECC657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterCombo.h : header file
//

#include "CJFlatComboBox.h"
#include "FilterComboBtn.h"


/////////////////////////////////////////////////////////////////////////////
// FilterCombo window

class FilterCombo : public CComboBox
{
// Construction
public:
	FilterCombo(FilterComboBtn* filterComboBtn);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FilterCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~FilterCombo();

	// Generated message map functions
protected:
	FilterComboBtn* _filterComboBtn;
		CImageList m_img;	
	//{{AFX_MSG(FilterCombo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERCOMBO_H__F6B4CE49_7A63_41B5_8B4F_2B9AA1ECC657__INCLUDED_)
