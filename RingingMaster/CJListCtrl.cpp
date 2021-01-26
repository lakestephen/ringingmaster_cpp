// CJListCtrl.cpp : implementation file
// Copyright � 1998-1999 CodeJock.com, All Rights Reserved.
// See ReadMe.txt for TERMS OF USE.
//
// Based on the CListView articles from http://www.codeguru.com/listview
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 10/30/99 1:52p $
 * $Revision: 10 $
 * $Archive: /CodeJock/CJLibrary/CJListCtrl.cpp $
 *
 * $History: CJListCtrl.cpp $
 * 
 * *****************  Version 10  *****************
 * User: Kirk Stowell Date: 10/30/99   Time: 1:52p
 * Updated in $/CodeJock/CJLibrary
 * Overrode OnEraseBkgnd(...) and OnPaint(...) to handle flicker free
 * drawing.
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 10/25/99   Time: 10:52p
 * Updated in $/CodeJock/CJLibrary
 * Modified resource include for static builds.
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 8/29/99    Time: 11:45p
 * Updated in $/CodeJockey/CJLibrary
 * Fixed memory lead with header  control allocation.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 8/29/99    Time: 9:14p
 * Updated in $/CodeJockey/CJLibrary
 * Added Unicode compliance, thanks to Barry Burton for his help with
 * this.
 * 
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:30a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 6/12/99    Time: 2:29a
 * Updated in $/CodeJockey/CJ60Lib
 * Cleaned up initialization, sorting and column info saving. Removed sort
 * functions, which were replaced with the virtual function SortList(...).
 * Override this function in your derived class to perform custom sorting.
 * See CCJShellList for an implementation of this virtual function.
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 8:38p
 * Updated in $/CodeJockey/CJ60Lib
 * Added comments and cleaned up code.
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 3/03/99    Time: 8:37p
 * Updated in $/CodeJockey/CJ60Lib
 * Added InitializeFlatHeader() in order to initialize the list control
 * manually.
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 1/16/99    Time: 8:37p
 * Created in $/CodeJockey/CJ60Lib
 * Initial release.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CJListCtrl.h"
#include "RingingMaster.h"

#define CCJ_COLUMN_DRAG_END   (WM_USER + 201)

//todo remove all ccj classes.


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCJListCtrl

CCJListCtrl::CCJListCtrl()
{
	m_nMinSize		= 0;
	m_nMinColWidth	= 50;
	m_nMaxColWidth	= 500;
	m_nSortedCol	= -1;
	m_bAscending	= true;
	m_bAutoSave		= false;	
	m_bNoColSizing	= false;
	m_clrText		= ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk 	= ::GetSysColor(COLOR_WINDOW);
	_usePressedString = false;
}

CCJListCtrl::~CCJListCtrl()
{
}

IMPLEMENT_DYNAMIC(CCJListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CCJListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCJListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()		
	ON_MESSAGE(CCJ_COLUMN_DRAG_END, OnColumnDragEnd)
	ON_WM_TIMER()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJListCtrl message handlers

// Taken from: Copying/Moving Rows in CListCtrl by James Spibey [spib@bigfoot.com]. 
// http://www.codeguru.com/listview/moving_rows.html
bool CCJListCtrl::MoveRow(int nFrom, int nTo)
{
	//Can't move to the same place, or from or to a negative index
	if(nFrom == nTo || nFrom < 0 || nTo < 0)
		return false;
	
	//First Copy the row to the new location
	if(CopyRow(nFrom, nTo))
	{
		//If we have just inserted a row before
		//this one in the list, we need to increment
		//our index.
		if(nFrom > nTo)
			DeleteItem(nFrom + 1);
		else
			DeleteItem(nFrom);
		
		return true;
	}
	else
		return false;
}

// Taken from: Copying/Moving Rows in CListCtrl by James Spibey [spib@bigfoot.com]. 
// http://www.codeguru.com/listview/moving_rows.html
bool CCJListCtrl::CopyRow(int nFrom, int nTo)
{
	//Can't move to the same place, or from or to a negative index
	if(nFrom == nTo || nFrom < 0 || nTo < 0)
		return false;

	//Copy the row to the new index
	InsertItem(nTo, GetItemText(nFrom, 0));

	//If row has been inserted before original
	//increment the original
	if(nFrom > nTo)
		nFrom++;

	//Loop through subitems
	for(int i = 1; i < GetColumnCount(); i++)
	{
		SetItemText(nTo, i, GetItemText(nFrom, i));
	}

	return true;
}

// Taken from: Getting the number of columns in report view by Zafir Anjum.
// http://www.codeguru.com/listview/num_cols.shtml
int CCJListCtrl::GetColumnCount() const
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	return pHeader->GetItemCount();
}

// Taken from: Simpler way to do this (using new version of Comctl32.dll) by Vidas [vibal@lrtc.lt].
// http://www.codeguru.com/mfc/comments/6432.shtml
void CCJListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	
    switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		// Request prepaint notifications for each item.
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
		
    case CDDS_ITEMPREPAINT: // Requested notification
		if(lplvcd->nmcd.dwItemSpec % 2)
		{
			lplvcd->clrText   = m_clrText;
			lplvcd->clrTextBk = m_clrTextBk;
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}

BOOL CCJListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *pHDNotify = (HD_NOTIFY*)lParam;

	switch( pHDNotify->hdr.code )
	{
	// Sorting the list when user clicks on column header by Zafir Anjum.
	// http://www.codeguru.com/listview/sort_on_col_click.shtml
	case HDN_ITEMCLICKA:
	case HDN_ITEMCLICKW:
		{
			if( pHDNotify->iButton == 0 )
			{
				// set the sort order.
				if( pHDNotify->iItem == m_nSortedCol )
					m_bAscending = !m_bAscending;
				else
					m_bAscending = true;

				// save the column index.
				m_nSortedCol = pHDNotify->iItem;

				// virtual call to sort list.
				SortList( m_nSortedCol, m_bAscending );
			}
		}
		break;

	// How to force a minimum column width Cees Mechielsen.
	// http://www.codeguru.com/mfc/comments/866.shtml
	case HDN_ITEMCHANGINGA:
	case HDN_ITEMCHANGINGW:
	case HDN_ENDTRACK:
		{
			if( pHDNotify->pitem->mask & HDI_WIDTH &&
				pHDNotify->pitem->cxy < m_nMinSize &&
				pHDNotify->pitem->cxy >= 0 )
			{
				pHDNotify->pitem->cxy = m_nMinSize;	// Set the column width
			}
		}
		break;

	// Prevent CListCtrl column resizing by Zafir Anjum.
	// http://www.codeguru.com/listview/no_col_resize.shtml
	case HDN_DIVIDERDBLCLICKA:
	case HDN_DIVIDERDBLCLICKW:
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
		{
			if(m_bNoColSizing)
			{
				*pResult = TRUE;                // disable tracking
				return TRUE;                    // Processed message
			}
			break;
		}
	case HDN_ENDDRAG:
		{
			PostMessage(CCJ_COLUMN_DRAG_END,0,0);			
		}
	}

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

bool CCJListCtrl::SortList(int nCol, bool bAscending)
{
#ifdef _DEBUG
	TRACE2("Column sort on column index %d, ascending=%d\r\n", nCol, bAscending);
#else
	UNUSED_ALWAYS( nCol );
	UNUSED_ALWAYS( bAscending );
#endif

	return true;
}

// Taken from: Adding a column by Zafir Anjum.
// http://www.codeguru.com/listview/add_column.shtml

// AddColumn      - adds column after the right most column
// Returns        - the index of the new column if successful or -1 otherwise.
// lpszColHeading - the column's heading
// nWidth         - Width of the column, in pixels. If this parameter is -1, 
//                  the column width is the same as previous column
// nFormat        - the alignment of the column. Can be LVCFMT_LEFT,
//                  LVCFMT_RIGHT, or LVCFMT_CENTER.
int CCJListCtrl::AddColumn(LPCTSTR lpszColHeading, int nWidth/*=-1*/, int nFormat/*=LVCFMT_LEFT*/)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	
	if( nWidth == -1 )
	{
		// The line below return zero for ICON views
		//nWidth = GetColumnWidth( nColumnCount - 1 );
		
		// Get the column width of the previous column from header control
		HD_ITEM hd_item;
		hd_item.mask = HDI_WIDTH;               //indicate that we want the width
		pHeader->GetItem( nColumnCount - 1, &hd_item );
		nWidth = hd_item.cxy;
	}
	return InsertColumn( nColumnCount, lpszColHeading, nFormat, nWidth, nColumnCount );
}

void CCJListCtrl::RemoveAllColumns()
{
	while (GetColumnCount() >0)
		DeleteColumn(0);
}



bool CCJListCtrl::BuildColumns(int nCols, int* nWidth, int* nColString)
{
	for( int i = 0; i < nCols; ++i ) {
		CString	strTemp; strTemp.LoadString( nColString[i] );
		if(AddColumn(strTemp, nWidth[i])==-1)
			return false;
	}

	return true;
}

bool CCJListCtrl::BuildColumns(int nCols, int* nWidth, CString* strColString)
{
	for( int i = 0; i < nCols; ++i ) {
		if(AddColumn(strColString[i], nWidth[i])==-1)
			return false;
	}

	return true;
}

// Taken from: Detecting column index of the item clicked by Zafir Anjum.
// http://www.codeguru.com/listview/col_index.shtml

// HitTestEx	- Determine the row index and column index for a point
// Returns		- the row index or -1 if point is not over a row
// point		- point to be tested.
// nCol			- to hold the column index
int CCJListCtrl::HitTestEx(CPoint& point, int* nCol) const
{
	int colnum = 0;
	int row = HitTest( point, NULL );
	
	if( nCol ) *nCol = 0;

	// Make sure that the ListView is in LVS_REPORT
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return row;

	// Get the top and bottom row visible
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if( bottom > GetItemCount() )
		bottom = GetItemCount();
	
	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <= bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = GetColumnWidth(colnum);
				if( point.x >= rect.left 
					&& point.x <= (rect.left + colwidth ) )
				{
					if( nCol ) *nCol = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

void CCJListCtrl::AutoSaveColumns(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	m_bAutoSave = true;

	// initialize registry strings.
	if( lpszSection == NULL ) m_strSection = _T("Columns");
	else m_strSection = lpszSection;

	ASSERT (lpszEntry != NULL ); 
	m_strEntry = lpszEntry;

	if( lpszDefault == NULL ) m_strDefault = _T("");
	else m_strDefault = lpszDefault;

	LoadColumnWidths();
}

void CCJListCtrl::SaveColumnWidths()
{
	// get a pointer to the header control.
	CHeaderCtrl* pHeader = GetHeaderCtrl();
//	ASSERT_KINDOF( CHeaderCtrl, pHeader );

	CString strValue;
	for( int i = 0; i < pHeader->GetItemCount(); ++i )
	{
		CString strTemp;
		strTemp.Format(_T("%d,"), GetColumnWidth( i ));
		strValue += strTemp;
	}

	AfxGetApp()->WriteProfileString( m_strSection, m_strEntry, strValue );
}

void CCJListCtrl::LoadColumnWidths()
{
	// get a pointer to the header control.
	CHeaderCtrl* pHeader = GetHeaderCtrl();
//	ASSERT_KINDOF( CHeaderCtrl, pHeader );

	for( int i = 0; i < pHeader->GetItemCount(); ++i )
		SetColumnWidth(i);
}

void CCJListCtrl::SetColumnWidth(int nCol)
{
	int nWidth = GetStoredWidth( nCol );
	if( nWidth > m_nMinColWidth )
		CListCtrl::SetColumnWidth( nCol, nWidth );
	else
		AutoSizeColumn( nCol );
}

int CCJListCtrl::GetStoredWidth(int nCol)
{
	// get the value from the registry.
	CString strValue = AfxGetApp()->GetProfileString(
		m_strSection, m_strEntry, m_strDefault );

	// extract the sub string to get the column width.
	CString strSubString;
	AfxExtractSubString( strSubString, strValue, nCol, _T(','));

	// return the width from the registry.
	return _ttoi( strSubString );
}

void CCJListCtrl::OnDestroy() 
{
	if( m_bAutoSave ) SaveColumnWidths();
	CListCtrl::OnDestroy();
}

// Taken from: Autosize a column to fit its content by Roger Onslow.
// http://www.codeguru.com/listview/autosize_col.shtml
// If you don't supply a column number, it will resize all columns.
void CCJListCtrl::AutoSizeColumn(int nCol/*=-1*/)
{
	// Call this after your list control is filled
	SetRedraw( false );
	
	int nMinCol = nCol < 0 ? 0 : nCol;
	int nMaxCol = nCol < 0 ? GetColumnCount()-1 : nCol;
	
	for (nCol = nMinCol; nCol <= nMaxCol; nCol++)
	{
		CListCtrl::SetColumnWidth( nCol, LVSCW_AUTOSIZE );
		int wc1 = GetColumnWidth( nCol );
		
		CListCtrl::SetColumnWidth( nCol, LVSCW_AUTOSIZE_USEHEADER );
		int wc2 = GetColumnWidth( nCol );
		int wc = max( m_nMinColWidth, max( wc1, wc2 ));
	
		if( wc > m_nMaxColWidth )
			wc = m_nMaxColWidth;
		
		// set the column width.
		CListCtrl::SetColumnWidth( nCol,wc );
	}
	
	SetRedraw();
}

BOOL CCJListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// KStowell - overridden for flicker-free drawing.
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CCJListCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// KStowell - Get the client rect.
	CRect rcClient, rcClip;
	dc.GetClipBox( &rcClip );
	GetClientRect( &rcClient );

	// Exclude the header control from being painted over.
	CWnd* pWnd = GetDlgItem(0);
	if(pWnd && pWnd->IsWindowVisible()) {
		CRect rcItem;
		pWnd->GetClientRect(&rcItem);
		dc.ExcludeClipRect(&rcItem);
	}

	// KStowell - Create a memory device-context. This is done to help reduce
	// screen flicker, since we will paint the entire control to the
	// off screen device context first.
	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// KStowell - Repaint the background.
	memDC.FillSolidRect( rcClient, ::GetSysColor(COLOR_WINDOW) );

    //demonstrate an empty list
	if (GetItemCount() <= 0)
	{
        memDC.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
        memDC.SetBkColor(::GetSysColor(COLOR_WINDOW));
        memDC.SelectStockObject(ANSI_VAR_FONT);
		memDC.DrawText(getEmptyListText(), -1, CRect(20,30,400,400), 
                      DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP );
	}
	else// let the control do its default drawing.
		CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	// KStowell - Copy the memory device context back into the original DC via BitBlt().
	dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, 
		rcClip.left, rcClip.top, SRCCOPY );

	// KStowell - Cleanup resources.
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

CString CCJListCtrl::getEmptyListText()
{
	return "Empty List";
}

int CCJListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	VERIFY(m_wndFlatHeader.SubclassWindow(::GetDlgItem(m_hWnd,0)))	;

	return 0;
}

LRESULT CCJListCtrl::OnColumnDragEnd(UINT /*wParam*/, LONG /*lParam*/) 
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	if (pHeader)
	{		  
		INT iCount = pHeader->GetItemCount();
		LPINT piArray = new INT[iCount*2];
		pHeader->GetOrderArray((LPINT)piArray, iCount);

		notifyOrderArray(iCount, piArray);

		delete [] piArray;
	}

	return 0;
}

void CCJListCtrl::notifyOrderArray(int /*iCount*/, LPINT /*piArray*/)
{
	//virtual in base class
}

void CCJListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//go through the list of items, looking at the sort criteria, and move to trhat row.
	TRACE("%c\r\n", nChar);

	if (isalnum(nChar)&&
		(_usePressedString)&&
		(m_nSortedCol>=0)&&
		(m_nSortedCol<GetColumnCount()))
	{
		CString testStr;
		_pressedString += ((char)nChar);
		int length = _pressedString.GetLength();
		SetTimer(AAA_TIMER_LIST_SCROLL, 600, NULL);
		TRACE("Finding Row: " + _pressedString + " flags: %d\r\n", nFlags); 

		//get the list of items being shown
		int i;
		for (i=0;i<GetItemCount();i++)
		{
			CString testStr = getPressedTestString(i, m_nSortedCol);
			testStr = testStr.Left(length);

			int compResult = testStr.CompareNoCase(_pressedString);

			//TRACE("m_bAscending %d, _pressedString = %s, testStr = %s, compResult = %d", m_bAscending, _pressedString, testStr, compResult);
			
			if (!testStr.IsEmpty())
			{
				if (( m_bAscending)&&(compResult >=0))
					break;
				if ((!m_bAscending)&&(compResult <=0))
					break;
			}
		}	  
	
		//deselect all items (for the benifit of multi select controls
		for (int j=0;j<GetItemCount();j++)
			SetItemState(j,0, LVIS_SELECTED | LVIS_FOCUSED);

		//if not break out of for loop, then will bve too high - 
		if (i >= GetItemCount())
			i = GetItemCount() -1;

		//select and show new item
		SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		EnsureVisible(i, false);

		TRACE("Select Row: %d \r\n", i);

	}  
	else
		CListCtrl::OnChar(nChar, nRepCnt, nFlags);
}


void CCJListCtrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == AAA_TIMER_LIST_SCROLL)
	{
		_pressedString.Empty();
		KillTimer(AAA_TIMER_LIST_SCROLL);
		TRACE("Clearing Row: " + _pressedString + "\r\n"); 
	}						  
	
	CListCtrl::OnTimer(nIDEvent);
}

void CCJListCtrl::setUsePressedString(bool usePressedString)
{
	_usePressedString = usePressedString;
}

CString CCJListCtrl::getPressedTestString(int /*row*/, int /*column*/)
{
	//if you have set _usePressedString to true, then you must override this method
	ASSERT(FALSE);
	return "";
}

void CCJListCtrl::selectIndex(int index)
{
	SetItemState(index,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	EnsureVisible(index,FALSE);
}