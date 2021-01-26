////////////////////////////////////////////////////////////////////////////
//	File:		FlatHeaderCtrl.cpp
//	Version:	1.0.2.0
//
//	Author:		Maarten Hoeben
//	E-mail:		hoeben@nwn.com
//
//	Implementation of the FlatHeaderCtrl and associated classes.
//
//	You are free to use, distribute or modify this code
//	as long as the header is not removed or modified.
//
//	Version history
//
//	1.0.0.1	- Initial release
//	1.0.1.0	- Fixed FHDragWnd destroy warning (thanks Philippe Terrier)
//			- Fixed double sent HDN_ITEMCLICK
//			- Added a property that adjusts for ListCtrls that use a static
//			  border for flat look.
//	1.0.2.0	- Fixed another destroy warning
//			- Fixed InsertItem array exception handling
//			- Fixed incorrect header width painting
//			- Changed DrawItem argument passing
//			- Changed HDITEMEX struct item names
//			- Added handler for HDM_SETIMAGELIST (precalculate image dimensions)
//			- Changed DrawImage to clip images
//			- Changed InsertItem ASSERT check to position limitation
//			- Added new-style "HotDivider" arrows
//			- Fixed some GDI objects
//			- Added 'don't drop cursor' support to indicate drag&drop 
//			  outside control
//			- Added drag&drop target window support
//			- Changed FHDragWnd to support externally created items
//			- Removed topmost-style from FHDropWnd
//			- Fixed OnSetHotDivider order bug
//			- Added extended styles
//			- Added item width estimation function
//
////////////////////////////////////////////////////////////////////////////


// FlatHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FlatHeaderCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FHDragWnd

FHDragWnd::FHDragWnd()
{
	// Register the window class if it has not already been registered.
	WNDCLASS wndclass;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if(!(::GetClassInfo(hInst, FHDRAGWND_CLASSNAME, &wndclass)))
	{
		// otherwise we need to register a new class
		wndclass.style = CS_SAVEBITS ;
		wndclass.lpfnWndProc = ::DefWindowProc;
		wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInst;
		wndclass.hIcon = NULL;
		wndclass.hCursor = LoadCursor( hInst, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1); 
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = FHDRAGWND_CLASSNAME;
		if (!AfxRegisterClass(&wndclass))
			AfxThrowResourceException();
	}

	m_pFlatHeaderCtrl = NULL;
	m_iItem = -1;
	m_lphdiItem = NULL;
}

FHDragWnd::~FHDragWnd()
{
}


BEGIN_MESSAGE_MAP(FHDragWnd, CWnd)
	//{{AFX_MSG_MAP(FHDragWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//todo get rid of this file is poss.

/////////////////////////////////////////////////////////////////////////////
// FHDragWnd message handlers

BOOL FHDragWnd::Create(CRect rect, FlatHeaderCtrl* pFlatHeaderCtrl, INT iItem, LPHDITEM lphdiItem)
{
	ASSERT_VALID(pFlatHeaderCtrl);
	ASSERT(pFlatHeaderCtrl->IsKindOf(RUNTIME_CLASS(FlatHeaderCtrl)));

	m_pFlatHeaderCtrl = pFlatHeaderCtrl;
	m_iItem = iItem;
	m_lphdiItem = lphdiItem;

	DWORD dwStyle = WS_POPUP|WS_DISABLED;
	DWORD dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;

	return CreateEx(dwExStyle, FHDRAGWND_CLASSNAME, NULL, dwStyle, 
		rect.left, rect.top, rect.Width(), rect.Height(),
		NULL, NULL, NULL );
}


void FHDragWnd::OnPaint() 
{
	CPaintDC dc(this);
	
    if(m_pFlatHeaderCtrl->m_bDoubleBuffer)
    {
        MemDC MemDC(&dc);
        OnDraw(&MemDC);
    }
    else
        OnDraw(&dc);
}

BOOL FHDragWnd::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void FHDragWnd::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

    pDC->FillSolidRect(rect, m_pFlatHeaderCtrl->m_cr3DFace);
	pDC->Draw3dRect(rect, m_pFlatHeaderCtrl->m_cr3DHighLight, m_pFlatHeaderCtrl->m_cr3DShadow);

	CPen* pPen = pDC->GetCurrentPen();
	CFont* pFont = pDC->SelectObject(m_pFlatHeaderCtrl->GetFont());

	pDC->SetBkColor(m_pFlatHeaderCtrl->m_cr3DFace);
	pDC->SetTextColor(m_pFlatHeaderCtrl->m_crText);

	rect.DeflateRect(m_pFlatHeaderCtrl->m_iSpacing, 0);
	m_pFlatHeaderCtrl->DrawItem(
		pDC,
		rect,
		m_lphdiItem,
		m_pFlatHeaderCtrl->m_iSortColumn == m_iItem,
		m_pFlatHeaderCtrl->m_bSortAscending
	);

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);
}

void FHDragWnd::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// FHDropWnd

FHDropWnd::FHDropWnd(COLORREF crColor)
{
	m_brush.CreateSolidBrush(crColor);

	// Register the window class if it has not already been registered.
	WNDCLASS wndclass;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if(!(::GetClassInfo(hInst, FHDROPWND_CLASSNAME, &wndclass)))
	{
		// otherwise we need to register a new class
		wndclass.style = CS_SAVEBITS ;
		wndclass.lpfnWndProc = ::DefWindowProc;
		wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInst;
		wndclass.hIcon = NULL;
		wndclass.hCursor = LoadCursor( hInst, IDC_ARROW );
		wndclass.hbrBackground = (HBRUSH)m_brush; 
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = FHDROPWND_CLASSNAME;
		if (!AfxRegisterClass(&wndclass))
			AfxThrowResourceException();
	}
}

FHDropWnd::~FHDropWnd()
{
}


BEGIN_MESSAGE_MAP(FHDropWnd, CWnd)
	//{{AFX_MSG_MAP(FHDropWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// FHDropWnd message handlers

BOOL FHDropWnd::Create(INT iHeight)
{
	m_iHeight = iHeight + 20;

	DWORD dwStyle = WS_POPUP|WS_DISABLED;
	DWORD dwExStyle = WS_EX_TOOLWINDOW ;

	BOOL bResult = CreateEx(dwExStyle, FHDROPWND_CLASSNAME, NULL, dwStyle, 
		0, 0, 12, m_iHeight,
		NULL, NULL, NULL );

	CRgn rgn1, rgn2;
	POINT ptArrow[7];

	ptArrow[0].x = 8;	ptArrow[0].y = 0;
	ptArrow[1].x = 8;	ptArrow[1].y = 4;
	ptArrow[2].x = 11;	ptArrow[2].y = 4;
	ptArrow[3].x = 6;	ptArrow[3].y = 9;
	ptArrow[4].x = 1;	ptArrow[4].y = 4;
	ptArrow[5].x = 4;	ptArrow[5].y = 4;
	ptArrow[6].x = 4;	ptArrow[6].y = 0;
	rgn1.CreatePolygonRgn(ptArrow, 7, ALTERNATE);


	ptArrow[0].x = 4;	ptArrow[0].y = m_iHeight;
	ptArrow[1].x = 4;	ptArrow[1].y = m_iHeight-4;
	ptArrow[2].x = 0;	ptArrow[2].y = m_iHeight-4;
	ptArrow[3].x = 6;	ptArrow[3].y = m_iHeight-10;
	ptArrow[4].x = 12;	ptArrow[4].y = m_iHeight-4;
	ptArrow[5].x = 8;	ptArrow[5].y = m_iHeight-4;
	ptArrow[6].x = 8;	ptArrow[6].y = m_iHeight;
	rgn2.CreatePolygonRgn(ptArrow, 7, ALTERNATE);

	m_rgn.CreateRectRgn(0, 0, 12, iHeight);
	m_rgn.CombineRgn(&rgn1, &rgn2, RGN_OR);
	SetWindowRgn(m_rgn, FALSE);

	rgn1.DeleteObject();
	rgn2.DeleteObject();

	return bResult;
}


void FHDropWnd::PostNcDestroy() 
{
	m_rgn.DeleteObject();

	CWnd::PostNcDestroy();
	delete this;
}

BOOL FHDropWnd::OnEraseBkgnd(CDC* pDC) 
{
	pDC->FillRect(CRect(0, 0, 12, m_iHeight), &m_brush);
	return TRUE;
}

void FHDropWnd::SetWindowPos(INT x, INT y)
{
	CWnd::SetWindowPos(
		&wndTop, 
		x-6, y-(m_iHeight/2),
		0, 0, SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE
	);
}

/////////////////////////////////////////////////////////////////////////////
// FlatHeaderCtrl

IMPLEMENT_DYNCREATE(FlatHeaderCtrl, CHeaderCtrl)

FlatHeaderCtrl::FlatHeaderCtrl()
{
	m_bDoubleBuffer = TRUE;
	m_iSpacing = 6;
	m_sizeArrow.cx = 8;
	m_sizeArrow.cy = 8;
	m_sizeImage.cx = 0;
	m_sizeImage.cy = 0;
	m_bStaticBorder = FALSE;
	m_nDontDropCursor = 0;
	m_hDropTarget = NULL;
	m_rcDropTarget.SetRectEmpty();
	m_iDropResult = 0;

	m_iHotIndex = -1;
	m_bHotItemResizable = FALSE;

	m_bResizing = FALSE;

	m_iHotDivider = -1;
	m_crHotDivider = 0x000000FF;
	m_pDropWnd = NULL;

	m_bDragging = FALSE;
	m_pDragWnd = NULL;

	m_nClickFlags = 0;

	m_bSortAscending = FALSE;
	m_iSortColumn = -1;
	m_arrayHdrItemEx.SetSize(0, 8);

	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace = ::GetSysColor(COLOR_3DFACE);
	m_crText = ::GetSysColor(COLOR_BTNTEXT);
}

FlatHeaderCtrl::~FlatHeaderCtrl()
{
	if(m_pDropWnd != NULL)
	{
		m_pDropWnd->DestroyWindow();
		m_pDropWnd = NULL;
	}

	if(m_pDragWnd != NULL)
	{
		m_pDragWnd->DestroyWindow();
		m_pDragWnd = NULL;
	}
}

BEGIN_MESSAGE_MAP(FlatHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(FlatHeaderCtrl)
	ON_MESSAGE(HDM_INSERTITEMA, OnInsertItem)
	ON_MESSAGE(HDM_INSERTITEMW, OnInsertItem)
	ON_MESSAGE(HDM_DELETEITEM, OnDeleteItem)
	ON_MESSAGE(HDM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE(HDM_SETHOTDIVIDER, OnSetHotDivider)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FlatHeaderCtrl attributes

BOOL FlatHeaderCtrl::ModifyProperty(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case FH_PROPERTY_SPACING:
		m_iSpacing = (INT)lParam;
		break;

	case FH_PROPERTY_ARROW:
		m_sizeArrow.cx = LOWORD(lParam);
		m_sizeArrow.cy = HIWORD(lParam);
		break;

	case FH_PROPERTY_STATICBORDER:
		m_bStaticBorder = (BOOL)lParam;
		break;

	case FH_PROPERTY_DONTDROPCURSOR:
		m_nDontDropCursor = (UINT)lParam;
		break;

	case FH_PROPERTY_DROPTARGET:
		m_hDropTarget = (HWND)lParam;
		break;

	default:
		return FALSE;
	}

	Invalidate();
	return TRUE;
}

BOOL FlatHeaderCtrl::GetItemEx(INT iPos, HDITEMEX* phditemex) const
{
	if(iPos>=m_arrayHdrItemEx.GetSize())
		return FALSE;

	*phditemex = m_arrayHdrItemEx[iPos];
	return TRUE;
}

BOOL FlatHeaderCtrl::SetItemEx(INT iPos, HDITEMEX* phditemex)
{
	if(iPos>=m_arrayHdrItemEx.GetSize())
		return FALSE;

	BOOL bUpdate = FALSE;

	HDITEM hditem;
	hditem.mask = HDI_WIDTH;
	if(!GetItem(iPos, &hditem))
		return FALSE;

	HDITEMEX hditemex = *phditemex;

	if(hditemex.nStyle&HDF_EX_AUTOWIDTH)
	{
		TCHAR szText[FLATHEADER_TEXT_MAX];

		HDITEM hdi;
		hdi.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hdi.pszText = szText;
		hdi.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iPos, &hdi));

		hditem.cxy = GetItemWidth(&hdi, hditemex.nStyle&HDF_EX_INCLUDESORT ? TRUE:FALSE);
		bUpdate = TRUE;
	}

	if((!(hditemex.nStyle&HDF_EX_FIXEDWIDTH)) && (hditemex.iMinWidth<=hditemex.iMaxWidth))
	{
		if(hditem.cxy < hditemex.iMinWidth)
		{
			hditem.cxy = hditemex.iMinWidth;
			bUpdate = TRUE;
		}

		if(hditem.cxy > hditemex.iMaxWidth)
		{
			hditem.cxy = hditemex.iMaxWidth;
			bUpdate = TRUE;
		}
	}

	if(bUpdate)
		SetItem(iPos, &hditem);

	m_arrayHdrItemEx.SetAt(iPos, hditemex);
	return TRUE;
}

INT FlatHeaderCtrl::GetItemWidth(LPHDITEM lphdi, BOOL bIncludeSort)
{
	INT iWidth = 0;

	CBitmap* pBitmap = NULL;
	BITMAP biBitmap;
	if(lphdi->fmt&HDF_BITMAP)
	{
		ASSERT(lphdi->mask&HDI_BITMAP);
		ASSERT(lphdi->hbm);

		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if(pBitmap)
		{
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &biBitmap));
			iWidth += lphdi->fmt&HDF_BITMAP ? biBitmap.bmWidth+m_iSpacing:0; //- sjl moved from the block of 3 iWidth below
		}

	}

	iWidth += m_iSpacing;
	iWidth += lphdi->fmt&HDF_IMAGE ? m_sizeImage.cx+m_iSpacing:0;
	iWidth += bIncludeSort ? m_sizeArrow.cx+m_iSpacing:0;

	if(lphdi->mask&HDI_TEXT && lphdi->fmt&HDF_STRING)
	{
		CClientDC dc(this);
		CFont* pFont = dc.SelectObject(GetFont());

		iWidth += dc.GetTextExtent(lphdi->pszText).cx + m_iSpacing;

		dc.SelectObject(pFont);
	}

	return iWidth;
}

void FlatHeaderCtrl::SetSortColumn(INT iPos, BOOL bSortAscending)
{
//	ASSERT(iPos < GetItemCount());
//removed to allow TowerListCtrl::recordManager_notifyApplyDefaultSort to work before the cols have been set up.

	m_bSortAscending = bSortAscending;
	m_iSortColumn = iPos;
	Invalidate();
}

INT FlatHeaderCtrl::GetSortColumn(BOOL* pbSortAscending)
{
	if(pbSortAscending)
		*pbSortAscending = m_bSortAscending;

	return m_iSortColumn;
}

INT FlatHeaderCtrl::GetDropResult()
{
	return m_iDropResult;
}

/////////////////////////////////////////////////////////////////////////////
// FlatHeaderCtrl implementation

void FlatHeaderCtrl::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);

    pDC->FillSolidRect(rectClip, m_cr3DFace);

	INT iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	CPen penShadow(PS_SOLID, 1, m_cr3DShadow);
	CPen* pPen = pDC->GetCurrentPen();

	CFont* pFont = pDC->SelectObject(GetFont());

	pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crText);

	INT iWidth = 0;

	for(INT i=0;i<iItems;i++)
	{
		INT iItem = OrderToIndex(i);

		TCHAR szText[FLATHEADER_TEXT_MAX];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rectItem));

		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right)
		{
			if(hditem.fmt&HDF_OWNERDRAW)
			{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType = ODT_BUTTON;
				disItem.CtlID = GetDlgCtrlID();
				disItem.itemID = iItem;
				disItem.itemAction = ODA_DRAWENTIRE;
				disItem.itemState = 0;
				disItem.hwndItem = m_hWnd;
				disItem.hDC = pDC->m_hDC;
				disItem.rcItem = rectItem;
				disItem.itemData = 0;

				DrawItem(&disItem);
			}
			else
			{
				rectItem.DeflateRect(m_iSpacing, 0);
				DrawItem(pDC, rectItem, &hditem, iItem == m_iSortColumn, m_bSortAscending);
				rectItem.InflateRect(m_iSpacing, 0);

				if(m_nClickFlags&MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags&HHT_ONHEADER)
					pDC->InvertRect(rectItem);
			}

			if(i < iItems-1)
			{
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rectItem.right-1, rectItem.top+2);
				pDC->LineTo(rectItem.right-1, rectItem.bottom-2);

				pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.right, rectItem.top+2);
				pDC->LineTo(rectItem.right, rectItem.bottom-2);
			}
		}

		iWidth += hditem.cxy;
	}

	if(iWidth > 0)
	{
		rectClient.right = rectClient.left + iWidth;
		pDC->Draw3dRect(rectClient, m_cr3DHighLight, m_cr3DShadow);
	}

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);

	penHighLight.DeleteObject();
	penShadow.DeleteObject();
}

void FlatHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
{
	ASSERT(FALSE);  // must override for self draw header controls
}

void FlatHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bSort, BOOL bSortAscending)
{
	ASSERT(lphdi->mask&HDI_FORMAT);

	INT iWidth = 0;

	CBitmap* pBitmap = NULL;
	BITMAP BitmapInfo;
	if(lphdi->fmt&HDF_BITMAP)
	{
		ASSERT(lphdi->mask&HDI_BITMAP);
		ASSERT(lphdi->hbm);

		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if(pBitmap)
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));
	}

	switch(lphdi->fmt&HDF_JUSTIFYMASK)
	{
	case HDF_LEFT:
		iWidth = DrawImage(pDC, rect, lphdi, FALSE); //- sjl
		rect.left += (iWidth) ? iWidth+m_iSpacing : 0;
		if(lphdi->fmt&HDF_IMAGE && !iWidth)
			break;
		rect.right -= bSort ? m_iSpacing+m_sizeArrow.cx : 0;
		iWidth = DrawText(pDC, rect, lphdi); // sjl
		rect.left += (iWidth) ? iWidth+m_iSpacing : 0;
		if(bSort)
		{
			rect.right += m_iSpacing+m_sizeArrow.cx;
			rect.left += DrawArrow(pDC, rect, bSortAscending, FALSE)+m_iSpacing;
		}
		DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE);
		break;

	case HDF_CENTER:
		iWidth = DrawImage(pDC, rect, lphdi, FALSE);//sjl
		rect.left += (iWidth) ? iWidth+m_iSpacing : 0;
		if(lphdi->fmt&HDF_IMAGE && !iWidth)
			break;

		rect.left += bSort ? m_iSpacing+m_sizeArrow.cx : 0;
		iWidth=DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE);//sjl
		rect.right -= (iWidth) ? iWidth+m_iSpacing:0;
		if(bSort)
		{
			rect.left -= m_iSpacing+m_sizeArrow.cx;
			rect.right -= DrawArrow(pDC, rect, bSortAscending, TRUE)+2*m_iSpacing;
		}
		DrawText(pDC, rect, lphdi);
		break;

	case HDF_RIGHT:
		if(!(lphdi->fmt&HDF_BITMAP_ON_RIGHT))
		{	
			iWidth=DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, FALSE);//sjl
			rect.left += (iWidth) ? iWidth+m_iSpacing:0;
		}

		iWidth = DrawImage(pDC, rect, lphdi, FALSE);//sjl
		rect.left += (iWidth) ? iWidth+m_iSpacing : 0;
		if(lphdi->fmt&HDF_IMAGE && !iWidth)
			break;

		rect.left += bSort && (lphdi->fmt&HDF_BITMAP_ON_RIGHT) ? m_iSpacing+m_sizeArrow.cx : 0;
		if(lphdi->fmt&HDF_BITMAP_ON_RIGHT)
		{
			iWidth=DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE);//sjl
			rect.right -= (iWidth) ? iWidth+m_iSpacing:0;
		}
		if(bSort)
		{
			rect.left -= (lphdi->fmt&HDF_BITMAP_ON_RIGHT) ? m_iSpacing+m_sizeArrow.cx:0;
			rect.right -= DrawArrow(pDC, rect, bSortAscending, TRUE)+2*m_iSpacing;
		}
		DrawText(pDC, rect, lphdi);
		break;
	}
}

INT FlatHeaderCtrl::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
{
	CImageList* pImageList = GetImageList();
	INT iWidth = 0;

	if(lphdi->mask&HDI_IMAGE && lphdi->fmt&HDF_IMAGE)
	{
		ASSERT(pImageList);
		ASSERT(lphdi->iImage>=0 && lphdi->iImage<pImageList->GetImageCount());

		if(rect.Width()>0)
		{
			POINT point;

			point.y = rect.CenterPoint().y - (m_sizeImage.cy>>1);

			if(bRight)
				point.x = rect.right - m_sizeImage.cx;
			else
				point.x = rect.left;

			SIZE size;
			size.cx = rect.Width()<m_sizeImage.cx ? rect.Width():m_sizeImage.cx;
			size.cy = m_sizeImage.cy;
			pImageList->DrawIndirect(pDC, lphdi->iImage, point, size, CPoint(0, 0));

			iWidth = m_sizeImage.cx;
		}
	}

	return iWidth;
}

INT FlatHeaderCtrl::DrawBitmap(CDC* pDC, CRect rect, LPHDITEM /*lphdi*/, CBitmap* pBitmap, BITMAP* pBitmapInfo, BOOL bRight)
{
	INT iWidth = 0;

	if(pBitmap)
	{
		iWidth = pBitmapInfo->bmWidth;
		if(iWidth<=rect.Width() && rect.Width()>0)
		{
			POINT point;

			point.y = rect.CenterPoint().y - (pBitmapInfo->bmHeight>>1);

			if(bRight)
				point.x = rect.right - iWidth;
			else
				point.x = rect.left;

			CDC dc;
			if(dc.CreateCompatibleDC(pDC) == TRUE) 
			{
				VERIFY(dc.SelectObject(pBitmap));
				iWidth = pDC->BitBlt(
					point.x, point.y, 
					pBitmapInfo->bmWidth, pBitmapInfo->bmHeight, 
					&dc, 
					0, 0, 
					SRCCOPY
				) ? iWidth:0;
			}
			else iWidth = 0;
		}
		else
			iWidth = 0;
	}

	return iWidth;
}

INT FlatHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	CSize size;

	if(rect.Width()>0 && lphdi->mask&HDI_TEXT && lphdi->fmt&HDF_STRING)
	{
		size = pDC->GetTextExtent(lphdi->pszText);

		switch(lphdi->fmt&HDF_JUSTIFYMASK)
		{
		case HDF_LEFT:
		case HDF_LEFT|HDF_RTLREADING:
			pDC->DrawText(lphdi->pszText, -1, rect, DT_LEFT|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
			break;
		case HDF_CENTER:
		case HDF_CENTER|HDF_RTLREADING:
			pDC->DrawText(lphdi->pszText, -1, rect, DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
			break;
		case HDF_RIGHT:
		case HDF_RIGHT|HDF_RTLREADING:
			pDC->DrawText(lphdi->pszText, -1, rect, DT_RIGHT|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
			break;
		}
	}

	size.cx = rect.Width()>size.cx ? size.cx:rect.Width();
	return size.cx>0 ? size.cx:0;
}

INT FlatHeaderCtrl::DrawArrow(CDC* pDC, CRect rect, BOOL bSortAscending, BOOL bRight)
{
	INT iWidth = 0;

	if(rect.Width()>0 && m_sizeArrow.cx<=rect.Width())
	{
		iWidth = m_sizeArrow.cx;

		rect.top += (rect.Height() - m_sizeArrow.cy - 1)>>1;
		rect.bottom = rect.top + m_sizeArrow.cy - 1;

		rect.left = bRight ? rect.right-m_sizeArrow.cy:rect.left;

		// Set up pens to use for drawing the triangle
		CPen penLight(PS_SOLID, 1, m_cr3DHighLight);
		CPen penShadow(PS_SOLID, 1, m_cr3DShadow);
		CPen *pPen = pDC->SelectObject(&penLight);

		if(bSortAscending)
		{
			// Draw triangle pointing upwards
			pDC->MoveTo(rect.left + ((m_sizeArrow.cx-1)>>1) + 1,	rect.top);
			pDC->LineTo(rect.left +  (m_sizeArrow.cx-1),			rect.top + m_sizeArrow.cy - 1);
			pDC->LineTo(rect.left,									rect.top + m_sizeArrow.cy - 1);

			pDC->SelectObject(&penShadow);
			pDC->MoveTo(rect.left + ((m_sizeArrow.cx-1)>>1),	rect.top);
			pDC->LineTo(rect.left,								rect.top + m_sizeArrow.cy - 1);
		}
		else	
		{
			// Draw triangle pointing downwards
			pDC->MoveTo(rect.left + ((m_sizeArrow.cx-1)>>1)+1,	rect.top + m_sizeArrow.cy - 1);
			pDC->LineTo(rect.left +  (m_sizeArrow.cx-1),		rect.top);

			pDC->SelectObject(&penShadow);
			pDC->MoveTo(rect.left + ((m_sizeArrow.cx-1)>>1),	rect.top + m_sizeArrow.cy - 1);
			pDC->LineTo(rect.left,								rect.top);
			pDC->LineTo(rect.left + m_sizeArrow.cx,				rect.top);
		}
		
		// Restore the pen
		pDC->SelectObject(pPen);

		penLight.DeleteObject();
		penShadow.DeleteObject();
	}

	return iWidth;
}

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrl message handlers

LRESULT FlatHeaderCtrl::OnInsertItem(WPARAM wParam, LPARAM /*lParam*/)
{
	HDITEMEX hditemex;
	hditemex.iMinWidth = 0;
	hditemex.iMaxWidth = -1;

	LRESULT lResult = -1;

	WORD wItems = (WORD)m_arrayHdrItemEx.GetSize();
	wParam = wParam<=wItems ? wParam:wItems;

	try
	{
		m_arrayHdrItemEx.InsertAt(wParam, hditemex);

		lResult = Default();
		if(lResult < 0)	// Cleanup
			m_arrayHdrItemEx.RemoveAt(wParam);
	}
	catch(CMemoryException* e)
	{
		e->Delete();
	}

	return lResult;
}

LRESULT FlatHeaderCtrl::OnDeleteItem(WPARAM wParam, LPARAM /*lParam*/)
{
	ASSERT((INT)wParam < m_arrayHdrItemEx.GetSize());
	m_arrayHdrItemEx.RemoveAt(wParam);

	return Default();
}

LRESULT FlatHeaderCtrl::OnSetImageList(WPARAM /*wParam*/, LPARAM lParam)
{
	CImageList* pImageList;
	pImageList = CImageList::FromHandle((HIMAGELIST)lParam);

	IMAGEINFO info;
	if(pImageList->GetImageInfo(0, &info))
	{
		m_sizeImage.cx = info.rcImage.right - info.rcImage.left;
		m_sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
	}

	return Default();
}

LRESULT FlatHeaderCtrl::OnSetHotDivider(WPARAM wParam, LPARAM lParam)
{
	if(wParam)
	{
		HDHITTESTINFO hdhti;
		hdhti.pt.x = LOWORD(lParam);
		hdhti.pt.y = HIWORD(lParam);
		ScreenToClient(&hdhti.pt);

		INT iHotIndex = SendMessage(HDM_HITTEST, 0, (LPARAM)(&hdhti));
		if(iHotIndex >= 0)
		{
			HDITEM hditem;
			hditem.mask = HDI_ORDER;
			VERIFY(GetItem(iHotIndex, &hditem));
			m_iHotDivider = hditem.iOrder;

			CRect rectItem;
			VERIFY(GetItemRect(iHotIndex, rectItem));
			if(hdhti.pt.x > rectItem.CenterPoint().x)
				m_iHotDivider++;
		}
		else
			m_iHotDivider = -1;
	}
	else
		m_iHotDivider = (INT)lParam;

	RECT rect;
	GetClientRect(&rect);

	INT iItems = GetItemCount();
	if(m_iHotDivider >= 0 && m_iHotDivider<=iItems+1)
	{
		if(m_pDropWnd == NULL)
		{
			m_pDropWnd = new FHDropWnd(m_crHotDivider);
			if(m_pDropWnd)
				m_pDropWnd->Create(rect.bottom - rect.top);
		}

		if(m_pDropWnd != NULL)
		{
			POINT pt;
			pt.y = (rect.bottom-rect.top)/2;

			if(m_iHotDivider<iItems)
			{
				GetItemRect(OrderToIndex(m_iHotDivider), &rect);
				pt.x = rect.left - 1;
			}
			else
			{
				GetItemRect(OrderToIndex(iItems-1), &rect);
				pt.x = rect.right;
			}

			ClientToScreen(&pt);
			m_pDropWnd->SetWindowPos(pt.x, pt.y);
		}
	}
	else
	{
		if(m_pDropWnd != NULL)
		{
			m_pDropWnd->DestroyWindow();
			m_pDropWnd = NULL;
		}
	}

	return(LRESULT)m_iHotDivider;
}

LRESULT FlatHeaderCtrl::OnLayout(WPARAM /*wParam*/, LPARAM lParam)
{
	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

	if(m_bStaticBorder)
		lphdlayout->prc->right += GetSystemMetrics(SM_CXBORDER)*2;

	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// FlatHeaderCtrl message handlers

void FlatHeaderCtrl::OnSysColorChange() 
{
	CHeaderCtrl::OnSysColorChange();
	
	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace = ::GetSysColor(COLOR_3DFACE);
	m_crText = ::GetSysColor(COLOR_BTNTEXT);
}

BOOL FlatHeaderCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void FlatHeaderCtrl::OnPaint() 
{
    CPaintDC dc(this);

    if (m_bDoubleBuffer)
    {
        MemDC MemDC(&dc);
        DrawCtrl(&MemDC);
    }
    else
        DrawCtrl(&dc);
}


LRESULT FlatHeaderCtrl::OnNcHitTest(CPoint point) 
{
	m_hdhtiHotItem.pt = point;
	ScreenToClient(&m_hdhtiHotItem.pt);

	m_iHotIndex = SendMessage(HDM_HITTEST, 0, (LPARAM)(&m_hdhtiHotItem));
	if(m_iHotIndex >= 0)
	{
		HDITEM hditem;
		hditem.mask = HDI_ORDER;
		VERIFY(GetItem(m_iHotIndex, &hditem));
		m_iHotOrder = hditem.iOrder;

		HDITEMEX hditemex;
		if(GetItemEx(m_iHotIndex, &hditemex))
			m_bHotItemResizable = hditemex.nStyle&HDF_EX_FIXEDWIDTH ? FALSE:TRUE;
	}

	return CHeaderCtrl::OnNcHitTest(point);
}


BOOL FlatHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_iHotIndex>=0 && m_hdhtiHotItem.flags&(HHT_ONDIVIDER|HHT_ONDIVOPEN) && !m_bHotItemResizable)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}

	return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

void FlatHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_nClickFlags = nFlags;
	m_ptClickPoint = point;

	if(m_iHotIndex >= 0)
	{
		m_hdiHotItem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP|HDI_ORDER|HDI_LPARAM;
		m_hdiHotItem.pszText = m_szHotItemText;
		m_hdiHotItem.cchTextMax = sizeof(m_szHotItemText);
		VERIFY(GetItem(m_iHotIndex, &m_hdiHotItem));

		if(m_hdhtiHotItem.flags&HHT_ONHEADER)
		{
			RECT rectItem;
			VERIFY(GetItemRect(m_iHotIndex, &rectItem));
			InvalidateRect(&rectItem);
		}

		if(m_hdhtiHotItem.flags&(HHT_ONDIVIDER|HHT_ONDIVOPEN))
		{
			if(!m_bHotItemResizable)
				return;

			HDITEMEX hditemex;
			VERIFY(GetItemEx(m_iHotIndex, &hditemex));

			CRect rectItem;
			GetItemRect(m_iHotIndex, rectItem);
			ClientToScreen(rectItem);

			if(hditemex.iMinWidth <= hditemex.iMaxWidth)
			{
				CRect rectClip;
				GetClipCursor(rectClip);

				POINT point;
				GetCursorPos(&point);

				INT iOffset = point.x - rectItem.right;

				rectClip.left = rectItem.left + hditemex.iMinWidth + iOffset;
				rectClip.right = rectItem.left + hditemex.iMaxWidth + iOffset;

				ClipCursor(rectClip);
			}

			m_bResizing = TRUE;
		}
	}

	CHeaderCtrl::OnLButtonDown(nFlags, point);
}


void FlatHeaderCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(m_iHotIndex>=0 && m_hdhtiHotItem.flags&(HHT_ONDIVIDER|HHT_ONDIVOPEN) && !m_bHotItemResizable)
		return;

	CHeaderCtrl::OnLButtonDblClk(nFlags, point);
}

void FlatHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_nClickFlags = nFlags;
	m_ptClickPoint = point;

	if(m_iHotIndex >= 0)
	{
		CWnd* pWnd = GetParent();

		if(m_hdhtiHotItem.flags&(HHT_ONDIVIDER|HHT_ONDIVOPEN))
		{
			if(m_bResizing)
			{
				ClipCursor(NULL);
				m_bResizing = FALSE;
			}
		}

		if(m_hdhtiHotItem.flags&HHT_ONHEADER)
		{
			if(m_bDragging)
			{
				NMHEADER nmhdr;
				nmhdr.hdr.hwndFrom = m_hWnd;
				nmhdr.hdr.idFrom = GetDlgCtrlID();
				nmhdr.hdr.code = HDN_ENDDRAG;
				nmhdr.iItem = m_iHotIndex;
				nmhdr.iButton =	0;
				nmhdr.pitem = &m_hdiHotItem;
				if(pWnd->SendMessage(WM_NOTIFY, 0, (LPARAM)&nmhdr)==FALSE && m_iHotDivider>=0)
				{
					INT iCount = GetItemCount();
					ASSERT(m_iHotOrder < iCount);
					ASSERT(m_iHotDivider <= iCount);

					LPINT piArray = new INT[iCount*2];
					if(piArray)
					{
						GetOrderArray((LPINT)piArray, iCount);

						for(INT i=0,j=0;i<iCount;i++)
						{
							if(j == m_iHotOrder)
								j++;

							if(
								(m_iHotOrder<m_iHotDivider && i == m_iHotDivider-1) ||
								(m_iHotOrder>=m_iHotDivider && i == m_iHotDivider)
							)
								piArray[iCount+i] = piArray[m_iHotOrder];
							else
								piArray[iCount+i] = piArray[j++];
						}						

						SetOrderArray(iCount, (LPINT)&piArray[iCount]);
						delete piArray;
					}
					else
						AfxThrowMemoryException();
				}

				if(m_pDragWnd != NULL)
				{
					m_pDragWnd->DestroyWindow();
					m_pDragWnd = NULL;
				}

				if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
					ReleaseCapture();

				m_bDragging = FALSE;
				OnSetHotDivider(FALSE, -1);

				Invalidate();
			}
			else
			{
				RECT rectItem;
				VERIFY(GetItemRect(m_iHotIndex, &rectItem));
				InvalidateRect(&rectItem);
			}
		}
	}

	CHeaderCtrl::OnLButtonUp(nFlags, point);
}

void FlatHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_nClickFlags&MK_LBUTTON && m_iHotIndex>=0)
	{
		if(m_bResizing)
		{
			CHeaderCtrl::OnMouseMove(nFlags, point);
			Invalidate();
			return;
		}

		if(m_hdhtiHotItem.flags&HHT_ONHEADER)
		{
			if(m_bDragging)
			{
				if(m_pDragWnd != NULL)
				{
					CRect rect;
					m_pDragWnd->GetWindowRect(&rect);

					CPoint pt = point;
					ClientToScreen(&pt);

					pt.Offset(-(rect.Width()>>1), -(rect.Height()>>1));

					m_pDragWnd->SetWindowPos(
						&wndTop, 
						pt.x, pt.y,
						0, 0, SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE
					);

					HDHITTESTINFO hdhti;
					hdhti.pt.x = point.x;
					hdhti.pt.y = point.y;

					INT iHotOrder = -1;
					INT iHotIndex = SendMessage(HDM_HITTEST, 0, (LPARAM)(&hdhti));
					if(iHotIndex >= 0)
					{
						HDITEM hditem;
						hditem.mask = HDI_ORDER;
						VERIFY(GetItem(iHotIndex, &hditem));
						iHotOrder = hditem.iOrder;

						CRect rectItem;
						VERIFY(GetItemRect(iHotIndex, rectItem));
						if(hdhti.pt.x > rectItem.CenterPoint().x)
							iHotOrder++;

						SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
						m_iDropResult = 0;
					}
					else
					{
						pt = point;
						ClientToScreen(&pt);

						if(!(m_hDropTarget && m_rcDropTarget.PtInRect(pt)))
						{
							if(m_nDontDropCursor)
								SetCursor(AfxGetApp()->LoadCursor(m_nDontDropCursor));
							else
								SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));

							m_iDropResult = -1;
						}
						else
						{
							SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
							m_iDropResult = 1;
						}
					}

					if(iHotOrder == m_iHotOrder || iHotOrder == m_iHotOrder+1)
						iHotOrder = -1;

					if(iHotOrder != m_iHotDivider)
						OnSetHotDivider(FALSE, iHotOrder);
				}

				return;
			}
			else if(GetStyle()&HDS_DRAGDROP)
			{
				INT iDragCX = GetSystemMetrics(SM_CXDRAG);
				INT iDragCY = GetSystemMetrics(SM_CYDRAG);
				CRect rectDrag(
					m_ptClickPoint.x-iDragCX, m_ptClickPoint.y-iDragCY,
					m_ptClickPoint.x+iDragCX, m_ptClickPoint.y+iDragCY
				);

				if(!rectDrag.PtInRect(point))
				{
					NMHEADER nmhdr;
					nmhdr.hdr.hwndFrom = m_hWnd;
					nmhdr.hdr.idFrom = GetDlgCtrlID();
					nmhdr.hdr.code = HDN_BEGINDRAG;
					nmhdr.iItem = m_iHotIndex;
					nmhdr.iButton =	1;
					nmhdr.pitem = &m_hdiHotItem;

					BOOL bBeginDrag = TRUE;
					CWnd* pWnd = GetParent();
					if(pWnd != NULL)
						bBeginDrag = pWnd->SendMessage(WM_NOTIFY, 0, (LPARAM)&nmhdr)==FALSE ? TRUE:FALSE;

					if(bBeginDrag)
					{
						ASSERT(m_pDragWnd == NULL);
						m_pDragWnd = new FHDragWnd;
						if(m_pDragWnd)
						{
							CRect rectItem;
							VERIFY(GetItemRect(m_iHotIndex, rectItem));
							ClientToScreen(&rectItem);

							m_pDragWnd->Create(rectItem, this, m_iHotIndex, &m_hdiHotItem);
						}

						BOOL bVisible = FALSE;
						if(m_hDropTarget != NULL)
						{
							bVisible = ::GetWindowLong(m_hDropTarget, GWL_STYLE)&WS_VISIBLE ? TRUE:FALSE;

							HWND hParent = ::GetParent(m_hDropTarget);
							if(hParent)
								bVisible = ::GetWindowLong(hParent, GWL_STYLE)&WS_VISIBLE ? TRUE:FALSE;
						}

						if(m_hDropTarget != NULL && bVisible)
							VERIFY(::GetWindowRect(m_hDropTarget, m_rcDropTarget));
						else
							m_rcDropTarget.SetRectEmpty();
					}
		
					SetCapture();
					m_bDragging = TRUE;
				}
			}
		}
	}
}


