// MethodSelectLeadLineWnd.cpp: implementation of the MethodSelectLeadLineWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodSelectLeadLineWnd.h"
#include "Lead.h"
#include "UserMessages.h"
#include "RegArchive.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(MethodSelectLeadLineWnd, MethodWnd)

MethodSelectLeadLineWnd::MethodSelectLeadLineWnd():
MethodWnd(ds_methodPreview),
_selecting(false)
{

}

MethodSelectLeadLineWnd::~MethodSelectLeadLineWnd()
{
   	RegArchive ar(CArchive::store, "Style", "MethodSelectWnd");
	_style.Serialize(ar);
}

BEGIN_MESSAGE_MAP(MethodSelectLeadLineWnd, MethodWnd)
	//{{AFX_MSG_MAP(MethodSelectLeadLineWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL MethodSelectLeadLineWnd::Create( DWORD dwExStyle, 
							   LPCTSTR lpszWindowName, 
							   DWORD dwStyle, 
							   const RECT& rect, 
							   CWnd* pParentWnd, 
							   UINT nID, 
							   MethodSelectLeadLine * method) 
{
	BOOL ret = MethodWnd::Create(dwExStyle, 
					 			lpszWindowName, 
					 			dwStyle, 
								rect, 
								pParentWnd, 
								nID, 
								method);
	if (ret)
	{
		//get the style for this window from the registry
		RegArchive ar(CArchive::load, "Style", "MethodSelectWnd");
		
		if (ar.isValid()) _style.Serialize(ar);	
		
		recalcLeadOrigins();
		recalcLeadBoxes();	
	}
	return ret;
}

void MethodSelectLeadLineWnd::invertLeadLine()
{
   /* todo
	if (!getSelectedPosition().isValid()) return;
	if (getSelectedPosition().getLeadLine() == _method->GetAt(0)->GetUpperBound())
	{
		getSelectedPosition().setInvalid();
		return;
	}

	CDC* pDC = GetDC();

	CPen pen(PS_SOLID, _style._gridWidthH, _style._gridColorH);
	CPen * oldPen = pDC->SelectObject(&pen);

	int OldMode = pDC->SetROP2(R2_NOT);

	CSize orig = getLeadOrigin(getSelectedPosition().getLead());
	orig = orig - GetScrollPosition();
	int vertPos = orig.cy + (_style._height * getSelectedPosition().getLeadLine()) + _style._height/2;

	pDC->MoveTo(orig.cx - (_style._width/2), vertPos);
	pDC->LineTo(orig.cx + ((_method->getNumber()-1) * _style._width)
						+ (_style._width/2), vertPos);
	pDC->SetROP2(OldMode);	  

	pDC->SelectObject(oldPen);

	ReleaseDC(pDC);
	*/
}

//overriding base class
void MethodSelectLeadLineWnd::drawLeadLines(CDC *pDC)
{
/*	if (!getSelectedPosition().isValid()) return;

	CPen pen(PS_SOLID, _style._gridWidthH, _style._gridColorH);
	CPen * oldPen = pDC->SelectObject(&pen);

	CSize orig = getLeadOrigin(getSelectedPosition().getLead());
	int vertPos = orig.cy + (_style._height * getSelectedPosition().getLeadLine()) + _style._height/2;

	pDC->MoveTo(orig.cx - (_style._width/2), vertPos);
	pDC->LineTo(orig.cx + ((_method->getNumber()-1) * _style._width)
						+ (_style._width/2), vertPos);

	pDC->SelectObject(oldPen);	*/ //todo

}

void MethodSelectLeadLineWnd::drawHilightRow(CDC * /*pDC*/)
{
}

void MethodSelectLeadLineWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (_selecting)
	{
		invertLeadLine();
/*
		if (rowHitTest(point + GetScrollPosition(), &getSelectedPosition()))
		{
			invertLeadLine();
		}
		*/ //todo
	}

	MethodWnd::OnMouseMove(nFlags, point);
}

void MethodSelectLeadLineWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
/*	if (rowHitTest(point + GetScrollPosition(), &getSelectedPosition()))
	{
		_selecting= true;
		invertLeadLine();
	}
*/ //todo	
	ScrollWnd::OnLButtonDown(nFlags, point);
	//the scroll wnd version called to avoid the calculations and invalidations done in MethodWnd
}

void MethodSelectLeadLineWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (_selecting) 
	{
		invertLeadLine();
		_selecting = false;
		Invalidate();
		SetTimer(AAA_TIMER_END_METHOD_WND, 350, NULL);
	}
	
	MethodWnd::OnLButtonUp(nFlags, point);
}

void MethodSelectLeadLineWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == AAA_TIMER_END_METHOD_WND)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_METHOD_WND_TERMINATE,0,0); //was m_hWnd
		KillTimer(AAA_TIMER_END_METHOD_WND);
	}	

	MethodWnd::OnTimer(nIDEvent);
}

UINT MethodSelectLeadLineWnd::getContectMenuId()
{ 
	return IDR_METHOD_STYLE_DEFAULT;
}

LRESULT MethodSelectLeadLineWnd::OnNcHitTest(CPoint point) 
{
	//used to disable the hit test to disable the resizing as it interfears with the selecting of rows
	return ScrollWnd::OnNcHitTest(point);
}
	 /* todo
MethodPosition& MethodSelectLeadLineWnd::getSelectedPosition()
{
	ASSERT(dynamic_cast<MethodSelect*>(_method));
	return (dynamic_cast<MethodSelect*>(_method))->_selectedPosition;
}
*/