// PopupList.h: interface for the PopupList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPUPLIST_H__0BBD00E2_77CB_11D5_BB7F_A0C33C4F3179__INCLUDED_)
#define AFX_POPUPLIST_H__0BBD00E2_77CB_11D5_BB7F_A0C33C4F3179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorListBox.h"

class PopupList : public ColorListBox  
{
public:
	PopupList();
	virtual ~PopupList();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PopupList)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(PopupList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG


	CFont _font;
	CWnd* _pParent;

	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_POPUPLIST_H__0BBD00E2_77CB_11D5_BB7F_A0C33C4F3179__INCLUDED_)
