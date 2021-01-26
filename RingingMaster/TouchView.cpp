// TouchView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchView.h"
#include "CellData.h"
#include "CellElement.h"
#include "ParseData.h"
#include "Method.h"
#include "EditBlockLetterDlg.h"
#include "Variance.h"
#include "Layout.h"
#include "PopupWnd.h"
#include "Definition.h"
#include "VarianceDlg.h"
#include "PrintData.h"
#include "PrintUtils.h"
#include "Row.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchView
const UINT TouchView::_findDialogMessage = RegisterWindowMessage(FINDMSGSTRING);
CString TouchView::_ClipboardTokenData = "Ringing Master Touch Data";
CString TouchView::_ClipboardTokenInfo = "Ringing Master Touch Info";


IMPLEMENT_DYNCREATE(TouchView, CScrollView32Bit)

TouchView::TouchView() :
_gridFontAscent(0),
_averageCharWidth(1),//set to 1 not 0 to avoid devide by zero problems
_extSelValid(false),
_extSelInMouse(false),
_dragging(false),
_resizeBlock(false),
_moveBlock(false),
_contextMouseDown(0,0),
_titleWidth(0),
_layout(NULL),
_popupAction(PA_NONE),
_popupElement(NULL),
_pFindDialog(NULL),
_statSize(0,0), 
_initedScrols(false)
{	
}

TouchView::~TouchView()
{
	_tip.DestroyWindow();
	delete _layout;
	cleanPopupInfo();
}


BEGIN_MESSAGE_MAP(TouchView, CScrollView32Bit)
	//{{AFX_MSG_MAP(TouchView)
	ON_COMMAND(ID_EDIT_WORD_LEFT, OnEditWordLeft)
	ON_COMMAND(ID_EDIT_WORD_RIGHT, OnEditWordRight)
	ON_COMMAND(ID_EDIT_LINE_DOWN, OnEditLineDown)
	ON_COMMAND(ID_EDIT_LINE_UP, OnEditLineUp)
	ON_COMMAND(ID_EDIT_CHAR_LEFT, OnEditCharLeft)
	ON_COMMAND(ID_EDIT_CHAR_RIGHT, OnEditCharRight)
	ON_COMMAND(ID_EDIT_TAB, OnEditTab)
	ON_COMMAND(ID_EDIT_UNTAB, OnEditUntab)
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_DELETE_BACK, OnEditDeleteBack)
	ON_COMMAND(ID_EDIT_LINE_HOME, OnEditLineHome)
	ON_COMMAND(ID_EDIT_LINE_END, OnEditLineEnd)
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_EDIT_DELETE_WORD, OnEditDeleteWord)
	ON_COMMAND(ID_EDIT_DELETE_WORD_BACK, OnEditDeleteWordBack)
	ON_COMMAND(ID_EDIT_TEXT_HOME, OnEditTextHome)
	ON_COMMAND(ID_EDIT_TEXT_END, OnEditTextEnd)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_EDIT_SCROLL_DOWN, OnEditScrollDown)
	ON_COMMAND(ID_EDIT_SCROLL_UP, OnEditScrollUp)
	ON_COMMAND(ID_EDIT_PAGE_DOWN, OnEditPageDown)
	ON_COMMAND(ID_EDIT_PAGE_UP, OnEditPageUp)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_EXT_CHAR_LEFT, OnEditExtCharLeft)
	ON_COMMAND(ID_EDIT_EXT_CHAR_RIGHT, OnEditExtCharRight)
	ON_COMMAND(ID_EDIT_EXT_LINE_DOWN, OnEditExtLineDown)
	ON_COMMAND(ID_EDIT_EXT_LINE_UP, OnEditExtLineUp)
	ON_COMMAND(ID_EDIT_EXT_PAGE_DOWN, OnEditExtPageDown)
	ON_COMMAND(ID_EDIT_EXT_PAGE_UP, OnEditExtPageUp)
	ON_COMMAND(ID_EDIT_EXT_WORD_LEFT, OnEditExtWordLeft)
	ON_COMMAND(ID_EDIT_EXT_WORD_RIGHT, OnEditExtWordRight)
	ON_COMMAND(ID_EDIT_EXT_LINE_END, OnEditExtLineEnd)
	ON_COMMAND(ID_EDIT_EXT_LINE_HOME, OnEditExtLineHome)
	ON_COMMAND(ID_EDIT_EXT_TEXT_END, OnEditExtTextEnd)
	ON_COMMAND(ID_EDIT_EXT_TEXT_HOME, OnEditExtTextHome)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDIT_SWITCH_OVRMODE, OnEditSwitchOvrmode)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID, OnUpdateShowGrid)
	ON_COMMAND(ID_SHOW_GRID, OnShowGrid)
	ON_COMMAND(ID_DELETE_COLUMN, OnDeleteColumn)
	ON_UPDATE_COMMAND_UI(ID_DELETE_COLUMN, OnUpdateDeleteColumn)
	ON_COMMAND(ID_DELETE_ROW, OnDeleteRow)
	ON_UPDATE_COMMAND_UI(ID_DELETE_ROW, OnUpdateDeleteRow)
	ON_COMMAND(ID_INSERT_COLUMN, OnInsertColumn)
	ON_COMMAND(ID_INSERT_ROW, OnInsertRow)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_ADDBLOCKDEFINITION, OnAddblockdefinition)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_DELETEBLOCK, OnDeleteblock)
	ON_UPDATE_COMMAND_UI(ID_DELETEBLOCK, OnUpdateDeleteblock)
	ON_UPDATE_COMMAND_UI(ID_ADDBLOCKDEFINITION, OnUpdateAddblockdefinition)
	ON_COMMAND(ID_ADDBLOCKDEFINITION_MENU, OnAddblockdefinitionMenu)
	ON_UPDATE_COMMAND_UI(ID_ADDBLOCKDEFINITION_MENU, OnUpdateAddblockdefinitionMenu)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDITBLOCKLETTER, OnEditblockletter)
	ON_UPDATE_COMMAND_UI(ID_EDITBLOCKLETTER, OnUpdateEditblockletter)
	ON_UPDATE_COMMAND_UI(ID_EDIT_VARIANCE, OnUpdateEditVariance)
	ON_COMMAND(ID_EDIT_VARIANCE, OnEditVariance)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_SHOW_COURSE_HEADS, OnShowCourseHeads)
	ON_UPDATE_COMMAND_UI(ID_SHOW_COURSE_HEADS, OnUpdateShowCourseHeads)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_FIND_PREVIOUS, OnEditFindPrevious)
	ON_COMMAND(ID_TOUCH_ADDVARIANCE, OnTouchAddvariance)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_TOOLBAR, OnFilePrintToolbar)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView32Bit::OnFilePrint)
	ON_MESSAGE(WM_POPUP_LIST_INDEX, OnPopupListTerm)	
    ON_REGISTERED_MESSAGE(_findDialogMessage, OnFindDialogMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TouchView printing

BOOL TouchView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return GetDocument()->OnPreparePrinting(pInfo, this);
}

void TouchView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->OnBeginPrinting(pDC, pInfo);
	CScrollView32Bit::OnBeginPrinting(pDC, pInfo);
}						 

void TouchView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	if (!_initedScrols)
	{
		SetScrollSizes(MM_TEXT, CSize(100,100));
		_initedScrols = true;
	}

	CScrollView32Bit::OnPrepareDC(pDC, pInfo);
	GetDocument()->OnPrepareDC(pDC, pInfo);
}

void TouchView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->OnEndPrinting(pDC, pInfo);
	CScrollView32Bit::OnEndPrinting(pDC, pInfo);
}

void TouchView::OnFilePrintPreview() 
{
	GetDocument()->OnFilePrintPreview(this);
}

void TouchView::OnFilePrint() 
{
	GetDocument()->OnFilePrint();
	CScrollView32Bit::OnFilePrint();
}

void TouchView::OnFilePrintToolbar() 
{
	GetDocument()->OnFilePrintToolbar();
	CScrollView32Bit::OnFilePrint();	
}

void TouchView::OnFilePrintMenu() 
{
	GetDocument()->OnFilePrintMenu();
	CScrollView32Bit::OnFilePrint();
}

void TouchView::OnPrintAnalysis() 
{
	int ID = GetDocument()->OnPrintAnalysis();

	if (ID == ID_PRINT)
		CScrollView32Bit::OnFilePrint();
	else if (ID == ID_PREVIEW)
		BCGPPrintPreview (this);
}



void TouchView::OnPopupPrintAnalysis() 
{
	GetDocument()->OnPopupPrintAnalysis();

#ifdef _DEBUG	 //always go to print preview in debug
	BCGPPrintPreview (this);
#else
	CScrollView32Bit::OnFilePrint();
#endif
	
}

void TouchView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{	
	if (GetDocument()->isPrintAnalysis())
		GetDocument()->doPrintAnalysis(pDC, pInfo); 
	else
		doPrintTouchView(pDC, pInfo);
}

void TouchView::doPrintTouchView(CDC* pDC, CPrintInfo* pInfo)
{
	//this gets called for each page in turn
	CPage*	page= new CPage(pInfo->m_rectDraw,pDC,MM_TEXT);

	//apply the appropriate offset;
	double vertSize = page->getLengthInches() - PrintUtils::getFootnoteHeight();
	page->setOffset(-(vertSize  * (pInfo->m_nCurPage -1)), 0.0);

	
	//leave the bottom clear
	//clear the area that has been printed on;
	page->ExcludeClipRect(page->getLengthInches() - PrintUtils::getFootnoteHeight(), 0.0,
							page->getLengthInches() + 0.1, 
							page->getWidthinches());


	double nextRow = (double)((double)(app()->GetProfileInt("Print", "Touch Top", 1000))/2540.0);		

	nextRow = printTitle(page, nextRow);
	nextRow = printTable(page, nextRow);
	nextRow = printDefinitions(page, nextRow);
	nextRow = printNotes(page, nextRow);

	//calculate the number of pages
	PrintData* printData = (PrintData*)pInfo->m_lpUserData;	   
	printData->_numPages = (int)(nextRow/vertSize) + 1;

	//before printing footnote and shareware stuff, remove the page offset / clip 
	page->setOffset(0.0,0.0);
	page->ResetClipRect();

	CString pageText;

	if (printData->_numPages > 1)
		pageText.Format("Page %d of %d",pInfo->m_nCurPage ,printData->_numPages);
		
	PrintUtils::printFootnote(page, pageText, PrintUtils::getFootnoteHeight());

	TRACE("Document has %d pages \r\n", printData->_numPages );

	if (!pInfo->m_bPreview)
		PrintUtils::printShareware(page);	

	delete page;
}

double TouchView::printTitle(CPage* page, double nextRow)
{
	if (GetDocument()->getName().IsEmpty())
		return nextRow;
	
	//get the larger font
	COLORREF oldColor = page->SetColor(GetDocument()->getTouchStyle().getFontInfoTitle()._color);
	CString oldFont=page->SetFont(GetDocument()->getTouchStyle().getFontInfoTitle()._name);
	int oldSize = page->SetFontSize(GetDocument()->getTouchStyle().getFontInfoTitle()._size/10);
	
	//print the name of the touch
	CString name = GetDocument()->getName();
	if (!name.IsEmpty())
	{
		double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	
		nextRow = page->Print(nextRow,column,name + " ");
		column = page->GetNextLogicalColumn(TRUE, TRUE);
	}

	//print the author of the touch
	page->SetColor(GetDocument()->getTouchStyle().getFontInfoAuthor()._color);
	page->SetFont(GetDocument()->getTouchStyle().getFontInfoAuthor()._name);
	page->SetFontSize(GetDocument()->getTouchStyle().getFontInfoAuthor()._size/10);
	CString author = GetDocument()->getAuthor();
	if (!author.IsEmpty())
	{
		double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	
		column += .15; //offset in inches
		nextRow = page->Print(nextRow,column,author + " ");
		column = page->GetNextLogicalColumn(TRUE, TRUE);
	}

	page->SetFont(oldFont);
	page->SetColor(oldColor);
	page->SetFontSize(oldSize);

	return nextRow;
}

double TouchView::printTable(CPage* page, double nextLine)
{			/* todo
	Method* method = GetDocument()->getMethod();

	int normalFontSize = max(GetDocument()->getTouchStyle().getFontInfoMain()._size/10,6);
	int smallerFontSize = max((int)((double)(normalFontSize*CellElement::SmallFontFactor)/10.0),6);

	BOOL showCourseHeads = GetDocument()->getShowCourseHeads();

	
	COLORREF oldColor = page->SetColor(_layout->_gridColor);
	CString oldFont=page->SetFont(GetDocument()->getTouchStyle().getFontInfoMain()._name);
	int oldSize = page->SetFontSize(normalFontSize);

	//convert 100s of mm to inches
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	

	const double offset = 0.05;
	const double topgap = 0.2; //this is the gap between the title and the grid

	TABLEHEADER* pTable=new TABLEHEADER;        
	pTable->PointSize=	max((normalFontSize) -3 , 6);
	pTable->UseInches=	TRUE;
	pTable->AutoSize=	FALSE;
	pTable->Border=		FALSE;
	pTable->FillFlag=	FILL_NONE;
	pTable->NumColumns=	GetDocument()->getGridArray()->GetWidth() + 
						((showCourseHeads)?GetDocument()->getNumber():0);
	pTable->NumRows=	GetDocument()->getGridArray()->GetHeight()-1;
	pTable->StartRow=	nextLine + topgap ;
	pTable->StartCol=	column;
	pTable->EndCol=		page->getWidthinches();
	pTable->HLines =	FALSE;
	pTable->VLines =	FALSE;
	pTable->NoHeader =	TRUE;
	if ((_layout->_gridColor == -1)||
		(GetDocument()->isShowGrid() == FALSE))
	{
		pTable->_leftVLinesLimit = 10000;
		pTable->_topHLinesLimit = 10000;
	}
	else //show grid
	{
		pTable->_topHLinesLimit = 0;
		pTable->_bottomHlinesLimit = GetDocument()->getGridArray()->GetHeight()-1;
		pTable->_leftVLinesLimit = ((showCourseHeads)?(GetDocument()->getNumber() + 1):0); //remembering gap
		pTable->_rightVlinesLimit = GetDocument()->getGridArray()->GetWidth() + ((showCourseHeads)?(GetDocument()->getNumber()): -2);
	}
	
	//find the correct spacings
	page->SetColor(RGB(255,255,255)); //so it will not be seen

	page->Print(0.0, 0.0, "1");
	const double leadHeadSpacing = page->GetNextLogicalColumn() * 1.2;

	if (showCourseHeads)
	{
		//spacing for the lead head
		for (int i=0;i<GetDocument()->getNumber();i++)
			pTable->ColDesc[i].Init(leadHeadSpacing,"");
		//space between the grid and the lead heads
		pTable->ColDesc[GetDocument()->getNumber()].Init(0.15,"");
	}
	//spacing for the main grid
	for (int i=0;i<GetDocument()->getGridArray()->GetWidth();i++)
	{
		double colWidth = .1; //min
		for (int j=0;j<GetDocument()->getGridArray()->GetHeight();j++)
		{
			CellData* cell = getCellData(GridPos(i,j));
			double temp_col = 0.0;
			for (int k=0;k<cell->getLength();k++)
			{
				CellElement* element = cell->getElement(k);
				CString temp_str(element->getChar());
				page->Print(0.0, temp_col, temp_str.GetBuffer(temp_str.GetLength()));
				temp_col = page->GetNextLogicalColumn();
				if (element->getChar() == '[')
				{
					page->SetFontSize(smallerFontSize);
					temp_str = element->getData();
					page->Print(0.0, temp_col, temp_str.GetBuffer(temp_str.GetLength()));
					page->SetFontSize(normalFontSize);
					temp_col = page->GetNextLogicalColumn();
				}
			}
			if (temp_col > colWidth)
				colWidth = page->GetNextLogicalColumn() ;
		}

		colWidth += (2*offset);

		pTable->ColDesc[i + ((showCourseHeads)? (GetDocument()->getNumber()+1):0)].Init(colWidth,"");
	}

	page->SetColor(_layout->_gridColor);

	page->Table(pTable);	

	double cellSpace = page->ConvertToInches(pTable->pClsTable->GetVerticalSpacing(),VERTRES);

	//do the course heads?
	if ((method != NULL)&&(showCourseHeads))
	{	
		CString thisStr, lastStr, thisChar;
		Row* lastRow = NULL;
		Row* thisRow = NULL;
		for (int j=0;j<method->getCourseHeads().GetHeight();j++)
		{
			thisRow = method->getCourseHeads().GetAt(0,j);
			if (thisRow)
			{
				if (lastRow)
					lastStr = lastRow->getAsText();
				else 
					lastStr.Empty();

				thisStr = thisRow->getAsTextDiff(lastRow);
				for(int i=0;i<thisStr.GetLength();i++)
				{
					thisChar = thisStr.GetAt(i);
					if (thisChar == ' ')
					{
						if (i < lastStr.GetLength())
						{
							thisChar = lastStr.GetAt(i);
						}
						page->SetColor(GetDocument()->getTouchStyle().getColor(tsc_unchangedLHColor));
					}
					else	
					{
						page->SetColor(GetDocument()->getTouchStyle().getColor(tsc_changedLHColor));
					}
					page->Print(pTable,j,i,normalFontSize,
						TEXT_CENTER, 0.0, 0.0, thisChar.GetBuffer(1));
					thisChar.ReleaseBuffer();
				}
				lastRow = thisRow;
			}
		}

		//course head line 

		double firstCell = cellSpace + nextLine + topgap ;
		int lineWidth = page->ConvertToMappedUnits(.02,VERTRES); //guess at the width of 2 pixels
		page->SetColor(_layout->_gridColor);
		page->Line(firstCell, column, firstCell, 
			column + (leadHeadSpacing * GetDocument()->getNumber()),lineWidth);
	}
	
	//do the main cells, including underlining
	for (int i=0;i<GetDocument()->getGridArray()->GetWidth();i++)
	{
		//find the left of the cell
		int temp = 0;
		for (int l=0;l<i + ((showCourseHeads)?GetDocument()->getNumber()+1:0);l++)
			temp += (int)pTable->ColDesc[l].Width;

		double cellLeft = column + page->ConvertToInches(temp, HORZRES);

		for (int j=0;j<GetDocument()->getGridArray()->GetHeight();j++)
		{
			CellData* cell = getCellData(GridPos(i,j));
			double startChar = offset;
			double startUnderlineLeft = 0.0;
			double startUnderlineTop = 0.0;
			bool inUnderline = false;
			
			for (int k=0;k<cell->getLength();k++)
			{
				CellElement* element = cell->getElement(k);

				if (inUnderline)
				{
					if (element->isValid())
					{
						double endUnderline = cellLeft + startChar;
						page->SetColor(RGB(255,0,0));
						page->underline(startUnderlineTop, startUnderlineLeft, endUnderline-startUnderlineLeft, 0.01);
						inUnderline = false;
					}
				}
				else
				{
					//start of the new underline
					if (!element->isValid())
					{
						inUnderline = true;
						startUnderlineLeft = cellLeft + startChar;
						startUnderlineTop = nextLine + topgap + (j*cellSpace) + (cellSpace*0.80);
					}
				}

				page->SetColor(getTextColor(element->getType()));
				CString letter(element->getChar());

			
				startChar  = page->Print(pTable,j,i + ((showCourseHeads)?GetDocument()->getNumber()+1:0),
								normalFontSize,
								TEXT_LEFT, startChar, -0.00, letter.GetBuffer(1));
				letter.ReleaseBuffer();
				if (letter == '[')
				{
					CString variant = element->getData();
					double vertOffset_small = page->getTextHeight("1");
					startChar  = page->Print(pTable,j,i + ((showCourseHeads)?GetDocument()->getNumber()+1:0),
									smallerFontSize, //smallet font
									TEXT_LEFT, startChar, -vertOffset_small, variant.GetBuffer(variant.GetLength()));
					variant.ReleaseBuffer();
				}				
			}
			if(inUnderline)
			{
				double endUnderline = cellLeft + startChar;
				page->SetColor(RGB(255,0,0));
				page->underline(startUnderlineTop, startUnderlineLeft, endUnderline-startUnderlineLeft, 0.01);
				inUnderline = false;
			}
		}
	}

	//now draw the brackets for the block defintions.
	//we need to use the device context for this
	TouchDoc * pDoc = GetDocument();
	
	page->SetColor(GetDocument()->getTouchStyle().getColor(tsc_blockColor));
	
	//get the table with (miss out the last col 
	double tableRight = 0.0;
	int bracketColumn = GetDocument()->getGridArray()->GetWidth() - 1 + 
						((showCourseHeads)?GetDocument()->getNumber() + 1:0);
	for(int i=0;i<bracketColumn;i++)
		tableRight += pTable->ColDesc[i].Width; //this is in mapped units
	tableRight = page->ConvertToInches((int)tableRight, HORZRES) + 
				+ column + 0.05; //right gap
	

	for (int i=0;i<pDoc->getTouchBlocks().GetSize();i++)
	{
		TouchBlock* tb = pDoc->getTouchBlocks().GetAt(i);
		
		//get the print versions of the position
		double bracketTop = nextLine + topgap + (cellSpace * tb->_start);
		double bracketHeight = cellSpace * (tb->_end-tb->_start);

		page->bracket(bracketTop, tableRight, leadHeadSpacing/3, bracketHeight, 0.02, tb->_name);
	}


	//we dont need this any more
	double lastTablePos=pTable->EndRow;
	delete pTable;
	pTable = NULL;


	page->SetFont(oldFont);
	page->SetColor(oldColor);
	page->SetFontSize(oldSize);

	return lastTablePos + cellSpace;

	*/ 

	return 0;

}

double TouchView::printNotes(CPage* page, double nextLine)
{
	TouchDoc * pDoc = GetDocument();

	if ((pDoc->isShowNotes())&&(!pDoc->getNotes().IsEmpty()))
	{

		const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	
		COLORREF oldColor = page->SetColor(GetDocument()->getTouchStyle().getColor(tsc_notesColor));
		CString oldFont=page->SetFont(GetDocument()->getTouchStyle().getFontInfoNotes()._name);
		int oldSize = page->SetFontSize(GetDocument()->getTouchStyle().getFontInfoNotes()._size/10);
		
		page->SetColor(getTextColor(PT_DEFINITION));
		nextLine = page->Print(nextLine, column ,pDoc->getNotes()); 
		
		//for some reason, this sets the next line correctly
		page->SetFontSize(6);
		nextLine = page->Print(nextLine, column ,""); 
		
		page->SetFont(oldFont);
		page->SetColor(oldColor);
		page->SetFontSize(oldSize);
	}

	return nextLine;
}

double TouchView::printDefinitions(CPage* page, double nextLine)
{
	TouchDoc * pDoc = GetDocument();
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	
	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont=page->SetFont(GetDocument()->getTouchStyle().getFontInfoMain()._name);
	int oldSize = page->SetFontSize(GetDocument()->getTouchStyle().getFontInfoMain()._size/10);
	
	for (int i=0;i<pDoc->getDefinitions().GetSize();i++)
	{
		Definition* definition = pDoc->getDefinitions().GetAt(i);
		if (pDoc->isDefinitionUsed(definition))
		{
			CellData* cell = definition->getCellData();
			//draw the name, and the equals
			CString str = definition->getName() + " = ";

			page->SetColor(getTextColor(PT_DEFINITION));
			double nextRow = page->Print(nextLine, column, str);

			//draw the rest 
			
			for (int j=0;j<cell->getLength();j++)
			{
				CellElement* element = cell->getElement(j);
				page->SetColor(getTextColor(element->getType()));
				CString letter(element->getChar());
				page->Print(nextLine, page->GetNextLogicalColumn() ,letter.GetBuffer(1)); 
				letter.ReleaseBuffer();
			}
			nextLine = nextRow;
		}
	}

	
	page->SetFont(oldFont);
	page->SetColor(oldColor);
	page->SetFontSize(oldSize);

	return nextLine;
}
/////////////////////////////////////////////////////////////////////////////
// TouchView drawing

void TouchView::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT, CSize(100,100));
	CScrollView32Bit::OnInitialUpdate();

	calculateFontSizes();
	GetDocument()->invalidateColumnWidths();

	SetTimer(AAA_TOUCH_CARET_TIMER, 2000, NULL);
	updateCaret();
}


/////////////////////////////////////////////////////////////////////////////
// TouchView diagnostics

#ifdef _DEBUG
void TouchView::AssertValid() const
{
	CScrollView32Bit::AssertValid();
}

void TouchView::Dump(CDumpContext& dc) const
{
	CScrollView32Bit::Dump(dc);
}

TouchDoc* TouchView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(TouchDoc)));
	return (TouchDoc*)m_pDocument;
}
 
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TouchView message handlers

void TouchView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	GetDocument()->setTab(0);

	if ((bActivate)&&(m_nMapMode > 0))
	{
		updateCaret();
	} 
	else 
	{
		HideCaret();
	}

	//remove the find dialog box.
	if ((_pFindDialog != NULL)&&
		((pActivateView != this)||
		 (!bActivate)))
	{
		_pFindDialog->DestroyWindow();
		_pFindDialog = NULL;
	}

	CScrollView32Bit::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void TouchView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ScreenPos scPos = screenHitTest(point + GetScrollPosition());
	if (scPos == SP_GRID)
	{
		if ((_extSelValid)&&
			(isInSelection(gridHitTest(point + GetScrollPosition()))))
		{
			_dragging = true;
			SetCursor(app()->LoadCursor((nFlags &= MK_CONTROL)?IDC_POINTER_COPY:IDC_POINTER_DRAG));
			SetCapture();
			HideCaret();
		}
		else 
		{
			GetDocument()->getGridPos()  = gridHitTest(point + GetScrollPosition());

			//set the extended selection 
			_extSelInMouse = true;

			startExtSel(GetDocument()->getGridPos());
			_extSelValid = false;

			Invalidate();
			updateCaret();
			SetCapture();
		}
	}
	else if (scPos == SP_BLOCKS)
	{
		BlockPos blockPos = blocksHitTest(point+ GetScrollPosition());
		if (blockPos._type == BlockPos::BP_NODE)
		{
			//find upto two blocks
			_startTouchBlock = NULL;
			_endTouchBlock = NULL;
			for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
			{
				TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
				if (tb->_start == blockPos._line)
					_startTouchBlock = tb;
				if (tb->_end == blockPos._line)
					_endTouchBlock = tb;
			}
			if ((_startTouchBlock)||(_endTouchBlock))
			{
				_resizeBlock = true;
				SetCapture();
				HideCaret();
			}
		}
		else if (blockPos._type == BlockPos::BP_BRACKET)
		{
					//find upto two blocks
			_startTouchBlock = NULL;
			_endTouchBlock = NULL;
			for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
			{
				TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
				if ((tb->_start <= blockPos._line)&&
					(tb->_end > blockPos._line))
				{
					_startTouchBlock = tb;
					_blockOffset = ( point.y + GetScrollPosition().y) - 
								   (tb->_start*_layout->_height) - 
								   (_layout->_height); 
					_moveBlock= true;
					SetCapture();
					HideCaret();
				}
			}
		}
	}

	CScrollView32Bit::OnLButtonDown(nFlags, point);
}

void TouchView::startExtSel(const GridPos& startGrid)
{
	_extSelValid = true;
	_extSelStart = startGrid;
	_extSelEnd = startGrid;
}

void TouchView::OnMouseMove(UINT nFlags, CPoint point) 
{
	_contextMouseDown = point + GetScrollPosition();

	ScreenPos scPos = screenHitTest(point + GetScrollPosition());
		
	//tool tips
	_tip.removeAllStrings(); 	
	
	GridPos gridPos = gridHitTest(point + GetScrollPosition(), true);
	CellData* cellData = getCellData(gridPos);
	if ((cellData)&&(scPos == SP_GRID))
	{
		if (cellData->getLength() > gridPos._c)
		{
			CString tipStr(cellData->getElement(gridPos._c)->getTipText());
			if (!tipStr.IsEmpty())
			{
				_tip.addString(tipStr);
				_tip.Set(point);
			}
		}
	}

	
	if (_dragging)
	{
		SetCursor(app()->LoadCursor((nFlags &= MK_CONTROL)?IDC_POINTER_COPY:IDC_POINTER_DRAG));
		GetDocument()->getGridPos()  = gridPos;
		updateCaret();
	}
	else if (_extSelInMouse)
	{
		//find the new selection
		GetDocument()->getGridPos()  = gridHitTest(point + GetScrollPosition());
		
		updateExtSelToCursor();

		HideCaret();

		//show the new selection
		Invalidate();
	}
	else if (_resizeBlock)
	{
		SetCursor(app()->LoadStandardCursor(IDC_SIZENS));		

		BlockPos bp = blocksHitTest(point+ GetScrollPosition(),true);
			
		if (bp._type == BlockPos::BP_INBETWEEN)
		{
			GetDocument()->negotiateBlockMove(_startTouchBlock, bp._line, true);
			GetDocument()->negotiateBlockMove(_endTouchBlock, bp._line, false);
		}

		GetDocument()->getGridPos()  = gridPos;
		updateCaret();

		HideCaret();

		//show the new block pos
		Invalidate();
	}
	else if (_moveBlock)
	{
		SetCursor(app()->LoadCursor(IDC_RESIZENS));

		BlockPos bp = blocksHitTest(point+ GetScrollPosition() - CPoint(0,_blockOffset),true);
			
		if (bp._type == BlockPos::BP_INBETWEEN)
		{
			int size = _startTouchBlock->_end - _startTouchBlock->_start;
			if (bp._line < _startTouchBlock->_start)
			{
				GetDocument()->negotiateBlockMove(_startTouchBlock, bp._line, true);
				GetDocument()->negotiateBlockMove(_startTouchBlock, bp._line+size, false);
				if (_startTouchBlock->_end - _startTouchBlock->_start !=size)
					_startTouchBlock->_end = _startTouchBlock->_start + size;
			}
			else
			{
				GetDocument()->negotiateBlockMove(_startTouchBlock, bp._line+size, false);
				GetDocument()->negotiateBlockMove(_startTouchBlock, bp._line, true);
				if (_startTouchBlock->_end - _startTouchBlock->_start !=size)
					_startTouchBlock->_start = _startTouchBlock->_end - size;

			}
		}

		GetDocument()->getGridPos()  = gridPos;
		updateCaret();

		HideCaret();

		//show the new block pos
		Invalidate();
	}

	
	CScrollView32Bit::OnMouseMove(nFlags, point);
}

void TouchView::updateExtSelToCursor()
{
	_extSelEnd = GetDocument()->getGridPos();
}

void TouchView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (_dragging)
	{
		GetDocument()->getGridPos()  = gridHitTest(point + GetScrollPosition());

		if (!isInSelection(GetDocument()->getGridPos()))
		{		
			//get the copy information
			TouchClipInfo clipInfo;
			CString dataSource = getCopyData(true, clipInfo);
			//delete the original cells
			if (!(nFlags &= MK_CONTROL))
			{
				GridPos start = _extSelStart;
				GridPos end = _extSelEnd;
				normaliseGridPair(start, end);
				GetDocument()->tvEditDeleteSelection(start, end, false);
			}
			
			doPaste(clipInfo, dataSource);
		}

		_extSelValid = false;
		GetDocument()->setUpdatePoint("Move Selection",true, true, true, true);//delete caret
	}
	else if (_extSelInMouse)
	{
		//find the new selection
		GetDocument()->getGridPos()  = gridHitTest(point + GetScrollPosition());

		updateExtSelToCursor();

		if (evaluateGridPos(_extSelStart, _extSelEnd) == 0)
		{
			_extSelValid = false;
		}
		else
		{
			_extSelValid = true;
		}
	}
	else if (_resizeBlock)
	{
		GetDocument()->setUpdatePoint("Edit Block");
	}
	else if (_moveBlock)
	{
		GetDocument()->setUpdatePoint("Move Block");
	}


	//switch off all flags
	_extSelInMouse = false;
	_dragging = false;
	_resizeBlock = false;
	_moveBlock = false;

	updateCaret();
	Invalidate();
	ReleaseCapture();

	CScrollView32Bit::OnLButtonUp(nFlags, point);
}

void TouchView::drawExtSelection(CDC* pDC)
{
	GridPos startGrid = _extSelStart;
	GridPos endGrid   = _extSelEnd;

	if (evaluateGridPos(_extSelStart, _extSelEnd) == 0)
		return;

	normaliseGridPair(startGrid, endGrid);
	
	int nOldMode = pDC->SetROP2(R2_NOT);

	CPoint start, end ;

	for (int i=startGrid._y; i<=endGrid._y;i++)
	{
		if (i == startGrid._y)
		{
			start = gridToClient(startGrid, true);
			start.x += CELL_PAD_LEFT;
		}
		else
			start = gridToClient(GridPos(0, i, 0), true);

		if (i == endGrid._y)
		{
			end = gridToClient(endGrid, true);
			end.x += CELL_PAD_LEFT;
		}
		else
			end = gridToClient(GridPos(GetDocument()->getGridArray()->GetWidth(),i, 0), true);

		end.y += _layout->_height;
		CRect rect(start, end);
		
		pDC->InvertRect(&rect);
	}
	
	pDC->SetROP2(nOldMode);
}


void TouchView::updateCaret(bool bScrollToPos)
{
	if (GetFocus() == this)
	{
		if (bScrollToPos)
			scrollToPos();
						   
		CreateSolidCaret(CARET_WIDTH, _layout->_caretHeight);
		CPoint caretPos = gridToClient(GetDocument()->getGridPos());
		caretPos.x += (CELL_PAD_LEFT - CARET_WIDTH);
		caretPos.y += CELL_PAD_TOP;
		ShowCaret();
		SetCaretPos(caretPos);
	}
	else
	{
		HideCaret();
	}
	setScrolls(); //to reset the scrolls after printing
}

void TouchView::scrollToPos(GridPos gridPos)
{
	CPoint caretPos;
	if ((gridPos._x == -1)||(gridPos._y == -1))
		caretPos = gridToClient(GetDocument()->getGridPos());
	else
		caretPos = gridToClient(gridPos,true);

	caretPos.x += (CELL_PAD_LEFT - CARET_WIDTH);
	caretPos.y += CELL_PAD_TOP;
	
	CRect rClient;
	GetClientRect(&rClient);

	CSize scrollBy(0,0);
	
	if (caretPos.y < 0)
	{
		scrollBy.cy = ((caretPos.y/m_lineDev.cy)-1)*m_lineDev.cy;
	}
	else if(caretPos.y + _layout->_height > rClient.bottom)
	{
		scrollBy.cy = (((caretPos.y + _layout->_height - rClient.bottom) 
								/m_lineDev.cy)+1)*m_lineDev.cy;
	}

	if (caretPos.x < 0)
	{
		scrollBy.cx = ((caretPos.x/m_lineDev.cx)-1)*m_lineDev.cx;
	}
	else if(caretPos.x + CARET_WIDTH > rClient.right)
	{
		scrollBy.cx = (((caretPos.x + CARET_WIDTH  - rClient.right) 
								/m_lineDev.cx)+1)*m_lineDev.cx;
	}

	OnScrollBy(scrollBy);
}

void TouchView::OnDraw(CDC* pDC)
{
		
	CBitmap memBitmap; // Offscreen bitmap
	CBitmap* oldBitmap; // bitmap originally found in memDC
	CRect drawRect; // Rectangle of drawing area.

   	//CPaintDC dc(this);
	CDC& dc = *pDC;

	CDC memDC;

	// Create a Memory DC
	memDC.CreateCompatibleDC(&dc);
	
	//set the map mode
	dc.SetMapMode(MM_TEXT);
	memDC.SetMapMode(MM_TEXT);

	//get the whole client size and offset to paint the correct area
	GetClientRect(&drawRect);
	drawRect += GetScrollPosition();

	//create a bitmap, and select into the memDC
	memBitmap.CreateCompatibleBitmap(&dc, drawRect.Width(), drawRect.Height());
	oldBitmap = memDC.SelectObject(&memBitmap);
	
	// by default shift viewport origins in negative direction of scroll
	memDC.SetViewportOrg(-GetScrollPosition());
	dc.SetViewportOrg(-GetScrollPosition());

	//erase the background
    memDC.FillSolidRect(&drawRect, GetSysColor(COLOR_WINDOW));  

	//perform the GDI calls
	doDraw(&memDC);

	// Copy the offscreen bitmap onto the screen.
	dc.BitBlt(drawRect.left, drawRect.top, drawRect.Width(), drawRect.Height(),
						&memDC, drawRect.left, drawRect.top, SRCCOPY);
	
	//Swap back the original bitmap.
	memDC.SelectObject(oldBitmap);

}

void TouchView::doDraw(CDC *pDC)
{
	if (!_layout)
		return;

	pDC->SetMapMode(MM_TEXT);
	pDC->SetBkMode(TRANSPARENT);
	
	CFont*  oldFont = pDC->SelectObject(&GetDocument()->getTouchStyle().getFontMain());	

	_layout->setTitleheight(drawTitle(pDC)); //this needs to be done first!!
	
	drawCourseHeads(pDC);
	drawGridLines(pDC);	
	drawGridText(pDC);	
	drawUnderLine(pDC);
	if((_extSelValid)||(_extSelInMouse))
		drawExtSelection(pDC);

	drawTouchBlocks(pDC);
	drawDefinitions(pDC);
	_layout->setNotesSize(drawNotes(pDC));

	pDC->SelectObject(oldFont);

}




int TouchView::drawTitle(CDC *pDC)
{
	TouchDoc * pTouchDoc = GetDocument();

	if ((pTouchDoc->getName().IsEmpty())&&
		(pTouchDoc->getAuthor().IsEmpty()))
		return -1;


	//ordinary font
	CSize ext(0,0);

	CFont* oldFont = pDC->SelectObject(&pTouchDoc->getTouchStyle().getFontTitle());
	CString name = pTouchDoc->getName();
    if (!name.IsEmpty())
	{	
		pDC->SetTextColor(pTouchDoc->getTouchStyle().getColor(tsc_titleColor));
		pDC->TextOut(_layout->getCourseHeadLeft(),_layout->getTop(true), name); 

		ext = pDC->GetTextExtent(name);
	}

	const int authorOffset = 13;
	CString author = pTouchDoc->getAuthor();
	if (!author.IsEmpty())
	{
		pDC->SelectObject(pTouchDoc->getTouchStyle().getFontAuthor());
		pDC->SetTextColor(pTouchDoc->getTouchStyle().getColor(tsc_authorColor));
		pDC->TextOut(_layout->getCourseHeadLeft() + authorOffset,_layout->getTop(true) + ext.cy, author); 

		CSize authorExt = pDC->GetTextExtent(author);
		ext.cx = max(ext.cx, (authorExt.cx + authorOffset));
		ext.cy += authorExt.cy;
	}

	pDC->SelectObject(oldFont);		 

	_titleWidth = (_layout->getCourseHeadLeft()*2) + ext.cx;

	return ext.cy;
}


void TouchView::drawDefinitions(CDC *pDC)
{
	TouchDoc * pDoc = GetDocument();
	
	int top = _layout->getTop() + (pDoc->getGridArray()->GetHeight() * _layout->_height);

	for (int i=0;i<pDoc->getDefinitions().GetSize();i++)
	{
		Definition* definition = pDoc->getDefinitions().GetAt(i);
		if (pDoc->isDefinitionUsed(definition))
		{
			int left = _layout->getCourseHeadLeft();

			CellData* cell = definition->getCellData();
			//draw the name, and the equals
			CString str = definition->getName() + " = ";

			pDC->SetTextColor(getTextColor(PT_DEFINITION));
			pDC->DrawText(str,  
						CRect(left,top,left + _averageCharWidth + 1, top + _layout->_height),
						DT_LEFT |DT_NOCLIP| DT_NOPREFIX); 
			left += pDC->GetTextExtent(str).cx;

			//draw the rest 
			
			for (int j=0;j<cell->getLength();j++)
			{
				CellElement* element = cell->getElement(j);
				pDC->SetTextColor(getTextColor(element->getType()));

				pDC->DrawText(CString(element->getChar()),
						CRect(left,top,left + _averageCharWidth + 1, top + _layout->_height),
						DT_LEFT |DT_NOCLIP| DT_NOPREFIX); 
				
				CString str(element->getChar());
				left += pDC->GetTextExtent(str).cx;
			}
			top += _layout->_height;
		}
	}
}

CSize TouchView::drawNotes(CDC *pDC)
{
	TouchDoc * pTouchDoc = GetDocument();

	if ((pTouchDoc->isShowNotes())&&(!pTouchDoc->getNotes().IsEmpty()))
	{
	
		int top = _layout->getTop() + (pTouchDoc->getGridArray()->GetHeight() * _layout->_height) + 
				   (pTouchDoc->getUsedDefinitionsCount() * _layout->_height) + 
				   (pTouchDoc->isShowGrid()?10:0);

		int left = _layout->getCourseHeadLeft();		 
		CRect winrect;
		GetClientRect(&winrect);
		int width = max((max(300, getSize(false).cx)), (winrect.Width()-left-20));

		CRect rect(left, top, left + width,100000);	 //really big arbetery number

		//get the notes font
		CFont* oldFont = pDC->SelectObject(&pTouchDoc->getTouchStyle().getFontNotes());
		pDC->SetTextColor(pTouchDoc->getTouchStyle().getColor(tsc_notesColor));
		int height = pDC->DrawText(pTouchDoc->getNotes(), rect, DT_WORDBREAK | DT_NOPREFIX );
		pDC->SelectObject(oldFont);		 

		return 	CSize(width, height);
	}
	else 
	{
		return CSize(0,0);
	}
}


void TouchView::drawCourseHeads(CDC *pDC)
{
	if (GetDocument()->getShowCourseHeads() == FALSE) return;
 /*	todo
	Method* method = GetDocument()->getMethod();

	if (method == NULL) return;	

	CPoint orig;
	int left = _layout->getCourseHeadLeft();;
	int top = _layout->getTop();
	CString thisStr, lastStr, thisChar;
	
	Row* lastRow = NULL;
	Row* thisRow = NULL;

	//draw line
	if (GetDocument()->isCourseBased())
	{
		CPen pen(PS_SOLID, 2, _layout->_gridColor);
		CPen * oldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(left,top + _layout->_height);
		pDC->LineTo(left + ((_averageCharWidth + 1)* method->getNumber()),top + _layout->_height);
		pDC->SelectObject(oldPen);
	}

	for (int j=0;j<method->getCourseHeads().GetHeight();j++)
	{
		left = _layout->getCourseHeadLeft();
		thisRow = method->getCourseHeads().GetAt(0,j);
		if (thisRow)
		{
			if (lastRow)
				lastStr = lastRow->getAsText();
			else 
				lastStr.Empty();

			thisStr = thisRow->getAsTextDiff(lastRow);
			for(int i=0;i<thisStr.GetLength();i++)
			{
				thisChar = thisStr.GetAt(i);
				if (thisChar == ' ')
				{
					if (i < lastStr.GetLength())
					{
						thisChar = lastStr.GetAt(i);
					}
					pDC->SetTextColor(GetDocument()->getTouchStyle().getColor(tsc_unchangedLHColor));
				}
				else
				{
					pDC->SetTextColor(GetDocument()->getTouchStyle().getColor(tsc_changedLHColor));
				}
				pDC->DrawText(thisChar,  
					CRect(left, top, left + _averageCharWidth + 1, top + _layout->_height),
					DT_CENTER |DT_NOCLIP| DT_NOPREFIX); 
				left += _averageCharWidth + 1;	  
			}
			lastRow = thisRow;
		}
		top += _layout->_height;
	}	*/
}





void TouchView::drawGridLines(CDC *pDC)
{
	TouchDoc * pDoc = GetDocument();

	if (_layout->_gridColor == -1) return;
	if (pDoc->isShowGrid() == FALSE) return;

	CPen pen(PS_SOLID, _layout->_gridWidth, _layout->_gridColor);
	CPen * oldPen = pDC->SelectObject(&pen);

	//vertical lines
	int bottom = _layout->getTop() + (pDoc->getGridArray()->GetHeight() * _layout->_height);
	int left = _layout->getGridLeft();
	for (int i=0;i<pDoc->getGridArray()->GetWidth() + 1;i++)
	{
		pDC->MoveTo(left, _layout->getTop());
		pDC->LineTo(left, bottom);
		left += pDoc->getColumnWidth(pDC, i);
	}
		
	//horizontal lines
	for (int i=0;i<pDoc->getGridArray()->GetHeight() + 1;i++)
	{
		pDC->MoveTo(_layout->getGridLeft(),_layout->getTop() + (_layout->_height * i));
		pDC->LineTo(left,    _layout->getTop() + (_layout->_height * i));
	}

	pDC->SelectObject(oldPen);
}

void TouchView::drawGridText(CDC *pDC)
{
	TouchDoc * pDoc = GetDocument();
	
	pDC->SetTextAlign(TA_LEFT);

	int left = _layout->getGridLeft();
	int startCell;

	for (int i=0;i<pDoc->getGridArray()->GetWidth();i++)
	{

		for (int j=0;j<pDoc->getGridArray()->GetHeight();j++)
		{
			startCell = left + CELL_PAD_LEFT;

			for (int k=0;k<getCellData(GridPos(i,j))->getLength();k++)
			{
				CellElement* element = getCellData(GridPos(i,j))->getElement(k);
				element->draw(startCell,
							  _layout->getTop()  + (j * _layout->_height) + CELL_PAD_TOP,
							  pDC,
							  getTextColor(element->getType()),
							  GetDocument()->getFontMain());
				startCell += element->getExtent(pDC,GetDocument()->getFontMain()).cx;
			}
		}
		left += pDoc->getColumnWidth(pDC, i);
	}
}

void TouchView::drawUnderLine(CDC *pDC)
{
	TouchDoc * pDoc = GetDocument();
	
	pDC->SetTextAlign(TA_LEFT);

	CPoint startCell;
	startCell.x = _layout->getGridLeft() + CELL_PAD_LEFT;

	bool inUnderline;
	CPoint startUnderline;
	int lengthUnderline, lengthChar, startChar;

	for (int i=0;i<pDoc->getGridArray()->GetWidth();i++)
	{
		for (int j=0;j<pDoc->getGridArray()->GetHeight();j++)
		{
			startCell.y = _layout->getTop()  + (j *_layout->_height) + _gridFontAscent + CELL_PAD_TOP;
			startChar = startCell.x;
			startUnderline.y = startCell.y;
			inUnderline = false;
			lengthUnderline = 0;
			//loop through every letter in cell
			for (int k=0;k<getCellData(GridPos(i,j))->getLength();k++) 
			{
				lengthChar = getCellData(GridPos(i,j))->getElement(k)->getExtent(pDC, GetDocument()->getFontMain()).cx;
				if (inUnderline)
				{
					if (!getCellData(GridPos(i,j))->isValid(k))
						lengthUnderline += lengthChar;
					else
					{
						underline(pDC, startUnderline, lengthUnderline);
						inUnderline = false;
					}
				}
				else 
				{
					if (!getCellData(GridPos(i,j))->isValid(k))
					{
						inUnderline = true;
						startUnderline.x = startChar;
						lengthUnderline = lengthChar;
					}
				}
				startChar += lengthChar;
			}
			if (inUnderline)
				underline(pDC, startUnderline, lengthUnderline);
		}
		startCell.x += pDoc->getColumnWidth(pDC, i);
	}
	//now draw the Definitions underlines;
	int usedDefs = 0;
	for (int i=0;i<pDoc->getDefinitions().GetSize();i++)
	{
		Definition* definition = pDoc->getDefinitions().GetAt(i);
		if (pDoc->isDefinitionUsed(definition))
		{
			startCell.y = _layout->getTop()  + ((usedDefs + pDoc->getGridArray()->GetHeight() ) *_layout->_height) 
							+ _gridFontAscent + CELL_PAD_TOP;
			startCell.x = _layout->getCourseHeadLeft();
			startCell.x += pDC->GetTextExtent(definition->getName() + " = ").cx;
			startChar = startCell.x;
			startUnderline.y = startCell.y;
			inUnderline = false;
			lengthUnderline = 0;
			//loop through every letter in cell
			CellData* cell = definition->getCellData();
			for (int k=0;k<cell->getLength();k++) 
			{
				lengthChar = cell->getElement(k)->getExtent(pDC, GetDocument()->getFontMain()).cx;
				if (inUnderline)
				{
					if (!cell->isValid(k))
						lengthUnderline += lengthChar;
					else
					{
						underline(pDC, startUnderline, lengthUnderline);
						inUnderline = false;
					}
				}
				else 
				{
					if (!cell->isValid(k))
					{
						inUnderline = true;
						startUnderline.x = startChar;
						lengthUnderline = lengthChar;
					}
				}
				startChar += lengthChar;
			}
			if (inUnderline)
				underline(pDC, startUnderline, lengthUnderline);

			usedDefs++;
		}		
	}
}

void TouchView::drawTouchBlocks(CDC *pDC)
{
	TouchDoc * pDoc = GetDocument();
	
	pDC->SetTextAlign(TA_LEFT);
	
	CPen pen(PS_SOLID, 2, GetDocument()->getTouchStyle().getColor(tsc_blockColor));
	CPen * oldPen = pDC->SelectObject(&pen);

	CRect rcGrid = getGridRect();

	pDC->SetTextColor(GetDocument()->getTouchStyle().getColor(tsc_blockColor));

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	

	for (int i=0;i<pDoc->getTouchBlocks().GetSize();i++)
	{
		TouchBlock* tb = pDoc->getTouchBlocks().GetAt(i);

		CPoint point = drawBracket(pDC, CPoint(rcGrid.right+ _layout->_blockSpace, rcGrid.top + _layout->_height * tb->_start+1), 
						_averageCharWidth/3, 
						_layout->_height * (tb->_end-tb->_start));

		pDC->TextOut(point.x + _layout->_blockSpace, point.y- halfTextHeight, tb->_name);

			
	}

	pDC->SelectObject(oldPen);
}

//returns the point of the bracket
CPoint TouchView::drawBracket(CDC *pDC, CPoint start, int width, int height)
{
	int dev = height/6;
	int half = start.y + height/2;
	int whole = start.y + height;

	pDC->Arc(	start.x-width,		start.y, 
				start.x+width,		start.y+dev+dev,
				start.x+width,		start.y+dev, 
				start.x,			start.y);

	pDC->MoveTo(start.x+width,		start.y+dev);
	pDC->LineTo(start.x+width,		half-dev);

	pDC->Arc(	start.x+width,		half - dev-dev,
				start.x+width+width+width,half,
				start.x+width,		half-dev,
				start.x+width+width,    half);

	pDC->Arc(	start.x+width,		half,
				start.x+width+width+width,half+dev+dev,
				start.x+width+width,    half,
				start.x+width,		half+dev);

	pDC->MoveTo(start.x+width,		half+dev);
	pDC->LineTo(start.x+width,		whole-dev);

	pDC->Arc(	start.x-width,		whole-dev-dev, 
				start.x+width,		whole,
				start.x,  			whole,
				start.x+width,		whole-dev);

	return CPoint(start.x+width+width, half);
}


void TouchView::underline(CDC *pDC, CPoint start, int length)
{
	int yOffset = 0;
	bool up = false;
	for (int i=0;i<length;i++)
	{
		pDC->SetPixel(start.x + i, start.y + yOffset, RGB(255,0,0));
		if (yOffset == 0) 
		{
			yOffset = 1;
			up = false;
		}
		else if (yOffset == 1)
		{
			if (up == true)
			{
				up = false;
				yOffset = 0;
			}
			else
			{
				up = true;
				yOffset = 2;
			}
		}
		else if (yOffset == 2) 
		{
			yOffset = 1;
			up = true;
		}
	}	
}

//takes client pos. returns grid pos
GridPos TouchView::gridHitTest(const CPoint& point, bool noRounding)
{
	GridPos pos(point.x, point.y);
	TouchDoc * pDoc = GetDocument();

	CDC * dc = GetDC();
	CFont * oldFont = dc->SelectObject(&GetDocument()->getTouchStyle().getFontMain());	

	int left = _layout->getGridLeft();
	for (int i=0;i<pDoc->getGridArray()->GetWidth();i++)
	{
		left += pDoc->getColumnWidth(dc,i);
		if (pos._x < left)
		{
			pos._x = i;
			break;
		}
	}

	int top  = _layout->getTop();
	for (int i=0;i<pDoc->getGridArray()->GetHeight();i++)
	{
		top += _layout->_height;
		if (pos._y <= top)
		{
			pos._y = i;
			break;
		}
	}

	if (pos._x <0) pos._x = 0;
	if (pos._y <0) pos._y = 0;

	if (pos._x > pDoc->getGridArray()->GetWidth ()-1) pos._x = pDoc->getGridArray()->GetWidth ()-1;
	if (pos._y > pDoc->getGridArray()->GetHeight()-1) pos._y = pDoc->getGridArray()->GetHeight()-1;

	//have a valid grid pos, now sort the cell
	int horzOffset = point.x - gridToClient(pos).x - CELL_PAD_LEFT - GetScrollPosition().x;

	int length = 0;
	CellData* cell = getCellData(pos);

	for	(pos._c=0;pos._c<cell->getLength();pos._c++)
	{
		int charLength = getCellData(pos)->getElement(pos._c)->getExtent(dc, GetDocument()->getFontMain()).cx;
		
		if (horzOffset < length + ((noRounding)?charLength:charLength/2))
		{
			break;
		}
		
		length += charLength;
	}
	
	dc->SelectObject(oldFont); 
	ReleaseDC(dc);

	return pos;
}

void TouchView::OnEditWordLeft() 
{
	terminateExtSel(true);
	GetDocument()->tvEditWordLeft();
	updateCaret();
}


void TouchView::OnEditExtWordLeft() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditWordLeft();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();
}

void TouchView::OnEditWordRight() 
{
	terminateExtSel(false);
	GetDocument()->tvEditWordRight();
	updateCaret();
}

void TouchView::OnEditExtWordRight() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditWordRight();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();
}

void TouchView::OnEditLineDown() 
{
	terminateExtSel(false);
	GetDocument()->tvEditLineDown();
	updateCaret();
}

void TouchView::OnEditExtLineDown() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditLineDown();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();
}

void TouchView::OnEditLineUp() 
{
	terminateExtSel(true);
	GetDocument()->tvEditLineUp();
	updateCaret();
}

void TouchView::OnEditExtLineUp() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditLineUp();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();
}

void TouchView::terminateExtSel(bool caretToStart)
{
	if (_extSelValid)
	{
		normaliseGridPair(_extSelStart, _extSelEnd);
		if (caretToStart)
		{
			GetDocument()->getGridPos() = _extSelStart;
		}
		else
		{
			GetDocument()->getGridPos() = _extSelEnd;
		}

		_extSelValid = false;
		Invalidate();
	}
}

void TouchView::OnEditCharLeft() 
{
	terminateExtSel(true);
	GetDocument()->tvEditCharLeft();
	updateCaret();
}

void TouchView::OnEditExtCharLeft() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditCharLeft();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();
}

void TouchView::OnEditCharRight() 
{
	terminateExtSel(false);
	GetDocument()->tvEditCharRight();
	updateCaret();
}

void TouchView::OnEditExtCharRight() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditCharRight();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();
}

void TouchView::OnEditTab() 
{
	terminateExtSel(false);
	GetDocument()->tvEditTab();
	updateCaret();
}

void TouchView::OnEditUntab() 
{
	terminateExtSel(true);
	GetDocument()->tvEditUntab();
	updateCaret();
}

void TouchView::OnEditDelete() 
{
	if (_extSelValid)
	{
		normaliseGridPair(_extSelStart, _extSelEnd);
		GetDocument()->tvEditDeleteSelection(_extSelStart, _extSelEnd);
		_extSelValid = false;
		Invalidate();
	}
	else
		GetDocument()->tvEditDelete();
	updateCaret();
}

void TouchView::OnEditDeleteBack() 
{
	if (_extSelValid)
	{
		normaliseGridPair(_extSelStart, _extSelEnd);
		GetDocument()->tvEditDeleteSelection(_extSelStart, _extSelEnd);
		_extSelValid = false;
		Invalidate();
	}
	else
		GetDocument()->tvEditDeleteBack();
	updateCaret();
}

void TouchView::OnEditLineHome() 
{
	terminateExtSel(true);
	GetDocument()->tvEditLineHome();
	updateCaret();
}

void TouchView::OnEditExtLineHome() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditLineHome();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();				
}

void TouchView::OnEditLineEnd() 
{
	terminateExtSel(false);
	GetDocument()->tvEditLineEnd();
	updateCaret();				
}

void TouchView::OnEditExtLineEnd() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditLineEnd();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();				
}

void TouchView::OnChar(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/) 
{
	if (_extSelValid)
	{
		if (nChar != 27) //escape key
		{
			normaliseGridPair(_extSelStart, _extSelEnd);
			GetDocument()->tvEditDeleteSelection(_extSelStart, _extSelEnd);
		}

		_extSelValid = false;
		Invalidate();
	}

	if (nChar == 13) //enter key
	{
		OnEditLineDown();
	}
	else if (nChar == '[')
	{
		Variances variances;
		GetDocument()->getUsedVariances(variances); 

		if (variances.GetSize() <= 0) 
		{
			newVarianceOnChar();
			return;
		}
		else 
		{
			CPoint clientPoint = gridToClient(GetDocument()->getGridPos()) ;
			ClientToScreen(&clientPoint);
			clientPoint.x -= 10;
			clientPoint.y += _layout->_height;
			
			//populate the list
			cleanPopupInfo();

			_popupStrings.Add(new StringColor(
									"New Variance",
									GetDocument()->getTouchStyle().getColor(tsc_varianceColor)));

			for (int i=0;i<variances.GetSize();i++)
				_popupStrings.Add(new StringColor(
									variances.GetAt(i)->getDisplayString(),
									GetDocument()->getTouchStyle().getColor(tsc_varianceColor)));

			//launch the dialog
			PopupWnd* popup = new PopupWnd;
			popup->Create(this);
			popup->Set(clientPoint, &_popupStrings);
			_popupAction = PA_NEW_VAR_LEFT;

			//tidy up
			for (int i=0;i<variances.GetSize();i++)
				delete variances.GetAt(i);

			return;
		}
	}
	else
	{
		GetDocument()->tvEditChar(nChar);
	}
	updateCaret();
}

CPoint TouchView::gridToClient(const GridPos& gridPos, bool extSel)
{
	//this is the client offset
	TouchDoc * pDoc = GetDocument();
	if ((extSel)||(pDoc->getGridArray()->IsValid(gridPos)))
	{
		CDC * dc = GetDC();
		CFont * oldFont = dc->SelectObject(&GetDocument()->getTouchStyle().getFontMain());	
	
		CPoint pos(_layout->getGridLeft(), _layout->getTop()  + (gridPos._y * _layout->_height));

		//get the width
		for (int i=0;i<gridPos._x;i++)
			pos.x += pDoc->getColumnWidth(dc,i);


		//now get the cell text offset if needed 
		if (gridPos._c !=0)
		{
			CellData* cell = getCellData(gridPos);

			for (int i=0;i<gridPos._c;i++)
				if (i<cell->getLength())
					pos.x += cell->getElement(i)->getExtent(dc, GetDocument()->getFontMain()).cx;
		}

		dc->SelectObject(oldFont); 
		ReleaseDC(dc);

		if (!extSel)
			pos -= GetScrollPosition();
		return pos;	
	}
	else 
		return CPoint(0,0);
}

void TouchView::calculateFontSizes()
{
	CDC * dc = GetDC();
	CFont * oldFont = dc->SelectObject(&GetDocument()->getTouchStyle().getFontMain());	

	TEXTMETRIC tm;
	dc->GetTextMetrics(&tm);
	_layout->_caretHeight = tm.tmHeight; 
	_layout->_height = _layout->_caretHeight + CELL_PAD_TOP + CELL_PAD_BOTTOM;	
	_gridFontAscent = tm.tmAscent;
	_averageCharWidth = (int)((double)tm.tmAveCharWidth *1.5);
	_layout->_courseHeadTextWidth = (_averageCharWidth + 1) * GetDocument()->getNumber();

	dc->SelectObject(oldFont);
	ReleaseDC(dc);

	GetDocument()->invalidateColumnWidths();
	setScrolls();
}

void TouchView::setScrolls()
{
	CSize size = getSize();	

	if (size != _statSize)
	{

		SetScrollSizes(MM_TEXT, size, 
								CSize(_averageCharWidth*3, _layout->_height*3),	
								CSize(_averageCharWidth, _layout->_height));	
		_statSize = size;
	}
}


CellData* TouchView::getCellData(const GridPos& gridPos)
{
	return GetDocument()->getCellData(gridPos);	
}


void TouchView::OnKillFocus(CWnd* pNewWnd) 
{
	CScrollView32Bit::OnKillFocus(pNewWnd);
	
	HideCaret();
}

COLORREF TouchView::getTextColor(ParseType pt)
{
	TouchStyleColorType touchStyleColorType = GetDocument()->getTouchStyleColorTypeFromParseType(pt);

	return GetDocument()->getTouchStyle().getColor(touchStyleColorType);				
}

void TouchView::OnEditDeleteWord() 
{
	if (_extSelValid)
	{
		normaliseGridPair(_extSelStart, _extSelEnd);
		GetDocument()->tvEditDeleteSelection(_extSelStart, _extSelEnd);
		_extSelValid = false;
		Invalidate();
	}
	else
		GetDocument()->tvEditDeleteWord();
	updateCaret();
}

void TouchView::OnEditDeleteWordBack() 
{
	if (_extSelValid)
	{
		normaliseGridPair(_extSelStart, _extSelEnd);
		GetDocument()->tvEditDeleteSelection(_extSelStart, _extSelEnd);
		_extSelValid = false;
		Invalidate();
	}
	else
		GetDocument()->tvEditDeleteWordBack();
	updateCaret();
}	

void TouchView::OnEditTextHome() 
{
	terminateExtSel(true);
	GetDocument()->tvEditTextHome();
	updateCaret();
}

void TouchView::OnEditExtTextHome() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditTextHome();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();
}

void TouchView::OnEditTextEnd() 
{
	terminateExtSel(false);
	GetDocument()->tvEditTextEnd();
	updateCaret();
}

void TouchView::OnEditExtTextEnd() 
{
	if (!_extSelValid) 
		startExtSel(GetDocument()->getGridPos());
	GetDocument()->tvEditTextEnd();
	updateExtSelToCursor();
	Invalidate();
	updateCaret();				
}

void TouchView::OnEditScrollDown() 
{
	terminateExtSel(true);

	DWORD style = GetStyle();

	if (style & WS_VSCROLL)
	{
		OnScrollBy(CSize(0,m_lineDev.cy));
		if (gridToClient(GetDocument()->getGridPos()).y < 0)
		{
			GetDocument()->tvEditLineDown();
		}
		updateCaret(false);
	}
}

void TouchView::OnEditScrollUp() 
{
	terminateExtSel(true);

	DWORD style = GetStyle();

	if (style & WS_VSCROLL)
	{
		CRect rClient;
		GetClientRect(&rClient);

		OnScrollBy(CSize(0,-m_lineDev.cy));

		if (gridToClient(GetDocument()->getGridPos()).y 
			+ _layout->_height > rClient.bottom)
		{
			GetDocument()->tvEditLineUp();
		}
		updateCaret(false);
	}
}

void TouchView::OnEditPageDown() 
{
	terminateExtSel(false);

	DWORD style = GetStyle();

	if (style & WS_VSCROLL)
	{
		CRect rClient;
		GetClientRect(&rClient);

		int lines = (rClient.bottom/m_lineDev.cy);
		OnScrollBy(CSize(0,(lines)*m_lineDev.cy));

		for (int i=0;i<lines;i++)
		{
			GetDocument()->tvEditLineDown();
		}
		updateCaret(false);
	}	
}

void TouchView::OnEditExtPageDown() 
{
	DWORD style = GetStyle();

	if (style & WS_VSCROLL)
	{
		if (!_extSelValid) 
			startExtSel(GetDocument()->getGridPos());
		
		CRect rClient;
		GetClientRect(&rClient);

		int lines = (rClient.bottom/m_lineDev.cy);
		OnScrollBy(CSize(0,(lines)*m_lineDev.cy));

		for (int i=0;i<lines;i++)
		{
			GetDocument()->tvEditLineDown();
		}

		updateExtSelToCursor();
		Invalidate();
		updateCaret(false);
	}	
}

void TouchView::OnEditPageUp() 
{
	terminateExtSel(true);

	DWORD style = GetStyle();

	if (style & WS_VSCROLL)
	{
		CRect rClient;
		GetClientRect(&rClient);

		int lines = (rClient.bottom/m_lineDev.cy);
		OnScrollBy(CSize(0,-(lines*m_lineDev.cy)));

		for (int i=0;i<lines;i++)
		{
			GetDocument()->tvEditLineUp();
		}

		updateCaret(false);
	}
}

void TouchView::OnEditExtPageUp() 
{
	DWORD style = GetStyle();

	if (style & WS_VSCROLL)
	{
		if (!_extSelValid) 
			startExtSel(GetDocument()->getGridPos());
		

		CRect rClient;
		GetClientRect(&rClient);

		int lines = (rClient.bottom/m_lineDev.cy);
		OnScrollBy(CSize(0,-(lines*m_lineDev.cy)));

		for (int i=0;i<lines;i++)
		{
			GetDocument()->tvEditLineUp();
		}

		updateExtSelToCursor();
		Invalidate();
		updateCaret(false);
	}										
}

void TouchView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CScrollView32Bit::OnUpdate(pSender, lHint, pHint); 	

	if (!_layout)
		_layout = new Layout(GetDocument());

	if (::IsWindow(m_hWnd)&&(GetDocument()->getGridArray()))
	{
		setScrolls();
		updateCaret();
	}
}	


CSize TouchView::getSize(bool includeNotes)	
{
	CSize size(0,0);
	TouchDoc* pTouchDoc = GetDocument();

	size.cy =	_layout->getTop() + //for bottom
				_layout->getTop(true) + //for top of grid
				(_layout->_height * pTouchDoc->getGridArray()->GetHeight());
	
	size.cx = _layout->getCourseHeadLeft() + _layout->getGridLeft() ;

	CDC* pDC = GetDC();
	CFont * oldFont = pDC->SelectObject(&GetDocument()->getTouchStyle().getFontMain());	

	
	for (int i=0;i<pTouchDoc->getGridArray()->GetWidth();i++)
		size.cx += pTouchDoc->getColumnWidth(pDC, i);

	pDC->SelectObject(oldFont); 
	ReleaseDC(pDC);

	//blocks
	if (pTouchDoc->getTouchBlocks().GetSize() > 0)
		size.cx += ((_layout->_blockSpace*2) + (int)((float)_averageCharWidth * 1.3));

	if (size.cx < _titleWidth)
		size.cx = _titleWidth;

	//definitions
	size.cy += (pTouchDoc->getUsedDefinitionsCount() * _layout->_height);

	//notes
	if (includeNotes)
	{
		size.cx = max(_layout->getNotesSize().cx + _layout->getCourseHeadLeft(), size.cx );
	
		size.cy += (pTouchDoc->isShowGrid()?10:0);

		size.cy += _layout->getNotesSize().cy;

	}

	return size;
}



int TouchView::evaluateGridPos(const GridPos& first, const GridPos& last)
{
	if(first._y < last._y)
		return 1;
	else if (first._y > last._y)
		return -1;
	else //grid y must be equal
	{
		if(first._x < last._x)
			return 1;
		else if (first._x > last._x)
			return -1;
		else //grid x must be equal
		{
			if(first._c < last._c)
				return 1;
			else if (first._c > last._c)
				return -1;
			else //cell must be equal
				return 0;
		}
	}
}

void TouchView::normaliseGridPair(GridPos& start, GridPos& end)
{
	if (evaluateGridPos(start, end) < 0)
	{
		GridPos temp = start;
		start = end;
		end = temp;
	}
}

void TouchView::OnEditSelectAll() 
{
	_extSelStart = GridPos();
	_extSelEnd= GridPos(GetDocument()->getGridArray()->GetWidth()-1,
							   GetDocument()->getGridArray()->GetHeight()-1);
	_extSelEnd = getCellData(_extSelEnd)->getLength();
	_extSelValid = true;	
	
	GetDocument()->getGridPos() = _extSelStart;

	Invalidate();
	updateCaret();
}

void TouchView::OnEditCopy() 
{
	doEditCopy();
}

bool TouchView::doEditCopy()
{
	//put your text in source
	if(OpenClipboard())
	{
		EmptyClipboard();

		TouchClipInfo clipInfo;

		CString strSource = getCopyData(false, clipInfo);//the clipinfo here is dumped
		CString dataSource = getCopyData(true, clipInfo);
		
		
		HGLOBAL strBuff, dataBuff, infoBuff;
		char * buffer;
		
		//str
		strBuff = GlobalAlloc(GMEM_DDESHARE, strSource.GetLength()+1);
		buffer = (char*)GlobalLock(strBuff);
		strcpy(buffer, LPCSTR(strSource));
		GlobalUnlock(strBuff);
		SetClipboardData(CF_TEXT,strBuff);

		//data
		dataBuff = GlobalAlloc(GMEM_DDESHARE, dataSource.GetLength()+1);
		buffer = (char*)GlobalLock(dataBuff);
			strcpy(buffer, LPCSTR(dataSource));
		GlobalUnlock(dataBuff);
		SetClipboardData(::RegisterClipboardFormat(_ClipboardTokenData),dataBuff);

		//info
		infoBuff = GlobalAlloc(GMEM_DDESHARE, sizeof(TouchClipInfo));
		TouchClipInfo * infoBuffer = (TouchClipInfo*)GlobalLock(infoBuff);
		*infoBuffer = clipInfo;
		GlobalUnlock(infoBuff);
		SetClipboardData(::RegisterClipboardFormat(_ClipboardTokenInfo),infoBuff);		

		CloseClipboard();
		return true;
	}
	return false;
}

CBitmap* TouchView::getCopyDataBitmap()
{
	CBitmap* memBitmap = new CBitmap(); //my new bitmap
	CBitmap* oldBitmap = NULL; // bitmap originally found in memDC

   	//device contexts
	CDC* pDC = GetDC();
	CDC bitmapDC;

	// Create a Bitmap DC
	bitmapDC.CreateCompatibleDC(pDC);
	
	//set the map mode
	pDC->SetMapMode(MM_TEXT);
	bitmapDC.SetMapMode(MM_TEXT);

	//get the whole client size and offset to paint the correct area
	CSize size = getSize();

	//create a bitmap, and select into the memDC
	memBitmap->CreateCompatibleBitmap(pDC, size.cx, size.cy);
	oldBitmap = bitmapDC.SelectObject(memBitmap);
	
	// by default shift viewport origins in negative direction of scroll
/*	memDC.SetViewportOrg(-GetScrollPosition());
	dc.SetViewportOrg(-GetScrollPosition());*/ //I have left this code here for when we have the ability to copy a sub set of the image

	
	//erase the background
    bitmapDC.FillSolidRect(0,0,size.cx, size.cy, GetSysColor(COLOR_WINDOW));  

	//perform the GDI calls
	OnDraw(&bitmapDC);

	
	//Swap back the original bitmap.
	bitmapDC.SelectObject(oldBitmap);

	ReleaseDC(pDC);

	return memBitmap;
}



CString TouchView::getCopyData(bool forData, TouchClipInfo& clipInfo)
{
	if (!_extSelValid) return "";

	GridPos startGrid = _extSelStart;
	GridPos endGrid   = _extSelEnd;

	//get the end after the start
	if (evaluateGridPos(_extSelStart, _extSelEnd) == 0)
		return "";

	normaliseGridPair(startGrid, endGrid);

	clipInfo._start = startGrid;
	clipInfo._end = endGrid;
	clipInfo._width = GetDocument()->getGridArray()->GetWidth(); 
	clipInfo._height= GetDocument()->getGridArray()->GetHeight(); 
	
	CString str = "";

	if (!forData)
		for (int k = 0;k<startGrid._x;k++)
			str += '\t';

	for (int i=startGrid._y; i<=endGrid._y;i++)
	{
		for (int j=((i==startGrid._y)?startGrid._x:0);
				 j<=((i==endGrid._y)?endGrid._x:GetDocument()->getGridArray()->GetWidth()-1);
				 j++)
		{
			CellData* cell = getCellData(GridPos(j,i));
			if (cell)
				str +=
				cell->getString((((i==startGrid._y)&&(j==startGrid._x))?startGrid._c:0 ),
				                (((i==endGrid._y  )&&(j==endGrid._x  ))?endGrid._c  :-1));
			str += '\t';
		}
		str += '\r';
		str += '\n';
	}
	return str;
}

void TouchView::OnEditPaste() 
{
	CString clipStr;
	TouchClipInfo clipInfo;
	if ( OpenClipboard() ) 
	{
		//data
		if (IsClipboardFormatAvailable(::RegisterClipboardFormat(_ClipboardTokenData)))
		{
			HANDLE hData = GetClipboardData( ::RegisterClipboardFormat(_ClipboardTokenData) );
			char * chBuffer = (char*)GlobalLock( hData );
			clipStr = chBuffer;
			GlobalUnlock( hData );
		}

		//info 
		if (IsClipboardFormatAvailable(::RegisterClipboardFormat(_ClipboardTokenInfo)))
		{
			HANDLE hInfo = GetClipboardData(::RegisterClipboardFormat(_ClipboardTokenInfo));
			TouchClipInfo * dataBuffer = (TouchClipInfo *)GlobalLock( hInfo );
			clipInfo = *dataBuffer;
			GlobalUnlock( hInfo );
		}

		CloseClipboard();
	}
	else
		return;

	TouchDoc* pDoc = GetDocument();
	doPaste(clipInfo, clipStr);


	_extSelValid = false;
	pDoc->setUpdatePoint("Paste Selection" );
	updateCaret();
}

void TouchView::doPaste(const TouchClipInfo& clipInfo, const CString& clipStr)
{
	TouchDoc* pDoc = GetDocument();
	
	//check for a short paste (internal to a single cell)
	if ((clipInfo._start._y == clipInfo._end._y)&&
		(clipInfo._start._x == clipInfo._end._x))
	{
		CellData* cell = getCellData(GetDocument()->getGridPos());
		if (cell)
		{
			for (int i=0;i<clipStr.GetLength();i++)
			{
				if (cell->onAddElement(clipStr.GetAt(i), 
									GetDocument()->getGridPos()._c) == true)
				{
					GetDocument()->getGridPos()._c++;
				}
			}
		}		
		
		//update
		pDoc->addRow();
		pDoc->addColumn();
		return;
	}	

	//insert any appropriate rows & columns
	int rowsNeeded = clipInfo._end._y - clipInfo._start._y;
	while (pDoc->getGridArray()->GetHeight() <= pDoc->getGridPos()._y + rowsNeeded )
		pDoc->addRow();

	int colsNeeded = clipInfo._width - clipInfo._start._x;
	while (pDoc->getGridArray()->GetWidth() <= pDoc->getGridPos()._x + colsNeeded)
		pDoc->addColumn();

	//calculate left & right point
	int left = pDoc->getGridPos()._x - clipInfo._start._x;
	int right = pDoc->getGridPos()._x + (clipInfo._width - clipInfo._start._x) - 1;
	int top = pDoc->getGridPos()._y;
	int bottom  =  top + clipInfo._end._y- clipInfo._start._y;
	
	//empty the cells
	int i = -1;
	int j;
	for (j=top;j<=bottom;j++)
	{
		int lineLeft = max(0,((j==top)?pDoc->getGridPos()._x:left));
		int lineRight= ((j==bottom)?left + clipInfo._end._x:right);
		for (i=lineLeft;i<=lineRight;i++)
		{
			CellData* cell = getCellData(GridPos(i,j));
			if (cell)
				cell->destroyElements();
		}
	}	 
	
	//re populate the cells
	int posX = pDoc->getGridPos()._x;
	int posY = pDoc->getGridPos()._y;
	for (int k=0;k<clipStr.GetLength();k++)
	{
		char ch = clipStr.GetAt(k);
		switch (ch)
		{
		case 9://'\t'
			posX++;
			break;
		case 13: //'/r': 
			posX = left;
			posY++;
			break;
		case 10: //'/n':
			break;
		default:
			{
				if ((posX >=0)&&
					(posX < GetDocument()->getGridArray()->GetWidth())&&
					(posY >=0)&&
					(posY < GetDocument()->getGridArray()->GetHeight()))
				{					
					CellData* cell = getCellData(GridPos(posX,posY));
					if (cell)
					{
						cell->onAddElement(ch, cell->getLength());
					}
				}
			}
		}	
	}

		//set the caret pos
	if (i != -1) 
	{
		pDoc->getGridPos()._x = i;
		pDoc->getGridPos()._y = j-1;

	}

	//update
	pDoc->addRow();
	pDoc->addColumn();
}



void TouchView::OnEditCut() 
{
	if ((doEditCopy())&&(_extSelValid))
	{
		normaliseGridPair(_extSelStart, _extSelEnd);
		GetDocument()->tvEditDeleteSelection(_extSelStart, _extSelEnd);
		_extSelValid = false;
		updateCaret();
	}
}

void TouchView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_extSelValid);
}

void TouchView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_extSelValid);
}

void TouchView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(
		IsClipboardFormatAvailable(::RegisterClipboardFormat(_ClipboardTokenData))||
		IsClipboardFormatAvailable(::RegisterClipboardFormat(_ClipboardTokenInfo)));
}

void TouchView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	CRect rect(0,0,point.x, point.y);
	ScreenToClient(&rect);
	ScreenPos sp = screenHitTest(rect.BottomRight()+ GetScrollPosition());
	_contextGridPos = gridHitTest(rect.BottomRight()+ GetScrollPosition(), true);
	if (sp == SP_GRID)
	{
		GetDocument()->getGridPos()  = gridHitTest(rect.BottomRight()+ GetScrollPosition());

		Invalidate();
		updateCaret();
		theApp.ShowPopupMenu(IDR_TOUCH_CONTEXT_GRID, point, this);
	}
	else if (sp == SP_BLOCKS)
		theApp.ShowPopupMenu(IDR_TOUCH_CONTEXT_BLOCKS, point, this);	
	else 
		theApp.ShowPopupMenu(IDR_TOUCH_CONTEXT_OFF, point, this);	

}																			



void TouchView::OnEditSwitchOvrmode() 
{
	globalDataManager()->toggleOverstrike();
}


void TouchView::OnDeleteColumn() 
{
	GetDocument()->tvDeleteColumn();
	updateCaret();
}

void TouchView::OnDeleteRow() 
{
	GetDocument()->tvDeleteRow();
	updateCaret();
}

void TouchView::OnUpdateDeleteColumn(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->getGridPos()._x < GetDocument()->getGridArray()->GetWidth()-1);
}

void TouchView::OnUpdateDeleteRow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->getGridPos()._y < GetDocument()->getGridArray()->GetHeight()-1);
}

void TouchView::OnInsertColumn() 
{
	GetDocument()->tvInsertColumn();
	updateCaret();
}

void TouchView::OnInsertRow() 
{
	GetDocument()->tvInsertRow();
	updateCaret();
}

void TouchView::OnTimer(UINT nIDEvent) 
{

	if (nIDEvent == AAA_TOUCH_CARET_TIMER)
	{
		updateCaret(false);

	}

	CScrollView32Bit::OnTimer(nIDEvent);
}

void TouchView::OnDestroy() 
{
	KillTimer(AAA_TOUCH_CARET_TIMER);

	if (_pFindDialog != NULL)
	{
		_pFindDialog->DestroyWindow();
		_pFindDialog = NULL;
	}	   

	CScrollView32Bit::OnDestroy();	
}

bool TouchView::isInSelection(const GridPos& gridPos)
{
	GridPos startGrid = _extSelStart;
	GridPos endGrid   = _extSelEnd;

	normaliseGridPair(startGrid, endGrid);

	if (gridPos._y < startGrid._y)
		return false;
	else if (gridPos._y == startGrid._y)
	{
		if (gridPos._x < startGrid._x)
			return false;
		if (gridPos._x == startGrid._x)
			if (gridPos._c < startGrid._c)
				return false;
	}
	else if (gridPos._x < 0)
			return false;

	if (gridPos._y > endGrid._y)
		return false;
	else if (gridPos._y == endGrid._y)
	{
		if (gridPos._x > endGrid._x)
			return false;
		if (gridPos._x == endGrid._x)
			if (gridPos._c > endGrid._c)
				return false;
	}
	else if (gridPos._x >= GetDocument()->getGridArray()->GetWidth())
		return false;

	return true;
}

TouchView::ScreenPos TouchView::screenHitTest(const CPoint &point)
{

	CRect rcGrid = getGridRect();

	if ((point.x > rcGrid.left)&&
		(point.x < rcGrid.right)&&
		(point.y > rcGrid.top)&&
		(point.y < rcGrid.bottom))
		return SP_GRID;

	if ((point.x > rcGrid.right)&&
		(point.y > rcGrid.top)&&
		(point.y < rcGrid.bottom))
		return SP_BLOCKS;	

	if ((point.x > _layout->getCourseHeadLeft())&&
		(point.y > rcGrid.bottom))
		return SP_DEFINITIONS;
	
	return SP_NONE;
}

void TouchView::OnAddblockdefinition() 
{
	//find the correct block
	BlockPos blockPos = blocksHitTest(_contextMouseDown, false, true);
	TouchBlock* touchBlock = NULL; 

	if (blockPos._type == BlockPos::BP_WHOLE_CELL)
	{
		for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
		{
			TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
			if ((tb->_start <= blockPos._line)&&
				(tb->_end > blockPos._line))
				touchBlock = tb;
		}
	}

	if (touchBlock) //should never get called
	{
		AfxMessageBox("Block " + touchBlock->_name +  " is already defined at this position"); 
	}
	else
	{
		//find the next letter 
		char letter = 'a';
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
			{
				TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
				if (tb->_name.GetLength() >0)
				{
					if (tb->_name == letter)
					{
						letter++;
						changed = true;
					}
				}
			}
		}  
		
		CPoint point= _contextMouseDown;
		ClientToScreen(&point);
		TouchBlock tb(blockPos._line, blockPos._line+1, "");
		EditBlockLetterDlg dlg( point,
								CString(letter), 
								GetDocument()->getTouchBlocks(), 
								blockPos._line+1, 
								blockPos._line+1,
								getBlockDefMin(&tb)+1,
								getBlockDefMax(&tb));
		if (dlg.DoModal() == IDOK)
		{
			GetDocument()->addTouchBlock(
				new TouchBlock(dlg._top-1,dlg._bottom,dlg._blockLetter));	
		}																			
	}
}

void TouchView::OnAddblockdefinitionMenu() 
{
	TouchBlock* touchBlock = NULL; 

	for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
	{
		TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
		if ((tb->_start <= GetDocument()->getGridPos()._y)&&
			(tb->_end > GetDocument()->getGridPos()._y))
			touchBlock = tb;
	}
	
	if (touchBlock) 
	{
		AfxMessageBox("Block " + touchBlock->_name +  " is already defined at insertion position"); 
	}
	else
	{
		//find the next letter 
		char letter = 'a';
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
			{
				TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
				if (tb->_name.GetLength() >0)
				{
					if (tb->_name == letter)
					{
						letter++;
						changed = true;
					}
				}
			}
		}  

		CPoint point = gridToClient(GetDocument()->getGridPos());
		ClientToScreen(&point);
		TouchBlock tb(GetDocument()->getGridPos()._y, GetDocument()->getGridPos()._y+1, "");
		EditBlockLetterDlg dlg(point, 
								CString(letter), 
								GetDocument()->getTouchBlocks(),
								GetDocument()->getGridPos()._y+1,
								GetDocument()->getGridPos()._y+1,
								getBlockDefMin(&tb)+1,
								getBlockDefMax(&tb));
		if (dlg.DoModal() == IDOK)
		{
			GetDocument()->addTouchBlock(new TouchBlock(dlg._top-1,
														dlg._bottom,
														dlg._blockLetter));	
		}			
	}
}


CRect TouchView::getGridRect()
{
	CRect rcGrid;
	TouchDoc* pTouchDoc = GetDocument();

	rcGrid.top    = _layout->getTop();
	rcGrid.bottom = _layout->getTop() + (_layout->_height * pTouchDoc->getGridArray()->GetHeight());
	rcGrid.left   = _layout->getGridLeft();
	rcGrid.right  = _layout->getGridLeft();

	CDC* pDC = GetDC();
	CFont * oldFont = pDC->SelectObject(&GetDocument()->getTouchStyle().getFontMain());	

	
	for (int i=0;i<pTouchDoc->getGridArray()->GetWidth();i++)
		rcGrid.right += pTouchDoc->getColumnWidth(pDC, i);
	
	pDC->SelectObject(oldFont); 
	ReleaseDC(pDC);
	
	return rcGrid;
}

BlockPos TouchView::blocksHitTest(const CPoint &point, bool inBetween, bool wholeCell)
{
	const int xBound = 3;
	const int yBound = 4;

	if (inBetween)
	{
		int pos = (point.y-_layout->getTop() + (_layout->_height/2)) / _layout->_height;
		return BlockPos(BlockPos::BP_INBETWEEN, pos);				
	}

	if (wholeCell)
	{
		int pos = (point.y-_layout->getTop() ) / _layout->_height;
		return BlockPos(BlockPos::BP_WHOLE_CELL, pos);				
	}

	if (screenHitTest(point) == SP_BLOCKS)
	{
		CRect rcGrid = getGridRect();
		if ((point.x > rcGrid.right + _layout->_blockSpace - xBound)&&
			(point.x < rcGrid.right + _layout->_blockSpace + xBound))
		{
			int pos = (point.y-_layout->getTop() + (_layout->_height/2)) / _layout->_height;
			if (abs((pos * _layout->_height) - (point.y-_layout->getTop())) < yBound)
				return BlockPos(BlockPos::BP_NODE, pos);
			
		}
		
		if ((point.x > rcGrid.right + _layout->_blockSpace )&&
			(point.x < rcGrid.right + _layout->_blockSpace +(_averageCharWidth/3)*2))
		{
			int pos = (point.y-_layout->getTop() ) / _layout->_height;
			return BlockPos(BlockPos::BP_BRACKET, pos);				
		}		
	}

	return BlockPos(BlockPos::BP_NONE, 0);
}

BOOL TouchView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	point += GetScrollPosition();

	BlockPos bp = blocksHitTest(point);
	if ((bp._type == BlockPos::BP_NODE)&&
		(GetDocument()->isTouchBlockOnNode(bp._line)))
	{
		 SetCursor(app()->LoadStandardCursor(IDC_SIZENS));
		return TRUE;
	}
	else if ((bp._type == BlockPos::BP_BRACKET)&&
			 (GetDocument()->getTouchBlockOnRow(bp._line)!=NULL))
	{
		SetCursor(app()->LoadCursor(IDC_RESIZENS));
		return TRUE;
	}
	return CScrollView32Bit::OnSetCursor(pWnd, nHitTest, message);
}

void TouchView::OnDeleteblock() 
{
	//find the correct block
	BlockPos blockPos = blocksHitTest(_contextMouseDown, false, true);

	if (blockPos._type == BlockPos::BP_WHOLE_CELL)
	{
		for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
		{
			TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
			if ((tb->_start <= blockPos._line)&&
				(tb->_end > blockPos._line))
			{
				delete tb;
				GetDocument()->getTouchBlocks().RemoveAt(i);
				GetDocument()->setUpdatePoint("Delete Block");
			}
		}
	}
}

void TouchView::OnUpdateDeleteblock(CCmdUI* pCmdUI) 
{
	//find the correct block
	BlockPos blockPos = blocksHitTest(_contextMouseDown, false, true);
	TouchBlock* touchBlock = NULL; 

	if (blockPos._type == BlockPos::BP_WHOLE_CELL)
	{
		for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
		{
			TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
			if ((tb->_start <= blockPos._line)&&
				(tb->_end > blockPos._line))
				touchBlock = tb;
		}
	}

	pCmdUI->Enable(touchBlock != NULL);
	if (touchBlock)
		pCmdUI->SetText("Delete Block \'"+ touchBlock->_name + "\'");
	else
		pCmdUI->SetText("Delete Block ");	
}

void TouchView::OnEditblockletter() 
{
	//find the correct block
	BlockPos blockPos = blocksHitTest(_contextMouseDown, false, true);

	if (blockPos._type == BlockPos::BP_WHOLE_CELL)
	{
		for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
		{
			TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
			if ((tb->_start <= blockPos._line)&&
				(tb->_end > blockPos._line))
			{
				CPoint point = _contextMouseDown;
				ClientToScreen(&point);
				EditBlockLetterDlg dlg( point,
										tb->_name, 
										GetDocument()->getTouchBlocks(),
										tb->_start+1,
										tb->_end,
										getBlockDefMin(tb)+1,
										getBlockDefMax(tb));
				if (dlg.DoModal() == IDOK)
				{
					tb->_name = dlg._blockLetter;
					tb->_start = dlg._top-1;
					tb->_end = dlg._bottom;
					GetDocument()->setUpdatePoint("Edit Block");
				}
			}
		}
	}	
}

void TouchView::OnUpdateEditblockletter(CCmdUI* pCmdUI) 
{
	//find the correct block
	BlockPos blockPos = blocksHitTest(_contextMouseDown, false, true);
	TouchBlock* touchBlock = NULL; 

	if (blockPos._type == BlockPos::BP_WHOLE_CELL)
	{
		for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
		{
			TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
			if ((tb->_start <= blockPos._line)&&
				(tb->_end > blockPos._line))
				touchBlock = tb;
		}
	}

	pCmdUI->Enable(touchBlock != NULL);
	if (touchBlock)
		pCmdUI->SetText("&Edit Block Definition \'"+ touchBlock->_name + "\'");
	else
		pCmdUI->SetText("&Edit Block Definition");	
}

void TouchView::OnUpdateAddblockdefinition(CCmdUI* pCmdUI) 
{
	//find the correct block
	BlockPos blockPos = blocksHitTest(_contextMouseDown, false, true);
	TouchBlock* touchBlock = NULL; 

	if (blockPos._type == BlockPos::BP_WHOLE_CELL)
	{
		for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
		{
			TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
			if ((tb->_start <= blockPos._line)&&
				(tb->_end > blockPos._line))
				touchBlock = tb;
		}
	}

	pCmdUI->Enable((touchBlock == NULL)&&
		(blockPos._line < GetDocument()->getGridArray()->GetHeight()-1)&&
		(blockPos._line >= (GetDocument()->isCourseBased()?1:0)));

}

void TouchView::OnUpdateAddblockdefinitionMenu(CCmdUI* pCmdUI) 
{
	//find the correct block
	TouchBlock* touchBlock = NULL; 

	for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
	{
		TouchBlock* tb = GetDocument()->getTouchBlocks().GetAt(i);
		if ((tb->_start <= GetDocument()->getGridPos()._y)&&
			(tb->_end > GetDocument()->getGridPos()._y))
			touchBlock = tb;
	}

	pCmdUI->Enable((touchBlock == NULL)&&
					(GetDocument()->getGridPos()._y < GetDocument()->getGridArray()->GetHeight()-1)&&
					(GetDocument()->getGridPos()._y >= (GetDocument()->isCourseBased()?1:0)));
}


int TouchView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView32Bit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	_tip.Create(this);		
	
	return 0;
}


void TouchView::OnUpdateEditVariance(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);

	CellData* cell = getCellData(_contextGridPos);
	if (cell)
	{
		CellElement* element = cell->getElement(_contextGridPos._c);
		if (element)
			pCmdUI->Enable(element->getChar() == '[');
	}
}


void TouchView::OnEditVariance() 
{
	CellData* cell = getCellData(_contextGridPos);
	if (cell)
	{
		CellElement* element = cell->getElement(_contextGridPos._c);
		if ((element)&&((element->getChar() == '[')))
		{
			//populate the list
			cleanPopupInfo();

			_popupElement = element;

			int selection = -1;

			Variances variances;
			GetDocument()->getUsedVariances(variances);

			if (variances.GetSize() <=1)
				editVarianceOnChar();
			else
			{

				_popupStrings.Add(new StringColor(
										"Edit Variance",
										GetDocument()->getTouchStyle().getColor(tsc_varianceColor)));

				for (int i=0;i<variances.GetSize();i++)
				{
					_popupStrings.Add(new StringColor(
										variances.GetAt(i)->getDisplayString(),
										GetDocument()->getTouchStyle().getColor(tsc_varianceColor)));
					if (element->getData() == variances.GetAt(i)->getGridString())
						selection = i;
				}


				//launch the wnd
				CPoint point = _contextMouseDown;
				ClientToScreen(&point);
				point.x -= 10;
				point.y += _layout->_height;

				PopupWnd* popup = new PopupWnd;
				popup->Create(this);
				popup->Set(point, &_popupStrings, selection);
				_popupAction = PA_EDIT_VAR;
			}

			//tidy up
			for (int i=0;i<variances.GetSize();i++)
				delete variances.GetAt(i);
		}	
	}	
}



void TouchView::OnLButtonDblClk(UINT /*nFlags*/, CPoint point) 
{

	GridPos gridPos = gridHitTest(point, true);
	
	CellData* cell = getCellData(gridPos);

	gridPos._c = cell->getFirstLetterPos(gridPos._c);

	if (gridPos._c != -1)
	{
		terminateExtSel(true);

		startExtSel(gridPos);

		gridPos._c += cell->getElement(gridPos._c)->getWordLength();

		_extSelEnd = gridPos;

		GetDocument()->getGridPos() = gridPos;

		Invalidate();
	}	
	
	theApp.OnViewDoubleClick (this, IDR_TOUCHTYPE);	
	
}



//this needs to be in the view, to stop the other views enabling the button
void TouchView::OnUpdateShowGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((GetDocument()->isShowGrid() == TRUE)?1:0);
	pCmdUI->SetText((GetDocument()->isShowGrid() == FALSE)?"Show &Gridlines":"Hide &Gridlines");	
}

//this needs to be in the view, to stop the other views enabling the button
void TouchView::OnShowGrid() 
{
	if (GetDocument()->isShowGrid() == TRUE)
		GetDocument()->setShowGrid(FALSE);	
	else 
		GetDocument()->setShowGrid(TRUE);	

	GetDocument()->setUpdatePoint("", false,false, false);	
}

//this needs to be in the view, to stop the other views enabling the button
void TouchView::OnShowCourseHeads() 
{
	if (GetDocument()->getShowCourseHeads() == TRUE)
		GetDocument()->setShowCourseHeads(FALSE);	
	else 
		GetDocument()->setShowCourseHeads(TRUE);	

	GetDocument()->setUpdatePoint("", false,false, false);	
}


//this needs to be in the view, to stop the other views enabling the button
void TouchView::OnUpdateShowCourseHeads(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((GetDocument()->getShowCourseHeads() == TRUE)?1:0);
	pCmdUI->SetText((GetDocument()->getShowCourseHeads() == FALSE)?"Show &Course Heads":"Hide &Course Heads");		
}


void TouchView::cleanPopupInfo()
{
	//delete the string colors
	for (int i=0;i<_popupStrings.GetSize();i++)
		delete _popupStrings.GetAt(i);
	_popupStrings.RemoveAll();
	_popupElement = NULL;
	_popupAction = PA_NONE;
}	 

void TouchView::newVarianceOnChar()
{
	Variance* var = new Variance;

	VarianceDlg dlg(var);
	if (dlg.DoModal() == IDOK)
	{
		GetDocument()->tvEditChar('[', var->getGridString());
	}

	delete var;
}

void TouchView::editVarianceOnChar()
{
	if (_popupElement)
	{
		Variance* var = new Variance(_popupElement->getData());

		VarianceDlg dlg(var);
		if (dlg.DoModal() == IDOK)
		{
			_popupElement->setData(var->getGridString());
			GetDocument()->setUpdatePoint("Edit Variance");
		}

		delete var;
	}
}

LRESULT TouchView::OnPopupListTerm(WPARAM wParam, LPARAM /*lParam*/)
{
	int selection = (int)wParam;
	if (selection != CB_ERR)
	{
		switch (_popupAction)
		{
		case PA_NONE:
			return 0;
			break;
		case PA_NEW_VAR_LEFT:	 			
			{
				if (selection == 0)
				{
					newVarianceOnChar();
				}
				else if (selection > 0) 
				{
					Variances vars;
					GetDocument()->getUsedVariances(vars);
			
					if ( selection <= vars.GetSize())
						GetDocument()->tvEditChar('[', vars.GetAt(selection-1)->getGridString());
					
					//tidy up
					for(int i=0;i<vars.GetSize();i++)
						delete vars.GetAt(i);
				}
			}
			break;
		case PA_EDIT_VAR:
			{
				//find the variance
				if (selection == 0)
				{
					editVarianceOnChar();

				}	 
				else if (selection > 0)
				{
					Variances vars;
					GetDocument()->getUsedVariances(vars);
		
					if ((selection <= vars.GetSize())&&
						(_popupElement))
					{
						_popupElement->setData(vars.GetAt(selection-1)->getGridString());
					}
					GetDocument()->setUpdatePoint("Edit Variance");

					//tidy up
					for(int i=0;i<vars.GetSize();i++)
						delete vars.GetAt(i);
				}

			}
			break;
		}
	}
	return 0;
}

BOOL TouchView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
	//return CScrollView32Bit::OnEraseBkgnd(pDC);
}

//this is the normal message handler
void TouchView::OnEditFind() 
{
	if (_pFindDialog == NULL)
	{
		_pFindDialog = new CFindReplaceDialog;
		_pFindDialog->Create( TRUE, _searchStr, NULL, FR_DOWN | FR_HIDEWHOLEWORD | FR_HIDEMATCHCASE, this);	
	}
	else
	{
		_pFindDialog->SetFocus();
	}
}

//this is the callback handler
LRESULT TouchView::OnFindDialogMessage(WPARAM /*wParam*/, LPARAM /*lParam*/)
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


void TouchView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((_pFindDialog != NULL)?1:0);	
}

void TouchView::doFind(CString findText, bool searchDown)
{
	CWaitCursor wait;
	GridPos initialPos = GetDocument()->getGridPos();
	GridPos gPos = GetDocument()->getGridPos();
	GridArray * grid = GetDocument()->getGridArray();
	int pos;

	int limit = (grid->GetWidth() * grid->GetHeight()) + 10;
	int count = 0;

	while (true)
	{
		//check the currentCell
		CellData* cell = GetDocument()->getCellData(gPos);
		if (cell)
		{
			CString cellStr = cell->getString();
			//for teh first cell, to save repeting the search just done
			cellStr = cellStr.Right(cellStr.GetLength() - gPos._c); //usually zero
			
			if ((pos = cellStr.Find(findText)) != -1)
			{
				//hilight the word, and put the cursor at the end of the word
				GetDocument()->getGridPos() = gPos;
				GetDocument()->getGridPos()._c += (pos + findText.GetLength());

				_extSelStart = GetDocument()->getGridPos();
				_extSelStart._c = pos + gPos._c;

				_extSelEnd = GetDocument()->getGridPos();
				_extSelValid = true;
				scrollToPos();
				Invalidate();
				updateCaret();	
				
				//need to scroll into view
				
				break;				
			}

			count++;
			gPos._c = 0;
			if (searchDown)
			{
				//increment cell;
				gPos._x++;


				if (gPos._x > grid->GetWidth()-1) 
				{
					gPos._x = 0;
					gPos._y++;
					if (gPos._y > grid->GetHeight()-1) 
					{
						gPos._y = 0;
					}
				}	
				
				if ((initialPos == gPos)||
					(count > limit)) //our get out clause
				{
					AfxMessageBox(findText + " not found");
					return;
				}
			}
			else
			{
				//increment cell;
				gPos._x--;

				if (gPos._x <0 ) 
				{
					gPos._x = grid->GetWidth()-1;
					gPos._y--;
					if (gPos._y < 0)
					{
						gPos._y = grid->GetHeight()-1 ;
					}
				}	
				
				if ((initialPos == gPos)||
					(count > limit)) //our get out clause
				{
					AfxMessageBox(findText + " not found");
					return;
				}
			}
		}
	}
}

void TouchView::OnEditFindPrevious() 
{
	if (!_searchStr.IsEmpty())
		doFind(_searchStr, true);	
}


void TouchView::OnTouchAddvariance() 
{
	OnChar('[',0,0);	
}

int TouchView::getBlockDefMax(TouchBlock* tb)
{
	//find the greatist position that is ABOVE our block
	
	int max = GetDocument()->getGridArray()->GetHeight() - 1;

	if (tb)
	{
		for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
		{
			TouchBlock* touchBlock = GetDocument()->getTouchBlocks().GetAt(i);
			if ((tb != touchBlock)&&
				(touchBlock->_start >= tb->_end)&&
				(touchBlock->_start < max))
			{	
				max = touchBlock->_start; 
			}
		}
	}

	return max;
}

int TouchView::getBlockDefMin(TouchBlock* tb)
{
	//find the smallest position that is BELOW  our block
	
	int min = ((GetDocument()->isCourseBased())?1:0);

	if (tb)
	{
		for (int i=0;i<GetDocument()->getTouchBlocks().GetSize();i++)
		{
			TouchBlock* touchBlock = GetDocument()->getTouchBlocks().GetAt(i);
			if ((tb != touchBlock)&&
				(touchBlock->_end <= tb->_start)&&
				(touchBlock->_end > min))
			{	
				min = touchBlock->_end; 
			}
		}
	}

	return min;
}



CxImage* TouchView::getImage()
{

	CxImage* image = new CxImage();

	CBitmap* bit = getCopyDataBitmap();
		
	image->CreateFromHBITMAP((HBITMAP)bit->GetSafeHandle());

	bit->Detach();

	delete bit;		

	return image;

}

void TouchView::resetExtSelValid()
{
	_extSelValid = false;
}
