// MethodPreviewWnd.cpp: implementation of the MethodPreviewWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodPreviewWnd.h"
#include "MethodPosition.h"
#include "Lead.h"
#include "StyleEditDlg.h"
#include "RegArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(MethodPreviewWnd, MethodWnd)

MethodPreviewWnd::MethodPreviewWnd() :
MethodWnd(ds_methodPreview)
{

}

MethodPreviewWnd::~MethodPreviewWnd()
{
   	RegArchive ar(CArchive::store, "Style", "MethodPreviewWnd");
	_style.Serialize(ar);
}

BEGIN_MESSAGE_MAP(MethodPreviewWnd, MethodWnd)
	//{{AFX_MSG_MAP(MethodPreviewWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL MethodPreviewWnd::Create( DWORD dwExStyle, 
							   LPCTSTR lpszWindowName, 
							   DWORD dwStyle, 
							   const RECT& rect, 
							   CWnd* pParentWnd, 
							   UINT nID, 
							   MethodPreview * method) 
{
	BOOL ret = MethodWnd::Create(	dwExStyle, 
					 			lpszWindowName, 
					 			dwStyle, 
								rect, 
								pParentWnd, 
								nID, 
								method);
	if (ret)
	{
		//get the style for this window from the registry
		RegArchive ar(CArchive::load, "Style", "MethodPreviewWnd");
		
		if (ar.isValid()) _style.Serialize(ar);					

		recalcLeadOrigins();
		recalcLeadBoxes();		
	}
	return ret;
}

void MethodPreviewWnd::OnDraw(CDC *pDC)
{
	//draw the base class
	MethodWnd::OnDraw(pDC);
/* todo	MethodPreview * methodPrev = dynamic_cast<MethodPreview*>(_method);
	
	ASSERT(methodPrev);
	if (!methodPrev) return;

	drawCallTitles(pDC);

	for (int i=0;i<methodPrev->_callPreviews.GetSize();i++)
	{
		drawNotation(pDC, &methodPrev->_callPreviews.GetAt(i)->_notationStrings, 
						methodPrev->GetSize() - methodPrev->_callPreviews.GetSize() + i);
	}
	 */

}

bool MethodPreviewWnd::rowHitTest(const CPoint & point, MethodPosition * pos, bool forStatus)
{
	bool val = MethodWnd::rowHitTest(point,pos);

	if (!val)
		return false;
	
/*	todo MethodPreview * methodPrev = dynamic_cast<MethodPreview*>(_method);
	
	ASSERT(methodPrev);
	if (!methodPrev) return val;

	if ((forStatus)&&
		(pos->getLead() >= (methodPrev->GetSize() - methodPrev->_callPreviews.GetSize())))
		return false;
*/	
	return val;
}

void MethodPreviewWnd::drawStarts(CDC *pDC, int /*max*/)
{
/*	todo MethodPreview * methodPrev = dynamic_cast<MethodPreview*>(_method);
	
	ASSERT(methodPrev);
	if (!methodPrev) return;
	
	MethodWnd::drawStarts(pDC, (methodPrev->GetSize() - methodPrev->_callPreviews.GetSize()));*/
}

void MethodPreviewWnd::recalcLeadOrigins()
{
	//do default calculation. 
	MethodWnd::recalcLeadOrigins();
			/*
	CRect box;
	CSize origin;

	MethodPreview * methodPrev = dynamic_cast<MethodPreview*>(_method);
	
	ASSERT(methodPrev);
	if (!methodPrev) return;

	//now adjust the call rectangles.
	int firstCall = methodPrev->GetSize() - methodPrev->_callPreviews.GetSize();
	
	//remove the origins
	_leadOrigins.RemoveAt(firstCall,methodPrev->_callPreviews.GetSize());
	
	//now add them back in again
	for (int i=0;i<methodPrev->_callPreviews.GetSize();i++)
	{
		//origins
		origin.cx = getLeadOrigin(_leadOrigins.GetUpperBound()).cx + 
					_style._left + 
					(_style._width * (_method->getNumber()-1)) + 20;
		origin.cy = _style.getTop();
		
		_leadOrigins.Add(origin);
	}

	//now re adjust all the boxes
	recalcLeadBoxes();
		*/ //todo
}


void MethodPreviewWnd::drawCallTitles(CDC *pDC)
{  

/*	MethodPreview * methodPrev = dynamic_cast<MethodPreview*>(_method);
	
	ASSERT(methodPrev);
	if (!methodPrev) return;

	//create the font
	CFont title, method;
	int top = 0;
	int left = 0;
	TEXTMETRIC tm;
	LOGFONT lf;
	
	//setup title font
	FontInfo titleInfo = _style._titleFont;
	if (titleInfo._size > 100) titleInfo._size -= 100; 
	titleInfo.getLogFont(&lf);
	title.CreateFontIndirect(&lf);

	CFont * oldFont = pDC->SelectObject(&title);

	int firstCall = methodPrev->GetSize() - methodPrev->_callPreviews.GetSize();

	for (int i=0;i<methodPrev->_callPreviews.GetSize();i++)
	{
		//get title string
		CString strTitle = methodPrev->_callPreviews.GetAt(i)->_name;
		//get notation string
		CString strNotation = "(" + methodPrev->_callPreviews.GetAt(i)->_notation + ")";

		//set up DC
		pDC->SetTextAlign(TA_LEFT);
		pDC->SetTextColor(_style._titleFont._color);

		left = getLeadOrigin(firstCall + i).cx;
		top = 10;

		//write the title text out
		pDC->SelectObject(&title);
		if (_style._titleShow)
		{
			//height += 5;
			pDC->GetTextMetrics(&tm);
			pDC->TextOut(left, top, strTitle);
			top += tm.tmHeight;
		}

	}
	//clean up
	pDC->SelectObject(oldFont);
			  */ //todo
}


void MethodPreviewWnd::doToolsStyle() 
{
	StyleEditDlg styleEditDlg(_style, this, _method->getNumber(), slt_preview);
	styleEditDlg.DoModal();
	notifyStyleChange();
	SetScrolls();
}


UINT MethodPreviewWnd::getContectMenuId()
{
	return IDR_METHOD_STYLE_DEFAULT;
}
