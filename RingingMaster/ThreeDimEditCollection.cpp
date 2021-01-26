// ThreeDimEditCollection.cpp: implementation of the ThreeDimEditCollection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "ThreeDimEditCollection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



ThreeDimEditCollection::ThreeDimEditCollection(UINT imagesID, int iconSize):
_pixelsPerMeter(25),
_linePitchMeters(1),
_lineColor(RGB(235,235,245)),
_originXMeters(0),
_originYMeters(0), 
_iconSize(iconSize)
{
	VERIFY(_images.Create(imagesID, iconSize,1,RGB(255, 0, 255)));
}

ThreeDimEditCollection::~ThreeDimEditCollection()
{
	removeAllItems();
}

COLORREF ThreeDimEditCollection::getLineColor()
{
	return _lineColor;
}  


CPoint ThreeDimEditCollection::getOriginPixels()
{
 	return CPoint ( -(convertMetersToPixels(_originXMeters)),
 					-(convertMetersToPixels(_originYMeters)));
}
				 
double ThreeDimEditCollection::getLinePitchMeters()
{
	return _linePitchMeters;
}

int ThreeDimEditCollection::convertMetersToPixels(double meters)
{
	return (int)(meters * (double)_pixelsPerMeter);
}

double ThreeDimEditCollection::convertPixelsToMeters(int pixels)
{
	return ((double)pixels / (double)_pixelsPerMeter);
}

void ThreeDimEditCollection::addItem(ThreeDimEditItem *item)
{
	_items.Add(item);
}

int ThreeDimEditCollection::getItemCount()
{
	return _items.GetSize();
}

ThreeDimEditItem* ThreeDimEditCollection::getItemUsingID(int id)
{
	for (int i=0;i<_items.GetSize();i++)
	{
		if (_items.GetAt(i)->getID() == id)
			return _items.GetAt(i);
	}
	
	return NULL;
}

ThreeDimEditItem* ThreeDimEditCollection::getItem(int index)
{
	ASSERT(index >= 0 && index <_items.GetSize());

	if (index >= 0 && index <_items.GetSize())
		return _items.GetAt(index);
	else
		return NULL;
}		

int ThreeDimEditCollection::getIconSize()
{
	return _iconSize;
}

void ThreeDimEditCollection::setupPosition(CPoint windowWidth, double xMeters, double yMeters)
{
	_originXMeters = -(convertPixelsToMeters(windowWidth.x)/2 - xMeters);
	_originYMeters = -(convertPixelsToMeters(windowWidth.y)/2 - yMeters);
}

bool ThreeDimEditCollection::setupPositionWouldMove(CPoint windowWidth, double xMeters, double yMeters)
{
	if (_originXMeters != -(convertPixelsToMeters(windowWidth.x)/2 - xMeters))
		return true;
	if (_originYMeters != -(convertPixelsToMeters(windowWidth.y)/2 - yMeters))
		return true;
	
	return false;
}

void ThreeDimEditCollection::moveOrigPixels(CPoint point)
{
	_originXMeters += convertPixelsToMeters(point.x);
	_originYMeters += convertPixelsToMeters(point.y);
}

void ThreeDimEditCollection::removeAllItems()
{
	for (int i=0;i<_items.GetSize();i++)
		delete _items.GetAt(i);
	_items.RemoveAll();
}

void ThreeDimEditCollection::setZoom(int pixelsPerMeter, double linePitchMeters)
{
	_pixelsPerMeter	= pixelsPerMeter;
	_linePitchMeters = linePitchMeters;
}
