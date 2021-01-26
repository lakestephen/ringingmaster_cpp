
#pragma once

enum ViewType;

class RecordGrab;
class RecordTower;

class RecordManagerEventListener  
{
public:
	RecordManagerEventListener() {};
	virtual ~RecordManagerEventListener() {};   

	virtual void recordManager_notifyHomeTowerChange(RecordTower* /*homeTower*/) {};

	virtual RecordTower* recordManager_notifyGetSelectedTower(ViewType /*ViewType*/) { return NULL;}//todo do we need this?

	virtual void recordManager_updateFilterList() {};
};

typedef CArray<RecordManagerEventListener*, RecordManagerEventListener*> RecordManagerEventListeners;

