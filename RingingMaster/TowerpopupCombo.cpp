// TowerpopupCombo.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerpopupCombo.h" 
#include "AutoPopup.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerpopupCombo

TowerpopupCombo::TowerpopupCombo() :
_pParent(0)
{
	_font.CreateStockObject (DEFAULT_GUI_FONT);

}

TowerpopupCombo::~TowerpopupCombo()
{
}


BEGIN_MESSAGE_MAP(TowerpopupCombo, CCJFlatComboBox)
	//{{AFX_MSG_MAP(TowerpopupCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerpopupCombo message handlers


BOOL TowerpopupCombo::PreTranslateMessage(MSG* pMsg) 
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
	
	return CCJFlatComboBox::PreTranslateMessage(pMsg);
}

BOOL TowerpopupCombo::Create(CWnd* pParentWnd) 
{
	_pParent = pParentWnd;
	
	BOOL res = CCJFlatComboBox::Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWN | WS_VSCROLL, 
						CRect(0,0,120,150), pParentWnd, AAA_POPUP_TOWER_COMBO);

	SetFont(&_font);
	SetFocus();
	return res;	
}
