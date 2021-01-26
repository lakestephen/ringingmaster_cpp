// AutoPopup.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AutoPopup.h"
#include "UserMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AutoPopup

// class data members
BOOL  AutoPopup::_registered = FALSE;
AutoPopup* AutoPopup::_autoPopup = NULL;

AutoPopup::AutoPopup():
_notifyParentDone(FALSE)
{
    RegisterWnd( );		

	//use this to delete all auto popup derived classes
	CleanUp();
	_autoPopup = this;
}



void AutoPopup::CleanUp()
{
	if (_autoPopup)
	{
		if (::IsWindow(_autoPopup->m_hWnd))
			_autoPopup->KillTimer(AAA_KILL_FOCUS_TIMER);
		_autoPopup->DestroyWindow();
	}
	delete _autoPopup;
	_autoPopup = NULL;
}



AutoPopup::~AutoPopup()
{
}


BEGIN_MESSAGE_MAP(AutoPopup, CWnd)
	//{{AFX_MSG_MAP(AutoPopup)
		ON_WM_KILLFOCUS()
		ON_MESSAGE(WM_KILL_POPUP, OnKillPopup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// AutoPopup message handlers
void AutoPopup::RegisterWnd( )
{
    // check for prior registration
    if (_registered) return;

    // initialise the basic information before registration
    HINSTANCE hInst = AfxGetInstanceHandle( );

    // initialise the window class information
	WNDCLASS wndcls;
	wndcls.style         = CS_SAVEBITS | CS_DBLCLKS;
    wndcls.lpfnWndProc   = ::DefWindowProc;
	wndcls.cbClsExtra    = 0;
    wndcls.cbWndExtra    = 0;
	wndcls.hInstance     = hInst;
	wndcls.hIcon         = NULL;
    wndcls.hCursor       = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = HBRUSH(COLOR_BTNFACE+1);
	wndcls.lpszMenuName  = NULL;
	wndcls.lpszClassName = "AutoPopup";

    // register the window class
    if (!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();

    _registered = TRUE;
}

BOOL AutoPopup::Create(CWnd* pParentWnd) 
{
    _parent = pParentWnd;
	return CWnd::CreateEx(WS_EX_LEFT | WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME,
                             "AutoPopup", 
                             NULL,
                             WS_POPUP |  WS_CLIPSIBLINGS, 
                             0, 0, 0, 0,
                             pParentWnd->m_hWnd, 
                             NULL);
}

void AutoPopup::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

	if ((IsWindow(m_hWnd))&&(pNewWnd != this)&&(!IsChild(pNewWnd)))
	{
		PostMessage(WM_KILL_POPUP);
	}
}

LRESULT AutoPopup::OnKillPopup(WPARAM /*wParam*/, LPARAM /*lParam*/)
{			 
	//TRACE("Kill Popup\r\n");
	
	if (!_notifyParentDone)
		notifyParent(_parent);
	_notifyParentDone = true;
	
	if (::IsWindow(m_hWnd))
		_autoPopup->KillTimer(AAA_KILL_FOCUS_TIMER);

	ShowWindow(SW_HIDE);

	return 0;
}

void AutoPopup::notifyParent(CWnd* /*parent*/)
{
	//implement some final processing in the derived class 
	//called imediatly prior to killing the window
}



