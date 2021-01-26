// MapFragment.h: interface for the MapFragment class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "MapGridRef.h"

class CxImage;

class MapFragment  
{
public:
	bool isMatchingRequestId(MapFragment& mapFragment);
	MapFragment(MapFragment& mapFragment);
	bool isValid();
	void setMapImage(CxImage* mapImage);
	CxImage* getMapImage();
	MapGridRef getMapGridRef();
	void setMapGridRef(MapGridRef mapGridRef);
	MapFragment();
	virtual ~MapFragment();

protected:
	static int getNextRequestId();
	MapGridRef _mapGridRef;	
	CxImage* _mapImage;
	int _requestId;	
	static int _NextRequestId;

};

