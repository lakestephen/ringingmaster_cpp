// PropertyCtrlBar.cpp: implementation of the PropertyCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PropertyCtrlBar.h"
#include "MatrixPropList.h"	
#include "InternetPropList.h"	
#include "CribPropList.h"	
#include "MapPropList.h"	
#include "PropListBase.h"

#include "TouchMainPropList.h"
#include "PropertyCtrlBar.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PropertyCtrlBar::PropertyCtrlBar() :
_touchMain(NULL),
_touchBlueLine(NULL), 
_touchSimulator(NULL),
_matrix(NULL),
_map(NULL),
_crib(NULL)
{
	mainFrame()->addEventListener(this);
}

PropertyCtrlBar::~PropertyCtrlBar()
{
	delete _touchMain;
	_touchMain = NULL;

	delete _touchBlueLine;
	_touchBlueLine = NULL;

	delete _touchSimulator;
	_touchSimulator = NULL;

	delete _matrix;
	_matrix = NULL;

	delete _internet;
	_internet = NULL;

	delete _crib;
	_crib = NULL;

	delete _map;
	_map = NULL;
}
   
IMPLEMENT_DYNAMIC(PropertyCtrlBar, CBCGPDockingControlBar)

BEGIN_MESSAGE_MAP(PropertyCtrlBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(PropertyCtrlBar)
	ON_WM_CREATE()
	ON_WM_SIZE() 
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP	  
	ON_COMMAND(ID_SORT_ALPHA, OnSortingAlpha)
	ON_UPDATE_COMMAND_UI(ID_SORT_ALPHA, OnUpdateSortingAlpha)

	ON_COMMAND(ID_SORT_HIRCH, OnSortingHirch)
	ON_UPDATE_COMMAND_UI(ID_SORT_HIRCH, OnUpdateSortingHirch)
	

END_MESSAGE_MAP()


BOOL PropertyCtrlBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CWnd* wnd = getViewTypeWnd(app()->getViewType());

	if (wnd && wnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

int PropertyCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty ();    


	_defaultToolBar.Create (this, dwDefaultToolbarStyle, IDR_PROP_DEFAULT);
	_defaultToolBar.LoadToolBar (IDR_PROP_DEFAULT, 0, 0, TRUE /* Is locked */);
	_defaultToolBar.SetBarStyle(_defaultToolBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY);
	_defaultToolBar.SetBarStyle (_defaultToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	_defaultToolBar.SetOwner (this);
	_defaultToolBar.SetRouteCommandsViaFrame (FALSE);	

	_touchMainToolBar.Create (this, dwDefaultToolbarStyle, IDR_PROP_TOUCH);
	_touchMainToolBar.LoadToolBar (IDR_PROP_TOUCH, 0, 0, TRUE /* Is locked */);
	_touchMainToolBar.SetBarStyle(_touchMainToolBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY);
	_touchMainToolBar.SetBarStyle (_touchMainToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	_touchMainToolBar.SetOwner (this);
	_touchMainToolBar.SetRouteCommandsViaFrame (FALSE);	

	_recordToolBar.Create (this, dwDefaultToolbarStyle, IDR_PROP_MATRIX);
	_recordToolBar.LoadToolBar (IDR_PROP_MATRIX, 0, 0, TRUE /* Is locked */);
	_recordToolBar.SetBarStyle(_recordToolBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY);
	_recordToolBar.SetBarStyle (_recordToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	_recordToolBar.SetOwner (this);
	_recordToolBar.SetRouteCommandsViaFrame (FALSE);	

	_mapToolBar.Create (this, dwDefaultToolbarStyle, IDR_PROP_MAP);
	_mapToolBar.LoadToolBar (IDR_PROP_MAP, 0, 0, TRUE /* Is locked */);
	_mapToolBar.SetBarStyle(_mapToolBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY);
	_mapToolBar.SetBarStyle (_mapToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	_mapToolBar.SetOwner (this);
	_mapToolBar.SetRouteCommandsViaFrame (FALSE);	

	_touchMain = new TouchMainPropList(vt_touch_main);
	if (!_touchMain->Create (WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_TOUCHMAINPROPLIST))
	{
		TRACE0("Failed to create _touchMain\n");
		return -1;      // fail to create
	}                           
	_touchMain->initPropList ();

	_touchBlueLine = new TouchMainPropList(vt_touch_blue_line);
	if (!_touchBlueLine->Create (WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_TOUCHBLUELINEPROPLIST))
	{
		TRACE0("Failed to create _touchBlueLine\n");
		return -1;      // fail to create
	}                           
	_touchBlueLine->initPropList ();

	_touchSimulator = new TouchMainPropList(vt_touch_simulator);
	if (!_touchSimulator->Create (WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_TOUCHSIMULATORPROPLIST))
	{
		TRACE0("Failed to create _touchSimulator\n");
		return -1;      // fail to create
	}                           
	_touchSimulator->initPropList ();
	
	_matrix = new MatrixPropList();
	if (!_matrix->Create (WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_MATRIXPROPLIST))
	{
		TRACE0("Failed to create _matrix\n");
		return -1;      // fail to create
	}                           
	_matrix->initPropList ();

	_crib = new CribPropList();
	if (!_crib->Create (WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_CRIBPROPLIST))
	{
		TRACE0("Failed to create _crib\n");
		return -1;      // fail to create
	}                           
	_crib->initPropList ();

	_map = new MapPropList();
	if (!_map->Create (WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_MAPPROPLIST)) 
	{
		TRACE0("Failed to create _map\n");
		return -1;      // fail to create
	}                           
	_map->initPropList ();

	_internet = new InternetPropList();
	_internet->Create(this);
	_internet->SetOwner(this);

	AdjustLayout ();
	return 0;
}

void PropertyCtrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void PropertyCtrlBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	for (int i=vt_none;i<=vt_null;i++)
	{
		CWnd* mainPropWnd = getViewTypeWnd((ViewType)i);

		if (mainPropWnd)
		{

			CRect rectClient;
			GetClientRect (rectClient);
			
			int cyTlb = 0;
			CBCGPToolBar* toolbar = getViewTypeToolbar(ViewType(i));

			if (toolbar)
			{
				cyTlb = toolbar->CalcFixedLayout (FALSE, TRUE).cy;

				toolbar->SetWindowPos (NULL,
									rectClient.left, 
									rectClient.top , 
									rectClient.Width (),
									cyTlb,
									SWP_NOACTIVATE | SWP_NOZORDER);
			}

			mainPropWnd->SetWindowPos (NULL,
											rectClient.left + 1, 
											rectClient.top + cyTlb + 1, 
											rectClient.Width () - 2,
											rectClient.Height () - (cyTlb) - 2,
											SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}
}

BOOL PropertyCtrlBar::DestroyWindow() 
{
	mainFrame()->removeEventListener(this);

	return CBCGPDockingControlBar::DestroyWindow();

}

BOOL PropertyCtrlBar::OnEraseBkgnd(CDC* pDC) 
{			  	
	if (GetActiveWindow())
		return TRUE;
	else
		return CBCGPDockingControlBar::OnEraseBkgnd(pDC); 
}

void PropertyCtrlBar::mainFrame_notifyDocViewChanged(ViewType /*oldViewType*/, ViewType newViewType, CDocument* /*oldDoc*/, CDocument* /*newDoc*/)
{
	PropListBase* propList = dynamic_cast<PropListBase*>(getViewTypeWnd(newViewType));

	if (propList)
		propList->loadExpandStatus();

	_touchMainToolBar.ShowWindow(((newViewType == vt_touch_main)||(newViewType == vt_touch_blue_line)||(newViewType == vt_touch_simulator))?SW_SHOW:SW_HIDE);
	_recordToolBar.ShowWindow(((newViewType == vt_tower)||(newViewType == vt_grab))?SW_SHOW:SW_HIDE);
	_mapToolBar.ShowWindow((newViewType == vt_map)?SW_SHOW:SW_HIDE);
	_defaultToolBar.ShowWindow((newViewType == vt_crib)?SW_SHOW:SW_HIDE);

	_touchMain->ShowWindow((newViewType == vt_touch_main)?SW_SHOW:SW_HIDE);
	_touchBlueLine->ShowWindow((newViewType == vt_touch_blue_line)?SW_SHOW:SW_HIDE);
	_touchSimulator->ShowWindow((newViewType == vt_touch_simulator)?SW_SHOW:SW_HIDE);
	_matrix->ShowWindow((newViewType == vt_grab || newViewType == vt_tower)?SW_SHOW:SW_HIDE);
	_internet->ShowWindow((newViewType == vt_internet)?SW_SHOW:SW_HIDE);
	_crib->ShowWindow((newViewType == vt_crib)?SW_SHOW:SW_HIDE);
	_map->ShowWindow((newViewType == vt_map)?SW_SHOW:SW_HIDE);

	CString windowName;
	switch (newViewType)
	{
	case vt_internet:
		windowName = "Favorites";
		break;
	default:
		windowName = "Properties";
		break;
	}
	SetWindowText(windowName);

	AdjustLayout();
			   
}

CBCGPToolBar* PropertyCtrlBar::getViewTypeToolbar(ViewType viewType) 
{
	switch (viewType)
	{
	case vt_touch_blue_line:
	case vt_touch_main:
		return &_touchMainToolBar;
	case vt_tower:
	case vt_grab:
		return &_recordToolBar;
	case vt_map:
		return &_mapToolBar;

	case vt_crib:
	case vt_touch_simulator:
		return &_defaultToolBar;

	default:
		ASSERT(FALSE);
	case vt_internet:
		return NULL;
	}               
}


PropListBase* PropertyCtrlBar::getPropListBase(ViewType viewType) 
{
	CWnd* pWnd = getViewTypeWnd(viewType);
	PropListBase* propList = dynamic_cast<PropListBase*>(pWnd);
	ASSERT(pWnd == propList);
	return propList;
}

CWnd* PropertyCtrlBar::getViewTypeWnd(ViewType viewType) 
{
	switch (viewType)
	{
	case vt_touch_main:
		return _touchMain;
	case vt_touch_blue_line:
		return _touchBlueLine;
	case vt_touch_simulator:
		return _touchSimulator;
	case vt_grab:
	case vt_tower:
		return _matrix;
	case vt_internet:
		return _internet;
	case vt_crib:
		return _crib;
	case vt_map:
		return _map;

	default:
		return NULL;
	}               
}   

void PropertyCtrlBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		   
	CWnd* activeWnd = getViewTypeWnd(app()->getViewType());

	if (activeWnd && ::IsWindow(activeWnd->GetSafeHwnd()))
	{
		CRect rectList;
		activeWnd->GetWindowRect (rectList);
		ScreenToClient (rectList);

		rectList.InflateRect (1, 1);
		dc.Draw3dRect (rectList, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
	}
}    

void PropertyCtrlBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	
	CWnd* wnd = getViewTypeWnd(app()->getViewType());
	if (wnd)
		wnd->SetFocus ();	
}

void PropertyCtrlBar::OnSortingAlpha()
{
	app()->WriteProfileInt("MainFrame", "Property Alpha Mode", TRUE);
	PropListBase* propListBase = getPropListBase(app()->getViewType());
	if (propListBase)
		propListBase->update();
}

void PropertyCtrlBar::OnUpdateSortingAlpha(CCmdUI* pCmdUI)
{ 
	pCmdUI->SetCheck (app()->GetProfileInt("MainFrame", "Property Alpha Mode", FALSE) == TRUE);
}

void PropertyCtrlBar::OnSortingHirch()
{
	app()->WriteProfileInt("MainFrame", "Property Alpha Mode", FALSE);
	PropListBase* propListBase = getPropListBase(app()->getViewType());
	if (propListBase)
		propListBase->update();
}

void PropertyCtrlBar::OnUpdateSortingHirch(CCmdUI* pCmdUI)
{  
	pCmdUI->SetCheck (app()->GetProfileInt("MainFrame", "Property Alpha Mode", FALSE) == FALSE);
}


