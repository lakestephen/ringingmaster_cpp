// MethodWndPrintCrib.cpp: implementation of the MethodWndPrintCrib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodWndPrintCrib.h"
#include "CribDoc.h"
#include "Crib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MethodWndPrintCrib::MethodWndPrintCrib(CDC* pDC, // do not hold on to this under any circumstances
									   MethodCrib* method, 
									   const Style& style, 
									   float scaleX, 
									   float scaleY,
									   int pageX,
									   int pageY,
									   int leftGap,
									   int topGap,
									   BOOL wholeColumns):
MethodWndPrint(pDC, // do not hold on to this under any circumstances
			   dynamic_cast<Method*>(method), 
			   style, 
			   scaleX, 
			   scaleY,
			   pageX,
			   pageY,
			   leftGap,
			   topGap,
			   wholeColumns)
{

}

MethodWndPrintCrib::~MethodWndPrintCrib()
{

}

void MethodWndPrintCrib::OnDraw(CDC* pDC, int pageNumber)
{
	MethodWndPrint::OnDraw(pDC, pageNumber);

	MethodCrib* methodCrib = dynamic_cast<MethodCrib*>(_method);
	
	ASSERT(methodCrib);
	if (!methodCrib)
		return;
	
	drawCrib(pDC, methodCrib);
	drawNotes(pDC, methodCrib);	
}

int MethodWndPrintCrib::getNumberOfHorzPages()
{
	return 1;
}

void MethodWndPrintCrib::drawCrib(CDC *pDC, MethodCrib* methodCrib)
{
	CribDoc* cribDoc = methodCrib->getDocument();

	ASSERT(cribDoc);

	if (cribDoc == NULL)
		return;


	pDC->SetMapMode(MM_TEXT);
	pDC->SetBkMode(TRANSPARENT);

	LOGFONT lf;

	CFont font;
	FontInfo scaledFont = cribDoc->getCribFont();
	scaledFont._size = scaleXY(scaledFont._size);
	scaledFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	CFont* oldFont = pDC->SelectObject(&font);	 
		
	COLORREF oldColor = pDC->SetTextColor(cribDoc->getCribFont()._color);

	Crib* crib = methodCrib->getCrib();
	if (crib)
	{
		CString strCrib = crib->getDescription();
		pDC->DrawText(strCrib,  getCribRect(pDC), DT_WORDBREAK| DT_NOPREFIX);
	}

	pDC->SetTextColor(oldColor);
	pDC->SelectObject(oldFont);		 
}

CRect MethodWndPrintCrib::getCribRect(CDC* pDC)
{
	CRect methodRect = getMethodRect();

	//now calculate the crib box
	CRect cribRect(methodRect.right, 
					methodRect.top, 
					_pageX, 
					methodRect.top + 200);

	//use drawrect to calculate the rect required
	MethodCrib* methodCrib = dynamic_cast<MethodCrib*>(_method);
	CribDoc* cribDoc = methodCrib->getDocument();

//	cribRect = translateRect(cribRect);

	if ((methodCrib)&&(cribDoc))
	{
		pDC->SetMapMode(MM_TEXT);
		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;

		CFont font;
		FontInfo scaledFont = cribDoc->getCribFont();
		scaledFont._size = scaleXY(scaledFont._size);
		scaledFont.getLogFont(&lf);
		font.CreateFontIndirect(&lf);
	    CFont* oldFont = pDC->SelectObject(&font);	 

		Crib* crib = methodCrib->getCrib();
		if (crib)
		{
			CString strCrib = crib->getDescription();
			pDC->DrawText(strCrib,  cribRect, DT_WORDBREAK|DT_CALCRECT| DT_NOPREFIX);
		}
		pDC->SelectObject(oldFont);		 
	}	

	return cribRect;
}

CRect MethodWndPrintCrib::getMethodRect()
{
	CRect rect (0,0,0,0);
/*	todo
	if ((_method)&&(_method->GetSize()>0))
	{
		rect = getLeadBox(0);
		
	}
  */
	return translateRect(rect);
}

void MethodWndPrintCrib::drawNotes(CDC *pDC, MethodCrib* methodCrib)
{
	CribDoc* cribDoc = methodCrib->getDocument();

	ASSERT(cribDoc);

	if (cribDoc == NULL)
		return;

	if (!cribDoc->getShowNotes())
		return;


	pDC->SetMapMode(MM_TEXT);
	pDC->SetBkMode(TRANSPARENT);

	LOGFONT lf;

	CFont font;
	FontInfo scaledFont = cribDoc->getCribNotesFont();
	scaledFont._size = scaleXY(scaledFont._size);
	scaledFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	CFont* oldFont = pDC->SelectObject(&font);	 
		
	COLORREF oldColor = pDC->SetTextColor(cribDoc->getCribNotesFont()._color);

	pDC->DrawText(cribDoc->getNotes(),  getNotesRect(pDC), DT_WORDBREAK| DT_NOPREFIX);

	pDC->SetTextColor(oldColor);
	pDC->SelectObject(oldFont);		 
}


CRect MethodWndPrintCrib::getNotesRect(CDC* pDC)
{
	CRect methodRect = getMethodRect();
	CRect cribRect = getCribRect(pDC);

	//now calculate the crib box
	int top = max(methodRect.bottom, cribRect.bottom) + 20;
	CRect notesRect(methodRect.left, 
					top, 
					_pageX, 
					top + 200);

	//use drawrect to calculate the rect required
	MethodCrib* methodCrib = dynamic_cast<MethodCrib*>(_method);
	CribDoc* cribDoc = methodCrib->getDocument();

//	notesRect = translateRect(cribRect);

	if ((methodCrib)&&(cribDoc))
	{
		pDC->SetMapMode(MM_TEXT);
		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;

		CFont font;
		FontInfo scaledFont = cribDoc->getCribNotesFont();
		scaledFont._size = scaleXY(scaledFont._size);
		scaledFont.getLogFont(&lf);
		font.CreateFontIndirect(&lf);
	    CFont* oldFont = pDC->SelectObject(&font);	 

		pDC->DrawText(cribDoc->getNotes(),  cribRect, DT_WORDBREAK|DT_CALCRECT| DT_NOPREFIX);

		pDC->SelectObject(oldFont);		 
	}	

	return notesRect;
}


