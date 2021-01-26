#if !defined(AFX_AUTOPOPUP_H__F5B3A956_7759_11D5_B607_009027BB3286__INCLUDED_)
#define AFX_AUTOPOPUP_H__F5B3A956_7759_11D5_B607_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoPopup.h : header file
//

#include "UserMessages.h"

/////////////////////////////////////////////////////////////////////////////
// AutoPopup window

class AutoPopup : public CWnd
{
// Construction
public:
	AutoPopup();

// Attributes
public:
	virtual BOOL Create(CWnd* pParentWnd);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AutoPopup)

	//}}AFX_VIRTUAL

// Implementation
public:
	static void CleanUp();
	virtual ~AutoPopup();

	

// Generated message map functions
protected:
	BOOL _notifyParentDone;
	//{{AFX_MSG(AutoPopup)
		afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
		afx_msg LRESULT OnKillPopup(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void notifyParent(CWnd* parent);
    static void RegisterWnd( );
    static BOOL _registered;

private:
	CWnd*   _parent;
	static AutoPopup* _autoPopup;
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOPOPUP_H__F5B3A956_7759_11D5_B607_009027BB3286__INCLUDED_)
