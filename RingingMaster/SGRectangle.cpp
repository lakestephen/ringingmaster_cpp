// SGRectangle.cpp: implementation of the SGRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SGRectangle.h"
#include "SuperGridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// Simple class SGRectangle for the + - sign, 
//
//////////////////////////////////////////////////////////////////////////

SGRectangle::SGRectangle(SuperGridCtrl* pCtrl, CDC* pDC, int iIndent, const CRect& rcBounds)
{
	m_pDC=pDC;
	int nHalfImage = (pCtrl->m_cxImage >> 1);
	int nBottomDown = rcBounds.top + nHalfImage + ((rcBounds.Height() - pCtrl->m_cyImage) >> 1);
	m_right_bottom.cx = (pCtrl->m_cxImage>>1)+2+1;
	m_right_bottom.cy = (pCtrl->m_cyImage>>1)+2+1;
	m_left = rcBounds.left  + iIndent * pCtrl->m_cxImage - nHalfImage;
	m_top = nBottomDown - (m_right_bottom.cy >> 1);
	m_left_top.x = m_left -  (m_right_bottom.cx >> 1);
	m_left_top.y = m_top;
	m_topdown = nBottomDown;
}

	
void SGRectangle::DrawRectangle(SuperGridCtrl* pCtrl)
{
	//erase bkgrnd
	CRect rc(m_left_top, m_right_bottom);
	m_pDC->FillRect(rc, &pCtrl->m_brushErase);
	//draw rectangle	
	CPen* pPenRectangle = m_pDC->SelectObject(&pCtrl->m_psRectangle);
	m_pDC->Rectangle(rc);
	m_pDC->SelectObject(pPenRectangle);		
}


SGRectangle::~SGRectangle()
{
}


BOOL SGRectangle::HitTest(CPoint pt)
{
	CRect rc = GetHitTestRect();
	return rc.PtInRect(pt);
}


void SGRectangle::DrawPlus(void)
{
	m_pDC->MoveTo(m_left, m_topdown-2);
	m_pDC->LineTo(m_left, m_topdown+3);

	m_pDC->MoveTo(m_left-2, m_topdown);
	m_pDC->LineTo(m_left+3, m_topdown);
}


void SGRectangle::DrawMinus(void)
{
	m_pDC->MoveTo(m_left-2, m_topdown);
	m_pDC->LineTo(m_left+3, m_topdown);
}




CRect SGRectangle::GetHitTestRect(void) 
{ 
	return CRect(m_left_top, m_right_bottom);
}
