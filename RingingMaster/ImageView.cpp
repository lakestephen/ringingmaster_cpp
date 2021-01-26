// ImageView.cpp : implementation of the ImageView class
//

#include "stdafx.h"
#include "RingingMaster.h"

#include "ImageDoc.h"
#include "ImageView.h"
#include "CPage.h"
#include "PrintUtils.h"
#include "PrintData.h"
#include "PrintScaleObject.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ImageView

IMPLEMENT_DYNCREATE(ImageView, CScrollView32Bit)

BEGIN_MESSAGE_MAP(ImageView, CScrollView32Bit)
	//{{AFX_MSG_MAP(ImageView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_TOOLBAR, OnFilePrintToolbar)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_MENU, OnUpdateFilePrintMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_TOOLBAR, OnUpdateFilePrintToolbar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ImageView construction/destruction

ImageView::ImageView()
{
   VERIFY(_brHatch.CreateHatchBrush(HS_DIAGCROSS, RGB(191, 191, 191)));
}

ImageView::~ImageView()
{
}

BOOL ImageView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CScrollView32Bit::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// ImageView drawing

void ImageView::OnDraw(CDC* pDC)
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

/////////////////////////////////////////////////////////////////////////////
// ImageView printing
BOOL ImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return GetDocument()->OnPreparePrinting(pInfo, this);
}

void ImageView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->OnBeginPrinting(pDC, pInfo);
	CScrollView32Bit::OnBeginPrinting(pDC, pInfo);
}						 

void ImageView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView32Bit::OnPrepareDC(pDC, pInfo);
	GetDocument()->OnPrepareDC(pDC, pInfo);
}

void ImageView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->OnEndPrinting(pDC, pInfo);
	CScrollView32Bit::OnEndPrinting(pDC, pInfo);
}

void ImageView::OnFilePrintPreview() 
{
	GetDocument()->OnFilePrintPreview(this);
}

void ImageView::OnFilePrint() 
{
	GetDocument()->OnFilePrint();
	CScrollView32Bit::OnFilePrint();
}

void ImageView::OnFilePrintToolbar() 
{
	GetDocument()->OnFilePrintToolbar();
	CScrollView32Bit::OnFilePrint();	
}

void ImageView::OnFilePrintMenu() 
{
	GetDocument()->OnFilePrintMenu();
	CScrollView32Bit::OnFilePrint();
}

void ImageView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{	
	doPrintImageView(pDC, pInfo);
}

void ImageView::doPrintImageView(CDC* pDC, CPrintInfo* pInfo)
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


void ImageView::printDraw(CDC *pDC, PrintScaleObject& printScaleObject)
{
	
	CxImage* image = GetDocument()->getImage();

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

	}
}



/////////////////////////////////////////////////////////////////////////////
// ImageView diagnostics

#ifdef _DEBUG
void ImageView::AssertValid() const
{
	CScrollView32Bit::AssertValid();
}

void ImageView::Dump(CDumpContext& dc) const
{
	CScrollView32Bit::Dump(dc);
}

ImageDoc* ImageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ImageDoc)));
	return (ImageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ImageView message handlers

void ImageView::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/) 
{
	theApp.OnViewDoubleClick (this, IDR_IMAGETYPE);		
}

void ImageView::OnInitialUpdate() 
{
	CScrollView32Bit::OnInitialUpdate();
	
	updateScrollSizes();
	
}

void ImageView::updateScrollSizes()
{
	SetScrollSizes(MM_TEXT, getSize());	
} 


CSize ImageView::getSize()
{
	CSize sizeTotal(100,100);
	
	CxImage* image = GetDocument()->getImage();
	float zoom = GetDocument()->GetZoomFactor();
	if (image)
	{
		CSize imageSize((int)((float)image->GetWidth()*zoom) , (int)((float)image->GetHeight()*zoom));

		sizeTotal = imageSize +  CSize(2,2);
	}

	return sizeTotal;
}



void ImageView::doDraw(CDC *pDC)
{
	CxImage* image = GetDocument()->getImage();

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


	if (image)
	{
		pDC->SetMapMode(MM_TEXT);
		pDC->SetBkMode(TRANSPARENT);
		
		updateScrollSizes();
		CPoint topLeft = getTopLeft();

		//draw the image
//		image->Draw(pDC->m_hDC, topLeft.x+1, topLeft.y+1);

		float zoom=GetDocument()->GetZoomFactor();
		image->Draw(pDC->GetSafeHdc(),
			CRect(0,0,(int)(image->GetWidth()*zoom),(int)(image->GetHeight()*zoom)));

	}
}

BOOL ImageView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;	
	
	//return CScrollView32Bit::OnEraseBkgnd(pDC);
}	

CPoint ImageView::getTopLeft()
{
	CxImage* image = GetDocument()->getImage();
	if (image)
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

void ImageView::OnEditCopy() 
{
	//put your text in source
	CxImage* image = GetDocument()->getImage();

	if (image && OpenClipboard())
	{
		EmptyClipboard();

		//bitmap
		HANDLE hDIB = image->CopyToHandle();
		
		::SetClipboardData (CF_DIB,hDIB ) ;

		CloseClipboard();
	}		
}


void ImageView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->getImage() != NULL);	
}

void ImageView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}
	
	CPoint hitPoint = point;
	ScreenToClient(&hitPoint);

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_IMAGE_VIEW ));
	

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, FALSE);		
}

void ImageView::OnUpdateFilePrintMenu(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->getImage() != NULL);	
}

void ImageView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->getImage() != NULL);	
}

void ImageView::OnUpdateFilePrintToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->getImage() != NULL);	
}



