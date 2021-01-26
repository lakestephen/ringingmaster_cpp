// TouchAnalysisFalseness.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisFalseness.h"
#include "TouchDoc.h"
#include "FalseRow.h"
#include "TouchHoldView.h"
#include "MethodAnalysis.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisFalseness

TouchAnalysisFalseness::TouchAnalysisFalseness()
{
}

TouchAnalysisFalseness::~TouchAnalysisFalseness()
{
}


BEGIN_MESSAGE_MAP(TouchAnalysisFalseness, SuperGridCtrl)
	//{{AFX_MSG_MAP(TouchAnalysisFalseness)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisFalseness message handlers



BOOL TouchAnalysisFalseness::Create(CWnd * pParentWnd)
{
	BOOL ret = SuperGridCtrl::Create(WS_CHILD | WS_VISIBLE , CRect(0,0,100,100),		
		pParentWnd, AAA_FALSENESS_LIST);
	
	mainFrame()->addEventListener(this);

	return  ret;
}

BOOL TouchAnalysisFalseness::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}

void TouchAnalysisFalseness::mainFrame_notifyUpdate(ViewType viewType)
{
	DeleteAll(false);
		   
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)) 
		return;

	SetRedraw(false); //has to be after the DeleteAll, else flag is reset
	
	TouchDoc * pDoc = mainFrame()->getActiveTouchDoc();

	CString str;

	//we retrive the method names.
	if ((pDoc)&&(pDoc->getMethod())&&(pDoc->getMethod()->hasAnalysis()))
	{
		Method * method = pDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis(); 
		const FalseRowArr& falseRowArr = analysis->getFalseRows();
				
		//create the root node
		SGItemInfo* falseRowsInfo = new SGItemInfo();

		CString str;   
		str.Format("%s Changes / Rows", analysis->getRequestedBlockCount() > 1?"Common":"False");
		falseRowsInfo->SetItemText(str, ::GetSysColor(COLOR_WINDOWTEXT), (COLORREF)-1);

		str.Format("%d / %d", falseRowArr.GetSize(), 
			analysis->getRequestedBlockCount() > 1?analysis->getCommonRowCount(): analysis->getFalseRowCount());
		falseRowsInfo->AddSubItemText(str);  
		
		falseRowsInfo->SetImage(( analysis->getFalseRowCount() == 0)?3:5);
		SGTreeItem* falseRows = InsertRootItem(falseRowsInfo);
	
		for (int i=0;i<falseRowArr.GetSize();i++)
		{
			FalseRow* curArr = falseRowArr.GetAt(i);

			addFalseNode(falseRows, curArr); 
		}

		Expand(falseRows, 0); 
	}	   
	
	SetRedraw(true); 
	Invalidate();
}

void TouchAnalysisFalseness::addFalseNode(SGTreeItem *parent, FalseRow *curArr)
{
	COLORREF textColor;
	textColor = GetSysColor(COLOR_WINDOWTEXT);
  
	CString change = curArr->getAsText();
	CString number;
	number.Format("%d", curArr->GetSize());
	
	SGItemInfo* lp = new SGItemInfo();
	
	lp->SetItemText(change, textColor, (COLORREF)-1);
	
	lp->SetImage(curArr->isMultiBlockTrue()?3:5);
	lp->AddSubItemText(number);
	
	
	SGTreeItem * falseParent = NULL;
	
	if (parent == NULL)
		falseParent = InsertRootItem(lp);
	else
		falseParent = InsertItem(parent, lp);
	
	if (falseParent)
	{
		for (int j=0;j<curArr->GetSize();j++)
		{
			lp = new SGItemInfo();
			lp->SetItemText(curArr->GetAt(j)->format(rpft_full), textColor, (COLORREF)-1);
			lp->SetImage(2);
			
			lp->_rowPosition = curArr->GetAt(j);
			
			InsertItem(falseParent, lp);
		}
	}   
}

int TouchAnalysisFalseness::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	/////////////////////
	//remember this	
	///////////////////

	//associate imagelist with listviewctrl, 
	//this imagelist is only for the first column re: the tree column	
	if(!_image.Create(IDB_FALSE_CTRL,16,1,RGB(255, 0, 255)))
		return -1;

	SetImageList(&_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;

	//If you want to assiciate images to subitems, create an imagelist
	//and call SetSubItemImageList
	//now the next time you want to map an image from this imagelist to a
	//particular subitem call SetSubItemText(col,text,image-index);

	CImageList imageSubItem;
	if(!imageSubItem.Create(IDB_SUBITEMS,16,1,RGB(255, 0, 255)))
		return -1;

    if(!SetSubItemImageList(&imageSubItem))
		return -1;
	/*
	ExtendedStyle support:
	LVS_EX_TRACKSELECT
	LVS_EX_GRIDLINES
	LVS_EX_FLATSB
	LVS_EX_CHECKBOXES
	LVS_EX_FLATSB
	all other ExStyles are not supported...buhhh and you call your self a windows-developer :(
	*/

//	SetExtendedStyle(LVS_EX_GRIDLINES);

	AddColumn("Changes / Rows");
	AddColumn("Count");

	AutoSaveColumns(NULL, "Falseness Columns", "200,75");

	
	return 0;
}


//this is my override of GetIcon, override this to set what ever icon suits you
int TouchAnalysisFalseness::GetIcon(const SGTreeItem* pItem)
{
	if(pItem!=NULL)
	{
		int iImage = GetData(pItem)->GetImage();

		if((ItemHasChildren(pItem))&&(!IsCollapsed(pItem)))
		{
			 iImage++;/*open icon*/
		}
		return iImage;
	}
	return 0;
}

BOOL TouchAnalysisFalseness::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();	
	
	if ((pTouchDoc)&&(pTouchDoc->getMethod())&&(ht.iItem != -1))
	{
		SGTreeItem *pSelItem = GetTreeItem(ht.iItem);
		if (pSelItem)
		{
			SGItemInfo *pItemInfo = GetData(pSelItem);
			if ((pItemInfo)&&(pItemInfo->_rowPosition))
			{
				pTouchDoc->doGoto(*pItemInfo->_rowPosition, false, true, true);
			}
		}
	}

	return 1;
}

CString TouchAnalysisFalseness::getEmptyListText()
{
	return "";
}


CImageList *TouchAnalysisFalseness::CreateDragImageEx(int /*nItem*/)
{
	return NULL; //this effectivly disables the drag drop
}

void TouchAnalysisFalseness::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
		pTouchDoc->getPrintWhatAnalysis()._bFalseness = TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}

}


void TouchAnalysisFalseness::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisFalseness::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisFalseness::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisFalseness::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}

CString TouchAnalysisFalseness::getCopyData(const Method& method, const MethodAnalysis& analysis)
{				  
	CString tempStr;   
	CString str;

	const FalseRowArr& falseRowArr = analysis.getFalseRows();

	tempStr.Format("%s Changes \t%d\r\n", analysis.getRequestedBlockCount() > 1?"Common":"False", falseRowArr.GetSize());
	str += tempStr;

	tempStr.Format("%s Rows \t%d\r\n\r\n", analysis.getRequestedBlockCount() > 1?"Common":"False", analysis.getRequestedBlockCount() > 1?analysis.getCommonRowCount(): analysis.getFalseRowCount());
	str += tempStr;

	for (int i=0;i<falseRowArr.GetSize();i++)
	{
		FalseRow* curArr = falseRowArr.GetAt(i);

		if (analysis.getRequestedBlockCount() > 1)
		{
			if (!curArr->isMultiBlockTrue())
			{
				str += "! ";
			}
		}
		
		str += curArr->getAsText();
		str += "\r\n";

		for (int j=0;j<curArr->GetSize();j++)
		{
			str += "\t";
			str += (curArr->GetAt(j)->format(rpft_full));
			str += "\r\n";
		}			
	}

	str += "\r\n";

					
	return str;
}

void TouchAnalysisFalseness::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);
}



double TouchAnalysisFalseness::print(CPage *page, double nextRow)
{
	/* todo 
	nextRow = PrintUtils::printTitle(page, "Falseness", nextRow, false);
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	
	if (_method)
	{
		CString str;
		FalseRowArr& falseRowArr = _method->getFalseRows();


		if (isMultiBlock())
		{
			page->Print(nextRow ,column, TEXT_LEFT, 8, "Common Changes");		
			str.Format("%d", falseRowArr.GetSize());
			nextRow = page->Print(nextRow ,PrintUtils::_AnalysisMargin+ column, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();					  
			page->Print(nextRow ,column, TEXT_LEFT, 8, "Common Rows");
			str.Format("%d", _method->getCommonRowCount());
			nextRow = page->Print(nextRow ,PrintUtils::_AnalysisMargin+ column, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();
		}
		else
		{
			page->Print(nextRow ,column, TEXT_LEFT, 8, "False Changes");		
			str.Format("%d", falseRowArr.GetSize());
			nextRow = page->Print(nextRow ,PrintUtils::_AnalysisMargin+ column, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();					  
			page->Print(nextRow ,column, TEXT_LEFT, 8, "False Rows");
			str.Format("%d", _method->getFalseRowCount());
			nextRow = page->Print(nextRow ,PrintUtils::_AnalysisMargin + column, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();
		}

		
		//commented out, as there is no reaql need to print all the false rows!! 
		//- Oh and the printer will not cope with more than 9 or so pages die to 16 bit GDI limitation 
		/*		nextRow += 0.1;

		for (int i=0;i<falseRowArr.GetSize();i++)
		{
			FalseRow* curArr = falseRowArr.GetAt(i);

			UINT flag;
			if (curArr->isMultiBlockTrue())
				flag = TEXT_LEFT | TEXT_ITALIC;
			else
				flag = TEXT_LEFT;
			
			str.Format("%s %s on %d: ",curArr->getRow().getAsText(), isMultiBlock()?"common":"false", curArr->GetSize());
			page->Print(nextRow, column, flag, 8, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();

			str.Empty();
			for (int j=0;j<curArr->GetSize();j++)
			{
				str += curArr->getRowString(j, false);
				if (j<curArr->GetSize()-1)
					str += ", ";
			}
			nextRow = page->Print(nextRow, max(column + PrintUtils::_AnalysisMargin, page->GetNextLogicalColumn()) , 
						flag, 8, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();

			//TRACE("Print Row %d, nextRow%f:\r\n", i, nextRow);
			
		}
	}

	page->SetFont(oldFont);
	page->SetColor(oldColor);
		 */
	return nextRow + 0.15;
}