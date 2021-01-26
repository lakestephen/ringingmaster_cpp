// MethodPreviewWnd.h: interface for the MethodPreviewWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METHODPREVIEWWND_H__06713381_3267_11D5_BB7F_444553540000__INCLUDED_)
#define AFX_METHODPREVIEWWND_H__06713381_3267_11D5_BB7F_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MethodWnd.h"
#include "MethodPreview.h"


class MethodPreviewWnd : public MethodWnd  
{
public:
	MethodPreviewWnd();
	virtual ~MethodPreviewWnd();
	void recalcLeadOrigins();
	bool rowHitTest(const CPoint & point, MethodPosition * pos, bool forStatus = false);

	DECLARE_DYNCREATE(MethodPreviewWnd)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodPreviewWnd)
	public:
	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, MethodPreview * method) ;
	//}}AFX_VIRTUAL

// Implementation
public:


protected:
	UINT getContectMenuId();
	void OnDraw(CDC* pDC);
	void drawStarts(CDC *pDC, int max);
	void drawCallTitles(CDC *pDC);

	void doToolsStyle() ;


	//{{AFX_MSG(MethodPreviewWnd)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_METHODPREVIEWWND_H__06713381_3267_11D5_BB7F_444553540000__INCLUDED_)
