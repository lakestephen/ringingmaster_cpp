// PopupWnd.h: interface for the PopupWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPUPWND_H__0BBD00E1_77CB_11D5_BB7F_A0C33C4F3179__INCLUDED_)
#define AFX_POPUPWND_H__0BBD00E1_77CB_11D5_BB7F_A0C33C4F3179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoPopup.h"
#include "PopupList.h"

class PopupWnd : public AutoPopup
{
// Construction
public:
	PopupWnd();

// Attributes
public:
	virtual BOOL Create(CWnd* pParentWnd);
    void Set(CPoint point, StringColors* strArr, int selection = -1);

 // class operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PopupWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PopupWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(PopupWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void notifyParent(CWnd* parent);

protected:
	PopupList _listBox;

};

#endif // !defined(AFX_POPUPWND_H__0BBD00E1_77CB_11D5_BB7F_A0C33C4F3179__INCLUDED_)
