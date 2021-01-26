// FilterPopupCombo.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "FilterPopupCombo.h"
#include "UserMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FilterPopupCombo

FilterPopupCombo::FilterPopupCombo() :
FilterCombo(NULL),
_pParent(0)
{
	_font.CreateStockObject (DEFAULT_GUI_FONT);

}

FilterPopupCombo::~FilterPopupCombo()
{
}


BEGIN_MESSAGE_MAP(FilterPopupCombo, FilterCombo)
	//{{AFX_MSG_MAP(FilterPopupCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FilterPopupCombo message handlers


BOOL FilterPopupCombo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			_pParent->PostMessage(WM_KILL_POPUP); 
			return TRUE;
		}

		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}
	
	return FilterCombo::PreTranslateMessage(pMsg);
}

BOOL FilterPopupCombo::Create(CWnd* pParentWnd) 
{
	_pParent = pParentWnd;

	BOOL res =FilterCombo::Create((WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_OWNERDRAWVARIABLE|CBS_HASSTRINGS|CBS_DROPDOWNLIST), CRect(0,0,120,150), pParentWnd, AAA_POPUP_FILTER_COMBO);

	SetFont(&_font);
	SetFocus();
	return res;	
}
