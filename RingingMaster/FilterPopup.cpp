// FilterPopup.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "FilterPopup.h"
#include "RecordManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//todo might not need this class if we use ribbon control.

/////////////////////////////////////////////////////////////////////////////
// FilterPopup


BEGIN_MESSAGE_MAP(FilterPopup, AutoPopup)
	//{{AFX_MSG_MAP(FilterPopup)
	ON_CBN_SELCHANGE(AAA_POPUP_FILTER_COMBO, OnSelchangeOptionsFilterCombo)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



FilterPopup::FilterPopup()
{
	recordManager()->addEventListener(this);
}

FilterPopup::~FilterPopup()
{
	recordManager()->removeEventListener(this);
}

BOOL FilterPopup::Create(CWnd* pParentWnd)
{
	BOOL res = AutoPopup::Create(pParentWnd);

	if (res)
	{
		_comboFilter.Create(this);
		SetTimer(AAA_KILL_FOCUS_TIMER, 100, NULL);
	}

    return res;
}

void FilterPopup::Set(CPoint point)
{
   	recordManager_updateFilterList();	

	// determine the window size and position
	// displayed above and centered on the origin
	CRect wndRect(point.x, 
			point.y, 
			point.x + 125,
			point.y + 27); 
			
	//ckeck it can be shown.
	CRect screen;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &screen, SPIF_UPDATEINIFILE);
	
	CPoint scrOffset;
	scrOffset.x = screen.right - wndRect.right - 8;
	scrOffset.y = screen.bottom - wndRect.bottom - 8;
	
	if (scrOffset.x > 0) scrOffset.x = 0;
	if (scrOffset.y > 0) scrOffset.y = 0;
	
	wndRect.OffsetRect(scrOffset);
	
	// update window position and display
	SetWindowPos(&wndTop, 
		wndRect.left,
		wndRect.top,
		wndRect.Width( ), 
		wndRect.Height( ), 
		SWP_SHOWWINDOW | SWP_NOACTIVATE);

//	_comboFilter.SetFocus(); 
}

void FilterPopup::OnSelchangeOptionsFilterCombo() 
{
	if (::IsWindow(m_hWnd))
	{
		mainFrame()->PostMessage(WM_APPLY_COMBINED_FILTER, _comboFilter.GetCurSel(),0);
		PostMessage(WM_KILL_POPUP); 
	}
}




void FilterPopup::OnTimer(UINT nIDEvent) 
{
	if ((::IsWindow(m_hWnd))&&(!IsChild(GetFocus())))
	{
		PostMessage(WM_KILL_POPUP); 
		AutoPopup::OnTimer(nIDEvent);
	}
}

void FilterPopup::recordManager_updateFilterList()
{	 /*
	if(IsWindow(m_hWnd))
	{
		_comboFilter.ResetContent();
		ViewType viewType = app()->getViewType();
		CombinedFilterItems combinedFilterItems;
		matrixManager()->getCombinedFilterList(viewType, combinedFilterItems);
		_comboFilter.AddString("<select filter>");

		for (int i=0;i<combinedFilterItems.GetSize();i++)
		{
			int index = _comboFilter.AddString(combinedFilterItems.GetAt(i)->_name);
			_comboFilter.SetItemData(index, combinedFilterItems.GetAt(i)->_userFilter?1:0);
			delete combinedFilterItems.GetAt(i);
		}
		_comboFilter.SetCurSel(0);
	}	  */ //todo
}

void FilterPopup::OnDestroy() 
{
	AutoPopup::OnDestroy();	
}
