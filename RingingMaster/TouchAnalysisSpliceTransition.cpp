F// TouchAnalysisSpliceTransition.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisSpliceTransition.h"
#include "TouchDoc.h"
#include "TouchHoldView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisSpliceTransition

TouchAnalysisSpliceTransition::TouchAnalysisSpliceTransition() :
_gridHeight(15),
_gridWidth(15)
{
}

TouchAnalysisSpliceTransition::~TouchAnalysisSpliceTransition()
{

}


BEGIN_MESSAGE_MAP(TouchAnalysisSpliceTransition, ScrollWnd)
	//{{AFX_MSG_MAP(TouchAnalysisSpliceTransition)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisSpliceTransition message handlers

BOOL TouchAnalysisSpliceTransition::Create(CWnd * pParentWnd)
{
	BOOL ret = ScrollWnd::Create(NULL, "Splice Transition Wnd", WS_TABSTOP, CRect(0,0,100,100),		
									pParentWnd, AAA_ANALYSIS_ALL_WORK);

	mainFrame()->addEventListener(this);

	return ret;	
}

BOOL TouchAnalysisSpliceTransition::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}


void TouchAnalysisSpliceTransition::mainFrame_notifyUpdate(ViewType viewType)
{
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)) 
		return;	
	
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	int leftConst = 3;
	int topConst = 2;
   /*
	if (pTouchDoc )
	{	 
		Method* method = pTouchDoc->getMethod();

		if (method) 
		{
			CDC* pDC = GetDC();
			calculateSizes(pDC);
			ReleaseDC(pDC);

			SetScrollSizes(CSize(_gridWidth * (method->getNotations().GetSize() + 1) + leftConst, 
								 _gridHeight * (method->getNotations().GetSize() + 1) + topConst));
		}
	}	*/ //todo

	Invalidate();
}

void TouchAnalysisSpliceTransition::OnDraw(CDC* pDC)
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc == NULL)
		return;

	Method* method = pTouchDoc->getMethod();

	if (method == NULL) 
		return;	

	calculateSizes(pDC);
	/*
	CString str;
	CPoint pos(0,0);

	pDC->SetBkMode (TRANSPARENT);

	//create a bold font
	FontInfo boldFontInfo("MS Sans Serif");
	boldFontInfo._weight = FW_BOLD;
	LOGFONT lf;
	boldFontInfo.getLogFont(&lf);
	CFont boldFont;
	boldFont.CreateFontIndirect(&lf);


	CFont * oldFont = pDC->SelectObject(&boldFont);
	int leftConst = 3;
	int topConst = 2;


	//top row
	pDC->SelectObject(&boldFont);
	pos.x = leftConst + _gridWidth;
	pos.y = topConst;

	for (int j=0;j<method->getNotations().GetSize();j++)
	{
		pDC->TextOut(pos.x, pos.y, method->getNotations().GetAt(j)->getSpliceLetter());					
		pos.x += _gridWidth;
	}

	//left bell numbers 
	pos.x = leftConst;
	pos.y = topConst + _gridHeight;

	for (int j=0;j<method->getNotations().GetSize();j++)
	{
		pDC->TextOut(pos.x, pos.y, method->getNotations().GetAt(j)->getSpliceLetter());					
		pos.y += _gridHeight;
	}

	//Main Data
	pDC->SelectObject(&app()->_fontStd);
	pos.x = leftConst + _gridWidth;
	pos.y = topConst + _gridHeight;

	for(int i=0;i<method->getNotations().GetSize();i++) // down
	{
		Notation* from = method->getNotations().GetAt(i);
		for(int j=0;j<method->getNotations().GetSize();j++) //accross
		{
			Notation* to = method->getNotations().GetAt(j);
			CString token = Method::getSpliceTransitionToken(from->getSpliceLetter(), to->getSpliceLetter());
			int count = 0;
			method->getSpliceTransitions().Lookup(token, count);

			if (count >0)
			{
				str.Format("%d", count);
				pDC->TextOut(pos.x, pos.y, str);
			}
			pos.x += _gridWidth;
		}
		pos.x = leftConst + _gridWidth;
		pos.y += _gridHeight;
	}			 

	pDC->SelectObject(oldFont); 

	*/ //todo
}


void TouchAnalysisSpliceTransition::calculateSizes(CDC* pDC)
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc == NULL)
		return;

	Method* method = pTouchDoc->getMethod();

	if (method == NULL) 
		return;	

	CFont* oldFont = pDC->SelectObject(&app()->_fontStd);

	/*
	//calculate the biggest cell width to do with numbers
	int largestNum = 0;
	for(int i=0;i<method->getNotations().GetSize();i++) // down
	{
		Notation* from = method->getNotations().GetAt(i);
		for(int j=0;j<method->getNotations().GetSize();j++) //accross
		{
			Notation* to = method->getNotations().GetAt(j);
			CString token = Method::getSpliceTransitionToken(from->getSpliceLetter(), to->getSpliceLetter());
			int count = 0;
			method->getSpliceTransitions().Lookup(token, count);
			if (count > largestNum)
			{
				largestNum = count;
			}
		}
	}

	CString num;
	num.Format("%d", largestNum);
	_gridWidth = max(pDC->GetTextExtent(num).cx, 14);

	FontInfo boldFontInfo("MS Sans Serif");
	boldFontInfo._weight = FW_BOLD;
	LOGFONT lf;
	boldFontInfo.getLogFont(&lf);
	CFont boldFont;
	boldFont.CreateFontIndirect(&lf);

	pDC->SelectObject(&boldFont);

	//now check the splice names
	for(int i=0;i<method->getNotations().GetSize();i++) // down
	{	
		if (pDC->GetTextExtent(method->getNotations().GetAt(i)->getSpliceLetter()).cx > _gridWidth)
			_gridWidth = pDC->GetTextExtent(method->getNotations().GetAt(i)->getSpliceLetter()).cx ;
	}	

	pDC->SelectObject(&oldFont);

	_gridWidth += 3;	*/ //todo
}

void TouchAnalysisSpliceTransition::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
		pTouchDoc->getPrintWhatAnalysis()._bSpliceTransitions = TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
	
}



void TouchAnalysisSpliceTransition::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisSpliceTransition::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisSpliceTransition::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisSpliceTransition::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}

CString TouchAnalysisSpliceTransition::getCopyData(const Method& method, const MethodAnalysis& analysis)
{
	CString temp;
	CString str = "\t";
	 /*
	for (int j=0;j<method->getNotations().GetSize();j++)
	{
		str += method->getNotations().GetAt(j)->getSpliceLetter();
		str += "\t";
	}

	str += "\r\n";

	for(int i=0;i<method->getNotations().GetSize();i++) // down
	{
		Notation* from = method->getNotations().GetAt(i);

		str += from->getSpliceLetter();
		str += "\t";

		for(int j=0;j<method->getNotations().GetSize();j++) //accross
		{
			Notation* to = method->getNotations().GetAt(j);
			CString token = Method::getSpliceTransitionToken(from->getSpliceLetter(), to->getSpliceLetter());
			int count = 0;
			method->getSpliceTransitions().Lookup(token, count);

			if (count >0)
			{
				temp.Format("%d", count);
				str += temp;
			}
			str += "\t";
		}

		str += "\r\n";
	}	*/ //todo		 

	return str;
}

void TouchAnalysisSpliceTransition::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);
}


double TouchAnalysisSpliceTransition::print(CPage *page, double nextRow)
{
/* todo	nextRow = PrintUtils::printTitle(page, "Splice Transitions", nextRow, false);
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	

	if (_method)
	{
		//find the widest number
		int largestNum = 0;
		for(int i=0;i<_method->getNotations().GetSize();i++) // down
		{
			Notation* from = _method->getNotations().GetAt(i);
			for(int j=0;j<_method->getNotations().GetSize();j++) //accross
			{
				Notation* to = _method->getNotations().GetAt(j);
				CString token = Method::getSpliceTransitionToken(from->getSpliceLetter(), to->getSpliceLetter());
				int count = 0;
				_method->getSpliceTransitions().Lookup(token, count);
				if (count > largestNum)
				{
					largestNum = count;
				}
			}
		}


		CString num;
		num.Format("%d", largestNum);
		double gridWidth = max(page->getTextWidth(num), 0.18);

		//check the labels
		//set bold

		//now check the splice names
		for(int i=0;i<_method->getNotations().GetSize();i++) // down
		{	
			if (page->getTextWidth(_method->getNotations().GetAt(i)->getSpliceLetter()) > gridWidth)
				gridWidth = page->getTextWidth(_method->getNotations().GetAt(i)->getSpliceLetter()) ;
		}	

		//print the grid
		TABLEHEADER* pTable=new TABLEHEADER;        
		pTable->PointSize=	8;
		pTable->UseInches=	TRUE;
		pTable->AutoSize=	FALSE;
		pTable->Border=		FALSE;
		pTable->FillFlag=	FILL_NONE;
		pTable->NumColumns=	_method->getNotations().GetSize() +1;
		pTable->NumRows=	_method->getNotations().GetSize() +1;
		pTable->StartRow=	nextRow;
		pTable->StartCol=	column;
		pTable->EndCol=		column+ (_method->getNotations().GetSize() +1 )* gridWidth;
		pTable->HLines =	FALSE;
		pTable->VLines =	FALSE;
		pTable->NoHeader =	TRUE;

		for (int i=0;i<_method->getNotations().GetSize() +1;i++)
			pTable->ColDesc[i].Init(gridWidth,"");

		pTable->_leftVLinesLimit = 10000;
		pTable->_topHLinesLimit = 10000;

		page->Table(pTable);

		nextRow = pTable->EndRow + .15;

		CString str;

		//top row
		for (int j=0;j<_method->getNotations().GetSize();j++)
		{
			str.Format("%s",_method->getNotations().GetAt(j)->getSpliceLetter());
			page->Print(pTable,0,j+1, 8, TEXT_LEFT|TEXT_BOLD, 0.0, 0.0,str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();
		}

		//left bell numbers 
		for (int j=0;j<_method->getNotations().GetSize();j++)
		{
			str.Format("%s",_method->getNotations().GetAt(j)->getSpliceLetter());
			page->Print(pTable,j+1,0, 8, TEXT_LEFT|TEXT_BOLD, 0.0, 0.0,str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();

		}

		//Main Data

		for(int i=0;i<_method->getNotations().GetSize();i++) // down
		{
			Notation* from = _method->getNotations().GetAt(i);
			for(int j=0;j<_method->getNotations().GetSize();j++) //accross
			{
				Notation* to = _method->getNotations().GetAt(j);
				CString token = Method::getSpliceTransitionToken(from->getSpliceLetter(), to->getSpliceLetter());
				int count = 0;
				_method->getSpliceTransitions().Lookup(token, count);

				if (count >0)
				{
					str.Format("%d", count);
					page->Print(pTable,i+1,j+1, 8, TEXT_LEFT, 0.0, 0.0,str.GetBuffer(str.GetLength()));
					str.ReleaseBuffer();
				}
			}
		}	
		

		delete pTable;		  

	}

	page->SetFont(oldFont);
	page->SetColor(oldColor);
			  */
	return nextRow;
}