#include "StdAfx.h"
#include ".\proplistbase.h"
#include "ringingmaster.h"

PropListBase::PropListBase(void)
{

}

PropListBase::~PropListBase(void)
{
}

BOOL PropListBase::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}


int PropListBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	mainFrame()->addEventListener(this);

	return 0;
}

BEGIN_MESSAGE_MAP(PropListBase, CBCGPPropList)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()

	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

void PropListBase::initPropList()
{
	EnableHeaderCtrl (FALSE);
	EnableDesciptionArea ();
	SetVSDotNetLook (TRUE);

	COLORREF clrBackground, clrText, clrGroupBackground, clrGroupText, clrDescriptionBackground, clrDescriptionText, clrLine;
	GetCustomColors(clrBackground,clrText,clrGroupBackground,clrGroupText,clrDescriptionBackground,clrDescriptionText,clrLine);
	clrGroupText = RGB(0,0,0);
	SetCustomColors(clrBackground,clrText,clrGroupBackground,clrGroupText,clrDescriptionBackground,clrDescriptionText,clrLine);

}

void PropListBase::mainFrame_notifyDocViewChanged(ViewType /*oldViewType*/, ViewType /*newViewType*/, CDocument* /*oldDoc*/, CDocument* /*newDoc*/)
{
	update();
}

void PropListBase::update()
{
	//-----------------------------------------
	//deal with the hint panel
	//-----------------------------------------
	int height = app()->GetProfileInt("MainFrame", "Property Window Hint Height", -1);
	if ((height != -1) &&(GetDescriptionHeight() != height))
		m_nDescrHeight = height;

	EnableDesciptionArea(app()->GetProfileInt("MainFrame", "Property Window Hint", TRUE));
	SetAlphabeticMode(app()->GetProfileInt("MainFrame", "Property Alpha Mode", FALSE));
}

BOOL PropListBase::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)	 //to2do remove when th accellerators are replaced with the On Char in the main touch window
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

void PropListBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	int height = GetDescriptionHeight();

	__super::OnLButtonUp(nFlags, point);

	if (height != GetDescriptionHeight())
		app()->WriteProfileInt("MainFrame", "Property Window Hint Height", GetDescriptionHeight());

	saveExpandStatus();
}

void PropListBase::OnRButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);

	//save away the expand status
	saveExpandStatus();
}

void PropListBase::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	__super::OnVScroll(nSBCode, nPos, pScrollBar);

	//save away the expand status
	saveExpandStatus();
}

void PropListBase::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

		//save away the expand status
	saveExpandStatus();
}



