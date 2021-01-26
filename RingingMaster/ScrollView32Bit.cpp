// 32BitScrollView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "ScrollView32Bit.h"
#include ".\scrollview32bit.h"


// C32BitScrollView

IMPLEMENT_DYNCREATE(CScrollView32Bit, CScrollView)

CScrollView32Bit::CScrollView32Bit()
{
}

CScrollView32Bit::~CScrollView32Bit()
{
}


BEGIN_MESSAGE_MAP(CScrollView32Bit, CScrollView)
END_MESSAGE_MAP()



void CScrollView32Bit::OnDraw(CDC* /*pDC*/)
{
	
}

BOOL CScrollView32Bit::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
    // force process SB_THUMBPOSITION since
    // it is NOT processed by default
    if((LOBYTE( nScrollCode ) == SB_THUMBPOSITION ) ||
		(LOBYTE( nScrollCode ) == SB_THUMBTRACK))
    {
        nScrollCode = MAKEWORD( SB_THUMBTRACK, HIBYTE( nScrollCode ));

        SCROLLINFO infoScroll;
        VERIFY( GetScrollInfo( SB_HORZ, &infoScroll, SIF_TRACKPOS ));

        nPos = infoScroll.nTrackPos; // Override with true 32-bit value
    }

    if((HIBYTE( nScrollCode ) == SB_THUMBPOSITION ) ||
		(HIBYTE( nScrollCode ) == SB_THUMBTRACK))
    {
        nScrollCode = MAKEWORD( LOBYTE( nScrollCode ), SB_THUMBTRACK );

        SCROLLINFO infoScroll;
        VERIFY( GetScrollInfo( SB_VERT, &infoScroll, SIF_TRACKPOS ));

        nPos = infoScroll.nTrackPos; // Override with true 32-bit value
    }

    return CScrollView::OnScroll( nScrollCode, nPos, bDoScroll );
}