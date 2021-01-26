
#pragma once


class MapCacheItem;

class MapManagerEventListener  
{
public:
	MapManagerEventListener();
	virtual ~MapManagerEventListener();		


	virtual void mapManager_notifyMapLoaded(int mapId) = 0;

};

typedef CArray<MapManagerEventListener*, MapManagerEventListener*> MapManagerEventListeners;
