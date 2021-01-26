// InternetView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "InternetView.h"
#include "InternetDoc.h"
#include "StatusBar.h"
#include "InternetDocumentInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// InternetView

IMPLEMENT_DYNCREATE(InternetView, CHtmlView)

InternetView::InternetView()
{
	//{{AFX_DATA_INIT(InternetView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

InternetView::~InternetView()
{
}

void InternetView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(InternetView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(InternetView, CHtmlView)
	//{{AFX_MSG_MAP(InternetView)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_VIEW_FONTS_LARGEST, OnViewFontsLargest)
	ON_COMMAND(ID_VIEW_FONTS_LARGE, OnViewFontsLarge)
	ON_COMMAND(ID_VIEW_FONTS_MEDIUM, OnViewFontsMedium)
	ON_COMMAND(ID_VIEW_FONTS_SMALL, OnViewFontsSmall)
	ON_COMMAND(ID_VIEW_FONTS_SMALLEST, OnViewFontsSmallest)
	ON_UPDATE_COMMAND_UI(ID_GO_FORWARD, OnUpdateGoForward)
	ON_UPDATE_COMMAND_UI(ID_GO_BACK, OnUpdateGoBack)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT_MENU, CHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_TOOLBAR, CHtmlView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InternetView diagnostics

#ifdef _DEBUG
void InternetView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void InternetView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}




InternetDoc* InternetView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(InternetDoc)));
	return (InternetDoc*)m_pDocument;
}

#endif //_DEBUG	 

/////////////////////////////////////////////////////////////////////////////
// InternetView message handlers

void InternetView::OnInitialUpdate() 
{
}

void InternetView::OnDraw(CDC* /*pDC*/)
{
}

void InternetView::setInitialData(CString URL)
{
    Navigate2(URL);
}

BOOL InternetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void InternetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void InternetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// InternetView diagnostics

/////////////////////////////////////////////////////////////////////////////
// InternetView message handlers
// these are all simple one-liners to do simple controlling of the browser
void InternetView::OnGoBack()
{
	InternetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	HistoryObj* pHistoryObj = pDoc->GoBack ();
	if (pHistoryObj != NULL)
	{
		Navigate2 (pHistoryObj->GetURL (), 0, NULL);
	}
}

void InternetView::OnGoForward()
{
//	GoForward();
	InternetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	HistoryObj* pHistoryObj = pDoc->GoForward ();
	if (pHistoryObj != NULL)
	{
		Navigate2 (pHistoryObj->GetURL (), 0, NULL);
	}
}

void InternetView::OnGoSearchTheWeb()
{
	GoSearch();
}

void InternetView::OnGoStartPage()
{
	GoHome();
}

void InternetView::OnViewStop()
{
	Stop();
}

void InternetView::OnViewRefresh()
{
	Refresh();
}



// these functions control the font size.  There is no explicit command in the
// CHtmlView class to do this, but we can do it by using the ExecWB() function.
void InternetView::OnViewFontsLargest()
{
	COleVariant vaZoomFactor(4l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void InternetView::OnViewFontsLarge()
{
	COleVariant vaZoomFactor(3l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void InternetView::OnViewFontsMedium()
{
	COleVariant vaZoomFactor(2l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void InternetView::OnViewFontsSmall()
{
	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void InternetView::OnViewFontsSmallest()
{
	COleVariant vaZoomFactor(0l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void InternetView::OnUpdateGoForward(CCmdUI* pCmdUI) 
{
	InternetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable (pDoc->IsFrwdAvailable ());
}

void InternetView::OnUpdateGoBack(CCmdUI* pCmdUI) 
{
	InternetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable (pDoc->IsBackAvailable ());
}

void InternetView::OnDocumentComplete(LPCTSTR lpszURL) 
{
	// make sure the main frame has the new URL.  This call also stops the animation
	mainFrame()->SetAddress(lpszURL);
	statusBar()->stopInternetAnimation();

	InternetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->AddURLToHistory (GetLocationName (), lpszURL);
	statusBar()->SetProgress (0, -1);	
}

void InternetView::OnTitleChange(LPCTSTR lpszText) 
{
	// this will change the main frame's title bar
	if (m_pDocument != NULL)
		m_pDocument->SetTitle(lpszText);
}

void InternetView::OnBeforeNavigate2(LPCTSTR /*lpszURL*/, DWORD /*nFlags*/, LPCTSTR /*lpszTargetFrameName*/, CByteArray& /*baPostedData*/, LPCTSTR /*lpszHeaders*/, BOOL* /*pbCancel*/) 
{
	// start the animation so that is plays while the new page is being loaded
	statusBar()->startInternetAnimation();
}


void InternetView::OnProgressChange(long nProgress, long nProgressMax) 
{
	statusBar()->SetProgress(nProgress, nProgressMax);

	CHtmlView::OnProgressChange(nProgress, nProgressMax);
}

void InternetView::OnDestroy() 
{
	CHtmlView::OnDestroy();
	CView::OnDestroy();		// Fixes CHtmlView bug
}

void InternetView::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/) 
{
theApp.OnViewDoubleClick (this, IDR_INTERNETTYPE);	
}

void InternetView::OnEditCopy() 
{
    ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL); 
}



void InternetView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);

}

void InternetView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	
}

InternetDocumentInfo* InternetView::getInternetDocumentInfo()
{
	return new InternetDocumentInfo(GetLocationURL());	
}
