// MapCacheItem.cpp: implementation of the MapCacheItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MapCacheItem.h"
#include "MapManager.h"
#include "MapHostWrapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const ENUM_CXIMAGE_FORMATS DiskCacheImageType = CXIMAGE_FORMAT_PNG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int MapCacheItem::_NextMapId = 1;


//constructor for use in archiving 
MapCacheItem::MapCacheItem() :
_image(NULL),
_fileLength(0)
{

}
						   
MapCacheItem::~MapCacheItem()
{
	delete _image;
}


//constructor for use in requesting
MapCacheItem::MapCacheItem(const MapGridRef& mapGridRef, ScaleType scale, int hostIndex)  :
_image(NULL),
_mapId(-1),
_scale(scale), 
_hostIndex(hostIndex),
_fileLength(0)
{
	calculateCenterOfMap(mapGridRef);
	calculateCorners();
	calculateImageSize();
	calculatePixPer100M();
	calculateNextMapDistances();
 /*
	TRACE("\r\n");
	TRACE("Input Ref = " + mapGridRef.toString() + "\r\n");
	TRACE("Center Ref = " + _centerOfMap.toString() + "\r\n");
	TRACE("_topRight Ref = " + _topRight.toString() + "\r\n");
	TRACE("_bottomLeft Ref = " + _bottomLeft.toString() + "\r\n");
	TRACE("\r\n");
   */
}

int MapCacheItem::assignMapId(int mapId) //can be -1 to get thenext in the sequence
{
	_mapId = mapId;
	
	if (_mapId == -1)
		_mapId = ++_NextMapId;

	return _mapId;
}

void MapCacheItem::updateNextMapId(int existingMapId)
{ 
	if (existingMapId > _NextMapId)
		_NextMapId = existingMapId +1;
}

bool MapCacheItem::calculateCenterOfMap(const MapGridRef& mapGridRef)
{
	if (mapManager()->isValid())
	{
		MapHostWrapper* wrapper = mapManager()->getMapHostWrapper();
		
		_centerOfMap = mapGridRef;

		wrapper->getHostGridRefImageCenter( _hostIndex, 
										_scale, 
										_centerOfMap);
	}

	return true;

}

bool MapCacheItem::calculateCorners()
{
	if (mapManager()->isValid())
	{
		MapHostWrapper* wrapper = mapManager()->getMapHostWrapper();

		wrapper->getHostGridRefCorners( _hostIndex, 
										_scale, 
										_centerOfMap, 
										_bottomLeft, 
										_topRight);
	}

	return true;
}



bool MapCacheItem::calculatePixPer100M()
{
	float width100M = (float)(_topRight.getEastings() - _bottomLeft.getEastings());
	float widthPix = (float)_imageSize.cx;

	_eastingsPixPer100M = (widthPix / width100M);

	
	float height100M = (float)(_topRight.getNorthings() - _bottomLeft.getNorthings());
	float heightPix = (float)_imageSize.cy;

	_northingsPixPer100M = (heightPix / height100M);


	return true;
}

bool MapCacheItem::calculateImageSize()
{

	if (mapManager()->isValid())
	{
		MapHostWrapper* wrapper = mapManager()->getMapHostWrapper();

		int width;
		int height;

		wrapper->getHostPostCropSize( _hostIndex, 
									  _scale, 
									  width,
									  height
									  );

		_imageSize.cx = width;
		_imageSize.cy = height;
		return true;
	}

	return false;
}




bool MapCacheItem::calculateNextMapDistances()
{
	if (mapManager()->isValid())
	{
		MapHostWrapper* wrapper = mapManager()->getMapHostWrapper();

		wrapper->getHostNextMapCenter( _hostIndex, 
									  _scale, 
									  _nextMapEastings, 
									  _nextMapNorthings);

		return true;
	}

	return false;
}

int MapCacheItem::getMapId()
{
	return _mapId;

}

void MapCacheItem::setImage(CxImage *image)
{
	_image = image;

	writeToDisk();
}	 

CString MapCacheItem::getPath()
{
	CString cachePath = mapManager()->getCachePath();

	CString str;
	str.Format("%sm%d.dat", cachePath, _mapId);

	return str;
}

bool MapCacheItem::writeToDisk()
{
	
	if ((_image != NULL)&&
		(!isOnDisk()))
	{
		CWaitCursor wait;
		_image->Save(getPath(), DiskCacheImageType);
		return true;
	}

	return false;
}

bool MapCacheItem::isImageAvailable()
{
	return ((_image)||(isOnDisk()));
}

CxImage* MapCacheItem::getMapImage()
{
	if (!_image)
		readFromDisk();

	if (!_image)
		requestFromInternet();

	return _image;
}
	 
void MapCacheItem::readFromDisk()
{
	if (isOnDisk())
		_image = new CxImage(getPath(), DiskCacheImageType);
}

void MapCacheItem::requestFromInternet()
{
	mapManager()->requestFromInternet(_mapId);
}

bool MapCacheItem::isOnDisk()
{
	CFileFind fileFind;
	return (fileFind.FindFile(getPath()) == TRUE);
}


void MapCacheItem::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
		ar << _mapId;
		_bottomLeft.Serialize(ar);;
		_topRight.Serialize(ar);
		_centerOfMap.Serialize(ar);
		ar << _nextMapEastings;
		ar << _nextMapNorthings;
		ar << _scale;
		ar << _imageSize;
		ar << _eastingsPixPer100M;
		ar << _northingsPixPer100M;
		ar << _hostIndex;
	}
	else
	{
		ar >> version;  
		ar >> _mapId;
		_bottomLeft.Serialize(ar);;
		_topRight.Serialize(ar);
		_centerOfMap.Serialize(ar);
		ar >> _nextMapEastings;
		ar >> _nextMapNorthings;
		int value;
		ar >> value;
		_scale = (ScaleType)value;
		ar >> _imageSize;
		ar >> _eastingsPixPer100M;
		ar >> _northingsPixPer100M;
		ar >> _hostIndex;
	}
}

bool MapCacheItem::isGridRefOnMap(MapGridRef mapGridRef)
{
	return ((mapGridRef.getEastings() >= _bottomLeft.getEastings())&&
			(mapGridRef.getEastings() <= _topRight.getEastings())&&
			(mapGridRef.getNorthings() >= _bottomLeft.getNorthings())&&
			(mapGridRef.getNorthings() <= _topRight.getNorthings()));
}

float MapCacheItem::getEastingsPixPer100M()
{
	return _eastingsPixPer100M;
}

float MapCacheItem::getNorthingsPixPer100M()
{
	return _northingsPixPer100M;
}

MapGridRef MapCacheItem::getBottomLeft()
{
	return _bottomLeft;
}

MapGridRef MapCacheItem::getTopRight()
{
	return _topRight;
}

CSize MapCacheItem::getImageSize()
{
	if (_image)
		return CSize(_image->GetWidth(), _image->GetHeight());
	else
		return _imageSize;
}




MapGridRef MapCacheItem::getNextMap(Direction direction) 
{
	MapGridRef centerOfMap = getCenterOfMap();

	switch (direction)
	{
	case d_n:
		return MapGridRef(centerOfMap.getEastings(),  
						  centerOfMap.getNorthings() + _nextMapNorthings);
		break;
	case d_s:
		return MapGridRef(centerOfMap.getEastings(),  
						  centerOfMap.getNorthings() - _nextMapNorthings);
		break;
	case d_e:
		return MapGridRef(centerOfMap.getEastings()  + _nextMapEastings, 
						  centerOfMap.getNorthings());
		break;
	case d_w:
		return MapGridRef(centerOfMap.getEastings()  - _nextMapEastings, 
						  centerOfMap.getNorthings());
		break;
	default:
		ASSERT(FALSE);	
		return centerOfMap;
		break;
	} 
}


MapGridRef MapCacheItem::getCenterOfMap() const
{	
	return _centerOfMap;
}
	  
bool MapCacheItem::isSameMap(const MapCacheItem &item)
{
	return ((_hostIndex == item._hostIndex)&&
			(_scale == item._scale)&&			
			(_centerOfMap == item._centerOfMap));

}

						  

int MapCacheItem::getHostIndex() const
{
	return _hostIndex;
}

void MapCacheItem::free()
{
	//write to disk if needed
 	writeToDisk();


 	//delete the image
 	delete _image;
 	_image = NULL;
}

void MapCacheItem::setFileDetails(DWORD fileLength, CTime lastAccessed)	//does not need to be COleDateTime
{
	_fileLength = fileLength;
	_lastAccessed = lastAccessed;

}

//inverse sort
int MapCacheItem::sortAccessTime( const void * arg1, const void * arg2 )
{
	MapCacheItem* mapCacheItem1 = (*(MapCacheItem**)arg1);
	MapCacheItem* mapCacheItem2 = (*(MapCacheItem**)arg2);

	if (mapCacheItem1->_lastAccessed < mapCacheItem2->_lastAccessed)
		return 1;
	if (mapCacheItem1->_lastAccessed > mapCacheItem2->_lastAccessed)
		return -1;


	return 0;
}



DWORD MapCacheItem::getFileLength()
{
	return _fileLength;
}

//deletes boththe current map, and the disk version
void MapCacheItem::destroyMap()
{
	if (isOnDisk())
	{
		::DeleteFile(getPath());
		TRACE("MapCacheItem::destroyMap Deleting File : " + getPath() + "\r\n");
	}

	delete _image;
	_image = NULL;
}
