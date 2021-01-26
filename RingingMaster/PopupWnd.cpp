// PopupWnd.cpp: implementation of the PopupWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PopupWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(PopupWnd, AutoPopup)
	//{{AFX_MSG_MAP(PopupWnd)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

PopupWnd::PopupWnd() 
{
}

PopupWnd::~PopupWnd()
{
}


BOOL PopupWnd::Create(CWnd* pParentWnd) 
{
	BOOL res = AutoPopup::Create(pParentWnd);

	if (res)
	{
		_listBox.Create(this);
	}

    return res;
}

void PopupWnd::Set(CPoint point,  StringColors* strArr, int selection)
{																		
	//add the passed strings
	for (int i=0;i<strArr->GetSize();i++)
	{
		_listBox.AddString(strArr->GetAt(i)->getString(),
							strArr->GetAt(i)->getColor()  );
	}	

	_listBox.SetCurSel(selection);

	
	// determine the window size and position
	// displayed above and centered on the origin
	CRect wndRect(point.x, 
			point.y, 
			point.x + 155,
			point.y + 115); 
			
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

	_listBox.SetFocus();
}


const int border = 1;

void PopupWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	


	if (IsWindow(_listBox.m_hWnd))
		_listBox.SetWindowPos (	NULL,
								border , 
								border, 
								cx-(2*border), 
								cy-(2*border),
				 SWP_NOACTIVATE | SWP_NOZORDER);  
		
}


void PopupWnd::notifyParent(CWnd *parent)
{
	::PostMessage(parent->m_hWnd, WM_POPUP_LIST_INDEX, _listBox.GetCurSel(),0);
}



