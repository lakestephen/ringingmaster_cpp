// MapView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MapView.h"			   
#include "MapDoc.h"		
#include "PrintScaleObject.h"

#include "PrintData.h"
#include "PrintUtils.h"
#include "MethodWndPrint.h"
#include "CPage.h"
#include "MapCacheItem.H"
#include "MapManager.H"
#include "RecordManager.H"
#include ".\mapview.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MapView

IMPLEMENT_DYNCREATE(MapView, CScrollView32Bit)

MapView::MapView() :
_mapViewHitTest(mvht_none),
_draggingBackground(false)
{
   VERIFY(_brHatch.CreateHatchBrush(HS_DIAGCROSS, RGB(191, 191, 191)));
}

MapView::~MapView()
{
	_tip.DestroyWindow();
}


BEGIN_MESSAGE_MAP(MapView, CScrollView32Bit)
	//{{AFX_MSG_MAP(MapView)
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_TOOLBAR, OnFilePrintToolbar)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_SHOW_PUSHPINS, OnShowPushpins)
	ON_UPDATE_COMMAND_UI(ID_SHOW_PUSHPINS, OnUpdateShowPushpins)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_WM_LBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_MENU, OnUpdateFilePrintMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_TOOLBAR, OnUpdateFilePrintToolbar)
	ON_COMMAND(ID_SHOW_MAP, OnShowMap)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MAP, OnUpdateShowMap)
	ON_COMMAND(ID_POPUP_GRABTOWER, OnPopupGrabtower)
	ON_UPDATE_COMMAND_UI(ID_POPUP_GRABTOWER, OnUpdatePopupGrabtower)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView32Bit::OnFilePrint)
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapView drawing

void MapView::OnInitialUpdate() 
{
	CScrollView32Bit::OnInitialUpdate();

	updateScrollSizes();
}

void MapView::OnDraw(CDC* pDC)
{
		CBitmap memBitmap; // Offscreen bitmap
	CBitmap* oldBitmap; // bitmap originally found in memDC
	CRect drawRect; // Rectangle of drawing area.

   	//CPaintDC dc(this);
	CDC& dc = *pDC;

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
	doDraw(&memDC);

	// Copy the offscreen bitmap onto the screen.
	dc.BitBlt(drawRect.left, drawRect.top, drawRect.Width(), drawRect.Height(),
						&memDC, drawRect.left, drawRect.top, SRCCOPY);
	
	//Swap back the original bitmap.
	memDC.SelectObject(oldBitmap);

}

void MapView::doDraw(CDC *pDC)
{
	if (_brHatch.m_hObject)
	{
		CRect rect;
		GetClientRect(&rect);
		rect.right  = max(rect.right , m_totalDev.cx);
		rect.bottom = max(rect.bottom, m_totalDev.cy);
		_brHatch.UnrealizeObject();
		CPoint pt(0, 0);
		pDC->LPtoDP(&pt);
		pt = pDC->SetBrushOrg(pt.x % 8, pt.y % 8);
		CBrush* old = pDC->SelectObject(&_brHatch);
		pDC->FillRect(&rect, &_brHatch);
		pDC->SelectObject(old);
	}

	MapCacheItem* mapCacheItem = GetDocument()->getMapCacheItem();

	if (mapCacheItem)
	{
		pDC->SetMapMode(MM_TEXT);
		pDC->SetBkMode(TRANSPARENT);
		
		updateScrollSizes();
		CPoint topLeft = getTopLeft();

		//draw the image
		if (mapManager()->isShowingMap())
		{
			if (mapCacheItem->getMapImage())
			{
				mapCacheItem->getMapImage()->Draw(pDC->m_hDC, topLeft.x+1, topLeft.y+1);
			}
			else
			{
				pDC->FillSolidRect( topLeft.x+1, 
									topLeft.y+1, 
									mapCacheItem->getImageSize().cx, 
									mapCacheItem->getImageSize().cy,
									RGB(255,255,255));

				CFont* oldFont = pDC->SelectObject(&app()->_fontStd);
				pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
				pDC->TextOut(topLeft.x +20,topLeft.y + 20,"Loading Map...");
				pDC->SelectObject(oldFont);

			}
		}
		else
		{
			pDC->FillSolidRect( topLeft.x+1, 
								topLeft.y+1, 
								mapCacheItem->getImageSize().cx, 
								mapCacheItem->getImageSize().cy,
								RGB(240,240,230));
		}

		

		//draw a rectangle
		CPen pen(PS_SOLID, 1, RGB(0,0,0));

		CPen* oldPen = pDC->SelectObject(&pen);

		CSize imageSize = mapCacheItem->getImageSize();

		pDC->MoveTo(topLeft.x,					topLeft.y);
		pDC->LineTo(topLeft.x + imageSize.cx,	topLeft.y);
		pDC->LineTo(topLeft.x + imageSize.cx,	topLeft.y + imageSize.cy);
		pDC->LineTo(topLeft.x,					topLeft.y + imageSize.cy);
		pDC->LineTo(topLeft.x,					topLeft.y);

		pDC->SelectObject(oldPen);

		//draw the pins
		if (mapManager()->isShowingPushPins())
		{
			RecordTowers& towers = GetDocument()->getTowersOnMap();

			for (int i=0;i<towers.GetSize();i++)
			{
				RecordTower* tower = towers.GetAt(i);
				if (tower)
				{
					MapGridRef gridRef = tower->getMapRef();
					if (gridRef.isValidOSGridRef())
					{
						CPoint pos = getPointFromMapGridRef(gridRef);

						drawPin(pDC, pos, tower->hasGrabbed()? mapManager()->getPinColorGrabbed():mapManager()->getPinColorNonGrabbed());
					}
				}			
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// MapView diagnostics

#ifdef _DEBUG
void MapView::AssertValid() const
{
	CScrollView32Bit::AssertValid();
}

void MapView::Dump(CDumpContext& dc) const
{
	CScrollView32Bit::Dump(dc);
}

MapDoc* MapView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(MapDoc)));
	return (MapDoc*)m_pDocument;
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MapView message handlers




void MapView::updateScrollSizes()
{
	SetScrollSizes(MM_TEXT, getSize());	
} 

CSize MapView::getSize()
{
	CSize sizeTotal(100,100);
	
	MapCacheItem* item = GetDocument()->getMapCacheItem();
	if (item)
	{
		sizeTotal = item->getImageSize() +  CSize(2,2);
	}

	return sizeTotal;
}


BOOL MapView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
	
//	return CScrollView32Bit::OnEraseBkgnd(pDC);
}




void MapView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	CPoint copyPoint(point);
	ScreenToClient(&copyPoint);
	GetDocument()->setContextTower(hitTest(copyPoint + GetScrollPosition()));

	theApp.ShowPopupMenu(IDR_MAP, point, this);	
}

void MapView::OnEditCopy() 
{
	//put your text in source
	if (GetDocument()->getMapCacheItem() && 
		GetDocument()->getMapCacheItem()->getMapImage() && 
		OpenClipboard())
	{
		EmptyClipboard();

		//bitmap
		CBitmap* bit = getCopyDataBitmap();
			
		::SetClipboardData (CF_BITMAP, bit->GetSafeHandle() ) ;

		bit->Detach();

		delete bit;

		CloseClipboard();
	}
	
}

void MapView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( GetDocument()->getMapCacheItem() && 
					GetDocument()->getMapCacheItem()->isImageAvailable());	
}

void MapView::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/) 
{
	theApp.OnViewDoubleClick (this, IDR_MAPTYPE);	
}


BOOL MapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return GetDocument()->OnPreparePrinting(pInfo, this);
}

void MapView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->OnBeginPrinting(pDC, pInfo);
	CScrollView32Bit::OnBeginPrinting(pDC, pInfo);
}						 

void MapView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView32Bit::OnPrepareDC(pDC, pInfo);
	GetDocument()->OnPrepareDC(pDC, pInfo);
}

void MapView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->OnEndPrinting(pDC, pInfo);
	CScrollView32Bit::OnEndPrinting(pDC, pInfo);
}

void MapView::OnFilePrintPreview() 
{
	GetDocument()->OnFilePrintPreview(this);
}

void MapView::OnFilePrint() 
{
	GetDocument()->OnFilePrint();
	CScrollView32Bit::OnFilePrint();
}

void MapView::OnFilePrintToolbar() 
{
	GetDocument()->OnFilePrintToolbar();
	CScrollView32Bit::OnFilePrint();	
}

void MapView::OnFilePrintMenu() 
{
	GetDocument()->OnFilePrintMenu();
	CScrollView32Bit::OnFilePrint();
}

void MapView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{	
	doPrintMapView(pDC, pInfo);
}


void MapView::doPrintMapView(CDC* pDC, CPrintInfo* pInfo)
{
	//this gets called for each page in turn
	CPage*	page= new CPage(pInfo->m_rectDraw,pDC,MM_TEXT);

	double nextRow = (double)((double)(app()->GetProfileInt("Print", "Touch Top", 1000))/2540.0);

	nextRow = PrintUtils::printTitle(page, GetDocument()->GetTitle(), nextRow, true);


	PrintData* printData = (PrintData*)pInfo->m_lpUserData;	   
	
	//determine the scales for the print
	CDC* scrnDC = GetDC();

	PrintScaleObject printScaleObject(scrnDC, pDC, page, pInfo, getSize());

	ReleaseDC(scrnDC);

	printData->_numPages = printScaleObject.getNumberOfPages();


	//leave the top gap clear 
	page->ExcludeClipRect(-0.1,
						  -0.1,
						  nextRow, 
						  page->getWidthinches() + 0.1);


	//leave the bottom clear for the footnote
	page->ExcludeClipRect(page->getLengthInches() - PrintUtils::getFootnoteHeight(), 
						  -0.1,
						  page->getLengthInches() + 0.1, 
						  page->getWidthinches() + 0.1);

	//do the actual drawing
	printDraw(pDC, printScaleObject);

	//tidy up 
	page->ResetClipRect();

	CString pageText;

	if (printData->_numPages > 1)
		pageText.Format("Page %d of %d",pInfo->m_nCurPage ,printData->_numPages);
		
	PrintUtils::printFootnote(page, pageText, PrintUtils::getFootnoteHeight());

	//TRACE("Document has %d pages \r\n", printData->_numPages );

	if (!pInfo->m_bPreview)
		PrintUtils::printShareware(page);	

	delete page;

}


void MapView::printDraw(CDC *pDC, PrintScaleObject& printScaleObject)
{
	
	MapCacheItem* item = GetDocument()->getMapCacheItem();
	if (item)
	{
		CxImage* image = item->getMapImage();

		if (image)
		{
			pDC->SetMapMode(MM_TEXT);
			pDC->SetBkMode(TRANSPARENT);

			CSize imageSize = getSize();

			//the offset in y is to allow the clip rect to be missed 
			const int yOffset = 40;

			//works for printing
			CRect rect(printScaleObject.translateX(1),
						printScaleObject.translateY(1+ yOffset),
						printScaleObject.translateX(image->GetWidth() + 1), 
						printScaleObject.translateY(image->GetHeight() + 1+ yOffset));

			image->Draw2(pDC->m_hDC, rect);
  
			//draw a rectangle			   
			CPen pen(PS_SOLID, printScaleObject.scaleXY(1), RGB(0,0,0));

			CPen* oldPen = pDC->SelectObject(&pen);

			pDC->MoveTo(printScaleObject.translateX(0),						printScaleObject.translateY(yOffset));
			pDC->LineTo(printScaleObject.translateX(image->GetWidth()+0),	printScaleObject.translateY(yOffset));
			pDC->LineTo(printScaleObject.translateX(image->GetWidth()+0),	printScaleObject.translateY(image->GetHeight()+yOffset));
			pDC->LineTo(printScaleObject.translateX(0),						printScaleObject.translateY(image->GetHeight()+yOffset));
			pDC->LineTo(printScaleObject.translateX(0),						printScaleObject.translateY(yOffset));
  
			pDC->SelectObject(oldPen);

			//to2do print the pins
		}
	} 
}


MapGridRef MapView::getMapGridRefFromPoint(const CPoint& point)
{
	MapGridRef mapGridRef;
	MapCacheItem* mapCacheItem = GetDocument()->getMapCacheItem();

	if (mapCacheItem)
	{
		CPoint diffPix = point - getTopLeft();

		int eastings  = (int)((double)diffPix.x/mapCacheItem->getEastingsPixPer100M());
		int northings = (int)((double)diffPix.y/mapCacheItem->getNorthingsPixPer100M());

		MapGridRef refBottomLeft = mapCacheItem->getBottomLeft();
		MapGridRef refTopRight = mapCacheItem->getTopRight();

		mapGridRef.setGridRef(refBottomLeft.getEastings() + eastings, refTopRight.getNorthings() - northings);
	}
	
	return mapGridRef;
}

CPoint MapView::getPointFromMapGridRef(const MapGridRef& mapGridref)
{
	CPoint point(0,0);
	MapCacheItem* mapCacheItem = GetDocument()->getMapCacheItem();
	
	if (mapCacheItem)
	{
		point = getTopLeft();

		point.x += (int)((double)mapGridref.getEastingsDiff100M(mapCacheItem->getBottomLeft()) * 
									mapCacheItem->getEastingsPixPer100M() );
		point.y -= (int)((double)mapGridref.getNorthingsDiff100M(mapCacheItem->getTopRight())  * 
									mapCacheItem->getNorthingsPixPer100M());

	}

	return point;

}

CPoint MapView::getTopLeft()
{
	MapCacheItem* item = GetDocument()->getMapCacheItem();
	if (item)
	{
		CSize imageSize = getSize();
		CRect clientRect;
		GetClientRect(clientRect);

		int left = 0;
		if (clientRect.Width() > imageSize.cx)
			left = (clientRect.Width() - imageSize.cx)/2;

		int top = 0;
		if (clientRect.Height() > imageSize.cy)
			top = (clientRect.Height() - imageSize.cy)/2;


		return CPoint(left+1, top+1);
	}
	return CPoint(0,0);
}

void MapView::drawPin(CDC *pDC, CPoint pos, COLORREF color)
{
	COLORREF white = RGB(255,255,255);

	//point
	pDC->SetPixel(pos, color);
	
	pos.y--;
	pDC->SetPixel(pos, color);
	
	pos.y--;
	pDC->SetPixel(pos, color);

	pos.y--;
	pDC->SetPixel(pos, color);
	
	pos.y--;
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	
	pos.y--;
	pos.x+=2;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);

	pos.y--;
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, white);
	pos.x++;
	pDC->SetPixel(pos, white);
	pos.x++;
	pDC->SetPixel(pos, white);
	pos.x++;
	pDC->SetPixel(pos, white);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);

	pos.y--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, white);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, white);
	pos.x--;
	pDC->SetPixel(pos, white);
	pos.x--;
	pDC->SetPixel(pos, color);

	pos.y--;
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, white);
	pos.x++;
	pDC->SetPixel(pos, white);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);

	pos.y--;
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, white);
	pos.x--;
	pDC->SetPixel(pos, white);
	pos.x--;
	pDC->SetPixel(pos, color);

	pos.y--;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	
	pos.y--;
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);

	pos.y--;
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, white);
	pos.x++;
	pDC->SetPixel(pos, white);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);

	pos.y--;
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, white);
	pos.x--;
	pDC->SetPixel(pos, white);
	pos.x--;
	pDC->SetPixel(pos, white);
	pos.x--;
	pDC->SetPixel(pos, color);
	pos.x--;
	pDC->SetPixel(pos, color);

	pos.y--;
	pos.x+=2;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
	pos.x++;
	pDC->SetPixel(pos, color);
}

void MapView::OnShowPushpins() 
{
	mapManager()->toggleShowingPushPins();
}

void MapView::OnUpdateShowPushpins(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(mapManager()->isShowingPushPins());	
}	


void MapView::OnShowMap() 
{
	mapManager()->toggleShowingMap();
}

void MapView::OnUpdateShowMap(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(mapManager()->isShowingMap());	
}


void MapView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (_draggingBackground)
	{
		CRect clientRect;
		GetClientRect(&clientRect);
		CSize size = getSize();
		CPoint backgroundDiff = _lastBackgroundPos - point;
		//only scroll if there is some scrolling to do.
		if (clientRect.Width() > size.cx)
			backgroundDiff.x = 0;
		if (clientRect.Height() > size.cy)
			backgroundDiff.y = 0;

		_lastBackgroundPos = point;

		if (backgroundDiff != CPoint(0,0))
			ScrollToPosition(GetScrollPosition() + backgroundDiff);

		Invalidate();
	}	


	RecordTower* tower = hitTest(point + GetScrollPosition() );

	//tool tips
	_tip.removeAllStrings(); 	

	if (tower)
	{
		CString tipString;
		tipString.Format("%s (%d), %s%s",  tower->getDisplayName(), 
										tower->getNumber(), 
										tower->getDedication(), 
										(tower->getGrabCount() > 0)?", Grabbed":"" );
		_tip.addString(tipString);
		_tip.Set(point);
	}

	//status bar
	CPoint topLeft = getTopLeft();
	MapCacheItem* cacheItem = GetDocument()->getMapCacheItem();

	if ((cacheItem)&&
		(point.x > topLeft.x)&&
		(point.y > topLeft.y)&&
		(point.x < topLeft.x + cacheItem->getImageSize().cx)&&
		(point.y < topLeft.y + cacheItem->getImageSize().cy))
		GetDocument()->setCurrentGridRef(getMapGridRefFromPoint(point));
	else
		GetDocument()->setCurrentGridRef(MapGridRef());
	
	CScrollView32Bit::OnMouseMove(nFlags, point);
}


void MapView::OnLButtonDown(UINT nFlags, CPoint point) 
{

	RecordTower* tower = hitTest(point + GetScrollPosition());

	_draggingBackground = false;

	if (tower)
		GetDocument()->setSelectedTowerIndex(tower->getIndex());
	else
	{
		_draggingBackground = true;
		SetCapture();

		CRect windowRect;
		GetWindowRect(windowRect);
		//do not let mouse leave the list box boundary
		::ClipCursor(windowRect);

		_lastBackgroundPos = point;

		Invalidate();
	}
	
	CScrollView32Bit::OnLButtonDown(nFlags, point);
}

	
void MapView::OnLButtonUp(UINT nFlags, CPoint point)
{
	_draggingBackground = false;
	//if mouse was captured then release it
	if (GetCapture()==this)
		::ReleaseCapture();

	::ClipCursor(NULL);

	Invalidate();


	CScrollView32Bit::OnLButtonUp(nFlags, point);
}
	
RecordTower* MapView::hitTest(CPoint point)
{
	RecordTowers& towers = GetDocument()->getTowersOnMap();

	RecordTower* closestTower = NULL;
	int closestDistance = 20; //set the range in picels here

	for (int i=0;i<towers.GetSize();i++)
	{
		RecordTower* tower = towers.GetAt(i);
		if (tower)
		{
			MapGridRef gridRef = tower->getMapRef();
			if (gridRef.isValidOSGridRef())
			{
				CPoint pos = getPointFromMapGridRef(gridRef);

				CSize diff = (pos- point);
				int iDiff = (int)sqrt((double)((diff.cx*diff.cx)+(diff.cy*diff.cy)));

				if (iDiff < closestDistance)
				{
					closestTower = tower;
					closestDistance = iDiff;
				}
			}
		}			
	}

	return closestTower;
}

int MapView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView32Bit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	_tip.Create(this);		
	
	return 0;
}

void MapView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}


void MapView::OnUpdateFilePrintMenu(CCmdUI* pCmdUI) 
{
	MapCacheItem* mapCacheItem = GetDocument()->getMapCacheItem();

	pCmdUI->Enable((mapCacheItem)&&(mapCacheItem->isImageAvailable()));	
}

void MapView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	MapCacheItem* mapCacheItem = GetDocument()->getMapCacheItem();

	pCmdUI->Enable((mapCacheItem)&&(mapCacheItem->isImageAvailable()));	
}

void MapView::OnUpdateFilePrintToolbar(CCmdUI* pCmdUI) 
{
	MapCacheItem* mapCacheItem = GetDocument()->getMapCacheItem();

	pCmdUI->Enable((mapCacheItem)&&(mapCacheItem->isImageAvailable()));	
}


void MapView::OnPopupGrabtower() 
{
	RecordTower* recordTower =  GetDocument()->getContextTower();
	if (recordTower)
		recordManager()->createGrab(recordTower->getIndex());
}

void MapView::OnUpdatePopupGrabtower(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->getContextTower() != NULL);	
}

CBitmap* MapView::getCopyDataBitmap()
{
	CBitmap* memBitmap = new CBitmap(); //my new bitmap
	CBitmap* oldBitmap = NULL; // bitmap originally found in memDC

   	//device contexts
	CDC* pDC = GetDC();
	CDC bitmapDC;

	// Create a Bitmap DC
	bitmapDC.CreateCompatibleDC(pDC);
	
	//set the map mode
	pDC->SetMapMode(MM_TEXT);
	bitmapDC.SetMapMode(MM_TEXT);

	//get the whole client size and offset to paint the correct area
	CSize size = getSize() - CSize(1,1);

	//create a bitmap, and select into the memDC
	memBitmap->CreateCompatibleBitmap(pDC, size.cx, size.cy);
	oldBitmap = bitmapDC.SelectObject(memBitmap);
	
	// by default shift viewport origins in negative direction of scroll
	bitmapDC.SetViewportOrg(-getTopLeft());
//	pDC->SetViewportOrg(-GetScrollPosition());
	//I have left this code here for when we have the ability to copy a sub set of the image

	
	//erase the background
    bitmapDC.FillSolidRect(0,0,size.cx, size.cy, GetSysColor(COLOR_WINDOW));  

	//perform the GDI calls
	doDraw(&bitmapDC);

	
	//Swap back the original bitmap.
	bitmapDC.SelectObject(oldBitmap);

	ReleaseDC(pDC);

	return memBitmap;
}

CxImage* MapView::getImage()
{

	CxImage* image = new CxImage();

	CBitmap* bit = getCopyDataBitmap();
		
	image->CreateFromHBITMAP((HBITMAP)bit->GetSafeHandle());

	bit->Detach();

	delete bit;		

	return image;

}

BOOL MapView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if ((_draggingBackground)||( _mapViewHitTest == mvht_map))
	{
		SetCursor(app()->LoadCursor(IDC_FPOINT));
		return TRUE;
	}
	else if (_mapViewHitTest == mvht_tower)
	{
		SetCursor(app()->LoadStandardCursor(IDC_CROSS));
		return TRUE;
	}

	return CScrollView32Bit::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT MapView::OnNcHitTest(CPoint point)
{
	const UINT nHitTest = CScrollView32Bit::OnNcHitTest(point);

	ScreenToClient(&point);
	point += GetScrollPosition();
	_mapViewHitTest = mvht_none;

	if (nHitTest == HTCLIENT)
	{
		RecordTower* tower = hitTest(point);	

		if (tower)
			_mapViewHitTest = mvht_tower;
		else
			_mapViewHitTest = mvht_map; //to2do test for the other states.
	}

	return nHitTest;
}

