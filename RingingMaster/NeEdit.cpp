#include "stdafx.h"
#include "NeEdit.h"
#include "NumericEdit.h"
#include "resource.h"

NeEdit::NeEdit() :
_parent(NULL)
{
    m_fOverFlow = FALSE;
    m_brError.CreateSolidBrush(RGB(255,128,128));
	
	if (!_fontStd.CreateStockObject(DEFAULT_GUI_FONT))
		if (!_fontStd.CreatePointFont(80, "MS Sans Serif"))
		{
			ASSERT(FALSE);
		}
}

 

// NeEdit

IMPLEMENT_DYNAMIC(NeEdit, CEdit)

BEGIN_MESSAGE_MAP(NeEdit, CEdit)
ON_WM_KEYDOWN()
ON_WM_CHAR()
ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

// NeEdit message handlers

BOOL NeEdit::Create(DWORD dwStyle, const RECT &rect, NumericEdit *pParentWnd)
{
	BOOL val = CEdit::Create(dwStyle, rect, pParentWnd, ID_NEEDIT);
	
	_parent = pParentWnd;
	SetFont(&_fontStd);
	
	return val;
	
}


//****************************************************************************
// NeEdit::OnKeyDown - Intercepts pressed keys while focused and passes UP
//                      and down to the parent. 
//****************************************************************************

void NeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP || nChar == VK_DOWN)
		GetParent()->SendMessage(WM_KEYDOWN,(WPARAM)nChar,MAKELPARAM(nRepCnt,nFlags));
	else
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

//****************************************************************************
// NeEdit::OnChar - Only allows numbers and backspace
//****************************************************************************

void NeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!_istdigit(nChar) && (nChar != 8) && (nChar != _T('-')) && (nChar != _T('.')))
	{
		MessageBeep(0);
		return;
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

//****************************************************************************
// NeEdit::OnEnChange - Converts current text contents to an int and passes
//                       it to the parent using a private message. Then it
//                       determines if it's out of range so that CtlColor()
//                       knows what background color to return
//****************************************************************************

void NeEdit::OnEnChange()
{
	CString str;
	float     nMin, nMax;
	
	GetWindowText(str);
	float nValue = (float)atof(str);
	
	_parent->GetRange(nMin, nMax);
	
	BOOL fNewOverFlow = (nValue < nMin) || (nValue > nMax);
	if (fNewOverFlow != m_fOverFlow)
	{
		m_fOverFlow = fNewOverFlow;
		Invalidate();
	}
	
	_parent->SetValue(nValue, false);
}

//****************************************************************************
// NeEdit::CtlColor - Depending on the state of m_bOverflow, causes the edit
//                     background to either show as default or the error color
//****************************************************************************

HBRUSH NeEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	
	if (m_fOverFlow)
		return m_brError;
	else
		return NULL;
}
