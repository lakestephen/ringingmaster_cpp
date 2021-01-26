/*
NumericEdit - A clone of the spinedit control used in Jasc's Paint Shop Pro
Contact : jg@jgsoftware.com
Use the code for anything, just submit suggestions and bug fixes to above address.

  4/13/2004 - 1.0 Initial release
  
	4/20/2004 - 1.02 Bug fixes and some enhancements
	
	  Fixed bug where the edit would only accept hex digits. Also now allow negative sign "-" to be entered 
	  Added NES_METER style to make the small bar under the edit field optional 
	  Control will now reconfigure itself when changing style bits 
	  Fixed max value bug. The full range is now accesible with the popup 
	  Added NES_LIVEUPDATE style. This style allows the control to notify it's parent during value tracking. Without this style, the parent is updated only at the end of tracking 
	  Demo has been updated to show the new features 
	  Added DDX_NumericEdit() function for DDX support 
	  Now using the active window caption color for the bar colors 
	  Changed hardcoded custom messages to use windows registered messages so as not to cause any conflict with other custom messages
*/

#include "stdafx.h"
#include "NumericEdit.h"
#include "RingingMaster.h"
#include ".\numericedit.h"


// NumericEdit


IMPLEMENT_DYNAMIC(NumericEdit, CWnd)

NumericEdit::NumericEdit()
{
	m_nMin = 0.0;
	m_nMax = 100.0;
	m_nValue = 0.0;
	m_fCaptured = FALSE;
	m_fTracking = FALSE;
	_fInFunction = FALSE;
}

//****************************************************************************
// NumericEdit::GetValue - 
//****************************************************************************

float NumericEdit::GetValue()
{
	return m_nValue;
}

//****************************************************************************
// NumericEdit::SetValue - 
//****************************************************************************

void NumericEdit::SetValue(float nValue, bool doUpdateText)
{
	
	if (_fInFunction) return;
	
	if (nValue != m_nValue)
	{
		_fInFunction = TRUE;
		
		//
		CString str;
		str.Format(_T("%.3f"),nValue);
		
		if (nValue < m_nMin) nValue = m_nMin;
		if (nValue > m_nMax) nValue = m_nMax;
		m_nValue = nValue;
		if (doUpdateText)
		{
			m_wndEdit.SetWindowText(str);
			int nLength = str.GetLength();
			m_wndEdit.SetSel(nLength,nLength);
		}
		m_wndSpinButton.SetPos((int)m_nValue);
		InvalidateRect(&m_rcBar,FALSE);
		
		if (!m_fTracking || (GetStyle() & NES_LIVEUPDATE))
			GetParent()->SendMessage(NEN_CHANGED,(WPARAM)this,0);
		
		_fInFunction = FALSE;
	}
}

//****************************************************************************
// NumericEdit::GetReadOnly - 
//****************************************************************************

BOOL NumericEdit::GetReadOnly()
{
	return (m_wndEdit.GetStyle() & ES_READONLY) > 0;
}

//****************************************************************************
// NumericEdit::SetReadOnly - 
//****************************************************************************

void NumericEdit::SetReadOnly(BOOL fReadOnly)
{
	m_wndEdit.SetReadOnly(fReadOnly);
}

//****************************************************************************
// NumericEdit::GetRange
//****************************************************************************

void NumericEdit::GetRange(float& nMin,float& nMax)
{
	nMin = m_nMin;
	nMax = m_nMax;  
}

//****************************************************************************
// NumericEdit::SetRange -
//****************************************************************************

void NumericEdit::SetRange(float nMin,float nMax)
{
	if (nMin != m_nMin || nMax != m_nMax)
	{
		m_nMin = nMin;
		m_nMax = nMax;
		m_wndSpinButton.SetRange32((int)m_nMin,(int)m_nMax);
		if (m_nValue < m_nMin) m_nValue = m_nMin;
		if (m_nValue < m_nMin) m_nValue = m_nMin;
		CString str;
		str.Format(_T("%.3f"),m_nValue);
		m_wndEdit.SetWindowText(str);
		int nLength = str.GetLength();
		m_wndEdit.SetSel(nLength,nLength);
		
		m_dPixelValue = (double)m_rcBar.Width() / (double)(m_nMax - m_nMin);
		InvalidateRect(&m_rcBar);
	}
}

//****************************************************************************
// NumericEdit::ResizeChildren - 
//****************************************************************************

#define BARHEIGHT 3 // Height of the bar

void NumericEdit::ResizeChildren(int cx,int cy)
{
	int nWidth = GetSystemMetrics(SM_CXHSCROLL);
	int nCurx = cx;
	
	if (GetStyle() & NES_POPUP)
	{
		nCurx-= nWidth;
		m_wndButton.MoveWindow(nCurx,0,nWidth,cy);
	}
	else
		m_wndButton.MoveWindow(0,0,0,0);
	
	if (GetStyle() & NES_SPIN)
	{
		nCurx-= nWidth;
		m_wndSpinButton.MoveWindow(nCurx,0,nWidth,cy);
	}
	else
		m_wndSpinButton.MoveWindow(0,0,0,0);
	
	int nEditHeight;
	if (GetStyle() & NES_METER) 
		nEditHeight= cy - BARHEIGHT - 1;
	else
		nEditHeight = cy;
	
	m_wndEdit.MoveWindow(0,0,nCurx,nEditHeight);
	
	m_rcBar.left = 0;
	m_rcBar.top = cy - BARHEIGHT;
	m_rcBar.right = nCurx;
	m_rcBar.bottom = cy;
	
	m_dPixelValue = (double)(nCurx) / (double)(m_nMax - m_nMin);
}

BEGIN_MESSAGE_MAP(NumericEdit, CWnd)
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_WM_VSCROLL()
ON_WM_SETFOCUS()
ON_WM_KEYDOWN()
ON_WM_STYLECHANGED()
END_MESSAGE_MAP()

// NumericEdit message handlers

//****************************************************************************
// NumericEdit::Create - Creates actual window
//****************************************************************************

BOOL NumericEdit::Create(DWORD dwStyle,const CRect& rcRect,CWnd* pParent,UINT /*nID*/)
{
	CString sClassName;
	
	sClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,::LoadCursor(NULL, IDC_ARROW),(HBRUSH)0);
	if(!CreateEx(WS_EX_CLIENTEDGE,sClassName,0,WS_CHILD | dwStyle,
		rcRect.left,rcRect.top,rcRect.Width(),rcRect.Height(),pParent->GetSafeHwnd(),(HMENU) 0))
	{
		TRACE0("Failed to create NumericEdit\n");
		return FALSE;
	}
	
	return TRUE;
}

//****************************************************************************
// NumericEdit::OnPaint - Draws bar and black line above it
//****************************************************************************

void NumericEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CFont* oldFont = dc.SelectObject(GetFont());
	
	CRect rcRect;
	GetClientRect(&rcRect);
	
	if (GetStyle() & NES_METER)
	{
		// Draw black line
		dc.MoveTo(m_rcBar.left,m_rcBar.top - 1);
		dc.LineTo(m_rcBar.right + 1,m_rcBar.top - 1);
		
		int nXPos = (int) (m_dPixelValue * ((double)(m_nValue - m_nMin)));
		dc.FillSolidRect(m_rcBar.left,m_rcBar.top,nXPos,m_rcBar.Height(),GetSysColor(COLOR_ACTIVECAPTION));
		dc.FillSolidRect(m_rcBar.left + nXPos + 1,m_rcBar.top,m_rcBar.Width() - nXPos,m_rcBar.Height(),GetSysColor(COLOR_BTNFACE));
	}
	
	dc.SelectObject(oldFont);
}

//****************************************************************************
// NumericEdit::OnSize - Moves and sizes contained controls. Calculates bar
//                        position and size. Calculates m_dPixelValue.
//****************************************************************************

void NumericEdit::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	ResizeChildren(cx,cy);
}

//****************************************************************************
// NumericEdit::OnCreate - Creates contained controls.
//****************************************************************************

int NumericEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;
	
	CRect r(0,0,0,0);										
	m_wndButton.Create(_T(""),WS_CHILD|WS_VISIBLE,r,this);
	m_wndButton.SetImage(IDB_COMBO_DOWN);
	m_wndEdit.Create(WS_CHILD|WS_VISIBLE,r,this);
	m_wndEdit.SetWindowText(_T("0"));
	m_wndSpinButton.Create(WS_CHILD|WS_VISIBLE|UDS_ARROWKEYS,r,this,3);
	m_wndSpinButton.SetRange32((int)m_nMin,(int)m_nMax);
	
	
	
	return 0;
}

//****************************************************************************
// NumericEdit::OnLButtonDown - Sets new value calculated from clicked 
//                               position then captures mouse.
//****************************************************************************

void NumericEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ((m_rcBar.PtInRect(point)) && (GetStyle() & NES_METER))
	{
		int nNewValue = (int)((point.x) / m_dPixelValue);
		SetValue(nNewValue + m_nMin);
		SetCapture();
		m_fCaptured = TRUE;
		m_fTracking = TRUE;
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

//****************************************************************************
// NumericEdit::OnMouseMove - Calculates new value.
//****************************************************************************

void NumericEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_fCaptured)
	{
		int nPos = point.x;
		if (nPos < 0) nPos = 0;
		if (nPos > m_rcBar.Width()) nPos = m_rcBar.Width();
		int nNewValue = (int)(nPos / m_dPixelValue);
		SetValue(nNewValue + m_nMin);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

//****************************************************************************
// NumericEdit::OnLButtonUp - Releases mouse capture
//****************************************************************************

void NumericEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_fCaptured)
	{
		ReleaseCapture();
		m_fCaptured = FALSE;
		m_fTracking = FALSE;
		GetParent()->SendMessage(NEN_CHANGED,(WPARAM)this,0);
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

//****************************************************************************
// NumericEdit::OnVScroll - Handles WM_VSCROLL messages from contained
//                           CSpinEditCtrl and sets as new value.
//****************************************************************************

void NumericEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SetValue((float)((int)nPos));//do the conversion to in first so we do not get INT_MIN instead of -1
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

//****************************************************************************
// NumericEdit::OnSetFocus - Passes focus on the contained CEdit
//****************************************************************************

void NumericEdit::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_wndEdit.SetFocus();
}

//****************************************************************************
// NumericEdit::OnKeyDown - Handles the UP and DOWN messages passed from
//                           contained CEdit and adjusts value accordingly.
//****************************************************************************

void NumericEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP && m_nValue < m_nMax)
		SetValue((float)(m_nValue + 1.0));
	else
		if (nChar == VK_DOWN && m_nValue > m_nMin)
			SetValue((float)(m_nValue - 1.0));
		else
			CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

//****************************************************************************
// NumericEdit::OnStyleChanged -
//****************************************************************************

void NumericEdit::OnStyleChanged(int nWhich, LPSTYLESTRUCT /*lpStyleStruct*/)
{
	if (nWhich == GWL_STYLE)
	{
		CRect rcRect;
		GetClientRect(&rcRect);
		ResizeChildren(rcRect.Width(),rcRect.Height());
		InvalidateRect(&m_rcBar);
	}
}


void NumericEdit::ShowPopup()
{
	POINT point;
	::GetCursorPos(&point);
	NeTrackPopup* pTrackPopup = new NeTrackPopup();
	
	CRect rcRect;
	m_wndButton.GetWindowRect(rcRect); // Get button location to determine popup location
	pTrackPopup->Create(point.x,rcRect.bottom + 4,m_nMin,m_nMax,m_nValue,this);
}

void NumericEdit::PopupStateChange(PopupStateChangeType popupStateChange)
{
	m_fTracking = popupStateChange == PUSC_OPENING;
	if (!m_fTracking)
	{
		m_wndButton.SetState(FALSE);
		GetParent()->SendMessage(NEN_CHANGED,(WPARAM)this,0);
		m_wndEdit.SetFocus();
	}
}


BOOL NumericEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)	 //to2do remove when th accellerators are replaced with the On Char in the main touch window
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}
