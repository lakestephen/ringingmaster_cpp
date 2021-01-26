#if !defined(AFX_TOWERPOPUPCOMBO_H__38166B48_7C1A_11D5_B60D_009027BB3286__INCLUDED_)
#define AFX_TOWERPOPUPCOMBO_H__38166B48_7C1A_11D5_B60D_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TowerpopupCombo.h : header file
//

#include "CJFlatComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// TowerpopupCombo window

class TowerpopupCombo : public CCJFlatComboBox
{
// Construction
public:
	TowerpopupCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerpopupCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TowerpopupCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(TowerpopupCombo)
	//}}AFX_MSG
				  
	
	CFont _font;
	CWnd* _pParent;


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOWERPOPUPCOMBO_H__38166B48_7C1A_11D5_B60D_009027BB3286__INCLUDED_)
