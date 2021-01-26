// ribView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "CribView.h"		   
#include "UserMessages.h"	
#include "MethodPosition.h"
#include "CPage.h"
#include "MethodWndPrintCrib.h"
#include "PrintData.h"
#include "PrintUtils.h"
#include "Method.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CribView

IMPLEMENT_DYNCREATE(CribView, CView)

BEGIN_MESSAGE_MAP(CribView, CView)
	//{{AFX_MSG_MAP(CribView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_TOOLS_STYLE, OnToolsStyle)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_TOOLBAR, OnFilePrintToolbar)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	
	ON_MESSAGE(WM_STYLE_CHANGED,OnStyleChanged)
	ON_MESSAGE(WM_SET_METHOD_POS,OnSetMethodPos)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CribView construction/destruction

CribView::CribView()
{

}

CribView::~CribView()
{
}

BOOL CribView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CribView drawing

//need to leave this function - as it auto invalidated the CDC
void CribView::OnDraw(CDC* /*pDC*/)
{


}

void CribView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP ;

	

	_methodWnd.Create(NULL, "", 
					dwStyle, CRect(0,0,0,0),  this, AAA_METHOD_WND, GetDocument()->getMethod()); 

	
	if (IsWindow(_methodWnd))  
	{
		CRect clientRect;
		GetClientRect(clientRect);
		_methodWnd.MoveWindow(0, 0, clientRect.Width(), clientRect.Height());	
	}

	Invalidate();	

}

/////////////////////////////////////////////////////////////////////////////
// CribView printing


BOOL CribView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return GetDocument()->OnPreparePrinting(pInfo, this);
}

void CribView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{	
	GetDocument()->OnBeginPrinting(pDC, pInfo);
	CView::OnBeginPrinting(pDC, pInfo);
}

void CribView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnPrepareDC(pDC, pInfo);
	GetDocument()->OnPrepareDC(pDC, pInfo);	
}


void CribView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->OnEndPrinting(pDC, pInfo);
	CView::OnEndPrinting(pDC, pInfo);
}

void CribView::OnFilePrintPreview() 
{
	GetDocument()->OnFilePrintPreview(this);
}

void CribView::OnFilePrint() 
{
	GetDocument()->OnFilePrint();
	CView::OnFilePrint();
}

void CribView::OnFilePrintToolbar() 
{
	GetDocument()->OnFilePrintToolbar();
	CView::OnFilePrint();
}

void CribView::OnFilePrintMenu() 
{
	GetDocument()->OnFilePrintMenu();
	CView::OnFilePrint();
}		 


void CribView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	doPrintCrib(pDC, pInfo);
}


MethodWndPrintCrib* CribView::getMethodWndPrint(CDC* pDC, CPage* page, CPrintInfo* pInfo) 
{
	PrintData* printData = (PrintData*)pInfo->m_lpUserData;	   

	delete printData->_methodWndPrt;
	printData->_methodWndPrt = 0;

	//determine the scales for the print
	CDC* scrnDC = GetDC();
	float pixPerInchXScrn = (float)scrnDC->GetDeviceCaps(LOGPIXELSX);
	float pixPerInchYScrn = (float)scrnDC->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(scrnDC);
	float pixPerInchXPrint = (float)pDC->GetDeviceCaps(LOGPIXELSX);
	float pixPerInchYPrint = (float)pDC->GetDeviceCaps(LOGPIXELSY);

	//calculate the page size
	double topGap = (double)((double)(app()->GetProfileInt("Print", "Touch Top", 1000))/2540.0);	
	double dVertPageSize = page->getLengthInches() - PrintUtils::getFootnoteHeight() - topGap;
	int iVertPageSize = page->ConvertToMappedUnits(dVertPageSize, VERTRES);

	double leftGap = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	
	double dHorzPageSize = page->getWidthinches() - leftGap;
	int iHorzPageSize = page->ConvertToMappedUnits(dHorzPageSize, HORZRES);

	BOOL wholeColumns = app()->GetProfileInt("Print", "Method Columns", TRUE);

	printData->_methodWndPrt = new MethodWndPrintCrib(
							pDC,
							GetDocument()->getMethod(), 
							getStyle(), 
							pixPerInchXPrint/pixPerInchXScrn, //X scale
							pixPerInchYPrint/pixPerInchYScrn,
							iHorzPageSize,
							iVertPageSize,
							page->ConvertToMappedUnits(leftGap,HORZRES),
							page->ConvertToMappedUnits(topGap,VERTRES),
							wholeColumns); 

	printData->_numPages = printData->_methodWndPrt->getNumberOfPages();

	return 	dynamic_cast<MethodWndPrintCrib*>(printData->_methodWndPrt);
}


void CribView::doPrintCrib(CDC* pDC, CPrintInfo* pInfo)
{
	//this gets called for each page in turn
	CPage*	page= new CPage(pInfo->m_rectDraw,pDC,MM_TEXT);

	MethodWndPrint* metPrint = getMethodWndPrint(pDC, page, pInfo);
	
	PrintData* printData = (PrintData*)pInfo->m_lpUserData;	   

	//leave the top gap clear 
	page->ExcludeClipRect(-0.1,
						  -0.1,
						  (double)((double)(app()->GetProfileInt("Print", "Touch Top", 1000))/2540.0), 
						  page->getWidthinches() + 0.1);


	//leave the bottom clear for the footnote
	page->ExcludeClipRect(page->getLengthInches() - PrintUtils::getFootnoteHeight(), 
						  -0.1,
						  page->getLengthInches() + 0.1, 
						  page->getWidthinches() + 0.1);

	//do the actual drawing
	metPrint->OnDraw(pDC, pInfo->m_nCurPage);

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

/////////////////////////////////////////////////////////////////////////////
// CribView diagnostics

#ifdef _DEBUG
void CribView::AssertValid() const
{
	CView::AssertValid();
}

void CribView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CribDoc* CribView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CribDoc)));
	return (CribDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CribView message handlers

void CribView::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/) 
{
	theApp.OnViewDoubleClick (this, IDR_CRIBTYPE);	
	
}

BOOL CribView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
//	return CView::OnEraseBkgnd(pDC);
}

void CribView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	
	if (IsWindow(_methodWnd))  
	{
		_methodWnd.MoveWindow(0, 0, cx, cy);	
	}
}

void CribView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) 
{
	_methodWnd.setMethod(GetDocument()->getMethod());
}

LRESULT CribView::OnSetMethodPos(WPARAM wParam, LPARAM lParam)
{
	MethodPosition * pos = (MethodPosition*)lParam;

	if (wParam == TRUE)
		GetDocument()->setMethodPositionStatusBar(pos);
	else
	{
		GetDocument()->setMethodPositionStatusBar(NULL);
		delete pos;
	}
	return 0;
}

LRESULT CribView::OnStyleChanged(WPARAM /*wParam*/, LPARAM /*lParam*/)
{	
	GetDocument()->setUpdatePoint("Style Change");
	return 0;
}

const Style& CribView::getStyle()
{
	return _methodWnd._style;
}

void CribView::setStyle(const Style &style)
{
	_methodWnd._style = style;
}



void CribView::OnToolsStyle() 
{
	_methodWnd.OnToolsStyle();	
}



void CribView::OnEditCopy() 
{
	_methodWnd.OnEditCopy();	
}

void CribView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	_methodWnd.OnUpdateEditCopy(pCmdUI);	
}

CxImage* CribView::getImage()
{
	CxImage* image = new CxImage();

	CBitmap* bit = _methodWnd.getCopyDataBitmap();
		
	image->CreateFromHBITMAP((HBITMAP)bit->GetSafeHandle());

	bit->Detach();

	delete bit;		

	return image;

}

MethodCribWnd& CribView::getMethodWnd()
{
	return _methodWnd;
}



