// PopupList.cpp: implementation of the PopupList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PopupList.h"

#include "AutoPopup.h"

#include "UserMessages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PopupList::PopupList() :
_pParent(0)
{
	_font.CreateStockObject (DEFAULT_GUI_FONT);
}

PopupList::~PopupList()
{

}

BEGIN_MESSAGE_MAP(PopupList, ColorListBox)
	//{{AFX_MSG_MAP(PopupList)
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL PopupList::Create(CWnd* pParentWnd)
{
	_pParent = pParentWnd;
	
	BOOL res = ColorListBox::Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL |
						LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_OWNERDRAWVARIABLE,
						CRect(0,0,0,0), pParentWnd, AAA_POPUP_LIST);

	SetFont(&_font);
	SetFocus();
	return res;	
}

void PopupList::OnKillFocus(CWnd* pNewWnd) 
{
	ColorListBox::OnKillFocus(pNewWnd);

	if ((IsWindow(m_hWnd))&&(pNewWnd != this)&&(pNewWnd != _pParent))
	{
		_pParent->PostMessage(WM_KILL_POPUP);
	}
}

void PopupList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	_pParent->PostMessage(WM_KILL_POPUP);
	
	ColorListBox::OnLButtonUp(nFlags, point);
}


BOOL PopupList::PreTranslateMessage(MSG* pMsg) 
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
	return ColorListBox::PreTranslateMessage(pMsg);
}
