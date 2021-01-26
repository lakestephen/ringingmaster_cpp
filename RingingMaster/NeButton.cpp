#include "stdafx.h"
#include "NeButton.h"
#include "NumericEdit.h"
#include "resource.h"



// NeButton

NeButton::NeButton():
_parent(NULL)
{
}


IMPLEMENT_DYNAMIC(NeButton, CButton)

BEGIN_MESSAGE_MAP(NeButton, CButton)
ON_MESSAGE(BM_SETSTYLE,OnBnSetStyle)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// NeButton message handlers


BOOL NeButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT &rect, NumericEdit *pParentWnd)
{
	_parent = pParentWnd;
	return CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, ID_NEBUTTON);
}


//****************************************************************************
// NeButton::OnBnSetStyle - Prevents the button from presenting itself with
//                           a black border. A black border is normally used
//                           to indicate the default button. 
//****************************************************************************

LRESULT NeButton::OnBnSetStyle(WPARAM wParam,LPARAM lParam)
{
	return DefWindowProc(BM_SETSTYLE, (wParam & ~BS_DEFPUSHBUTTON), lParam);
}

//****************************************************************************
// NeButton::OnLButtonDown - Creates and displays a CTrackPopup window. The
//                            message is NOT forwarded to the base class
//                            because that would cause the button to capture
//                            the mouse and we need to capture the mouse in
//                            the popup window. SetState() is called to redraw
//                            the button in the pressed state.
//****************************************************************************

void NeButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CButton::OnLButtonDown(nFlags,point);
	_parent->ShowPopup();
	
	SetState(TRUE);
}