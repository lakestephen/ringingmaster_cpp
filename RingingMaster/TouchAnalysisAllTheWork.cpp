// TouchAnalysisAllTheWork.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisAllTheWork.h"
#include "TouchDoc.h"
#include "AllTheWorkBell.h"
#include "TouchHoldView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisAllTheWork

TouchAnalysisAllTheWork::TouchAnalysisAllTheWork() :
_gridHeight(15),
_gridWidth(15)
{

}

TouchAnalysisAllTheWork::~TouchAnalysisAllTheWork()
{

}


BEGIN_MESSAGE_MAP(TouchAnalysisAllTheWork, ScrollWnd)
	//{{AFX_MSG_MAP(TouchAnalysisAllTheWork)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisAllTheWork message handlers

BOOL TouchAnalysisAllTheWork::Create(CWnd * pParentWnd)
{
	BOOL ret = ScrollWnd::Create(NULL, "All Work Wnd", WS_TABSTOP, CRect(0,0,100,100),		
									pParentWnd, AAA_ANALYSIS_ALL_WORK);
	
	mainFrame()->addEventListener(this);

	return ret;	
}


BOOL TouchAnalysisAllTheWork::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}


void TouchAnalysisAllTheWork::mainFrame_notifyUpdate(ViewType viewType)
{
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)) 
		return;
	  /* todo
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc )
	{	 
		Method* method = pTouchDoc->getMethod();

		if (method) 
		{

			CDC* pDC = GetDC();
			CFont * oldFont = pDC->SelectObject(&app()->_fontStd);
			calculateSizes(pDC);
			pDC->SelectObject(oldFont); 
			ReleaseDC(pDC);

			//now calculate the overall scroll size
			int activeMethods = 0;
			AllTheWorkBell* aTWHold = NULL;
			for(int i=0;i<method->getNotations().GetSize();i++)
			{
				Notation* not = method->getNotations().GetAt(i);
				AllTheWorkBell* aTW = not->_allTheWork;

				if ((aTW)&&
					(not->_spliceRowCount != 0)) //indicator of if the method is used in the touch
				{
					activeMethods++;
					aTWHold = aTW;
				}
			}
			if(aTWHold)
			{

				int methodHeight = _gridHeight * (method->getNotations().GetAt(0)->getNumber() + 2);

				//add for title
				if(method->isSpliced())		
					methodHeight +=22;

				//add for splice pos title
				if (aTWHold->getSpliceCount() > 1)
					methodHeight += 19 ;


				int methodWidth = _gridWidth * (method->getNumber() +2);



				SetScrollSizes(CSize(methodWidth*aTWHold->getSpliceCount(),activeMethods*methodHeight));
			}
		}
	}
			 *///todo
	Invalidate();
}

void TouchAnalysisAllTheWork::OnDraw(CDC* pDC)
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc == NULL)
		return;

	Method* method = pTouchDoc->getMethod();

	if (method == NULL) 
		return;	

	calculateSizes(pDC);
	
	int leftConst = 3;

	pDC->SetBkMode (TRANSPARENT);

	//create a title font
	LOGFONT lf;
	FontInfo titleFontInfo("Arial", 95);
	titleFontInfo.getLogFont(&lf);
	CFont titleFont;
	titleFont.CreateFontIndirect(&lf);

	//get the old font now
	CFont * oldFont = pDC->SelectObject(&app()->_fontStd);
		
	CPoint origin(leftConst, 2);
	CString str;
  /*	   todo
	for(int i=0;i<method->getNotations().GetSize();i++)
	{
		Notation* not = method->getNotations().GetAt(i);
		AllTheWorkBell* aTW = not->_allTheWork;

		if ((aTW)&&
			(not->_spliceRowCount != 0)) //indicator of if the method is used in the touch
		{

			//title
			origin.x = leftConst;
			if (method->_proofInput->getSplices().GetSize() > 0) //spliced
			{
				pDC->SelectObject(titleFont);
				pDC->TextOut(origin.x, origin.y, not->getName());
				origin.y += 22;
			}
		
			//calculate if we want to bring the last splice row to the front i.e.
			bool bringLastRowForward = false;
			if ((not->getSplicePositionCount() > 1)&&
			    (not->getSplicePosition(not->getSplicePositionCount()-1) == not->getChangesPerPlainLead()))
				bringLastRowForward = true;
			
			//do we draw the splice title
			bool drawSpliceTitle = false;
			if ((aTW->getSpliceCount() > 1)&&
				(not->getSplicePositionCount() == aTW->getSpliceCount()))
				drawSpliceTitle = true;

			//draw the main table
			origin.x = leftConst;
			if (bringLastRowForward)
			{
				//draw the last row first
				drawGrid(pDC, aTW, origin, aTW->getSpliceCount()-1, drawSpliceTitle, not, true);
				origin.x += (_gridWidth * (aTW->getNumber()+2));

				//draw all other rows
				for (int i=0;i<aTW->getSpliceCount()-1;i++)
				{
					drawGrid(pDC, aTW, origin, i, drawSpliceTitle, not);
					origin.x += (_gridWidth * (aTW->getNumber()+2));
				}

			}
			else
			{
				//main information
				for (int i=0;i<aTW->getSpliceCount();i++)
				{
					drawGrid(pDC, aTW, origin, i, drawSpliceTitle, not);
					origin.x += (_gridWidth * (aTW->getNumber()+2));
				}
			}

			//sort out the origin
			if (drawSpliceTitle)
				origin.y += 19;

			origin.x = leftConst;
			origin.y += (_gridHeight*(aTW->getNumber()+2));
		}
	}		  
				*/
	pDC->SelectObject(oldFont); 
}

void TouchAnalysisAllTheWork::drawGrid(CDC* pDC, AllTheWorkBell* aTW, CPoint origin, int spliceIndex, bool drawSpliceTitle, Notation* not, bool subtractPlainCourseLength)
{
	
	CString str;
	CPoint pos(origin);

	//create a bold font
	FontInfo boldFontInfo("MS Sans Serif");
	boldFontInfo._weight = FW_BOLD;
	LOGFONT lf;
	boldFontInfo.getLogFont(&lf);
	CFont boldFont;
	boldFont.CreateFontIndirect(&lf);

	FontInfo titleFontInfo("Arial", 90);
	titleFontInfo.getLogFont(&lf);
	CFont titleFont;
	titleFont.CreateFontIndirect(&lf);
	
	//splice title
	if (drawSpliceTitle)
	{
		pDC->SelectObject(&titleFont);
		if (subtractPlainCourseLength)
		{
			str.Format("%sow %d (%d)", ((aTW->getNumber()>5)?"Splice r":"R"),
				not->getSplicePosition(spliceIndex)+1-not->getChangesPerPlainLead(),
				not->getSplicePosition(spliceIndex)+1);
		}
		else
		{
			str.Format("%sow %d", ((aTW->getNumber()>5)?"Splice r":"R"),
				not->getSplicePosition(spliceIndex)+1);
		}

		pDC->TextOut(pos.x, pos.y, str);
		origin.y += 19;
	}

	//top row
	pDC->SelectObject(&boldFont);
	pos = origin;
	pos.x += _gridWidth;
	for (int j=0;j<aTW->getNumber();j++)
	{
		str.Format("%d",j+1);
		pDC->TextOut(pos.x, pos.y, str);					
		pos.x += _gridWidth;
	}

	//left bell numbers 
	pos = origin;
	pos.y += _gridHeight;

	for (int j=0;j<aTW->getNumber();j++)
	{
		pDC->TextOut(pos.x, pos.y, CString(GlobalFunctions::bellNumbersToChar(j+1)));					
		pos.y += _gridHeight;
	}


	pDC->SelectObject(&app()->_fontStd);

	pos = origin;
	pos.x += _gridWidth;
	pos.y += _gridHeight;
	

	//Main Data
	for (int j=0;j<aTW->getNumber();j++)  //down
	{
		for (int k=0;k<aTW->getNumber();k++) //accross
		{
			int count = aTW->getPositionCount(j,k,spliceIndex);
			if (count >0)
			{
				str.Format("%d", count);
				pDC->TextOut(pos.x, pos.y, str);
			}
			pos.x += _gridWidth;
		}
		pos.x = origin.x + _gridWidth;
		pos.y += _gridHeight;
	}			 
  
}

void TouchAnalysisAllTheWork::calculateSizes(CDC* pDC)
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc == NULL)
		return;

	Method* method = pTouchDoc->getMethod();

	if (method == NULL) 
		return;	
   /*
	//calculate the biggest cell width
	int largestNum = 0;
	for(int i=0;i<method->getNotations().GetSize();i++)
	{
		Notation* not = method->getNotations().GetAt(i);
		AllTheWorkBell* aTW = not->_allTheWork;

		if (aTW)
		{
			for (int j=0;j<aTW->getNumber();j++)  //down
			{
				for (int k=0;k<aTW->getNumber();k++) //accross
				{
					for (int l=0;l<aTW->getSpliceCount();l++) //accross
					{
						if (aTW->getPositionCount(j,k,l) > largestNum)
						{
							largestNum = aTW->getPositionCount(j,k,l);
						}
					}
				}
			}
		}
	}

	CString num;
	num.Format("%d", largestNum);
	_gridWidth = max(pDC->GetTextExtent(num).cx, 14);

				*/ //todo
}

void TouchAnalysisAllTheWork::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
		pTouchDoc->getPrintWhatAnalysis()._bAllWork = TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
	
}

void TouchAnalysisAllTheWork::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisAllTheWork::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisAllTheWork::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisAllTheWork::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}


void TouchAnalysisAllTheWork::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);
}


CString TouchAnalysisAllTheWork::getCopyData(const Method& method, const MethodAnalysis& analysis)
{
	CString str;
/*
	for(int i=0;i<method->getNotations().GetSize();i++)
	{
		Notation* not = method->getNotations().GetAt(i);
		AllTheWorkBell* aTW = not->_allTheWork;

		if ((aTW)&&
			(not->_spliceRowCount != 0)) //indicator of if the method is used in the touch
		{

			//title
			if (method->_proofInput->getSplices().GetSize() > 0) //spliced
			{
				str += not->getName();
				str += "\r\n";
			}
		
			//calculate if we want to bring the last splice row to the front i.e.
			bool bringLastRowForward = false;
			if ((not->getSplicePositionCount() > 1)&&
			    (not->getSplicePosition(not->getSplicePositionCount()-1) == not->getChangesPerPlainLead()))
				bringLastRowForward = true;
			
			//do we draw the splice title
			bool drawSpliceTitle = false;
			if ((aTW->getSpliceCount() > 1)&&
				(not->getSplicePositionCount() == aTW->getSpliceCount()))
				drawSpliceTitle = true;

			//draw the main table
			if (bringLastRowForward)
			{
				//draw the last row first
				str += getCopyGrid(aTW, aTW->getSpliceCount()-1, drawSpliceTitle, not, true);

				//draw all other rows
				for (int i=0;i<aTW->getSpliceCount()-1;i++)
				{
					str += getCopyGrid(aTW, i, drawSpliceTitle, not);
				}

			}
			else
			{
				//main information
				for (int i=0;i<aTW->getSpliceCount();i++)
				{
					str += getCopyGrid(aTW, i, drawSpliceTitle, not);
				}
			}
		}
	}		 
		 */ //todo
	return str;
}

CString TouchAnalysisAllTheWork::getCopyGrid(AllTheWorkBell* aTW, int spliceIndex, bool drawSpliceTitle, Notation* not, bool subtractPlainCourseLength)
{
	
	CString tempStr;
	CString str;
	
	//splice title
	if (drawSpliceTitle)
	{
		if (subtractPlainCourseLength)
		{
			tempStr.Format("%sow %d (%d)", ((aTW->getNumber()>5)?"Splice r":"R"),
				not->getSplicePosition(spliceIndex)+1-not->getChangesPerPlainLead(),
				not->getSplicePosition(spliceIndex)+1);
		}
		else
		{
			tempStr.Format("%sow %d", ((aTW->getNumber()>5)?"Splice r":"R"),
				not->getSplicePosition(spliceIndex)+1);
		}

		str += tempStr;
		str += "\r\n" ;
	}

	//top row

	str += "\t";

	for (int j=0;j<aTW->getNumber();j++)
	{
		tempStr.Format("%d\t",j+1);
		str += tempStr;
	}

	str += "\r\n";

	//Main Data
	for (int j=0;j<aTW->getNumber();j++)  //down
	{
		str += GlobalFunctions::bellNumbersToChar(j+1);					//left numbers 
		 str += "\t";

		for (int k=0;k<aTW->getNumber();k++) //accross
		{
			int count = aTW->getPositionCount(j,k,spliceIndex);
			if (count >0)
			{
				tempStr.Format("%d", count);
				str += tempStr;
			}
			str += "\t";
		} 
		str += "\r\n";

	}			

	str += "\r\n\r\n";

	
	return str;
  
}


double TouchAnalysisAllTheWork::print(CPage *page, double nextRow)
{
/*	nextRow = PrintUtils::printTitle(page, "All The Work", nextRow, false);
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	

	if (_method)
	{
		//find the widest number
		int largestNum = 0;
		for(int i=0;i<_method->getNotations().GetSize();i++)
		{
			Notation* not = _method->getNotations().GetAt(i);
			AllTheWorkBell* aTW = not->_allTheWork;

			if (aTW)
			{
				for (int j=0;j<aTW->getNumber();j++)  //down
				{
					for (int k=0;k<aTW->getNumber();k++) //accross
					{
						for (int l=0;l<aTW->getSpliceCount();l++) //accross
						{
							if (aTW->getPositionCount(j,k,l) > largestNum)
							{
								largestNum = aTW->getPositionCount(j,k,l);
							}
						}
					}
				}
			}
		}

		CString num;
		num.Format("%d", largestNum);
		double gridWidth = max(page->getTextWidth(num), 0.18);


		for(int i=0;i<_method->getNotations().GetSize();i++)
		{
			Notation* not = _method->getNotations().GetAt(i);
			AllTheWorkBell* aTW = not->_allTheWork;
			double nextColumn = column;

			if ((aTW)&&
				(not->_spliceRowCount != 0)) //indicator of if the method is used in the touch
			{
				//title
				if (_method->isSpliced()) //spliced
					nextRow = page->Print(nextRow, nextColumn, TEXT_LEFT, 9, not->getName()) + .05;
				
				//calculate if we want to bring the last splice row to the front i.e.
				bool bringLastRowForward = false;
				if ((not->getSplicePositionCount() > 1)&&
					(not->getSplicePosition(not->getSplicePositionCount()-1) == not->getChangesPerPlainLead()))
					bringLastRowForward = true;
				
				//do we draw the splice title
				bool drawSpliceTitle = false;
				if ((aTW->getSpliceCount() > 1)&&
					(not->getSplicePositionCount() == aTW->getSpliceCount()))
					drawSpliceTitle = true;
				double holdNextRow = nextRow;

				//draw the main table
				if (bringLastRowForward)
				{
					//draw the last row first
					holdNextRow = printAllTheWorkGrid(page, nextRow, nextColumn, gridWidth, aTW, aTW->getSpliceCount()-1, 
											drawSpliceTitle, not, true);

					//draw all other rows
					for (int i=0;i<aTW->getSpliceCount()-1;i++)
					{
						holdNextRow = printAllTheWorkGrid(page, nextRow, nextColumn, gridWidth, aTW, i, drawSpliceTitle, not);
					}

				}
				else
				{
					//main information
					for (int i=0;i<aTW->getSpliceCount();i++)
					{
						holdNextRow = printAllTheWorkGrid(page, nextRow, nextColumn, gridWidth, aTW, i, drawSpliceTitle, not);
					}
				}
				nextRow = holdNextRow;
			}
		}		  
	}

	page->SetFont(oldFont);
	page->SetColor(oldColor);
						 */ //todo
	return nextRow;
}


double TouchAnalysisAllTheWork::printAllTheWorkGrid(CPage *page, double nextRow, double& nextColumn, double gridWidth, 
									 AllTheWorkBell* aTW, int spliceIndex, bool drawSpliceTitle, 
									 Notation* not, bool subtractPlainCourseLength)
{	/* todo
	CString str;

	//splice title
	if (drawSpliceTitle)
	{
		if (subtractPlainCourseLength)
		{
			str.Format("%sow %d (%d)", ((aTW->getNumber()>5)?"Splice r":"R"),
				not->getSplicePosition(spliceIndex)+1-not->getChangesPerPlainLead(),
				not->getSplicePosition(spliceIndex)+1);
		}
		else
		{
			str.Format("%sow %d", ((aTW->getNumber()>5)?"Splice r":"R"),
				not->getSplicePosition(spliceIndex)+1);
		}


		nextRow = page->Print(nextRow, nextColumn, TEXT_LEFT, 9, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();

	}

	TABLEHEADER* pTable=new TABLEHEADER;        
	pTable->PointSize=	8;
	pTable->UseInches=	TRUE;
	pTable->AutoSize=	FALSE;
	pTable->Border=		FALSE;
	pTable->FillFlag=	FILL_NONE;
	pTable->NumColumns=	getNumber() +1;
	pTable->NumRows=	getNumber() +1;
	pTable->StartRow=	nextRow;
	pTable->StartCol=	nextColumn;
	pTable->EndCol=		nextColumn + (getNumber() +1 )* gridWidth;
	pTable->HLines =	FALSE;
	pTable->VLines =	FALSE;
	pTable->NoHeader =	TRUE;

	for (int i=0;i<getNumber() +1;i++)
		pTable->ColDesc[i].Init(gridWidth,"");

	pTable->_leftVLinesLimit = 10000;
	pTable->_topHLinesLimit = 10000;

	nextColumn = pTable->EndCol + .1;

	page->Table(pTable);

	nextRow = pTable->EndRow + .15;

	//top row
	for (int j=0;j<aTW->getNumber();j++)
	{
		str.Format("%d",j+1);
		page->Print(pTable,0,j+1, 8, TEXT_LEFT|TEXT_BOLD, 0.0, 0.0,str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();
	}

	//left bell numbers 
	for (int j=0;j<aTW->getNumber();j++)
	{
		str.Format("%c",GlobalFunctions::bellNumbersToChar(j+1));
		page->Print(pTable,j+1,0, 8, TEXT_LEFT|TEXT_BOLD, 0.0, 0.0,str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();

	}

	//Main Data
	for (int j=0;j<aTW->getNumber();j++)  //down
	{
		for (int k=0;k<aTW->getNumber();k++) //accross
		{
			int count = aTW->getPositionCount(j,k,spliceIndex);
			if (count >0)
			{
				str.Format("%d", count);
				page->Print(pTable,j+1,k+1, 8, TEXT_LEFT, 0.0, 0.0,str.GetBuffer(str.GetLength()));
				str.ReleaseBuffer();
			}
		}
	}	
	
	delete pTable;
			 */
	return nextRow;
}
