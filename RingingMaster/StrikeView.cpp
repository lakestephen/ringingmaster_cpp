// StrikeView.cpp : implementation of the StrikeView class
//

#include "stdafx.h"
#include "ringingmaster.h"

#include "StrikeDoc.h"
#include "StrikeView.h"
#include ".\strikeview.h"
#include "MemDC.h"
#include "StrikingData.h"
#include "StrikingRow.h"
#include "StrikeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// StrikeView

IMPLEMENT_DYNCREATE(StrikeView, CScrollView32Bit)

BEGIN_MESSAGE_MAP(StrikeView, CScrollView32Bit)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView32Bit::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView32Bit::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView32Bit::OnFilePrintPreview)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_MENU, OnUpdateFilePrintMenu)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// StrikeView construction/destruction

StrikeView::StrikeView()
{
	_imageList.Create(IDB_SPEECH_BUBBLE, 16, 1, RGB(192, 192, 192));
}

StrikeView::~StrikeView()
{
	clearHitTestArray();
	_tip.DestroyWindow();
}

BOOL StrikeView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView32Bit::PreCreateWindow(cs);
}

// StrikeView drawing

void StrikeView::OnDraw(CDC* pDC)
{

	MemDC memdc(pDC);

	CRect rect;
	GetClientRect(rect);	
	CSize size = getSize();

	rect.right = max(size.cx, rect.right);
	rect.bottom = max(size.cy, rect.bottom);

	memdc->FillSolidRect(&rect, GetSysColor(COLOR_WINDOW));  
	memdc->SetMapMode(MM_TEXT);
	memdc->SetBkMode(TRANSPARENT);

	  
	doDraw(memdc);
}


// StrikeView printing

BOOL StrikeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void StrikeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	//to2do get printing working
}

void StrikeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// StrikeView diagnostics

#ifdef _DEBUG
void StrikeView::AssertValid() const
{
	CScrollView32Bit::AssertValid();
}

void StrikeView::Dump(CDumpContext& dc) const
{
	CScrollView32Bit::Dump(dc);
}

StrikeDoc* StrikeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(StrikeDoc)));
	return (StrikeDoc*)m_pDocument;
}
#endif //_DEBUG


// StrikeView message handlers

void StrikeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	theApp.OnViewDoubleClick (this, IDR_STRIKETYPE);	

	CScrollView32Bit::OnLButtonDblClk(nFlags, point);
}

void StrikeView::OnInitialUpdate() 
{
	CScrollView32Bit::OnInitialUpdate();
	
	updateScrollSizes();
	
}

void StrikeView::updateScrollSizes()
{
	CSize size = getSize();
	if (_lastScrollSize	!= size)
	{
		_lastScrollSize = size;
		SetScrollSizes(MM_TEXT, _lastScrollSize);	
	}
} 


CSize StrikeView::getSize()
{
	CSize sizeTotal(10,100);
	
	sizeTotal.cy = GetDocument()->getStrikingData().GetSize() * STRIKE_VIEW_PITCH;

	return sizeTotal;
}


void StrikeView::doDraw(CDC* pDC)
{

	//to2do ensure we ae not out of bounds

	StrikeDoc* pDoc = GetDocument();   
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const StrikingData& strikingData = pDoc->getStrikingData();
	const int number = strikingData.getNumber();	

	clearHitTestArray();

	CRect rect;
	GetClientRect(rect);
	rect += GetScrollPosition();

	CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));
	CPen* oldPen = pDC->SelectObject(&pen);
	CFont * oldFont = pDC->SelectObject(&app()->_fontStd);	

	CString lineNumber;
	lineNumber.Format("%d", strikingData.GetSize());

	int STRIKE_VIEW_ROW_NUMBER_WIDTH = pDC->GetTextExtent(lineNumber).cx + (STRIKE_VIEW_TEXT_GUTTER * 2) ;

	//draw solid patch
	pDC->FillSolidRect(CRect(0,0,STRIKE_VIEW_ROW_NUMBER_WIDTH + STRIKE_VIEW_HAND_BACK_WIDTH, rect.bottom), ::GetSysColor(COLOR_3DLIGHT));

	//draw the horizontal rows                                                        	
	for (int i=0;i<=strikingData.GetSize();i++)
	{
		int y = (i*STRIKE_VIEW_PITCH);
		
		if (y>=rect.top && y <=rect.bottom)
		{    
			if (GetDocument()->getSelectedRow() == i)
				pDC->FillSolidRect(CRect(rect.left, y, rect.right, y + STRIKE_VIEW_PITCH), ::GetSysColor(COLOR_3DFACE));

			pDC->MoveTo(rect.left, y);
			pDC->LineTo(rect.right, y);	 
		}

	}                                          

	//draw the vertical rows

	int left = 0;
	pDC->MoveTo(left,  0);
	pDC->LineTo(left, rect.bottom);	  
	left += STRIKE_VIEW_ROW_NUMBER_WIDTH;
	pDC->MoveTo(left,  0);
	pDC->LineTo(left, rect.bottom);	  
	left += STRIKE_VIEW_HAND_BACK_WIDTH;
	pDC->MoveTo(left,  0);
	pDC->LineTo(left, rect.bottom);	  

	for (int i=1;i < number+1;i++)
	{
		int x = left + (int)((double)((double)(rect.Width() - left)/(double)(number+1)) * (double)i);
		pDC->MoveTo(x,  0);
		pDC->LineTo(x, rect.bottom);	  
	}


	//draw the bells
	int top = 0, line=0;
	CSize szText = pDC->GetTextExtent("A");
	for ( POSITION pos = strikingData.GetHeadPosition (); pos != NULL; line++)
	{
		const StrikingRow* strikingRow = strikingData.GetNext (pos);

		if (top>=(rect.top-100) && top <=(rect.bottom+100))
		{    
			
			CRect textRect(STRIKE_VIEW_TEXT_GUTTER,top+1, rect.right, top +  STRIKE_VIEW_PITCH);

		
			lineNumber.Format("%d", line+1);
			pDC->DrawText(lineNumber, textRect,DT_LEFT|DT_SINGLELINE | DT_VCENTER);

			textRect.left = STRIKE_VIEW_ROW_NUMBER_WIDTH + STRIKE_VIEW_TEXT_GUTTER;
			pDC->DrawText(((strikingRow->getStroke() == hand)?"h":"b"), textRect,DT_LEFT|DT_SINGLELINE | DT_VCENTER);

			textRect.left = STRIKE_VIEW_ROW_NUMBER_WIDTH + STRIKE_VIEW_HAND_BACK_WIDTH;

			POSITION rowPos = strikingRow->GetHeadPosition();
			StrikingItem* realStrikeItem = NULL;

			while(rowPos) 
			{
				realStrikeItem  = (StrikingItem *) strikingRow->GetNext(rowPos);

				if (realStrikeItem->isComment())
				{
					int place1Interval = (int)strikingRow->getPerfectTimeBell(0).getTimestamp();
					int commentInterval = (int)realStrikeItem->getTimestamp();
					
					int x = (int)(((double)commentInterval / (double)(number * place1Interval)) * (double)textRect.Width()) + textRect.left;

					x = max(textRect.left+8,x);

					StrikeHitTestPackage* package = new StrikeHitTestPackage;
					package->_rect = CRect(x - 8 ,textRect.top, x+8, textRect.bottom); //note image is 6 pixels wide
					package->_string = realStrikeItem->getComment();
					_hitTestArray.Add(package);
					_imageList.Draw(pDC, 0,package->_rect.TopLeft(), ILD_TRANSPARENT);

					

				}
			}

			rowPos = strikingRow->GetHeadPosition();
			realStrikeItem = NULL;
			while(rowPos) 
			{
				realStrikeItem  = (StrikingItem *) strikingRow->GetNext(rowPos);

				if (!realStrikeItem->isComment())
				{
					int place = strikingRow->getPlaceFromBell(realStrikeItem->getBell());

					if (place >= 0)
					{

						CString str(GlobalFunctions::bellNumbersToChar(realStrikeItem->getBell()+1));
						double percent = strikingRow->getPercentOut(realStrikeItem, place);
						//shadow
						int x = 0;
						if (strikeManager()->getOrderByBells())
							x = (int)((double)((double)(realStrikeItem->getBell()+1.0+percent)/(double)(number+1)) * textRect.Width()) - (szText.cx/2) + textRect.left;
						else
							x = (int)((double)((double)(place+1.0+percent)/(double)(number+1)) * textRect.Width()) - (szText.cx/2) + textRect.left;

						StrikeHitTestPackage* package = new StrikeHitTestPackage;
						package->_rect = CRect(x,textRect.top, x + szText.cx, textRect.bottom);
						package->_string.Format("%.2f%% %s",abs(percent*100), ((percent == 0)?"":((percent> 0)?"Late":"Early")) );
						_hitTestArray.Add(package);

						pDC->DrawText(str, package->_rect,
							DT_CENTER| DT_VCENTER);
					}
				}
			}
		}

		top += STRIKE_VIEW_PITCH;
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldFont);

	updateScrollSizes();
}	

BOOL StrikeView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void StrikeView::OnFilePrintPreview()
{
	//to2do get printing working
}

void StrikeView::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE); //to2do get printing working
}

void StrikeView::OnFilePrint()
{

}

void StrikeView::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE); //to2do get printing working	
}

void StrikeView::OnFilePrintMenu()
{
	
}

void StrikeView::OnUpdateFilePrintMenu(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE); //to2do get printing working
}

void StrikeView::clearHitTestArray()
{
	for (int i=0;i<_hitTestArray.GetSize();i++)
		delete _hitTestArray.GetAt(i);

	_hitTestArray.RemoveAll();
}

void StrikeView::OnMouseMove(UINT nFlags, CPoint point)
{

	CPoint scrolledPoint = point + GetScrollPosition();

	_tip.removeAllStrings(); 	

	for (int i=0;i<_hitTestArray.GetSize();i++)
	{
		StrikeHitTestPackage* package =  _hitTestArray.GetAt(i);
		if (package->_rect.PtInRect(scrolledPoint))
			_tip.addString(package->_string);
	}

	_tip.Set(point); 

	CScrollView32Bit::OnMouseMove(nFlags, point);
}

int StrikeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView32Bit::OnCreate(lpCreateStruct) == -1)
		return -1;

	_tip.Create(this);		

	return 0;
}



