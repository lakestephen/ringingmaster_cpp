#include "stdafx.h"
#include "InfoManagerBase.h"

InfoManagerBase::InfoManagerBase()
{
}

InfoManagerBase::~InfoManagerBase()
{
}

GrabInfo* InfoManagerBase::findGrab(long key)
{
	return _allgrabs.Find(key);
}

int InfoManagerBase::getGrabCount()
{
	return _allgrabs.Count();
}

TowerInfo* InfoManagerBase::findTower(long key)
{
	return _alltowers.Find(key);
}

int InfoManagerBase::getTowerCount()
{
	return _alltowers.Count();
}

