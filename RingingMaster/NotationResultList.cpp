// NotationResultList.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationResultList.h"
#include "NotationResultsDlg.h"
#include "Notation.h"
#include "MethodPosition.h"
#include "ExplorerFolder.h"
#include "Lead.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationResultList

NotationResultList::NotationResultList() :
_parent(NULL)
{
}

NotationResultList::~NotationResultList()
{
}


BEGIN_MESSAGE_MAP(NotationResultList, NotationListCtrl)
	//{{AFX_MSG_MAP(NotationResultList)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationResultList message handlers

void NotationResultList::OnDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	if (_parent->_noSelect)
	{
		CPoint hitPoint;
		GetCursorPos(&hitPoint);
		ScreenToClient(&hitPoint);
		_lastContextItem = HitTest(hitPoint);
		if (_lastContextItem != -1)
			OnMePreview();
	}
	else
		_parent->makeSelection();
	
	*pResult = 0;
}


void NotationResultList::init()
{
	NotationListCtrl::init();

	AutoSaveColumns(NULL, "Notation Results Columns", "148,52,173,38,168");

	m_bAscending = (app()->GetProfileInt("Explorer", "Last Results Sort Direction", TRUE) == TRUE);
	m_nSortedCol = app()->GetProfileInt("Explorer", "Last Results Sort Column", 0);
}

void NotationResultList::setParent(NotationResultsDlg *parent)
{
	_parent = parent;
}

void NotationResultList::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
	
	CPoint hitPoint = point;
	ScreenToClient(&hitPoint);

	_lastContextItem = HitTest(hitPoint);

	CMenu menu;
	if (_lastContextItem != -1)
	{
		VERIFY(menu.LoadMenu (IDR_RESULTS_LIST));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);		
	}
}




bool NotationResultList::SortList(int nCol, bool bAscending)
{	
	bool val = NotationListCtrl::SortList(nCol, bAscending);

	app()->WriteProfileInt("Explorer", "Last Results Sort Direction", m_bAscending);
	app()->WriteProfileInt("Explorer", "Last Results Sort Column", m_nSortedCol);

	Invalidate();

	return val;
}




void NotationResultList::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO* pLvdi = (LV_DISPINFO*)pNMHDR;

	NotationLite* lite = _liteList[pLvdi->item.iItem];

	if (lite->getFolderID() < ExplorerFolder::_pivotPoint)
	{
		*pResult = TRUE; //stop the edit
		return;
	}

	*pResult = FALSE;
}




