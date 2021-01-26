#include "stdafx.h"
#include "RingingMaster.h"
#include "resource.h"
#include "DataTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// define the offset above cursor
static const CPoint _defOffset(4, 20);

// border area around the text
static const short _border = 2;

// define the active border for data tips
static const short _horizontal  = 2;
static const short _vertical    = 2;

// class data members
BOOL  DataTip::_registered = FALSE;
short DataTip::_onDelay = 500;
short DataTip::_offDelay = 6000;
short DataTip::_count = 0;

// GDI objects used for drawing operations in module
static CFont   _font;
static CBrush* _brush = NULL;

// hook information
HHOOK DataTip::_hookProc = NULL;
DataTip* DataTip::_current = NULL;


// special destructor object for clearing up GDI objects
struct _brush_destructor
{
    ~_brush_destructor( )
    {
        delete _brush;
        _brush = NULL;
    }
};
static const _brush_destructor _destroyer;

//This method registers the window class used by the DataTip windows. 
//This must be called after the class background brush has been constructed.
void DataTip::RegisterWnd( )
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
    wndcls.hCursor       = app()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = *_brush;
	wndcls.lpszMenuName  = NULL;
	wndcls.lpszClassName = "DataTip";

    // register the window class
    if (!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();

    _registered = TRUE;
}


//This method initialise the DataTip class. It creates the default font used 
//for DataTips, the brush used to paint windows background, and registers 
//the window class. 
//If there is currently no hook procedure instaklled for the class it alos 
//installs a keyboard hook procedure.
void DataTip::Initialise( )
{
    if (_font.GetSafeHandle( ) == NULL)
    {
        // create the default tip font
        LOGFONT lFont;
        GetObject(GetStockObject(ANSI_VAR_FONT), sizeof lFont, &lFont);
        _font.CreateFontIndirect(&lFont);

        // create the other GDI objects
        _brush = new CBrush(::GetSysColor(COLOR_INFOBK));
    }

    // register the window class
    RegisterWnd( );

    // install the keyboard hook procedure
    if (_hookProc == NULL)
    {
        _hookProc = ::SetWindowsHookEx(WH_KEYBOARD, 
                                       (HOOKPROC)KeyboardHookCallback,
                                       NULL,
                                       ::GetCurrentThreadId( ));
    }
}

//This method provides the keyboard hook callback procedure.If there is 
//currently an active DataTip then when a key is pressed it will hide the 
//tip, before passing on the message to the next procedure in the hook chain.
LRESULT CALLBACK DataTip::KeyboardHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    // if keypress has occured then hide any visible data tips
    if (code >= 0)
    {
        if (_current != NULL)
        {
            _current->Hide( );
        }
    }

    // chain to the next hook procedure
    return ::CallNextHookEx(_hookProc, code, wParam, lParam);
}


//This method contructs a DataTip object. IT calls the class intialisation 
//procedure before initialising the object data members. Finally it increments 
//the object count for the class.
DataTip::DataTip()
{
    Initialise( );
    _ready  = FALSE;
    _origin = CPoint(0, 0);
    _offset = _defOffset;

    // incrment the object count
    _count++;
}

//This method destructs the DataTip object, decrementing the class object count.
//If this is the last object to be destroyed then the hook procedure is removed.
DataTip::~DataTip()
{
    _count--;
    if ( (_count == 0) && (_hookProc != NULL) )
    {
        ::UnhookWindowsHookEx(_hookProc);
        _hookProc = NULL;
    }
}

//This method creates the DataTip window. This is an instantiation of the 
//pre-registered DataTip window class. If the creation is successful then 
//the method returns TRUE.
BOOL DataTip::Create(CWnd* pParentWnd) 
{
    _parent = pParentWnd;
	_ready = CWnd::CreateEx(WS_EX_LEFT | WS_EX_TOOLWINDOW,
                             "DataTip", 
                             NULL,
                             WS_POPUP | WS_BORDER | WS_CLIPSIBLINGS, 
                             0, 0, 10, 10,
                             pParentWnd->m_hWnd, 
                             NULL);

    return _ready;
}

//This method hides the DataTip window, killing the associated timer, and 
//releasing the mouse capture. The class active pointer is set to NULL.
void DataTip::Hide( )
{
	_current = NULL;

	if ((IsWindow(m_hWnd))&&(_ready))
	{
		ShowWindow(SW_HIDE);
		KillTimer(AAA_TIMER_DATA_ON);
		KillTimer(AAA_TIMER_DATA_OFF);
		::ReleaseCapture( );	   
	}
}

//This method sets the values to be used for a DataTip, at the same restarting 
//the delay timer if the provided location is within the parent window's borders.
void DataTip::Set(CPoint point)
{
    // check whether tips are turned on
    if (_strings.GetSize() < 1) return;

    // determine whether the point is in the parent window
    CRect rect;
    _parent->GetClientRect(&rect);
    if (rect.PtInRect(point))
    {
        // change data tip if position has changed
        _parent->ClientToScreen(&point);
        if (point != _origin)
        {
            _origin = point;

            // re-start the timer
            KillTimer(AAA_TIMER_DATA_ON);
            KillTimer(AAA_TIMER_DATA_OFF);
            SetTimer(AAA_TIMER_DATA_ON, _onDelay, NULL);
//			TRACE("Timer SET\n");
        }
    }
}
    
//This method displays the DataTip window. If there is a currently active 
//DataTip this is hidden before the tip size is calculated and the tip 
//positioned at the specified offset from the mouse pointer location.
void DataTip::Display( )
{
    if (!_ready) return;
	if (_strings.GetSize() < 1) return;

    // hide any other currently visible data tips
    if (_current != NULL)
    {
        _current->Hide( );
    }

    // determine the size of the data tip text 
    // in order to size the window correctly
    CClientDC dc(this);
    dc.SelectObject(&_font);
    CSize size(0,0);
	for (int i=0;i<_strings.GetSize();i++)
	{
		CSize thisSize = dc.GetTextExtent(_strings[i]);
		size.cy += thisSize.cy;
		if (size.cx < thisSize.cx) size.cx = thisSize.cx;
	}

    // capture the mouse in order to be able to 
    // determine when to hide the window
    SetCapture( );

    // determine the window size and position
    // displayed above and centered on the origin
    CRect wndRect(_origin.x, 
                  _origin.y, 
                  _origin.x + size.cx,
                  _origin.y + size.cy); 

    // adjust window for offset and borders
    wndRect.InflateRect(2 * _border, _border);
    wndRect.OffsetRect(_offset);
	
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

    // define the active area around the cursor position
    _captureRect = CRect(_origin.x - _horizontal, 
                          _origin.y - _vertical,
                          _origin.x + _horizontal,
                          _origin.y + _vertical);

    // set the current data tip window
    _current = this;
}


//This method ensures that when the window is destroyed the timer goes also
BOOL DataTip::DestroyWindow() 
{
	if (::IsWindow(m_hWnd))
	{
		KillTimer(AAA_TIMER_DATA_ON);
		KillTimer(AAA_TIMER_DATA_OFF);
	}
	return CWnd::DestroyWindow();
}

//This method is called by MFC giving the tip chance to 
//hide itself if the user presses one of the mouse buttons.
BOOL DataTip::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
    {
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_CANCELMODE:
            Hide( );
            break;
    }

	return CWnd::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(DataTip, CWnd)
	//{{AFX_MSG_MAP(DataTip)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// This method handles the WM_PAINT message. It paints the tip text into the 
//tip window area.
void DataTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    CRect rect;
    GetClientRect(&rect);

    // initialise the device context for drawing
    CFont* pOldFont = dc.SelectObject(&_font);
    dc.SetBkColor(::GetSysColor(COLOR_INFOBK));
    dc.SetTextColor(::GetSysColor(COLOR_INFOTEXT));

    // draw the data tip
    dc.SetTextAlign(TA_TOP | TA_LEFT);
    
	ASSERT(_strings.GetSize() > 0);	  //problem with tip text setting 
	int y = (_border / 2);
	for (int i=0;i<_strings.GetSize();i++)
	{
		dc.TextOut(_border, y , _strings[i]);
		y += dc.GetTextExtent(_strings[i]).cy;
	}
	

    // restore the device context
    dc.SelectObject(pOldFont);
}

//This method determines whether the mouse has moved outside
//of the capture rectangle and if so hides the tip window.
void DataTip::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
    // determine if a mouse button has been pressed or
    // we have moved outside of the active capture area
    ClientToScreen(&point);
    if (!_captureRect.PtInRect(point))
    {
        Hide( );
    }
}

//This method handles the WM_TIMER message. Provided the mouse has not moved 
//from its original location set when the tip was initialised then the tip is 
//displayed at this location.
void DataTip::OnTimer(UINT nIDEvent) 
{
    if (nIDEvent == AAA_TIMER_DATA_ON)
	{
	    // make sure that cursor hasn't moved before displaying the data tip
		CPoint point;
		if (::GetCursorPos(&point))
		{
			if (point == _origin)
			{
				Display( );
			}
		}

		KillTimer(AAA_TIMER_DATA_ON);
		SetTimer(AAA_TIMER_DATA_OFF, _offDelay, NULL);
	}
    else if (nIDEvent == AAA_TIMER_DATA_OFF)
	{
		KillTimer(AAA_TIMER_DATA_ON);
		KillTimer(AAA_TIMER_DATA_OFF);
		Hide();
	}

}			

void DataTip::addString(CString &str)
{
	if (str.GetLength() >0)
		_strings.Add(str);
}

void DataTip::removeAllStrings()
{
	_strings.RemoveAll();
}
