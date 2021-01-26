// TouchAnalysisLeadEnds.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisLeadEnds.h"
#include "TouchDoc.h"
#include "TouchHoldView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisLeadEnds

TouchAnalysisLeadEnds::TouchAnalysisLeadEnds()
{

}

TouchAnalysisLeadEnds::~TouchAnalysisLeadEnds()
{
	
}


BEGIN_MESSAGE_MAP(TouchAnalysisLeadEnds, ScrollWnd)
	//{{AFX_MSG_MAP(TouchAnalysisLeadEnds)
		ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisLeadEnds message handlers
	   

BOOL TouchAnalysisLeadEnds::Create(CWnd * pParentWnd)
{
	BOOL ret = ScrollWnd::Create(NULL, "Lean End Wnd", WS_TABSTOP, CRect(0,0,100,100),		
									pParentWnd, AAA_LEAD_END);

	mainFrame()->addEventListener(this);

	return ret;	
}


BOOL TouchAnalysisLeadEnds::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}

void TouchAnalysisLeadEnds::mainFrame_notifyUpdate(ViewType viewType)
{
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)) 
		return;

	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
   /*
	if (pTouchDoc )
	{	 
		Method* method = pTouchDoc->getMethod();

		if (method) 
		{

			int number = pTouchDoc->getNumber();
			CString testStr;

			//work out the width of a single notation
			for (int i=0;i<number;i++)
				testStr+= GlobalFunctions::bellNumbersToChar(i+1);

			CDC* pDC = GetDC();

			CFont * oldFont = pDC->SelectObject(&app()->_fontStd);
			
			CSize size = pDC->GetTextExtent(testStr);
			pDC->SelectObject(oldFont); 

			ReleaseDC(pDC);

			size.cx += 10;

			CSize scrollSize = size;
			scrollSize.cx *= method->getLeadEnds().GetWidth();
			scrollSize.cy *= (method->getLeadEnds().GetHeight()+2);
			scrollSize.cx += 10;
			scrollSize.cy += 10;
			SetScrollSizes(scrollSize);
		}
	}
			 */ //todo
	Invalidate();

}


void TouchAnalysisLeadEnds::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
		pTouchDoc->getPrintWhatAnalysis()._bLeadEnds = TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
}
	  

void TouchAnalysisLeadEnds::OnDraw(CDC* pDC)
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc == NULL)
		return;

	Method* method = pTouchDoc->getMethod();

	if (method == NULL) 
		return;	

	pDC->SetBkMode (TRANSPARENT);

	int number = pTouchDoc->getNumber();
	CString testStr;

	//work out the width of a single notation
	for (int i=0;i<number;i++)
		testStr+= GlobalFunctions::bellNumbersToChar(i+1);

	CFont * oldFont = pDC->SelectObject(&app()->_fontStd);
	
	CSize size = pDC->GetTextExtent(testStr);
	size.cx += 10;

	int left = 3;
	int top = 2;
	CString str;
	
	//create a bold font

	FontInfo boldFontInfo("MS Sans Serif");
	boldFontInfo._weight = FW_BOLD;
	LOGFONT lf;
	boldFontInfo.getLogFont(&lf);
	CFont boldFont;
	boldFont.CreateFontIndirect(&lf);

	pDC->SelectObject(&boldFont);
		/*
	//title;
	for (int i=0;i<method->getLeadEnds().GetWidth();i++)
	{
		str.Format("Part %d", i+1);
		CSize partSize = pDC->GetTextExtent(str);
		pDC->TextOut(left + (size.cx/2) - (partSize.cx/2) -2 ,top,str);
		left += size.cx;
	}

	pDC->SelectObject(&app()->_fontStd);

	top += (int)((float)size.cy*1.3);

	left = 3;
	for (int i=0;i<	method->getLeadEnds().GetWidth();i++)
	{
		pDC->MoveTo(left, top + size.cy +1 );
		pDC->LineTo(left + size.cx - 10, top+ size.cy +1);
		left += size.cx;
	}


	for (int j=0;j<method->getLeadEnds().GetHeight();j++)
	{
		left = 3;
		for (int i=0;i<method->getLeadEnds().GetWidth();i++)
		{
			if (method->getLeadEnds().GetAt(i,j))
			{
				pDC->TextOut(left,top,method->getLeadEnds().GetAt(i,j)->getAsText());
			}


			left += size.cx;
		}
		top += size.cy ;
		if (j==0) top += 3  ;
	}

		  */ //todo

	pDC->SelectObject(oldFont); 

}

void TouchAnalysisLeadEnds::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisLeadEnds::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisLeadEnds::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisLeadEnds::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}


CString TouchAnalysisLeadEnds::getCopyData(const Method& method, const MethodAnalysis& analysis)
{
	CString tempStr;
	CString str;
/* todo
	//title;
	for (int i=0;i<method->getLeadEnds().GetWidth();i++)
	{
		tempStr.Format("Part %d", i+1);
		str += tempStr;
		str += "\t";
	}

	str += "\r\n";

	for (int j=0;j<method->getLeadEnds().GetHeight();j++)
	{
		for (int i=0;i<method->getLeadEnds().GetWidth();i++)
		{
			if (method->getLeadEnds().GetAt(i,j))
			{
				str += method->getLeadEnds().GetAt(i,j)->getAsText();
			}
			str += "\t";
		}
		str += "\r\n";
	}	   */
	
	return str;
}

void TouchAnalysisLeadEnds::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);	
}


double TouchAnalysisLeadEnds::print(CPage *page, double nextRow)
{			  /*
	nextRow = PrintUtils::printTitle(page, "Lead Ends", nextRow, false);
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");
		 
	if (_method) 
	{
		CString str;

		//work out the width of a single notation
		for (int i=0;i<getNumber();i++)
			str += GlobalFunctions::bellNumbersToChar(i+1);

		double singleWidth = page->getTextWidth(str);
		double holdCurrCol = column;
		double holdNextRow = nextRow;

		int linesize = page->ConvertToMappedUnits(0.01, VERTRES);

		for (int i=0;i<_method->getLeadEnds().GetWidth();i++)
		{
			double currRow = nextRow;
			const double currCol = holdCurrCol;
			str.Format("Part %d", i+1);
			currRow = page->Print(currRow ,currCol, TEXT_LEFT|TEXT_BOLD, 8, str.GetBuffer(str.GetLength())) + .02;
			str.ReleaseBuffer();

			//what is the next starting point?
			holdCurrCol = page->GetNextLogicalColumn() + 0.2;

			for (int j=0;j<_method->getLeadEnds().GetHeight();j++)			
			{
				if (_method->getLeadEnds().GetAt(i,j))
				{
					str = _method->getLeadEnds().GetAt(i,j)->getAsText();
					currRow = page->Print(currRow ,currCol, TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));
					str.ReleaseBuffer();
				}
				
				if (holdCurrCol < (page->GetNextLogicalColumn() + 0.2))
					holdCurrCol = page->GetNextLogicalColumn() + 0.2;

				if (j==0)
				{
					page->Line(currRow, currCol, currRow, holdCurrCol - 0.2, linesize);
					currRow += 0.02;
				}
			}

			if (currRow > holdNextRow)
				holdNextRow = currRow;

			if ((holdCurrCol + singleWidth + 0.2) > page->getWidthinches())
			{
				holdCurrCol = column;
				nextRow = holdNextRow + .1;
			}
		}
		nextRow = holdNextRow + 0.15;
	}

	

	page->SetFont(oldFont);
	page->SetColor(oldColor);
					   */ //todo
	return nextRow;
}

