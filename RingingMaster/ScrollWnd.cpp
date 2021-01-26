#include "stdafx.h"
#include "ScrollWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ScrollWnd

IMPLEMENT_DYNAMIC(ScrollWnd, CWnd)

BEGIN_MESSAGE_MAP(ScrollWnd, CWnd)
	//{{AFX_MSG_MAP(ScrollWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ScrollWnd construction/destruction

ScrollWnd::ScrollWnd():
_insideUpdate(FALSE),
_totalSize(0,0),
_lineSize(0,0),	
_pageSize(0,0)
{
}

ScrollWnd::~ScrollWnd()
{
}

int ScrollWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_hWnd != NULL) 
	{
		SetScrollSizes(CSize(0,0));
	}
						  	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// ScrollWnd painting

void ScrollWnd::OnPaint() 
{
	CBitmap memBitmap; // Offscreen bitmap
	CBitmap* oldBitmap; // bitmap originally found in memDC
	CRect drawRect; // Rectangle of drawing area.

   	CPaintDC dc(this);

	CDC memDC;

	// Create a Memory DC
	memDC.CreateCompatibleDC(&dc);
	
	//set the map mode
	dc.SetMapMode(MM_TEXT);
	memDC.SetMapMode(MM_TEXT);

	//get the whole client size and offset to paint the correct area
	GetClientRect(&drawRect);
	drawRect += GetScrollPosition();

	//create a bitmap, and select into the memDC
	memBitmap.CreateCompatibleBitmap(&dc, drawRect.Width(), drawRect.Height());
	oldBitmap = memDC.SelectObject(&memBitmap);
	
	// by default shift viewport origins in negative direction of scroll
	memDC.SetViewportOrg(-GetScrollPosition());
	dc.SetViewportOrg(-GetScrollPosition());

	//erase the background
    memDC.FillSolidRect(&drawRect, GetSysColor(COLOR_WINDOW));  

	//perform the GDI calls
	OnDraw(&memDC);

	// Copy the offscreen bitmap onto the screen.
	dc.BitBlt(drawRect.left, drawRect.top, drawRect.Width(), drawRect.Height(),
						&memDC, drawRect.left, drawRect.top, SRCCOPY);
	
	//Swap back the original bitmap.
	memDC.SelectObject(oldBitmap);
}	
 
void ScrollWnd::OnDraw(CDC* /*pDC*/)
{//virtual - overide in derived classes
}

/////////////////////////////////////////////////////////////////////////////
// ScrollWnd scrolling

void ScrollWnd::SetScrollSizes(CSize totalSize, const CSize pageSize, 
											const CSize lineSize)
{
	//check for bounds
	ASSERT(totalSize.cx >= 0 && totalSize.cy >=0);
	_totalSize = totalSize;
	_pageSize  = pageSize;
	_lineSize  = lineSize;

	//adjust device specific sizes
	if (_pageSize.cx == 0)
	{
		_pageSize.cx = _totalSize.cx / 5;
	}
	if (_pageSize.cy == 0)
	{
		_pageSize.cy = _totalSize.cy / 5;
	}
	if (_lineSize.cx == 0)
	{
		_lineSize.cx = _pageSize.cx  / 5;
	}
	if (_lineSize.cy == 0)
	{
		_lineSize.cy = _pageSize.cy  / 5;
	}
	
	if (m_hWnd != NULL)
	{
		UpdateBars();
		Invalidate(TRUE);
	}
}

void ScrollWnd::ScrollToPosition(POINT scrollPos)
{
	// limit if out of range
	int xMax = GetScrollLimit(SB_HORZ);
	int yMax = GetScrollLimit(SB_VERT);
	if (scrollPos.x < 0)
		scrollPos.x = 0;
	else if (scrollPos.x > xMax)
		scrollPos.x = xMax;
	if (scrollPos.y < 0)
		scrollPos.y = 0;
	else if (scrollPos.y > yMax)
		scrollPos.y = yMax;

	//do the scrolling
	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos(SB_HORZ, scrollPos.x);
	int yOrig = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, scrollPos.y);
	ScrollWindow(xOrig - scrollPos.x, yOrig - scrollPos.y);
}

void ScrollWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
}


void ScrollWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)     
{
	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	OnScroll(MAKEWORD(-1, nSBCode), nPos);
}

BOOL ScrollWnd::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	//MS ARTICAL Q166473
	
      SCROLLINFO info;
      info.cbSize = sizeof(SCROLLINFO);
      info.fMask = SIF_TRACKPOS;

      if (LOBYTE(nScrollCode) == SB_THUMBTRACK)
      {
         GetScrollInfo(SB_HORZ, &info);
         nPos = info.nTrackPos;
      }

      if (HIBYTE(nScrollCode) == SB_THUMBTRACK)
      {
         GetScrollInfo(SB_VERT, &info);
         nPos = info.nTrackPos;
      }

	
	
	// calc new x position
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= _lineSize.cx;
		break;
	case SB_LINEDOWN:
		x += _lineSize.cx;
		break;
	case SB_PAGEUP:
		x -= _pageSize.cx;
		break;
	case SB_PAGEDOWN:
		x += _pageSize.cx;
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= _lineSize.cy;
		break;
	case SB_LINEDOWN:
		y += _lineSize.cy;
		break;
	case SB_PAGEUP:
		y -= _pageSize.cy;
		break;
	case SB_PAGEDOWN:
		y += _pageSize.cy;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}
	
//	TRACE("%ld,%ld\n",x,y);


	BOOL result = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (result && bDoScroll)
		UpdateWindow();

	return result;
}

BOOL ScrollWnd::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD style = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(style & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(style & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		ScrollWindow(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// ScrollWnd getting information

BOOL ScrollWnd::GetTrueClientSize(CSize &clientNew, CSize &scrollBar)
{
	CRect client;
	GetClientRect(&client);
	ASSERT(client.top == 0 && client.left == 0);
	clientNew.cx = client.right;
	clientNew.cy = client.bottom;
	DWORD style = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(scrollBar);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (scrollBar.cx != 0 && (style & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		clientNew.cx += scrollBar.cx;   // currently on - adjust now
	}
	if (scrollBar.cy != 0 && (style & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		clientNew.cy += scrollBar.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (clientNew.cx > scrollBar.cx && clientNew.cy > scrollBar.cy);
}

void ScrollWnd::GetScrollBarState(CSize client, CSize &needScrollBar, 
							   CSize &range, CPoint &move, BOOL InsideClient)
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeScrollBar;
	GetScrollBarSizes(sizeScrollBar);

	// enough room to add scrollbars
	range = _totalSize - client;
		// > 0 => need to scroll
	move = GetScrollPosition();
		// point to move to (start at current scroll pos)

	BOOL needH = range.cx > 0;
	if (!needH)
		move.x = 0;                       // jump back to origin
	else if (InsideClient)
		range.cy += sizeScrollBar.cy;          // need room for a scroll bar

	BOOL needV = range.cy > 0;
	if (!needV)
		move.y = 0;                       // jump back to origin
	else if (InsideClient)
		range.cx += sizeScrollBar.cx;          // need room for a scroll bar

	if (needV && !needH && range.cx > 0)
	{
		ASSERT(InsideClient);
		// need a horizontal scrollbar after all
		needH = TRUE;
		range.cy += sizeScrollBar.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (range.cx > 0 && move.x >= range.cx)
		move.x = range.cx;
	if (range.cy > 0 && move.y >= range.cy)
		move.y = range.cy;

	// now update the bars as appropriate
	needScrollBar.cx = needH;
	needScrollBar.cy = needV;

	// needSb, range, and ptMove area now all updated
}

void ScrollWnd::GetScrollBarSizes(CSize &scrollBar)
{
	scrollBar.cx = scrollBar.cy = 0;
	DWORD style = GetStyle();

	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		scrollBar.cx = GetSystemMetrics(SM_CXVSCROLL);
		if (style & WS_BORDER)
			scrollBar.cx -= GetSystemMetrics(SM_CXBORDER); 
	}
	if (GetScrollBarCtrl(SB_HORZ) == NULL)
	{
		// horz scrollbars will impact client area of this window
		scrollBar.cy = GetSystemMetrics(SM_CYVSCROLL);
		if (style & WS_BORDER)
			scrollBar.cy -= GetSystemMetrics(SM_CYBORDER);
	}
}

CPoint ScrollWnd::GetScrollPosition() const
{
	CPoint scrollPos(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(scrollPos.x >= 0 && scrollPos.y >= 0);

	return scrollPos;
}

void ScrollWnd::GetScrollSizes(SIZE &total, SIZE &page, SIZE &line) const
{
	total = _totalSize;
	page  = _pageSize;
	line  = _lineSize;
}

/////////////////////////////////////////////////////////////////////////////
// ScrollWnd other functions

void ScrollWnd::UpdateBars()
{
	// UpdateBars may cause window to be resized - ignore those resizings
	if (_insideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	_insideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT(_totalSize.cx >= 0 && _totalSize.cy >= 0);

	CSize client;
	CSize scrollBar;

	// get client rect
	if (!GetTrueClientSize(client, scrollBar))
	{
		// no room for scroll bars (common for zero sized elements)
		CRect rect;
		GetClientRect(&rect);
		if (rect.right > 0 && rect.bottom > 0)
		{
			// if entire client area is not invisible, assume we have
			//  control over our scrollbars
			EnableScrollBarCtrl(SB_BOTH, FALSE);
		}
		_insideUpdate = FALSE;
		return;
	}

	// enough room to add scrollbars
	CSize range;
	CPoint move;
	CSize needScrollBar;

	// get the current scroll bar state given the true client area
	GetScrollBarState(client, needScrollBar, range, move, TRUE);
	if (needScrollBar.cx)
		client.cy -= scrollBar.cy;
	if (needScrollBar.cy)
		client.cx -= scrollBar.cx;

	// first scroll the window as needed
	ScrollToPosition(move); // will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needScrollBar.cx);
	if (needScrollBar.cx)
	{
		info.nPage = client.cx;
		info.nMax = _totalSize.cx-1;
		if (!SetScrollInfo(SB_HORZ, &info, TRUE))
			SetScrollRange(SB_HORZ, 0, range.cx, TRUE);
	}
	EnableScrollBarCtrl(SB_VERT, needScrollBar.cy);
	if (needScrollBar.cy)
	{
		info.nPage = client.cy;
		info.nMax = _totalSize.cy-1;
		if (!SetScrollInfo(SB_VERT, &info, TRUE))
			SetScrollRange(SB_VERT, 0, range.cy, TRUE);
	}

	// remove recursion lockout
	_insideUpdate = FALSE;
}

void ScrollWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (m_hWnd != NULL)
		UpdateBars();
}



/////////////////////////////////////////////////////////////////////////////
// ScrollWnd diagnostics

#ifdef _DEBUG

void ScrollWnd::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);

	dc << "totalPix = "			<< _totalSize;
	dc << "\npagePix = "		<< _pageSize;
	dc << "\nlinePix = "		<< _lineSize;
	dc << "\ninsideUpdate = "	<< _insideUpdate;
	dc << "\n";
}

#endif //_DEBUG

BOOL ScrollWnd::Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	dwStyle |= WS_CHILD|WS_HSCROLL|WS_VISIBLE|WS_VSCROLL|WS_CLIPCHILDREN;	
	
	if (lpszWindowName == NULL) 
	{
		lpszWindowName = "ScrollWnd";
	}

	return CWnd::CreateEx(dwExStyle, AfxRegisterWndClass(0, 0, 0, 0), 
		lpszWindowName, dwStyle, rect, pParentWnd, nID, NULL);
}

BOOL ScrollWnd::OnEraseBkgnd(CDC* /*pDC*/) 
{
	//no erasing is necessary as the paint function is using memDC, and is 
	//painting the whole background
	return TRUE; 
}

BOOL ScrollWnd::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/) 
{
	DWORD style = GetStyle();

	if (style & WS_VSCROLL)
	{
		OnScrollBy(CSize(0,(zDelta>0)?-(_lineSize.cy*3):(_lineSize.cy*3)));
	}
	else 
		OnScrollBy(CSize((zDelta>0)?-(_lineSize.cx*5):(_lineSize.cx*5),0));
	

	return TRUE;	
}
