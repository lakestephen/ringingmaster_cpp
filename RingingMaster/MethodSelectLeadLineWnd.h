
#pragma once


#include "MethodWnd.h"
#include "MethodSelectLeadLine.h" 
#include "MethodPosition.h"



class MethodSelectLeadLineWnd : public MethodWnd  
{
public:
	MethodSelectLeadLineWnd();
	virtual ~MethodSelectLeadLineWnd();

	DECLARE_DYNCREATE(MethodSelectLeadLineWnd)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodSelectLeadLineWnd)
	public:
	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, MethodSelectLeadLine * method) ;
	//}}AFX_VIRTUAL

// Implementation
public:


protected:
	// todo MethodPosition& getSelectedPosition();
	UINT getContectMenuId();

	bool _selecting;
	void drawLeadLines(CDC *pDC); //overriding base class
	void invertLeadLine();
	void drawHilightRow(CDC *pDC);

	//{{AFX_MSG(MethodSelectLeadLineWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

