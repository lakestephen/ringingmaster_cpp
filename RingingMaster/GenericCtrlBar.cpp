// GenericCtrlBar.cpp: implementation of the GenericCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "GenericCtrlBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GenericCtrlBar::GenericCtrlBar() :
_child(NULL)
{
}

GenericCtrlBar::~GenericCtrlBar()
{
	delete _child;
	_child = NULL;
}
   
IMPLEMENT_DYNAMIC(GenericCtrlBar, CBCGPDockingControlBar)

BEGIN_MESSAGE_MAP(GenericCtrlBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(GenericCtrlBar)
	ON_WM_CREATE()
	ON_WM_SIZE() 
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()

int GenericCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	AdjustLayout();

	return 0;
}

void GenericCtrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();

}

void GenericCtrlBar::AdjustLayout ()
{
	if ((GetSafeHwnd () == NULL)||(_child == NULL)||(!::IsWindow(_child->GetSafeHwnd())))
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);
	rectClient.NormalizeRect();


	_child->SetWindowPos (NULL,
							rectClient.left , 
							rectClient.top  , 
							rectClient.Width(),
							rectClient.Height(),
							SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL GenericCtrlBar::DestroyWindow() 
{
	//these windows are automatically destroyed by the tab control
	_child->DestroyWindow();

	   
	return CBCGPDockingControlBar::DestroyWindow();
}


BOOL GenericCtrlBar::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}


void GenericCtrlBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
		/*   
	if (_child && ::IsWindow(_child->GetSafeHwnd()))
	{
		CRect rectList;
		_child->GetWindowRect (rectList);
		ScreenToClient (rectList);

		rectList.InflateRect (1, 1);
		dc.Draw3dRect (rectList, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
	}*/
}

//we take ownership of this, and are responsable for deleting it.
BOOL GenericCtrlBar::setChild(CWnd* child)
{
	ASSERT(child != NULL);
	_child = child;

	AdjustLayout();

	return TRUE;
}
