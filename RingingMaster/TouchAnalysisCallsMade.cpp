// TouchAnalysisCallsMade.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisCallsMade.h"
#include "Row.h"
#include "TouchDoc.h"
#include "Method.h"
#include "MethodAnalysis.h"
#include "TouchHoldView.h"
#include "PrintUtils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisCallsMade

TouchAnalysisCallsMade::TouchAnalysisCallsMade()
{

}

TouchAnalysisCallsMade::~TouchAnalysisCallsMade()
{
}


BEGIN_MESSAGE_MAP(TouchAnalysisCallsMade, ScrollWnd)
	//{{AFX_MSG_MAP(TouchAnalysisCallsMade)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisCallsMade message handlers

void TouchAnalysisCallsMade::mainFrame_notifyUpdate(ViewType viewType)
{
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)&& (viewType != vt_touch_simulator)) 
		return;
	
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc && pTouchDoc->getMethod() && pTouchDoc->getMethod()->hasAnalysis())
	{	 
		Method* method = pTouchDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis();

		CDC* pDC = GetDC();

		CFont * oldFont = pDC->SelectObject(&app()->_fontStd);
		
		//get the max individual string size
		_size = getMaxStringSize(method, pDC);

		//get the max number of strings
		int maxNumStrings = 1;
		for (int i=0;i<analysis->getCallsMade().GetSize();i++)
		{
			CStringArray* callMade = analysis->getCallsMade().GetAt(i);
			if (callMade != NULL)
			{
				if (callMade->GetSize() > maxNumStrings)
					maxNumStrings = callMade->GetSize();
			}
		}		  

		//multiply 
		_size .cx *= maxNumStrings;
        _size .cy = (int)((float)_size .cy*1.3);
		_size .cy *= (analysis->getCallsMade().GetSize());
		if (method->isSpliced())
			_size .cy *=2;

		//now deal with the part label
		//create a bold font
		FontInfo boldFontInfo("MS Sans Serif");
		boldFontInfo._weight = FW_BOLD;
		LOGFONT lf;
		boldFontInfo.getLogFont(&lf);
		CFont boldFont;
		boldFont.CreateFontIndirect(&lf);
		pDC->SelectObject(&boldFont);

		//get the size of the largest Part Label
		CString str;
		str.Format("Part %d:",analysis->getCallsMade().GetSize());
		CSize labelSize = pDC->GetTextExtent(str);

		//add on
		_size .cx += labelSize.cx;

		pDC->SelectObject(oldFont); 

		ReleaseDC(pDC);

		_size .cx += 20; //including 10 for space between label and data
		_size .cy += 10;
	    
		bool width = _size .cx >= SHRT_MAX;
		bool height = _size .cy >= SHRT_MAX;

		if ((width)||(height))
		    SetScrollSizes(500,500);
		else
			SetScrollSizes(_size );
	}

	Invalidate();
}

BOOL TouchAnalysisCallsMade::Create(CWnd * pParentWnd)
{
	BOOL ret = ScrollWnd::Create(NULL, "Course Head Wnd", WS_TABSTOP, CRect(0,0,100,100),		
									pParentWnd, AAA_COURSE_HEAD);

	mainFrame()->addEventListener(this);

	return ret;	
}

BOOL TouchAnalysisCallsMade::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}

void TouchAnalysisCallsMade::OnDraw(CDC* pDC)
{
	bool width = _size .cx >= SHRT_MAX;
	bool height = _size .cy >= SHRT_MAX;

	if ((width)||(height))
	{
		drawOverload(pDC);
		return;
	}

	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc && pTouchDoc->getMethod() && pTouchDoc->getMethod()->hasAnalysis())
	{	 
		Method* method = pTouchDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis();


		pDC->SetBkMode (TRANSPARENT);

		//get the size of the largest string
		CFont * oldFont = pDC->SelectObject(&app()->_fontStd);
		CSize size = getMaxStringSize(method, pDC);

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

		//get the size of the largest Part Label
		str.Format("Part %d:",analysis->getCallsMade().GetSize());
		CSize labelSize = pDC->GetTextExtent(str);

		//do the drawing

		int topIncrement = (int)((float)size.cy*1.3);

		for (int i=0;i<analysis->getCallsMade().GetSize();i++)
		{
			//title;
			pDC->SelectObject(&boldFont);
			str.Format("Part %d:", i+1);
			CSize partSize = pDC->GetTextExtent(str);
			pDC->TextOut(3,top,str);

			//text			
			pDC->SelectObject(&app()->_fontStd);

			CStringArray* callMade = analysis->getCallsMade().GetAt(i);
			if (callMade != NULL)
			{
				//calls
				for (int j=0;j<callMade->GetSize();j++) 
				{
					pDC->TextOut(labelSize.cx + 10 + (size.cx*j),top,callMade->GetAt(j));
				}

			}

			if (method->isSpliced())
			{
				CString lastSplice;
				CStringArray* splicesMade = analysis->getSplicesMade().GetAt(i);
				if (splicesMade != NULL)
				{		
					top += topIncrement;
					//calls
					for (int j=0;j<splicesMade->GetSize();j++) 
					{
						if (lastSplice != splicesMade->GetAt(j))   
						{
							lastSplice = splicesMade->GetAt(j);
							pDC->TextOut(labelSize.cx + 10 + (size.cx*j),top,lastSplice);
						}
					}	
				}
			}


			top += topIncrement;
		}

		pDC->SelectObject(oldFont); 
	}

}

void TouchAnalysisCallsMade::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
		pTouchDoc->getPrintWhatAnalysis()._bCallsMade= TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
}




//make sure that you have the correct font selected!!
CSize TouchAnalysisCallsMade::getMaxStringSize(Method* method, CDC* pDC)
{
	CSize size(1,1); 

	const MethodAnalysis* analysis = method->getAnalysis();

	for (int i=0;i<analysis->getCallsMade().GetSize();i++)
	{
		CStringArray* callMade = analysis->getCallsMade().GetAt(i);
		if (callMade != NULL)
		{
    		for (int j=0;j<callMade->GetSize();j++) 
			{
				CSize temp = pDC->GetTextExtent(callMade->GetAt(j));				
				if (temp.cx > size.cx) size.cx = temp.cx;
				if (temp.cy > size.cy) size.cy = temp.cy;
			}
		}
	}
	if (method->isSpliced())
	{
		for (int i=0;i<analysis->getSplicesMade().GetSize();i++)
		{
			CStringArray* splicesMade = analysis->getSplicesMade().GetAt(i);
			if (splicesMade != NULL)
			{
    			for (int j=0;j<splicesMade->GetSize();j++) 
				{
					CSize temp = pDC->GetTextExtent(splicesMade->GetAt(j));				
					if (temp.cx > size.cx) size.cx = temp.cx;
					if (temp.cy > size.cy) size.cy = temp.cy;
				}
			}
		}
	}

	size.cx += 2;

	return size;
}
			  

void TouchAnalysisCallsMade::drawOverload(CDC *pDC)
{
	pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	CString str;
	
	pDC->TextOut(20,20,"Your operating system is ");
	pDC->TextOut(25,40,"unable to show the calls made.");
	pDC->TextOut(30,50,"due to the size.");

}


void TouchAnalysisCallsMade::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisCallsMade::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisCallsMade::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisCallsMade::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}



CString TouchAnalysisCallsMade::getCopyData(const Method& method, const MethodAnalysis& analysis)
{
	CString str;
	CString temp;

	for (int i=0;i<analysis.getCallsMade().GetSize();i++)
	{
		//title;
		temp.Format("Part %d:\t", i+1);

		str += temp;
	
		//text			
		CStringArray* callMade = analysis.getCallsMade().GetAt(i);
		if (callMade != NULL)
		{
			//calls
			for (int j=0;j<callMade->GetSize();j++) 
			{
				str += callMade->GetAt(j);
				str += " ";
			}

		}

		if (method.isSpliced())
		{
			str += "\r\n\t";
			CStringArray* splicesMade = analysis.getSplicesMade().GetAt(i);
			CString lastSplice;
			if (splicesMade != NULL)
			{		
				//calls
				for (int j=0;j<splicesMade->GetSize();j++) 
				{
					if (lastSplice != splicesMade->GetAt(j))
					{
						lastSplice = splicesMade->GetAt(j);
						str += lastSplice;
					}
					else
					{
						str += " ";
					}

					str += " ";
				}	
			}
		}


		str += "\r\n";
	}
	return str;
}

void TouchAnalysisCallsMade::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);
}


double TouchAnalysisCallsMade::print(CPage *page, double nextRow)
{
	nextRow = PrintUtils::printTitle(page, "Calls Made", nextRow, false);
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc && pTouchDoc->getMethod() && pTouchDoc->getMethod()->hasAnalysis())
	{	 
		Method* method = pTouchDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis();

		CString str;

		double width = 0.0;
		//get the width of the largest string 
		for (int i=0;i<analysis->getCallsMade().GetSize();i++)
		{
			CStringArray* callMade= analysis->getCallsMade().GetAt(i);
			if (callMade != NULL)
			{
    			for (int j=0;j<callMade->GetSize();j++) 
				{
					double temp= page->getTextWidth(callMade->GetAt(j));
					if (temp > width) width = temp;
				}
			}
		}	

		for (int i=0;i<analysis->getCallsMade().GetSize();i++)
		{
			//print the label
			str.Format("Part %d", i+1);
			double holdNextRow = page->Print(nextRow, column, TEXT_LEFT|TEXT_BOLD, 8, str.GetBuffer(str.GetLength()));
			str.ReleaseBuffer();

			double holdLeft = page->GetNextLogicalColumn(TRUE, TRUE);
			double left = holdLeft; 

			CStringArray* callMade = analysis->getCallsMade().GetAt(i);

			if (callMade != NULL)
			{
			    //print the calls
    			for (int j=0;j<callMade->GetSize();j++) 
				{
					str = callMade->GetAt(j);
					holdNextRow = page->Print(nextRow, left , 
						TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));

					left += width;

					if ((left+ 0.2) >= page->getWidthinches()) 
					{
						left = holdLeft;
						nextRow = holdNextRow;
					}

					
					str.ReleaseBuffer();
				}

			    //print the splices
				if (method->isSpliced()) 
				{
					left = holdLeft;
					nextRow = holdNextRow;
		
					CStringArray* splicesMade = analysis->getSplicesMade().GetAt(i); 

					CString lastSplice;

					for (int j=0;j<splicesMade->GetSize();j++) 
					{
						if (lastSplice != splicesMade->GetAt(j))
							str = splicesMade->GetAt(j);
						else
							str = "";
						
						lastSplice = splicesMade->GetAt(j);

						holdNextRow = page->Print(nextRow, left , 
							TEXT_LEFT, 8, str.GetBuffer(str.GetLength()));

						left += width;

						if ((left+ 0.2) >= page->getWidthinches()) 
						{
							left = holdLeft;
							nextRow = holdNextRow;
						}

						str.ReleaseBuffer();
					}
				}
			}
			nextRow = holdNextRow;
		}						   
	}
	

	page->SetFont(oldFont);
	page->SetColor(oldColor);

	return nextRow;
}