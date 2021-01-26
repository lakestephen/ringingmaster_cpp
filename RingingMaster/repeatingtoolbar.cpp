// repeatingtoolbar.cpp : implementation file
//

#include "stdafx.h"
#include "ringingmaster.h"
#include "repeatingtoolbar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RepeatingToolbar


RepeatingToolbar::RepeatingToolbar():
_timerId(0),
_commandId(0)
{
}

RepeatingToolbar::~RepeatingToolbar()
{
}


BEGIN_MESSAGE_MAP(RepeatingToolbar, CBCGPToolBar)
	//{{AFX_MSG_MAP(RepeatingToolbar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RepeatingToolbar message handlers

void RepeatingToolbar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBCGPToolBar::OnLButtonDown(nFlags, point);

	int iButton = HitTest(point);

	if (iButton < 0)
		return;

	CBCGPToolbarButton* pButton = GetButton(iButton);
	if (pButton == NULL)
	{
		ASSERT (FALSE);
		return;
	}
	
	_commandId = pButton->m_nID;
	_cycles = 0;

	if (_timerId == 0)
	{
		for (int i=0;i<_buttonIds.GetSize();i++)
		{
			if ((_buttonIds.GetAt(i)) == _commandId)
			{
				_timerId = SetTimer(10000, 75, NULL);
				return;
			}
		}
	}   
}


void RepeatingToolbar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (_timerId != 0)
	{
		KillTimer(_timerId);
		_timerId = 0;
	}
	
	CBCGPToolBar::OnLButtonUp(nFlags, point);
}

void RepeatingToolbar::OnTimer(UINT nIDEvent) 
{
	CPoint curPos;
	::GetCursorPos(&curPos);
	::ScreenToClient(m_hWnd, &curPos);
	  
	int iButton = HitTest(curPos);

	if (iButton < 0)
		return;

	CBCGPToolbarButton* pButton = GetButton(iButton);
	if (pButton == NULL)
	{
		ASSERT (FALSE);
		return;
	}
	
	int commandId  = pButton->m_nID;

	if (nIDEvent == _timerId &&
		_commandId == commandId)
	{
		DWORD mousePos = 0;
		CPoint curPos;
		::GetCursorPos(&curPos);
		::ScreenToClient(m_hWnd, &curPos);
		mousePos = MAKEWPARAM(curPos.x, curPos.y);
		::SendMessage(GetParent()->m_hWnd, WM_COMMAND, _commandId, 0);

		//provided an accelleration to the repeat
		if (_cycles > 80)
		{
			KillTimer(nIDEvent);
			_timerId = SetTimer(10000, 5, NULL);
		}
		else if (_cycles > 40)
		{
			KillTimer(nIDEvent);
			_timerId = SetTimer(10000, 15, NULL);
		}
		else if (_cycles > 20)
		{
			KillTimer(nIDEvent);
			_timerId = SetTimer(10000, 35, NULL);
		}
		else if (_cycles > 10)
		{
			KillTimer(nIDEvent);
			_timerId = SetTimer(10000, 55, NULL);
		}
		_cycles++;
	}


	CBCGPToolBar::OnTimer(nIDEvent);
}


void RepeatingToolbar::addRepeatingButtonsIds(CArray<int,int>& buttonIds)
{
	for (int i=0;i<buttonIds.GetSize();i++)
    	_buttonIds.Add(buttonIds.GetAt(i));
}

