// MapCacheItem.h: interface for the MapCacheItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPCACHEITEM_H__6A8C2054_9743_11D7_B6CD_000255162CF0__INCLUDED_)
#define AFX_MAPCACHEITEM_H__6A8C2054_9743_11D7_B6CD_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapGridRef.h"
#include "TowerDataStatic.h"


class CxImage;

enum ScaleType;


class MapCacheItem  
{
public:	
	void destroyMap();
	DWORD getFileLength();
	static int MapCacheItem::sortAccessTime( const void * arg1, const void * arg2 );
	void setFileDetails(DWORD fileLength, CTime lastAccessed);	//does not need to be COleDateTime
	void free();
	int getHostIndex() const;
	void requestFromInternet();
	bool isSameMap(const MapCacheItem& item);
	bool isOnDisk();

	static void updateNextMapId(int existingMapId);

	MapGridRef getCenterOfMap() const;
	MapGridRef getNextMap(Direction direction);
	void setImage(CxImage* image);
	CSize getImageSize();
	MapGridRef getBottomLeft();
	MapGridRef getTopRight();
	float getEastingsPixPer100M();
	float getNorthingsPixPer100M();
	bool isGridRefOnMap(MapGridRef mapGridRef);
	CxImage* getMapImage();
	bool isImageAvailable();
	int getMapId();
	MapCacheItem(const MapGridRef& mapGridRef, ScaleType scale, int hostIndex);
	MapCacheItem::MapCacheItem();
	virtual ~MapCacheItem();
	void Serialize(CArchive &ar);
	int assignMapId(int mapId = -1);
	CString getPath();

	ScaleType getScale() const {return _scale;}

protected:
	bool calculatePixPer100M();
	//serialised data
	int _mapId;
	MapGridRef _topRight;
	MapGridRef _bottomLeft;
	MapGridRef _centerOfMap;
	ScaleType _scale;
	CSize _imageSize;
	float _eastingsPixPer100M;
	float _northingsPixPer100M;
	int _nextMapEastings;
	int _nextMapNorthings;
	int _hostIndex;
	
	//non serialised data
	CxImage* _image;

	//functions
	void readFromDisk();
	bool writeToDisk();

	bool calculateImageSize();
	bool calculateCorners();
	bool calculateNextMapDistances();
	bool calculateCenterOfMap(const MapGridRef& mapGridRef);


	//for use in trimming cache
	DWORD _fileLength;
	CTime _lastAccessed;  //does not need to be COleDateTime

	static int _NextMapId;
};


typedef CArray<MapCacheItem*, MapCacheItem*> MapCacheItems; 

#endif // !defined(AFX_MAPCACHEITEM_H__6A8C2054_9743_11D7_B6CD_000255162CF0__INCLUDED_)
