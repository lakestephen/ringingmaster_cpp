// ThreeDimEditItem.cpp: implementation of the ThreeDimEditItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "ThreeDimEditItem.h"
#include "ThreeDimEditCollection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ThreeDimEditItem::ThreeDimEditItem(ThreeDimEditCollection& threeDimEditCollection, int id, double x, double y, int imageIndex, bool selectable, CString tooltip, CString text) :
_x(x),
_y(y),
_threeDimEditCollection(threeDimEditCollection),
_imageIndex(imageIndex),
_text(text),
_selectable(selectable),
_tooltip(tooltip),
_id(id)
{

}

ThreeDimEditItem::~ThreeDimEditItem()
{

}


void ThreeDimEditItem::draw(CDC *pDC, bool selected)
{
	int oldBKMode = pDC->SetBkMode(TRANSPARENT);

	int halfIconSize = _threeDimEditCollection.getIconSize() / 2;

	_threeDimEditCollection._images.Draw(pDC, _imageIndex, 
		CPoint(getItemPosition() - CPoint(halfIconSize,halfIconSize)), selected?ILD_SELECTED:ILD_NORMAL);
	
	
	CRect rect (getItemPosition() + CPoint(halfIconSize, -(halfIconSize+1)), CSize(100,100) );
	pDC->DrawText(_text, rect, DT_SINGLELINE|DT_TOP);

	 pDC->SetBkMode(oldBKMode);
}



CPoint ThreeDimEditItem::getItemPosition()
{
	return _threeDimEditCollection.getOriginPixels() +
			CPoint(_threeDimEditCollection.convertMetersToPixels(_x),
				   _threeDimEditCollection.convertMetersToPixels(_y));
}

void ThreeDimEditItem::setItemPosition(CPoint point)
{
	point -= _threeDimEditCollection.getOriginPixels();

	_x = _threeDimEditCollection.convertPixelsToMeters(point.x);
	_y = _threeDimEditCollection.convertPixelsToMeters(point.y);
}

bool ThreeDimEditItem::isSelectable()
{
	return _selectable;
}

double ThreeDimEditItem::getXMeters()
{
	return _x;
}

double ThreeDimEditItem::getYMeters()
{
	return _y;
}

CString& ThreeDimEditItem::getTooltip()
{
	return _tooltip;
}

int ThreeDimEditItem::getID()
{
	return _id;
}
