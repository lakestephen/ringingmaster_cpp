// TouchAnalysisMultiBlock.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisMultiBlock.h"
#include "PropItem.h"
#include "TouchDoc.h"
#include "Method.h"
#include "MethodAnalysis.h"
#include "TouchHoldView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisMultiBlock

TouchAnalysisMultiBlock::TouchAnalysisMultiBlock()
{
	
}

TouchAnalysisMultiBlock::~TouchAnalysisMultiBlock()
{

}


BEGIN_MESSAGE_MAP(TouchAnalysisMultiBlock, PropList)
	//{{AFX_MSG_MAP(TouchAnalysisMultiBlock)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisMultiBlock message handlers

BOOL TouchAnalysisMultiBlock::Create(CWnd * pParentWnd)
{
	BOOL result = PropList::Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|
								LBS_OWNERDRAWVARIABLE|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_HASSTRINGS, 
								CRect(0,0,0,0),
								pParentWnd,
								AAA_ANALYSIS_STATUS);	

	_devider[0] = 150;	

	if (result)
		SetFont(&app()->_fontStd);

	mainFrame()->addEventListener(this);

	return  result;
}


BOOL TouchAnalysisMultiBlock::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}

void TouchAnalysisMultiBlock::mainFrame_notifyUpdate(ViewType viewType)
{
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)) 
		return;

	CString str, blockNo;

	deleteAllPropItems();

	TouchDoc * pDoc = mainFrame()->getActiveTouchDoc();

	//we retrive the method names.
	if ((pDoc)&&(pDoc->getMethod())&&(pDoc->getMethod()->hasAnalysis()))
	{
		Method * method = pDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis(); 

		//report on the complete / partial blocks
		str.Format("%d", analysis->getTotalBlocksCount());
		addLine("Total blocks", str);

		str.Format("%d", analysis->getCompleteBlockCount());
		addLine("Complete blocks", str);

		str.Format("%d", analysis->getPartialBlockCount());
		addLine("Partial blocks", str, (analysis->getPartialBlockCount() >1)?RGB(255, 120, 120):-1);
			
		
		//report on the number of changes in a blocks
		const Ints& rowBlockCounts = analysis->getBlockRowCounts();
		for (int i=0;i<rowBlockCounts.GetSize();i++)
		{
			if (rowBlockCounts.GetAt(i) !=0)
			{
				str.Format("%d", rowBlockCounts.GetAt(i));
				blockNo.Format("Changes in %d block%s",i, (i==1)?"":"s");
				addLine(blockNo, str);
			}

		}
	}
		 

}

void TouchAnalysisMultiBlock::addLine(CString col0, CString col1, COLORREF color)
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

void TouchAnalysisMultiBlock::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
		pTouchDoc->getPrintWhatAnalysis()._bMultiBlock = TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
}

void TouchAnalysisMultiBlock::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisMultiBlock::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisMultiBlock::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisMultiBlock::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}



void TouchAnalysisMultiBlock::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);
}


CString TouchAnalysisMultiBlock::getCopyData(const Method& method, const MethodAnalysis& analysis)
{
	CString tempStr, blockNo, str;

	//report on the complete / partial blocks
	tempStr.Format("Total blocks \t%d\r\n", analysis.getTotalBlocksCount());
	str += tempStr;

	tempStr.Format("Complete blocks \t%d\r\n", analysis.getCompleteBlockCount());
	str += tempStr;

	tempStr.Format("Partial blocks \t%d\r\n", analysis.getPartialBlockCount());
	str += tempStr;
	
	//report on the number of changes in a blocks
	const Ints& rowBlockCounts = analysis.getBlockRowCounts();
	for (int i=0;i<rowBlockCounts.GetSize();i++)
	{
		if (rowBlockCounts.GetAt(i) !=0)
		{
			tempStr.Format("Changes in %d block%s \t%d\r\n",i, (i==1)?"":"s", rowBlockCounts.GetAt(i));
			str += tempStr;
		}

	}

	return str;
}




double TouchAnalysisMultiBlock::print(CPage *page, double nextRow)
{
/* todo
	nextRow = PrintUtils::printTitle(page, "Multi Block", nextRow, false);
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0) + PrintUtils::_AnalysisIndent;	

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	CString str;

		//we retrive the method names.
	if (_method)
	{
		//report on the complete / partial blocks
		page->Print(nextRow, column, TEXT_LEFT, 8, "Total blocks");
		str.Format("%d", _method->getTotalBlocksCount());
		nextRow = page->Print(nextRow, column + PrintUtils::_AnalysisMargin, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();

		page->Print(nextRow, column, TEXT_LEFT, 8, "Complete blocks");
		str.Format("%d", _method->getCompleteBlockCount());
		nextRow = page->Print(nextRow, column + PrintUtils::_AnalysisMargin, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();

		page->Print(nextRow, column, TEXT_LEFT, 8, "Partial blocks");
		str.Format("%d", _method->getPartialBlockCount());
		nextRow = page->Print(nextRow, column + PrintUtils::_AnalysisMargin, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();

		nextRow += 0.1;

		//report on the number of changes in a blocks
		for (int i=0;i<_method->getBlockRowCounts().GetSize();i++)
		{
			if (_method->getBlockRowCounts().GetAt(i) !=0)
			{
				str.Format("Changes in %d block%s",i, (i==1)?"":"s");
				page->Print(nextRow, column, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
				str.ReleaseBuffer();
				str.Format("%d", _method->getBlockRowCounts().GetAt(i));
				nextRow = page->Print(nextRow, column + PrintUtils::_AnalysisMargin, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
				str.ReleaseBuffer();
			}
		}
	}


	page->SetFont(oldFont);
	page->SetColor(oldColor);
								 */ //todo
	return nextRow + 0.15;
}