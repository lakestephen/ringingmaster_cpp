#pragma once

#include "InfoListDefs.h"

class InfoManagerBase
{
public:
	InfoManagerBase();
	~InfoManagerBase();

	GrabInfo* findGrab(long key);
	int getGrabCount();
	TowerInfo* findTower(long key);
	int getTowerCount();

private:
	GrabInfoCollection _allgrabs;
	TowerInfoCollection _alltowers;
};
