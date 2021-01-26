// TouchAnalysisSplice.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisSplice.h"
#include "TouchDoc.h"
#include "Method.h"
#include "PropItem.h"
#include "SpliceCountPair.h"
#include "TouchHoldView.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisSplice

TouchAnalysisSplice::TouchAnalysisSplice()
{

}

TouchAnalysisSplice::~TouchAnalysisSplice()
{

}


BEGIN_MESSAGE_MAP(TouchAnalysisSplice, PropList)
	//{{AFX_MSG_MAP(TouchAnalysisSplice)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisSplice message handlers

BOOL TouchAnalysisSplice::Create(CWnd * pParentWnd)
{
	BOOL result = PropList::Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|
								LBS_OWNERDRAWVARIABLE|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_HASSTRINGS, 
								CRect(0,0,0,0),
								pParentWnd,
								AAA_ANALYSIS_SPLICE);	

	_devider[0] = 150;	

	if (result)
		SetFont(&app()->_fontStd);

	mainFrame()->addEventListener(this);

	return  result;
}

BOOL TouchAnalysisSplice::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}

void TouchAnalysisSplice::mainFrame_notifyUpdate(ViewType viewType)
{
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)) 
		return;

	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
	
	CString str;
	
	deleteAllPropItems();
				 /* todo
	//we retrive the method names.
	if ((pTouchDoc)&&(pTouchDoc->getMethod()))
	{
		SpliceCountPairs spc;
		pTouchDoc->getMethod()->getSpliceCountPairs(spc);

		str.Format("%d", spc.GetSize());
		addLine("Number of methods", str);
		str.Format("%d", pTouchDoc->getMethod()->getSpliceCount());
		addLine("Number of method changes", str);
	
		for (int i=0;i<spc.GetSize();i++)
		{
			str.Format("%d", spc.GetAt(i)->_number);
			addLine(spc.GetAt(i)->_name + " rows", str);						
			delete spc.GetAt(i); //tidy up
		}
	} */	
}

void TouchAnalysisSplice::addLine(CString col0, CString col1, COLORREF color)
{

	PropItem* pItem = NULL; 

	pItem = new PropItem(PT_STATIC);

	AddPropItem(pItem);

	pItem->_col[0] =  col0;
	pItem->_col[1] =  col1;

	if (color == -1)
		color = ::GetSysColor(COLOR_WINDOW);

	pItem->_backgroundColor = color;


}


void TouchAnalysisSplice::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
	if (pTouchDoc)		
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
		
		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_TOUCH_ANALYSIS));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		//set what we want to print
		pTouchDoc->getPrintWhatAnalysis().unSetAll();
		pTouchDoc->getPrintWhatAnalysis()._bSplice = TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
}

void TouchAnalysisSplice::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisSplice::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisSplice::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisSplice::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}

CString TouchAnalysisSplice::getCopyData(const Method& method, const MethodAnalysis& analysis)
{
	CString tempStr, str;
	
/* todo	//we retrive the method names.
	if ((pTouchDoc)&&(pTouchDoc->getMethod()))
	{
		SpliceCountPairs spc;
		pTouchDoc->getMethod()->getSpliceCountPairs(spc);

		tempStr.Format("Number of methods\t%d\r\n", spc.GetSize());
		str += tempStr;
		tempStr.Format("Number of method changes\t%d\r\n", pTouchDoc->getMethod()->getSpliceCount());
		str += tempStr;
	
		for (int i=0;i<spc.GetSize();i++)
		{
			tempStr.Format("%s rows\t%d\r\n", spc.GetAt(i)->_name, spc.GetAt(i)->_number);
			str += tempStr;
			delete spc.GetAt(i); //tidy up
		}
	}		*/
	
	return str;

}

void TouchAnalysisSplice::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);
}


double TouchAnalysisSplice::print(CPage *page, double nextRow)
{
/*	nextRow = PrintUtils::printTitle(page, "Splice", nextRow, false);
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	CString str;
	
	if (_method)
	{
		SpliceCountPairs spc;
		_method->getSpliceCountPairs(spc);

		TABLEHEADER* pTable=new TABLEHEADER;        
		pTable->PointSize=	8;
		pTable->UseInches=	TRUE;
		pTable->AutoSize=	FALSE;
		pTable->Border=		FALSE;
		pTable->FillFlag=	FILL_NONE;
		pTable->NumColumns=	2;
		pTable->NumRows=	2 + spc.GetSize();
		pTable->StartRow=	nextRow;
		pTable->StartCol=	column;
		pTable->EndCol=		page->getWidthinches();
		pTable->HLines =	FALSE;
		pTable->VLines =	FALSE;
		pTable->NoHeader =	TRUE;

		pTable->ColDesc[0].Init(PrintUtils::_AnalysisMargin,"");
		pTable->ColDesc[1].Init(3.0,"");

		pTable->_leftVLinesLimit = 10000;
		pTable->_topHLinesLimit = 10000;

		page->Table(pTable);

		page->Print(pTable,0,0, 8, TEXT_LEFT, 0.0, 0.0, "Number of methods");
		str.Format("%d", spc.GetSize());
		page->Print(pTable,0,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();

		page->Print(pTable,1,0, 8, TEXT_LEFT, 0.0, 0.0, "Number of method changes");
		str.Format("%d", _method->getSpliceCount());
		page->Print(pTable,1,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();
		
	
		for (int i=0;i<spc.GetSize();i++)
		{
			str.Format("%s rows", spc.GetAt(i)->_name);
			page->Print(pTable,i+2,0, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();
			
			str.Format("%d", spc.GetAt(i)->_number);
			page->Print(pTable,i+2,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();

			delete spc.GetAt(i); //tidy up
		}

		nextRow = pTable->EndRow + .1;
	
		delete pTable;
		pTable = NULL;

	} 	

	page->SetFont(oldFont);
	page->SetColor(oldColor);
	   */ //todo
	return nextRow;
}

