// TouchSimulatorView.cpp : implementation of the TouchSimulatorView class
//

#include "stdafx.h"
#include "RingingMaster.h"

#include "TouchDoc.h"
#include "TouchSimulatorView.h"
#include "TouchView.h"
#include "TouchBlueLineView.h"
#include "UserMessages.h"
#include "MethodPosition.h"
#include "MemDC.h"
#include "SimulatorManager.h"

#include ".\touchsimulatorview.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchSimulatorView

IMPLEMENT_DYNCREATE(TouchSimulatorView, CView)

BEGIN_MESSAGE_MAP(TouchSimulatorView, CView)
	//{{AFX_MSG_MAP(TouchSimulatorView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_STRIKING, OnStriking)
	ON_UPDATE_COMMAND_UI(ID_STRIKING, OnUpdateStriking)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TouchSimulatorView construction/destruction

TouchSimulatorView::TouchSimulatorView() 
{
}

TouchSimulatorView::~TouchSimulatorView()
{
}

BOOL TouchSimulatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// TouchSimulatorView drawing

void TouchSimulatorView::OnDraw(CDC* pDC)
{
	MemDC memdc(pDC);

	CRect rect;
	GetClientRect(rect);	
	memdc->FillSolidRect(&rect, GetSysColor(COLOR_WINDOW));  
	memdc->SetMapMode(MM_TEXT);
	memdc->SetBkMode(TRANSPARENT);
	memdc->SetTextColor(RGB(0,0,0));


	CString msg;
	if (simulatorManager()->isValid(msg, GetDocument()))
	{
		for (int i=0;i<simulatorManager()->getNumber();i++)
		{										 
			simulatorManager()->drawBell(i, &memdc, GetDocument());
		} 
	}
	else
	{
        memdc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		CFont* oldFont = (CFont*)memdc.SelectObject(&app()->_fontStd);

		rect.DeflateRect(20, 20);
		memdc->DrawText(msg, rect, DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP );

		memdc->SelectObject(oldFont);
	}

}

/////////////////////////////////////////////////////////////////////////////
// TouchSimulatorView printing
/////////////////////////////////////////////////////////////////////////////
// TouchSimulatorView diagnostics

#ifdef _DEBUG
void TouchSimulatorView::AssertValid() const
{
	CView::AssertValid();
}

void TouchSimulatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

TouchDoc* TouchSimulatorView::GetDocument() // non-debug version is inline
{
	if (m_pDocument == NULL)
		return NULL;

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(TouchDoc)));
	return (TouchDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TouchSimulatorView message handlers

void TouchSimulatorView::OnSize(UINT nType, int cx, int cy) 
{
	simulatorManager()->calculateBellBoxes(cx, cy, GetDocument());
	Invalidate();

	CView::OnSize(nType, cx, cy);
}

void TouchSimulatorView::simulatorManager_notifyProfileChange()
{
	updateSimulatorGUI();
}

BOOL TouchSimulatorView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void TouchSimulatorView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
}

void TouchSimulatorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	GetDocument()->setTab(2);
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}



void TouchSimulatorView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CPoint clientPoint(point);
	ScreenToClient(&clientPoint);
	_mouseDownBell = simulatorManager()->hitTestBell(clientPoint);
	theApp.ShowPopupMenu(IDR_SIMULATOR, point, this);	
}

void TouchSimulatorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//hit test 
	int bell = simulatorManager()->hitTestBell(point);


	//if valid, then set.
	if (bell >=0)
	{
		simulatorManager()->setFirstSimulatedBell(bell);
	}

	CView::OnLButtonDown(nFlags, point);
}


void TouchSimulatorView::simulatorManager_notifyBellStateChangeFromSeperateThread(int bell)
{
	//get the rect for the bell
	if (bell == -1)
		Invalidate();
	else
		InvalidateRect(simulatorManager()->getGUIBell(bell).getBellRect());
}

void TouchSimulatorView::simulatorManager_notifySimulatorConnected(bool /*connected*/)
{
	updateSimulatorGUI();
}

void TouchSimulatorView::updateSimulatorGUI()
{
	CRect rect;
	GetClientRect(&rect);
	simulatorManager()->calculateBellBoxes(rect.Width(), rect.Height(), GetDocument());
	Invalidate();
}

int TouchSimulatorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	simulatorManager()->addEventListener(this);

	return 0;
}

void TouchSimulatorView::OnDestroy()
{
	simulatorManager()->removeEventListener(this);

	__super::OnDestroy();

}

void TouchSimulatorView::OnStriking()
{
	simulatorManager()->editStrikingControls(_mouseDownBell); 
}

void TouchSimulatorView::OnUpdateStriking(CCmdUI *pCmdUI)
{
	CString msg;
	pCmdUI->Enable(simulatorManager()->isValid(msg, GetDocument()));
}
