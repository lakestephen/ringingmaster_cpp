// TowerManagerEventListener.h: interface for the TowerManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class TowerManagerEventListener
{
public:
	TowerManagerEventListener();
	virtual ~TowerManagerEventListener();

	virtual void towerManager_notifyUpdateTowerList() = 0;


};

typedef CArray<TowerManagerEventListener*, TowerManagerEventListener*> TowerManagerEventListeners;

