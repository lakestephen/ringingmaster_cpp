// MethodWndPrintCrib.h: interface for the MethodWndPrintCrib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METHODWNDPRINTCRIB_H__91A92B45_798A_4E9E_8F56_B30892418ED2__INCLUDED_)
#define AFX_METHODWNDPRINTCRIB_H__91A92B45_798A_4E9E_8F56_B30892418ED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MethodWndPrint.h"
#include "MethodCrib.h"

class MethodWndPrintCrib : public MethodWndPrint  
{
public:
	MethodWndPrintCrib(CDC* pDC, MethodCrib* method, const Style& style, 
					float scaleX, float scaleY,
				   int pageWidth, int pageHeight,
				   int leftGap, int topGap, 
				   BOOL wholeColumns);

	virtual ~MethodWndPrintCrib();

	virtual void OnDraw(CDC* pDC, int pageNumber);

protected:
	CRect getMethodRect();
	CRect getCribRect(CDC* pDC);
	CRect getNotesRect(CDC* pDC);
	int getNumberOfHorzPages();
	void drawCrib(CDC *pDC, MethodCrib* methodCrib);
	void drawNotes(CDC *pDC, MethodCrib* methodCrib);
};

#endif // !defined(AFX_METHODWNDPRINTCRIB_H__91A92B45_798A_4E9E_8F56_B30892418ED2__INCLUDED_)
