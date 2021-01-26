// TouchHoldView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchHoldView.h"
#include "TouchDoc.h"
#include "resource.h"

#include "TouchView.h"
#include "TouchBlueLineView.h"
#include "TouchSimulatorView.h"

#include "SimulatorManager.h"
#include ".\touchholdview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchHoldView

IMPLEMENT_DYNCREATE(TouchHoldView, CBCGPTabView)

TouchHoldView::TouchHoldView()
{
}

TouchHoldView::~TouchHoldView()
{
}


BEGIN_MESSAGE_MAP(TouchHoldView, CBCGPTabView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(TouchHoldView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TouchHoldView drawing

void TouchHoldView::OnInitialUpdate()
{
	CBCGPTabView::OnInitialUpdate();

	SetActiveView(1);
	SetActiveView(0);
	SetActiveView(GetDocument()->getTab());
}

void TouchHoldView::OnDraw(CDC* /*pDC*/)
{

}
/////////////////////////////////////////////////////////////////////////////
// TouchHoldView diagnostics

#ifdef _DEBUG
void TouchHoldView::AssertValid() const
{
	CBCGPTabView::AssertValid();
}

void TouchHoldView::Dump(CDumpContext& dc) const
{
	CBCGPTabView::Dump(dc);
}

TouchDoc* TouchHoldView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(TouchDoc)));
	return (TouchDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TouchHoldView message handlers

 
int TouchHoldView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddView (RUNTIME_CLASS (TouchView), _T("Touch"));
	AddView (RUNTIME_CLASS (TouchBlueLineView), _T("Blue Line"));
	if (simulatorManager()->isSimulatorAvailable()&& simulatorManager()->anyInputsAvailable())
		AddView (RUNTIME_CLASS (TouchSimulatorView), _T("Simulator"));	

	///make sure we have at least one change of tab to get the 
	//menu enabling working ????? - see OnInitialUpdate()
	SetActiveView(1);
	SetActiveView(0); 

	return 0;
}

BOOL TouchHoldView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;	
}

BOOL TouchHoldView::DestroyWindow()
{
	return CBCGPTabView::DestroyWindow();
}
