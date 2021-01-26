// MatrixFilterDescriptionWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MatrixFilterDescriptionWnd.h"
#include "MatrixFilterDescriptionItem.h"
#include "MatrixFilterSort.h"
#include "MatrixItemBase.h"
#include "LogicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MatrixFilterDescriptionWnd

MatrixFilterDescriptionWnd::MatrixFilterDescriptionWnd(MatrixFilterSort& filterSort) :
_lastScrollSize(0,0), 
_filterSort(filterSort),
_selectedSort(-1),
_selectedColumn(-1),
_selectedFilter(-1)
{
}

MatrixFilterDescriptionWnd::~MatrixFilterDescriptionWnd()
{


}


BEGIN_MESSAGE_MAP(MatrixFilterDescriptionWnd, ScrollWnd)
	//{{AFX_MSG_MAP(MatrixFilterDescriptionWnd)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MatrixFilterDescriptionWnd message handlers

BOOL MatrixFilterDescriptionWnd::Create(const RECT& rect, CWnd* pParentWnd)
{
	if (!ScrollWnd::Create(WS_EX_CLIENTEDGE, "Matrix Filter Description Wnd", WS_TABSTOP, rect,		
									pParentWnd, IDC_MATRIX_FILTER_DESCRIP))
	{
		return FALSE;
	}

	if (!_stdFont.CreateStockObject(DEFAULT_GUI_FONT))
	{
		return FALSE;
	}

	LOGFONT lf;
	_stdFont.GetLogFont(&lf);
	lf.lfUnderline = TRUE;
	if (!_underlineFont.CreateFontIndirect(&lf))
	{
		return FALSE;
	}


	return TRUE;	

}

void MatrixFilterDescriptionWnd::update()
{
	Invalidate();
}				   		

void MatrixFilterDescriptionWnd::OnDraw(CDC* pDC)
{
	int maxWidth = 0;
	int maxHeight = 0;

	//filters
	if (_filterSort.getFilterCount() > 0)
	{
		CSize thisSize = drawItems(pDC, mfdt_filter);
		if (maxWidth < thisSize.cx)
			maxWidth = thisSize.cx;
		if (maxHeight < thisSize.cy)
			maxHeight = thisSize.cy;
	}

	//sorting
	if (_filterSort.getSortCount() > 0)
	{
		CSize thisSize = drawItems(pDC, mfdt_sort);
		if (maxWidth < thisSize.cx)
			maxWidth = thisSize.cx;
		if (maxHeight < thisSize.cy)
			maxHeight = thisSize.cy;
	}
	
	if (_filterSort.getColumnCount() > 0)
	{
		CSize thisSize = drawItems(pDC, mfdt_column);
		if (maxWidth < thisSize.cx)
			maxWidth = thisSize.cx;
		if (maxHeight < thisSize.cy)
			maxHeight = thisSize.cy;
	}

	CSize size(maxWidth + 10, maxHeight + 10);

	if (size != _lastScrollSize)
	{
		_lastScrollSize = size;
		SetScrollSizes(size);
	}	  
}			

const int leftOffset = 3;
const int leftIndent = 13;
const int topOffset = 3;
const int vertSpacing = 13;

CSize MatrixFilterDescriptionWnd::drawItems(CDC* pDC, MatrixFilterDescriptionType type)
{
	//title
	CFont* oldFont = pDC->SelectObject(&_stdFont);
	COLORREF oldColor = pDC->SetTextColor(RGB(0,0,0));
	COLORREF oldBKColor = pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	
	int top = 0;
	
	switch (type)
	{ //NOTE: the breaks are deliberatly missing to allow the accumulation of the value.
	case mfdt_column:
		if (_filterSort.getSortCount() > 0)
			top += (vertSpacing * (_filterSort.getSortCount()+1));
	case mfdt_sort:
		if (_filterSort.getFilterCount() > 0)
			top += (vertSpacing * (_filterSort.getFilterCount()+1));
	case mfdt_filter:
        top += topOffset;
	}

	CString entityName = _filterSort.getEntityName();
	entityName.MakeLower();
 
	CString label;


	switch (type)
	{
	case mfdt_filter:
		label.Format("Show %s ", entityName);
		break;
	case mfdt_sort:
		label.Format("Sort %s by", entityName);
		break;
	case mfdt_column:
		label.Format("Show columns");
		break;
	}
		
	pDC->TextOut(leftOffset, top, label);

	int maxWidth = 0;
	int maxHeight = 0;

	if ((type == mfdt_sort)&&(_selectedSort>=0)&&(_selectedSort < _filterSort.getSortCount()))
	{
		CRect rect(0,    top + (vertSpacing * (_selectedSort+1)),
				   10000,top + (vertSpacing * (_selectedSort+2)));
		pDC->FillSolidRect(&rect,::GetSysColor(COLOR_HIGHLIGHT));
	}
	if ((type == mfdt_column)&&(_selectedColumn>=0)&&(_selectedColumn < _filterSort.getColumnCount()))
	{
		CRect rect(0,    top + (vertSpacing * (_selectedColumn+1)),
				   10000,top + (vertSpacing * (_selectedColumn+2)));
		pDC->FillSolidRect(&rect,::GetSysColor(COLOR_HIGHLIGHT));
	}
	if ((type == mfdt_filter)&&(_selectedFilter>=0)&&(_selectedFilter < _filterSort.getFilterCount()))
	{
		CRect rect(0,    top + (vertSpacing * (_selectedFilter+1)),
				   10000,top + (vertSpacing * (_selectedFilter+2)));
		pDC->FillSolidRect(&rect,::GetSysColor(COLOR_HIGHLIGHT));
	}


	int displayStringCount = 0;

	switch (type)
	{
	case mfdt_filter:
		displayStringCount = _filterSort.getFilterCount();
		break;
	case mfdt_sort:
		displayStringCount = _filterSort.getSortCount();
		break;
	case mfdt_column:
		displayStringCount = _filterSort.getColumnCount();
		break;
	}

	for (int i=0;i<displayStringCount;i++)
	{

		MatrixItemBase* matrixItemBase = NULL;
		
		switch (type)
		{
		case mfdt_filter:
			matrixItemBase = _filterSort.getFilter(i);
			break;
		case mfdt_sort:
			matrixItemBase = _filterSort.getSort(i);
			break;
		case mfdt_column:
			matrixItemBase = _filterSort.getColumn(i);
			break;
		}

		if (!matrixItemBase)
		{
			ASSERT(FALSE);
			break;
		}      

		MatrixFilterDescriptionItems& matrixFilterDescriptionItems = 
				(type == mfdt_filter)?matrixItemBase->getFilterDescriptionItems(_filterSort.getLogicType()):
				((type == mfdt_sort)?matrixItemBase->getSortDescriptionItems():matrixItemBase->getColumnDescriptionItems());

		int left = leftIndent;

		for (int j=0;j<matrixFilterDescriptionItems.GetSize()  - ((i== displayStringCount-1)?1:0) ;j++)
		{
			MatrixFilterDescriptionItem* item = matrixFilterDescriptionItems.GetAt(j);

			//set colors
			pDC->SetBkColor(((((type == mfdt_sort)  &&(i == _selectedSort  ))||
				              ((type == mfdt_column)&&(i == _selectedColumn))||
							  ((type == mfdt_filter)&&(i == _selectedFilter))) && (!item->isLink()))?(::GetSysColor(COLOR_HIGHLIGHT)):(::GetSysColor(COLOR_WINDOW)));

			if (matrixFilterDescriptionItems.GetAt(j)->isLink())
			{
				pDC->SetTextColor((matrixItemBase->isInvoked()||item->isLogic())? RGB(0,0,255):RGB(255,0,0));
				pDC->SelectObject(_underlineFont);
			}
			else
			{
				pDC->SetTextColor(((i == _selectedSort && type == mfdt_sort)||
								   (i == _selectedColumn && type == mfdt_column)||
								   (i == _selectedFilter && type == mfdt_filter))?(::GetSysColor(COLOR_HIGHLIGHTTEXT)):(::GetSysColor(COLOR_WINDOWTEXT)));
				pDC->SelectObject(_stdFont);
			}
			
			//draw the text
			int thisTop = (top + (vertSpacing * i) + vertSpacing);
			pDC->TextOut(left, thisTop, item->getText());			

			//store the position
			int width = pDC->GetTextExtent(item->getText()).cx;
			item->setPositions(left, left + width);
			left += width;

			//get the widest point.
			if (left > maxWidth)
				maxWidth = left;
			if (thisTop > maxHeight)
				maxHeight = thisTop;
		}
	}

	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);
	pDC->SetBkColor(oldBKColor);

	
	return CSize(maxWidth, maxHeight + vertSpacing);			 

	return CSize(0,0);
}			

			
void MatrixFilterDescriptionWnd::hitTest(CPoint point, MatrixFilterDescriptionType& type,  int& index)
{
	point.y -= topOffset;

	int row = point.y / vertSpacing;

//	TRACE("row: %d\r\n",row);

	//we are a sorting
	type = mfdt_none;
	index = -1;

	//are we on a title row?
	if (row == 0) 
		return;
	
	if (_filterSort.getFilterCount() > 0)
	{
		row--;

		//are we a filter
		if ((row >=0)&&(row < _filterSort.getFilterCount()))
		{
			type = mfdt_filter;
			index = row;
			return;
		}

		row -= _filterSort.getFilterCount();
	}

	if (_filterSort.getSortCount() > 0)
	{
		row--;

		//are we a filter
		if ((row >=0)&&(row < _filterSort.getSortCount()))
		{
			type = mfdt_sort;
			index = row;
			return;
		}

		row -= _filterSort.getSortCount();
	}

	if (_filterSort.getColumnCount() > 0)
	{
		row--;

		//are we a filter
		if ((row >=0)&&(row < _filterSort.getColumnCount()))
		{
			type = mfdt_column;
			index = row;
			return;
		}

		row -= _filterSort.getColumnCount();
	}	

}
		 
BOOL MatrixFilterDescriptionWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{	 
	if (nHitTest==HTCLIENT)
	{
		MatrixFilterDescriptionType type = mfdt_none;
		int index;

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		point += GetScrollPosition();

		hitTest(point, type, index);	

		
		
		if (((type == mfdt_filter)&&(_filterSort.getFilter(index)->isLink(point.x)))||
			((type == mfdt_sort)&&(_filterSort.getSort(index)->isLink(point.x)))||
			((type == mfdt_column)&&(_filterSort.getColumn(index)->isLink(point.x))))

		{
			SetCursor(app()->LoadCursor(IDC_FPOINT));
			return TRUE;
		}
	}

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return TRUE;

}
		  
void MatrixFilterDescriptionWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	point += GetScrollPosition();

	MatrixFilterDescriptionType type = mfdt_none;
	int index;

	_selectedSort = -1; 
	_selectedColumn = -1; 
	_selectedFilter = -1; 
	Invalidate(); //get the selection redrawn
	
	hitTest(point, type, index);	

	switch (type)
	{
	case mfdt_filter:
		_selectedFilter = index; 
		Invalidate(); //get the selection redrawn
		_filterSort.getFilter(index)->clickLink(this, point.x, &_filterSort);
		break;
	case mfdt_sort:
		_selectedSort = index; 
		Invalidate(); //get the selection redrawn
		_filterSort.getSort  (index)->clickLink(this, point.x, &_filterSort);
		break;
	case mfdt_column:
		_selectedColumn = index; 
		Invalidate(); //get the selection redrawn
		_filterSort.getColumn (index)->clickLink(this, point.x, &_filterSort);
		break;
	case mfdt_none:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
		 
	update();
	
	ScrollWnd::OnLButtonDown(nFlags, point);
}
				  
int MatrixFilterDescriptionWnd::getSelectedSort()
{
	return _selectedSort;
}

void MatrixFilterDescriptionWnd::setSelectedSort(int index)
{
	_selectedSort = index;

	if ((index >= 0)&&(index < _filterSort.getSortCount()))
		_selectedSort = index;

	Invalidate();
}
											 
int MatrixFilterDescriptionWnd::getSelectedColumn()
{
	return _selectedColumn;
}

void MatrixFilterDescriptionWnd::setSelectedColumn(int index)
{
	_selectedColumn = index;

	if ((index >= 0)&&(index < _filterSort.getColumnCount()))
		_selectedColumn = index;

	Invalidate();
}

int MatrixFilterDescriptionWnd::getSelectedFilter()
{
	return _selectedFilter;
}

void MatrixFilterDescriptionWnd::setSelectedFilter(int index)
{
	_selectedFilter = -1;

	if ((index >= 0)&&(index < _filterSort.getFilterCount())) 
		_selectedFilter = index;

	Invalidate();
}


