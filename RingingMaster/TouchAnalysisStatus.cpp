// TouchAnalysisStatus.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisStatus.h"
#include "TouchDoc.h"
#include "Method.h"
#include "Row.h"
#include "MethodAnalysis.h"
#include "PropItem.h"
#include "TouchHoldView.h"
#include "PrintUtils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisStatus

TouchAnalysisStatus::TouchAnalysisStatus()
{
}

TouchAnalysisStatus::~TouchAnalysisStatus()
{

}


BEGIN_MESSAGE_MAP(TouchAnalysisStatus, PropList)
	//{{AFX_MSG_MAP(TouchAnalysisStatus)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisStatus message handlers

void TouchAnalysisStatus::mainFrame_notifyUpdate(ViewType viewType)
{
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)) 
		return;

	
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	CString str;

	//we retrive the method names.
	if ((pTouchDoc)&&(pTouchDoc->getMethod())&&(pTouchDoc->getMethod()->hasAnalysis()))
	{
		Method * method = pTouchDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis(); 
		
		if (analysis->isTrue())
			addLine("The touch is:", "TRUE", RGB(120, 255, 120));	 
		else
			addLine("The touch is:", "FALSE", RGB(255, 120, 120));    

		//sort termination reason
		switch (method->getTerminateReason())
		{
		case TR_ROWS:
			str.Format("Row limit (%d)", method->getRowCount());
			addLine("Termination:", str, RGB(255, 120, 255));
			break;
		case TR_LEADS:
			str.Format("Lead limit (%d)", method->getLeadCount());
			addLine("Termination:", str, RGB(255, 120, 255));
			break;
		case TR_PARTS:
			str.Format("Part limit (%d)", method->getPartCount());	//todo check this is reporting correctly(previous encarnation has +1 - see source control)
			addLine("Termination:", str, RGB(255, 120, 255));
			break;
		case TR_CHANGE:
			str.Format("End change (%s)", method->getLastRow()->getAsText());
			addLine("Termination:", str);
			break;
		case TR_EMPTY_PARTS:
			addLine("Termination:", "Aborted - Empty parts found", RGB(255, 120, 120));
			break;
		case TR_CIRCLE:
			addLine("Termination:", "Aborted - Circular touch", RGB(255, 120, 120));
			break;
			
		default:
			break;
		}
		//number of parts.
		str.Format("%d", method->getPartCount());//todo check this is reporting correctly(previous encarnation has +1 - see source control)
		addLine("Number of parts:", str);

		//number of leads.
		str.Format("%d", method->getLeadCount());
		addLine("Number of leads:" , str);

		//number of rows.
		str.Format("%d", method->getRowCount());
		addLine("Number of changes:", str);

		//number of calls.
		str.Format("%d %s", method->getCallCount(), 
			((method->getCallCount() == 0)&&(!method->isSpliced()))? "- Plain Course":"");
		addLine("Number of calls:" , str);

		//start chaneg.
		str.Format("%s", method->getFirstRow()->getAsText());
		addLine("Start Change:",  str);

		//end chaneg.
		str.Format("%s", method->getLastRow()->getAsText());		
		addLine("End Change:",  str);

		//stroke.
		Stroke stroke = method->getLastRow()->getStroke();
		str.Format("%s", (stroke == hand)?Handstroke:Backstroke);
		addLine("Final stroke:",  str);

	//timings.
#ifdef _DEBUG
		str.Format("%.3f seconds", float((float)method->getProofTime()/1000));
		addLine("Proof & analysis time:", str);
#else
		str.Format("%.2f seconds", float((float)method->getProofTime()/1000));
		addLine("Proof & analysis time:", str);
#endif
	}
	else
	{
		deleteAllPropItems();
	}		


	Invalidate();
}

BOOL TouchAnalysisStatus::Create(CWnd * pParentWnd)
{
	BOOL result = PropList::Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|
								LBS_OWNERDRAWVARIABLE|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_HASSTRINGS, 
								CRect(0,0,0,0),
								pParentWnd,
								AAA_ANALYSIS_STATUS);	

	_devider[0] = 130;	

	if (result)
		SetFont(&app()->_fontStd);

	mainFrame()->addEventListener(this);

	return  result;
}


BOOL TouchAnalysisStatus::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}


void TouchAnalysisStatus::addLine(CString col0, CString col1, COLORREF color)
{
	PropItem* pItem; 

	pItem = findItem(col0);
	
	if (pItem == NULL)
	{
		pItem = new PropItem(PT_STATIC);

		AddPropItem(pItem);
	}
	pItem->_col[0] =  col0;
	pItem->_col[1] =  col1;

	if (color == -1)
		color = ::GetSysColor(COLOR_WINDOW);

	pItem->_backgroundColor = color;

}

void TouchAnalysisStatus::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
		pTouchDoc->getPrintWhatAnalysis()._bStatus = TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
	
}

void TouchAnalysisStatus::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisStatus::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisStatus::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisStatus::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}

CString TouchAnalysisStatus::getCopyData(const Method& method, const MethodAnalysis& analysis)
{
	CString tempStr, str;

	
	if (analysis.isTrue())
		str += copyLine("The touch is:", "TRUE");
	else
		str += copyLine("The touch is:", "FALSE");

	//sort termination reason
	switch (method.getTerminateReason())
	{
	case TR_ROWS:
		tempStr.Format("Row limit (%d)", method.getRowCount());
		str += copyLine("Termination:", tempStr);
		break;
	case TR_LEADS:
		tempStr.Format("Lead limit (%d)", method.getLeadCount());
		str += copyLine("Termination:", tempStr);
		break;
	case TR_PARTS:
		tempStr.Format("Part limit (%d)", method.getPartCount());   //todo check this is reporting correctly(previous encarnation has +1 - see source control)
		str += copyLine("Termination:", tempStr);
		break;
	case TR_CHANGE:
		tempStr.Format("End change (%s)", method.getLastRow()->getAsText());
		str += copyLine("Termination:", tempStr);
		break;
	case TR_EMPTY_PARTS:
		str += copyLine("Termination:", "Aborted - Empty parts found");
		break;
	case TR_CIRCLE:
		str += copyLine("Termination:", "Aborted - Circular touch");
		break;
		
	default:
		break;
	}
	//number of parts.
	tempStr.Format("%d", method.getPartCount());	//todo check this is reporting correctly(previous encarnation has +1 - see source control)
	str += copyLine("Number of parts:", tempStr);

	//number of leads.
	tempStr.Format("%d", method.getLeadCount());
	str += copyLine("Number of leads:" , tempStr);

	//number of rows.
	tempStr.Format("%d", method.getRowCount());
	str += copyLine("Number of changes:", tempStr);

	//number of calls.
	str.Format("%d %s", method.getCallCount(), 
		((method.getCallCount() == 0)&&(!method.isSpliced()))? "- Plain Course":"");
	str += copyLine("Number of calls:" , tempStr);

	//start chaneg.
	str.Format("%s", method.getFirstRow()->getAsText());
	str += copyLine("Start Change:",  tempStr);

	//end chaneg.
	tempStr.Format("%s", method.getLastRow()->getAsText());		
	str += copyLine("End Change:",  tempStr);

	//stroke.
	Stroke stroke = method.getLastRow()->getStroke();
	tempStr.Format("%s", (stroke == hand)?Handstroke:Backstroke);
	str += copyLine("Final stroke:",  tempStr);

	//timings.
	tempStr.Format("%.2f seconds", float((float)method.getProofTime()/1000));
	str += copyLine("Proof & analysis time:", tempStr);

	return str;
}

CString TouchAnalysisStatus::copyLine(CString col0, CString col1)
{
	return col0 + "\t" + col1 + " \r\n";
}		

void TouchAnalysisStatus::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);
}

double TouchAnalysisStatus::print(CPage *page, double nextRow)
{
	nextRow = PrintUtils::printTitle(page, "Status", nextRow, false);

	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	

	TABLEHEADER* pTable=new TABLEHEADER;        
	pTable->PointSize=	8;
	pTable->UseInches=	TRUE;
	pTable->AutoSize=	FALSE;
	pTable->Border=		FALSE;
	pTable->FillFlag=	FILL_NONE;
	pTable->NumColumns=	2;
	pTable->NumRows=	10;
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

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if ((pTouchDoc)&&(pTouchDoc->getMethod())&&(pTouchDoc->getMethod()->hasAnalysis()))
	{
		Method * method = pTouchDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis(); 

		//True / False
		page->Print(pTable,0,0, 8, TEXT_LEFT, 0.0, 0.0, "The Touch is:");
		if (analysis->isTrue())	  
		{
			page->SetColor(RGB(0,255,0));
			page->Print(pTable,0,1 , 8, TEXT_LEFT, 0.0, 0.0, "TRUE");
		}
		else
		{
			page->SetColor(RGB(255,0,0));
			page->Print(pTable,0,1, 8, TEXT_LEFT, 0.0, 0.0, "FALSE");
		}

		//termination
		page->SetColor(RGB(0,0,0));
		page->Print(pTable,1,0, 8, TEXT_LEFT, 0.0, 0.0, "Termination:");
		CString str;
		switch (method->getTerminateReason())
		{
		case TR_ROWS:
			page->SetColor(RGB(255, 120, 255));
			str.Format("Row limit (%d)",  method->getRowCount());
			break;
		case TR_LEADS:
			page->SetColor(RGB(255, 120, 255));
			str.Format("Lead limit (%d)", method->getLeadCount());
			break;
		case TR_PARTS:
			page->SetColor(RGB(255, 120, 255));
			str.Format("Part limit (%d)", method->getPartCount());   //todo check this is reporting correctly(previous encarnation has +1 - see source control)
			break;
		case TR_CHANGE:
			page->SetColor(RGB(0,0,0));
			str.Format("End change (%s)", method->getLastRow()->getAsText());
			break;
		case TR_EMPTY_PARTS:
			page->SetColor(RGB(255, 120, 120));
			str = "Aborted - Empty parts found";
			break;
		case TR_CIRCLE:
			page->SetColor(RGB(255, 120, 120));
			str = "Aborted - Circular touch";
			break;
			
		default:
			ASSERT(FALSE);
			break;
		}					  

		page->Print(pTable,1,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();
		
		page->SetColor(RGB(0,0,0));



		//number of parts.
		page->Print(pTable,2,0, 8, TEXT_LEFT, 0.0, 0.0, "Number of parts:");	
		str.Format("%d",method->getPartCount());//todo check this is reporting correctly(previous encarnation has +1 - see source control)
		page->Print(pTable,2,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();


		//number of leads.
		page->Print(pTable,3,0, 8, TEXT_LEFT, 0.0, 0.0, "Number of leads:");
		str.Format("%d", method->getLeadCount());
		page->Print(pTable,3,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();											  
		

		//number of rows.
		page->Print(pTable,4,0, 8, TEXT_LEFT, 0.0, 0.0, "Number of changes:");
		str.Format("%d", method->getRowCount());
		page->Print(pTable,4,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();											  

		//number of calls.
		page->Print(pTable,5,0, 8, TEXT_LEFT, 0.0, 0.0, "Number of calls:");
		str.Format("%d %s", method->getCallCount(), 
			((method->getCallCount() == 0)&&(!method->isSpliced()))? "- Plain Course":"");
		page->Print(pTable,5,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();											  

		//start chaneg.
		page->Print(pTable,6,0, 8, TEXT_LEFT, 0.0, 0.0, "Start Change:");
		str.Format("%s", method->getFirstRow()->getAsText());
		page->Print(pTable,6,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();											  

		//end chaneg.
		page->Print(pTable,7,0, 8, TEXT_LEFT, 0.0, 0.0, "End Change:");
		str.Format("%s", method->getLastRow()->getAsText());		
		page->Print(pTable,7,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();											  
		
		//stroke.
		page->Print(pTable,8,0, 8, TEXT_LEFT, 0.0, 0.0, "Final stroke:");
		Stroke stroke = method->getLastRow()->getStroke();
		str.Format("%s", (stroke == hand)?Handstroke:Backstroke);
		page->Print(pTable,8,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();											  

		//timeings
		page->Print(pTable,9,0, 8, TEXT_LEFT, 0.0, 0.0, "Proof & analysis time:");
		str.Format("%.2f seconds", float((float)method->getProofTime()/1000));
		page->Print(pTable,9,1, 8, TEXT_LEFT, 0.0, 0.0, str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();											  
	}																	  
	

	page->SetFont(oldFont);
	page->SetColor(oldColor);
	
	nextRow = pTable->EndRow + .1;

	delete pTable;
	pTable = NULL;

	return nextRow;

}