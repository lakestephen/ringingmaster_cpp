// MapFragment.cpp: implementation of the MapFragment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MapFragment.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int MapFragment::_NextRequestId = 1;

MapFragment::MapFragment() :
_mapImage(NULL), 
_requestId(getNextRequestId())
{

}

int MapFragment::getNextRequestId()
{
	return _NextRequestId++;
}

MapFragment::MapFragment(MapFragment &mapFragment) :
_mapGridRef(mapFragment._mapGridRef),
_mapImage(NULL), 
_requestId(mapFragment._requestId)
{
	if (mapFragment._mapImage)
		_mapImage = new CxImage(*mapFragment._mapImage);
}

MapFragment::~MapFragment()
{
	delete _mapImage;
}

void MapFragment::setMapGridRef(MapGridRef mapGridRef)
{
	_mapGridRef = mapGridRef;
}

MapGridRef MapFragment::getMapGridRef()
{
	return _mapGridRef;
}


CxImage* MapFragment::getMapImage()
{
	return _mapImage;
}

void MapFragment::setMapImage(CxImage *mapImage)
{
	_mapImage = mapImage;
}

bool MapFragment::isValid()
{
	return (_mapImage != NULL);
}


bool MapFragment::isMatchingRequestId(MapFragment &mapFragment)
{
	return (mapFragment._requestId == _requestId);
}
