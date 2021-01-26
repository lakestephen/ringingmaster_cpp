
#pragma once


class CxImage;
class RecordTower;
class MapHostWrapper;
class MapDocumentInfo;

#include "InternetSession.h"
#include "MapGridRef.h"
#include "MapManagerEventListener.h"
#include "MapReqThread.h" 
#include "MapCacheItem.h"
#include "ManagerBase.h"

class MapManager : public CCmdTarget, public ManagerBase
{
public:

	COLORREF getPinColorGrabbed();
	COLORREF getPinColorNonGrabbed();
	void setPinColorGrabbed(COLORREF color);
	void setPinColorNonGrabbed(COLORREF color);
	ScaleType getDefaultScale();
	void setDefaultScale(ScaleType scale);
	CString getCurrentProviderName();
	CString getCacheDataFilePath();
	void tidyCache();
	CString getScaleString(ScaleType scale);
	void getSupportedScaleList(ScaleTypes &scaleTypes);
	BOOL isShowingPushPins();
	void toggleShowingPushPins();
	BOOL isShowingMap();
	void toggleShowingMap();
	void load();
	void save(){};
	void closeDown();
	CString getCachePath();
	void cacheTimerEvent();
	MapCacheItem* getMapCacheItem(int mapId);
	CString getDLLVerison();
	void setDiskCacheSizeMB(int size);
	int getDiskCacheSizeMB();
	void setCurrentHostIndex(int index);
	void requestFromInternet(int mapId);
	int getCurentHostIndex();
	void openCurrentMapProviderWebSite();
	void mapLoadedFromThread(int mapId);
	void trimMapCacheToSize();
	void destroyMapCache(bool includeDocumets);
	bool checkForNewMapConVersion(bool showMessages);
	void getProviderList(CStringArray& strings);
	BOOL isValid();
	MapManager();
	virtual ~MapManager();

	const static CString MapSection;
	CDocument* showTowerMapDoc(const RecordTower* recordTower);
	CDocument* showTowerMapDoc(MapDocumentInfo* info);

	void addEventListener(MapManagerEventListener* mapManagerEventListener);
	void removeEventListener(MapManagerEventListener *mapManagerEventListener);

	int requestMapId(MapGridRef gridRef, ScaleType scale);
			
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapManager)
	public:
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(MapManager)
	afx_msg void OnMapOptions();
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()


protected:	 
	bool isNewerMapConVersionAvailable();
	//updating
	MapManagerEventListeners _listenerList;

	MapHostWrapper* getMapHostWrapper();
	MapHostWrapper* _wrapper;

	int isMapInCache(const MapCacheItem& request);


	MapReqThread* _mapReqThread;
	
	bool _bCheckForNewMapConVersion;
	
	MapCacheItems _mapCacheItems;

	bool _cachePathChecked;
	friend class MapReqThread;
	friend class MapCacheItem;
};

