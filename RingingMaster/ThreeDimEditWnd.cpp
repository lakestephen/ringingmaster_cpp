// ThreeDimEditWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "ThreeDimEditWnd.h"
#include "UserMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ThreeDimEditWnd

ThreeDimEditWnd::ThreeDimEditWnd() :
_selectedItem(-1),
_draggingItem(false),
_draggingBackground(false)
{
}

ThreeDimEditWnd::~ThreeDimEditWnd()
{
	_tip.DestroyWindow();
}


BEGIN_MESSAGE_MAP(ThreeDimEditWnd, ScrollWnd)
	//{{AFX_MSG_MAP(ThreeDimEditWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ThreeDimEditWnd message handlers

BOOL ThreeDimEditWnd::Create(CWnd *pParentWnd, CRect rect, ThreeDimEditCollection* threeDimEditCollection)
{
	_threeDimEditCollection = threeDimEditCollection;
	BOOL ret = ScrollWnd::Create(WS_EX_CLIENTEDGE, "3Dim Wnd", WS_TABSTOP, rect,		
									pParentWnd, IDC_3D_WND);

	_tip.Create(this);
	return ret;	

}

void ThreeDimEditWnd::OnDraw(CDC* pDC)
{
	drawGrid(pDC);	
	drawItems(pDC);	
}

void ThreeDimEditWnd::drawItems(CDC *pDC )
{
	CFont* oldFont = pDC->SelectObject(&app()->_fontStd);

	for (int i=_threeDimEditCollection->getItemCount()-1;i>=0;i--)
	{
		ThreeDimEditItem* item = _threeDimEditCollection->getItem(i);

		item->draw(pDC, (i==_selectedItem)?true:false);
	}

	pDC->SelectObject(oldFont);
}

void ThreeDimEditWnd::drawGrid(CDC *pDC)
{	   
	CPen pen(PS_SOLID, 2, _threeDimEditCollection->getLineColor());
	CPen* oldPen = pDC->SelectObject(&pen);
	
	CPoint origin = _threeDimEditCollection->getOriginPixels();

	//line down the middle
	//find the middle of the axis
	pDC->MoveTo(origin.x,0);
	pDC->LineTo(origin.x,1000);
	
	pDC->MoveTo(0,origin.y);
	pDC->LineTo(1000,origin.y);


	//the other lines
	CPen pen2(PS_SOLID, 1, _threeDimEditCollection->getLineColor());
	pDC->SelectObject(&pen2);


	for (double i=0;i<100;i += _threeDimEditCollection->getLinePitchMeters())
	{
		int pos = _threeDimEditCollection->convertMetersToPixels(i);

		//horisontal lines
		//positive lines
		pDC->MoveTo(0, origin.y + pos);
		pDC->LineTo(1000,origin.y + pos);
		//negative lines
		pDC->MoveTo(0, origin.y - pos);
		pDC->LineTo(1000,origin.y - pos);

		//vertical lines
		//positive lines
		pDC->MoveTo(origin.x + pos, 0);
		pDC->LineTo(origin.x + pos, 1000);
		//negative lines
		pDC->MoveTo(origin.x - pos,0);
		pDC->LineTo(origin.x - pos, 1000);
	}	

	pDC->SelectObject(oldPen);
}



void ThreeDimEditWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//attempt to find an item
	int selectedItem = hitTest(point);
	if ((selectedItem != -1)&&
		(_threeDimEditCollection->getItem(selectedItem))&&
		(_threeDimEditCollection->getItem(selectedItem)->isSelectable()))
	{
		_draggingItem = true;
		_selectedItem = selectedItem;
	}
	else
	{
		_draggingBackground = true;
		SetCursor(app()->LoadCursor(IDC_FPOINT));
		_lastBackgroundPos = point;
	}

	SetCapture();

	CRect windowRect;
	GetWindowRect(windowRect);
	//do not let mouse leave the list box boundary
	::ClipCursor(windowRect);

	Invalidate();
	
	ScrollWnd::OnLButtonDown(nFlags, point);
}

void ThreeDimEditWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (_draggingItem)
	{
		ThreeDimEditItem* item = _threeDimEditCollection->getItem(_selectedItem);
		if (item)
			item->setItemPosition(point);
		Invalidate();
	}
	else if (_draggingBackground)
	{
		CPoint backgroundDiff = _lastBackgroundPos - point;
		_lastBackgroundPos = point;
		_threeDimEditCollection->moveOrigPixels(backgroundDiff);
		Invalidate();
	}	
	else
	{
		_tip.removeAllStrings();
		int selectedItem = hitTest(point + GetScrollPosition() );
		if ((selectedItem != -1)&&
			(_threeDimEditCollection->getItem(selectedItem)))
		{	
			_tip.addString(_threeDimEditCollection->getItem(selectedItem)->getTooltip());
			_tip.Set(point);
		}
	}
	
	CString* strStatus = new CString;
	CPoint pos = point - _threeDimEditCollection->getOriginPixels();
	double x = _threeDimEditCollection->convertPixelsToMeters(pos.x) ;
	double y = _threeDimEditCollection->convertPixelsToMeters(pos.y);

	strStatus->Format("%s %.2fM, %s %.2fM", ((x<0)?"Left":"Right"), fabs(x),((y<0)?"Front":"Back"), fabs(y));

	GetParent()->PostMessage(THREE_D_EDIT_UPDATE_STATUS, 0, (long)strStatus);


	ScrollWnd::OnMouseMove(nFlags, point);
}


void ThreeDimEditWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (_draggingItem)
		GetParent()->PostMessage(THREE_D_EDIT_ITEM_MOVED, 0, 0);

	_draggingItem = false;
	_draggingBackground = false;

	//if mouse was captured then release it
	if (GetCapture()==this)
		::ReleaseCapture();

	::ClipCursor(NULL);

	Invalidate();
	
	ScrollWnd::OnLButtonUp(nFlags, point);
}

//returns the integer of the item selected.
int ThreeDimEditWnd::hitTest(CPoint point)
{
	int index = -1;
	double dDiffMin = 8;

	for (int i=0;i<_threeDimEditCollection->getItemCount();i++)
	{

		CPoint diff = _threeDimEditCollection->getItem(i)->getItemPosition() - point;

		double dDiff = sqrt((double)((diff.x*diff.x)+(diff.y*diff.y)));

		if (dDiff < dDiffMin) 
		{
			dDiffMin = dDiff;
			index = i;
		}		
	}
	
	return index;
}


BOOL ThreeDimEditWnd::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	point += GetScrollPosition();

	int selectedItem = hitTest(point);

	if ((_draggingBackground)||(_draggingItem))
	{
		//do nothing
	}
	else if ((selectedItem != -1)&&
		(_threeDimEditCollection->getItem(selectedItem))&&
		(_threeDimEditCollection->getItem(selectedItem)->isSelectable()))
	{
		SetCursor(app()->LoadStandardCursor(IDC_CROSS));
	}
	else 
	{
		SetCursor(app()->LoadStandardCursor(IDC_ARROW));
	}


	return TRUE;
	
//	return ScrollWnd::OnSetCursor(pWnd, nHitTest, message);
}
