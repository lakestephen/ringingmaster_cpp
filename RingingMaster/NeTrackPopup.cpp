#include "stdafx.h"
#include "NeTrackPopup.h"
#include "NumericEdit.h"


// NeTrackPopup

#define THUMB_WIDTH 6 // Width of the track thumb

NeTrackPopup::NeTrackPopup():
_parent(NULL)
{
}


IMPLEMENT_DYNAMIC(NeTrackPopup, CWnd)

BEGIN_MESSAGE_MAP(NeTrackPopup, CWnd)
ON_WM_LBUTTONUP()
ON_WM_PAINT()
ON_WM_MOUSEMOVE()
ON_WM_SIZE()
END_MESSAGE_MAP()

//****************************************************************************
// NeTrackPopup::Create - Creates the actual popup window while saving
//                         nesassery values. Also calculates the x position of
//                         the window so that the thumb will line up with the
//                         mouse cursor. Finally call SetCapture()
//****************************************************************************

BOOL NeTrackPopup::Create(int x,int y,float nMin,float nMax,float nValue,NumericEdit* pParent)
{
	CString sClassName;

	_parent = pParent;
	
	m_nMin = nMin;
	m_nMax = nMax;
	m_nValue = nValue;
	
	int nSBHeight = GetSystemMetrics(SM_CYHSCROLL);
	
	sClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,::LoadCursor(NULL, IDC_SIZEWE),(HBRUSH)0);
	if(!CreateEx(0,sClassName,0,WS_POPUP|WS_VISIBLE|WS_DLGFRAME,x,y,400,nSBHeight,pParent->GetSafeHwnd(),(HMENU) 0))
	{
		TRACE0("Failed to create NumericEdit popup\n");
		return FALSE;
	}
	
	// Calculate x position
	int nDlgBorderWidth = GetSystemMetrics(SM_CXDLGFRAME);
	nValue-= nMin;
	x-= (int)((m_dPixelValue * nValue) + 0.5);
	x-= nDlgBorderWidth;
	SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	
	_parent->PopupStateChange(PUSC_OPENING);
	
	SetCapture();
	
	return TRUE;
}

// NeButton message handlers

//****************************************************************************
// NeTrackPopup::OnPaint - Calculates and paints the bar and thumb
//****************************************************************************

void NeTrackPopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcRect;
	GetClientRect(&rcRect);
	
	int nXPos = (int)((m_dPixelValue * (double)(m_nValue - m_nMin)));
	
	// Draw bar
	dc.FillSolidRect(rcRect.left,rcRect.top,nXPos,rcRect.Height(),GetSysColor(COLOR_ACTIVECAPTION));
	dc.FillSolidRect(rcRect.left + nXPos + 1,rcRect.top,rcRect.Width() - nXPos,rcRect.Height(),GetSysColor(COLOR_BTNFACE));
	
	// Draw thumb
	rcRect.left+= nXPos;
	rcRect.right = rcRect.left + THUMB_WIDTH;
	dc.FillSolidRect(&rcRect,GetSysColor(COLOR_BTNFACE));
	dc.DrawEdge(&rcRect,EDGE_RAISED,BF_RECT);
}

//****************************************************************************
// NeTrackPopup::OnSize - Calculates the m_dPixelvalue private var.
//****************************************************************************

void NeTrackPopup::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	m_dPixelValue = (double)(cx - THUMB_WIDTH) / (double)(m_nMax - m_nMin);
}

//****************************************************************************
// NeTrackPopup::OnLButtonUp - Releases the mouse and sends a closing message
//                              to the pNotify window. Finally destroyes
//                              itself and releases it's memory.
//****************************************************************************

void NeTrackPopup::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	
	CWnd::OnLButtonUp(nFlags, point);
	_parent->PopupStateChange(PUSC_CLOSING);
	
	DestroyWindow();
	delete this;
}

//****************************************************************************
// NeTrackPopup::OnMouseMove - Calculates new value and sends it to the
//                              pNotify window. Also invalidates the bar area
//                              causing a repaint.
//****************************************************************************

void NeTrackPopup::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rcRect;
	GetClientRect(&rcRect);
	
	int nPos = point.x;
	if (nPos < 0) nPos = 0;
	if (nPos > rcRect.right - THUMB_WIDTH) nPos = rcRect.right - THUMB_WIDTH;
	m_nValue = (float)((((double)nPos) / m_dPixelValue) + m_nMin);
	InvalidateRect(&rcRect);
	_parent->SetValue(m_nValue);
	
	CWnd::OnMouseMove(nFlags, point);
}
