// TouchBlueLineView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchBlueLineView.h"
#include "MethodPosition.h"
#include "Lead.h"
#include "UserMessages.h"		 
#include "PrintData.h"
#include "PrintUtils.h"
#include "MethodWndPrint.h"
#include "Row.h"






#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchBlueLineView

const UINT TouchBlueLineView::_findDialogMessage = RegisterWindowMessage(FINDMSGSTRING);

IMPLEMENT_DYNCREATE(TouchBlueLineView, CView)

TouchBlueLineView::TouchBlueLineView() :
_pFindDialog(NULL)
{
}

TouchBlueLineView::~TouchBlueLineView()
{
}


BEGIN_MESSAGE_MAP(TouchBlueLineView, CView)
	//{{AFX_MSG_MAP(TouchBlueLineView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_TOOLS_STYLE, OnUpdateToolsStyle)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_TOOLBAR, OnFilePrintToolbar)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_STYLE_MENU_SAVE_AS, OnUpdateStyleMenuSaveAs)
	ON_COMMAND(ID_TOOLS_STYLE, OnToolsStyle)
	ON_COMMAND(ID_STYLE_MENU_SAVE_AS, OnStyleMenuSaveAs)
	ON_COMMAND(ID_STYLE_MENU_MORE, OnStyleMenuMore)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_FIND_PREVIOUS, OnEditFindPrevious)
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DO_CONVERT_TO_LEAD_BASED,OnDoConvertToLeadBased)
	ON_MESSAGE(WM_SET_METHOD_POS,OnSetMethodPos)
	ON_MESSAGE(WM_STYLE_CHANGED,OnStyleChanged)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_STYLE_MENU_00, ID_STYLE_MENU_09, OnApplyStyle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_STYLE_MENU_00, ID_STYLE_MENU_09, OnUpdateToolsStyle)

    ON_REGISTERED_MESSAGE(_findDialogMessage, OnFindDialogMessage)

END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// TouchBlueLineView drawing

//need to leave this function - as it auto invalidated the CDC
void TouchBlueLineView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// TouchBlueLineView printing

BOOL TouchBlueLineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return GetDocument()->OnPreparePrinting(pInfo, this);
}

void TouchBlueLineView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{	
	GetDocument()->OnBeginPrinting(pDC, pInfo);
	CView::OnBeginPrinting(pDC, pInfo);
}

void TouchBlueLineView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnPrepareDC(pDC, pInfo);
	GetDocument()->OnPrepareDC(pDC, pInfo);	
}


void TouchBlueLineView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->OnEndPrinting(pDC, pInfo);
	CView::OnEndPrinting(pDC, pInfo);
}

void TouchBlueLineView::OnFilePrintPreview() 
{
	GetDocument()->OnFilePrintPreview(this);
}

void TouchBlueLineView::OnFilePrint() 
{
	GetDocument()->OnFilePrint();
	CView::OnFilePrint();
}

void TouchBlueLineView::OnFilePrintToolbar() 
{
	GetDocument()->OnFilePrintToolbar();
	CView::OnFilePrint();
}

void TouchBlueLineView::OnFilePrintMenu() 
{
	GetDocument()->OnFilePrintMenu();
	CView::OnFilePrint();
}

void TouchBlueLineView::OnPrintAnalysis() 
{
	int ID = GetDocument()->OnPrintAnalysis();

	if (ID == ID_PRINT)
		CView::OnFilePrint();
	else if (ID == ID_PREVIEW)
		BCGPPrintPreview (this);
}		 

void TouchBlueLineView::OnPopupPrintAnalysis() 
{
	GetDocument()->OnPopupPrintAnalysis();

#ifdef _DEBUG	 //always go to print preview in debug
	BCGPPrintPreview (this);
#else
	CView::OnFilePrint();
#endif
	
}		   

void TouchBlueLineView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	if (GetDocument()->isPrintAnalysis())
		GetDocument()->doPrintAnalysis(pDC, pInfo); 
	else
		doPrintBlueLineView(pDC, pInfo);
}

MethodWndPrint* TouchBlueLineView::getMethodWndPrint(CDC* pDC, CPage* page, CPrintInfo* pInfo) 
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

	printData->_methodWndPrt = new MethodWndPrint(
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

	return 	printData->_methodWndPrt;
}

void TouchBlueLineView::doPrintBlueLineView(CDC* pDC, CPrintInfo* pInfo)
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
// TouchBlueLineView diagnostics

#ifdef _DEBUG
void TouchBlueLineView::AssertValid() const
{
	CView::AssertValid();
}

void TouchBlueLineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

TouchDoc* TouchBlueLineView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(TouchDoc)));
	return (TouchDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TouchBlueLineView message handlers

void TouchBlueLineView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (IsWindow(_methodWnd))  
		_methodWnd.MoveWindow(0, 0, cx, cy);	
}

BOOL TouchBlueLineView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;	
}

void TouchBlueLineView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) 
{
	_methodWnd.setMethod(GetDocument()->getMethod());
}


LRESULT TouchBlueLineView::OnSetMethodPos(WPARAM wParam, LPARAM lParam)
{
	MethodPosition * pos = (MethodPosition*)lParam;

	if (wParam == TRUE)
		GetDocument()->setMethodPositionBlueLineStatusBar(pos);
	else
	{
		GetDocument()->setMethodPositionBlueLineStatusBar(NULL);
		delete pos;
	}

	return 0;
}

void TouchBlueLineView::OnUpdateToolsStyle(CCmdUI* pCmdUI) 
{
	_methodWnd.OnUpdateToolsStyle(pCmdUI);	
}

void TouchBlueLineView::OnToolsStyle() 
{
	_methodWnd.OnToolsStyle();	
}

void TouchBlueLineView::OnUpdateStyleMenuSaveAs(CCmdUI* pCmdUI) 
{
	_methodWnd.OnUpdateStyleMenuSaveAs(pCmdUI);
}

void TouchBlueLineView::OnApplyStyle(UINT nID) 
{
	_methodWnd.OnApplyStyle(nID);
}

void TouchBlueLineView::OnStyleMenuSaveAs() 
{
	_methodWnd.OnStyleMenuSaveAs();
}

void TouchBlueLineView::OnStyleMenuMore() 
{
	_methodWnd.OnStyleMenuMore();	
}

BOOL TouchBlueLineView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	return _methodWnd.OnMouseWheel(nFlags, zDelta, pt);
}

void TouchBlueLineView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	GetDocument()->setTab(1);

	//remove the find dialog box.
	if ((_pFindDialog != NULL)&&
		((pActivateView != this)||
		 (!bActivate)))
	{
		_pFindDialog->DestroyWindow();
		_pFindDialog = NULL;
	}
	

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void TouchBlueLineView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP ;

	_methodWnd.Create(NULL, "", 
					dwStyle, CRect(0,0,0,0),  this, AAA_METHOD_WND, GetDocument()->getMethod()); 

	Invalidate();	
}	  

//this is the normal message handler
void TouchBlueLineView::OnEditFind() 
{
	if (_pFindDialog == NULL)
	{
		_pFindDialog = new CFindReplaceDialog;
		Row* row = _methodWnd.getHilightRow();
		if ((row)&&(!row->getAsText().IsEmpty()))
		{
			_searchStr = row->getAsText();
		}
		_pFindDialog->Create( TRUE, _searchStr, NULL, FR_DOWN | FR_HIDEUPDOWN | FR_HIDEWHOLEWORD | FR_HIDEMATCHCASE, this);	
	}
	else
	{
		_pFindDialog->SetFocus();
	}
}

void TouchBlueLineView::OnEditFindPrevious() 
{
	if (!_searchStr.IsEmpty())
		doFind(_searchStr, true);
	
}

void TouchBlueLineView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((_pFindDialog != NULL)?1:0);
	pCmdUI->Enable(GetDocument()->getMethod() != NULL);
}

//this is the callback handler
LRESULT TouchBlueLineView::OnFindDialogMessage(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    ASSERT(_pFindDialog != NULL);

    // If the FR_DIALOGTERM flag is set,
    // invalidate the handle identifying the dialog box.
    if (_pFindDialog->IsTerminating())
    {
        _pFindDialog = NULL;
        return 0;
    }

    // If the FR_FINDNEXT flag is set,
    // call the application-defined search routine
    // to search for the requested string.
    if(_pFindDialog->FindNext())
    {
        //read data from dialog
        CString findName = _pFindDialog->GetFindString();
		_searchStr = findName;
        bool bSearchDown = _pFindDialog->SearchDown() == TRUE;

        //with given name do search
        doFind(findName, bSearchDown);
    }

    return 0;
}

void TouchBlueLineView::doFind(CString findText, bool /*searchDown*/)
{
	/* todo	can we use the row looping?
	if (GetDocument()->getMethod() != NULL)
	{
		CWaitCursor wait;
		Method* met = GetDocument()->getMethod();
		int iStart=0;
		int jStart=0;
		Row* row = NULL;
		Lead* lead = NULL;
		bool firstRow = false;;// if we are starting from the start, then dont ignore the first row

		//attempt to find the hilight row
		// dont worry about the last row - if it is that, 
		// the search should start from the start anyhow
		Row* hilightRow =  _methodWnd.getHilightRow();
		
		if (hilightRow)
		{
			for (int i=0;i<met->GetSize();i++)
			{
				lead = met->GetAt(i);
				for (int j=0;j<lead->GetSize();j++) // miss off the last row
				{
					row = lead->GetAt(j);
					if (row == hilightRow)
					{
						iStart = i;
						jStart = j;
						firstRow = true;
						break; // only gets us out of the first loop
					}
				}
				if (iStart != 0)
					break;
			}
		}

		int count = 0;
		Lead * lastLead = met->GetAt(met->GetUpperBound());
		Row* lastRow = lastLead->GetAt(lastLead->GetUpperBound());
		int size = lastRow->getRowIndex();
		

		while (count < size + 100)
		{
			for (int i=iStart;i<met->GetSize();i++)
			{
				Lead* lead = met->GetAt(i);
				for (int j=jStart;j<lead->GetUpperBound();j++) // miss off the last row
				{
					Row* row = lead->GetAt(j);
					CString rowStr = row->getAsText();
					
					if ((!firstRow)&&(rowStr.Find(findText) != -1))
					{
						_methodWnd.hilightRow(row);	
						return;
					}
					firstRow = false;
					count++;
				}
				jStart = 0;
			}
			iStart = 0;

			//do the last row
			CString rowStr = lastRow->getAsText();
			if ((!firstRow)&&(rowStr.Find(findText) != -1))
			{
				_methodWnd.hilightRow(lastRow);	
				return;
			}
			count++;
			firstRow = false;
		}
	}
		  */ 
	AfxMessageBox(findText + " not found");
}

void TouchBlueLineView::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/) 
{
	theApp.OnViewDoubleClick (this, IDR_TOUCHTYPE);	
}

LRESULT TouchBlueLineView::OnDoConvertToLeadBased(WPARAM /*wParam*/, LPARAM /*lParam*/)
{ 
	GetDocument()->OnToolsConvertoleadbaseFinish(FALSE); //to2do why is the is in the view
	return 0;
}

const Style& TouchBlueLineView::getStyle()
{
	return _methodWnd._style;
}

void TouchBlueLineView::setStyle(const Style &style)
{
	_methodWnd._style = style;
}

LRESULT TouchBlueLineView::OnStyleChanged(WPARAM /*wParam*/, LPARAM /*lParam*/)
{	
	if (app()->GetProfileInt("Style", "Save With Method", TRUE))
		GetDocument()->setUpdatePoint("Style Change", true, true, false, false);
	return 0;
}

void TouchBlueLineView::OnEditCopy() 
{
	_methodWnd.OnEditCopy();
}

void TouchBlueLineView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	_methodWnd.OnUpdateEditCopy(pCmdUI);
}

void TouchBlueLineView::OnDestroy() 
{
	if (_pFindDialog != NULL)
	{
		_pFindDialog->DestroyWindow();
		_pFindDialog = NULL;
	}

	CView::OnDestroy();
}

CxImage* TouchBlueLineView::getImage()
{
	return _methodWnd.getImage();
}

MethodWndBlueLine& TouchBlueLineView::getMethodWnd()
{
	return _methodWnd;
}

