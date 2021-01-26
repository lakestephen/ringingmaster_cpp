// MethodCallChangeWnd.h: interface for the MethodCallChangeWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METHODCALLCHANGEWND_H__F1D4F14E_C965_11D6_B60F_000255162CF0__INCLUDED_)
#define AFX_METHODCALLCHANGEWND_H__F1D4F14E_C965_11D6_B60F_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MethodWnd.h"
#include "MethodCallChange.h"
#include "StdAfx.h"	// Added by ClassView


class MethodCallChangeWnd : public MethodWnd  
{
public:
	MethodCallChangeWnd();
	virtual ~MethodCallChangeWnd();

	DECLARE_DYNCREATE(MethodCallChangeWnd)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodCallChangeWnd)
	public:
	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, MethodCallChange * method) ;
	//}}AFX_VIRTUAL

// Implementation
public:
	CSize getSize();


protected:
	int _commentsWidth;
	int getTabStops(CDC *pDC);
	void getStrings(CStringArray& strings, CString str);
	UINT getContectMenuId();

	void doToolsStyle() ;
	void drawComments(CDC *pDC);


	//{{AFX_MSG(MethodCallChangeWnd)
	afx_msg void OnCCOptions();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};


#endif // !defined(AFX_METHODCALLCHANGEWND_H__F1D4F14E_C965_11D6_B60F_000255162CF0__INCLUDED_)
