// MethodSelectRowWnd.h: interface for the MethodSelectRowWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METHODSELECTROWWND_H__06713383_3267_11D5_BB7F_444553540000__INCLUDED_)
#define AFX_METHODSELECTROWWND_H__06713383_3267_11D5_BB7F_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MethodWnd.h"
#include "MethodSelectRow.h"
#include "MethodPosition.h"

class MethodSelectRowWnd : public MethodWnd  
{
public:
	MethodSelectRowWnd();
	virtual ~MethodSelectRowWnd();

	DECLARE_DYNCREATE(MethodSelectRowWnd)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodSelectRowWnd)
	public:
	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, MethodSelectRow * method) ;
	//}}AFX_VIRTUAL

// Implementation

protected:		 
	UINT getContectMenuId();
	MethodPosition& getSelectedPosition();
	Ints* getCallPositions();


	bool _selecting;
	void drawHilightRow(CDC *pDC);
	
	void drawFalseRows(CDC *pDC);
	void invertRow();


	//{{AFX_MSG(MethodSelectRowWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_METHODSELECTROWWND_H__06713383_3267_11D5_BB7F_444553540000__INCLUDED_)
