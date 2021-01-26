#pragma once

#include "SaveableObject.h"
#include "InfoListDefs.h"

enum BellsGo;
enum BellsSound;

class GrabInfo : SaveableObject
{
public:
	GrabInfo(long key);
	GrabInfo(CArchive& ar);
	GrabInfo(const GrabInfo& grabInfo);
	~GrabInfo();

	GrabInfo& operator =(const GrabInfo& grabInfo);
public:
	void Serialize(CArchive &ar);

public:
	COleDateTime getGrabDate();
	void setGrabDate(COleDateTime grabdate);

	BellsGo getBellsGo();
	void setBellsGo(BellsGo bellsgo);

	BellsSound getBellsSound();
	void setBellsSound(BellsSound bellssound);

	CString getComments();
	void setComments(CString comments);

	TowerInfo* getParentTower();
	long getParentTowerKey();
	void setParentTower(TowerInfo* parenttower);
	void setParentTowerKey(long parenttowerKey);


private:
	COleDateTime _grabdate;
	BellsGo _bellsgo;
	BellsSound _bellssound;
	CString _comments;
	long _parenttowerKey;
	TowerInfo* _parenttower;
};
