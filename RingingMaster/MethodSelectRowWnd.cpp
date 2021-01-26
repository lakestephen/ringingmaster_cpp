// MethodSelectRowWnd.cpp: implementation of the MethodSelectRowWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodSelectRowWnd.h"	 
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

IMPLEMENT_DYNCREATE(MethodSelectRowWnd, MethodWnd)

MethodSelectRowWnd::MethodSelectRowWnd() :
MethodWnd(ds_methodPreview),
_selecting(false)
{


}

MethodSelectRowWnd::~MethodSelectRowWnd()
{
   	RegArchive ar(CArchive::store, "Style", "MethodSelectWnd");
	_style.Serialize(ar);
}


BEGIN_MESSAGE_MAP(MethodSelectRowWnd, MethodWnd)
	//{{AFX_MSG_MAP(MethodSelectRowWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL MethodSelectRowWnd::Create( DWORD dwExStyle, 
							   LPCTSTR lpszWindowName, 
							   DWORD dwStyle, 
							   const RECT& rect, 
							   CWnd* pParentWnd, 
							   UINT nID, 
							   MethodSelectRow * method) 
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
		RegArchive ar(CArchive::load, "Style", "MethodSelectWnd");
		
		if (ar.isValid()) _style.Serialize(ar);					

		recalcLeadOrigins();
		recalcLeadBoxes();	
	}
	return ret;
}

void MethodSelectRowWnd::drawHilightRow(CDC * /*pDC*/)
{
}

void MethodSelectRowWnd::invertRow()
{
	if (!getSelectedPosition().isValid()) return;

	bool valid = true;
	if (getCallPositions())
	{
		valid = false;
		for (int i=0;i<getCallPositions()->GetSize();i++)
		{
			if (getSelectedPosition().getLeadRow() == getCallPositions()->GetAt(i))
			{
				valid = true;
			}
		}
	}
	
	if (!valid) 
	{
		getSelectedPosition().setInvalid();
		return;
	}

	CDC* pDC = GetDC();

	CSize orig = getLeadOrigin(getSelectedPosition().getLead());
	orig = orig - GetScrollPosition();

	CRect rcRow(orig.cx - (_style._width/2)                          + 1,
				orig.cy + (_style._height*getSelectedPosition().getLeadRow())- (_style._height/2),
				orig.cx + (_style._width * (_method->getNumber()-1)) + (_style._width /2),
				orig.cy + (_style._height*getSelectedPosition().getLeadRow())+ (_style._height/2) -1);

	pDC->InvertRect(rcRow);

	ReleaseDC(pDC);
}

void MethodSelectRowWnd::drawFalseRows(CDC *pDC)
{
	if (!getSelectedPosition().isValid() ) return;

	CSize orig = getLeadOrigin(getSelectedPosition().getLead());
	CRect rcRow(orig.cx - (_style._width/2)                          + 1,
				orig.cy + (_style._height*getSelectedPosition().getLeadRow())- (_style._height/2),
				orig.cx + (_style._width * (_method->getNumber()-1)) + (_style._width /2),
				orig.cy + (_style._height*getSelectedPosition().getLeadRow())+ (_style._height/2) -1);

	pDC->FillSolidRect(rcRow, _style._falseRowsColor);
}



void MethodSelectRowWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (_selecting) 
	{
		invertRow();

		if (rowHitTest(point + GetScrollPosition(), &getSelectedPosition()))
		{
			invertRow();
		}
	}	

	MethodWnd::OnMouseMove(nFlags, point);
}

void MethodSelectRowWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (rowHitTest(point + GetScrollPosition(), &getSelectedPosition()))
	{
		_selecting = true;
		invertRow();
	}
	
	ScrollWnd::OnLButtonDown(nFlags, point);
	//the scroll wnd version called to avoid the calculations and invalidations done in MethodWnd
}

void MethodSelectRowWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (_selecting) 
	{
		invertRow();
		_selecting = false;
		Invalidate();
		SetTimer(AAA_TIMER_END_METHOD_WND, 350, NULL);
	}
	
	MethodWnd::OnLButtonUp(nFlags, point);
}

void MethodSelectRowWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == AAA_TIMER_END_METHOD_WND)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_METHOD_WND_TERMINATE,0,0); //was m_hWnd
		KillTimer(AAA_TIMER_END_METHOD_WND);
	}
	
	MethodWnd::OnTimer(nIDEvent);
}

UINT MethodSelectRowWnd::getContectMenuId()
{
	return IDR_METHOD_STYLE_DEFAULT;
}

LRESULT MethodSelectRowWnd::OnNcHitTest(CPoint point) 
{
	//used to disable the hit test to disable the resizing as it interfears with the selecting of rows
	return ScrollWnd::OnNcHitTest(point);
}

MethodPosition& MethodSelectRowWnd::getSelectedPosition()
{
	ASSERT(dynamic_cast<MethodSelect*>(_method));
	return (dynamic_cast<MethodSelect*>(_method))->_selectedPosition;
}

Ints* MethodSelectRowWnd::getCallPositions()
{
	ASSERT(dynamic_cast<MethodSelectRow*>(_method));
	return (dynamic_cast<MethodSelectRow*>(_method))->getCallPositions();
}
