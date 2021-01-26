#include "stdafx.h"
#include "GrabInfo.h"



GrabInfo::GrabInfo(long key) :
SaveableObject(key)
, _parenttower(0)
{
}

GrabInfo::GrabInfo(CArchive& ar) :
SaveableObject(-1)
, _parenttower(0)
{
	ASSERT(!ar.IsStoring());
	Serialize(ar);
	ASSERT(getKey() != -1);
}

GrabInfo::GrabInfo(const GrabInfo& grabInfo) : 
SaveableObject(grabInfo)
, _parenttower(0)
{
	operator = (grabInfo);
}

GrabInfo::~GrabInfo()
{
}

GrabInfo& GrabInfo::operator =(const GrabInfo& grabInfo)
{
	_grabdate = grabInfo._grabdate;
	_bellsgo = grabInfo._bellsgo;
	_bellssound = grabInfo._bellssound;
	_comments = grabInfo._comments;
	_parenttowerKey = grabInfo._parenttowerKey;
	_parenttower = grabInfo._parenttower;

	return *this;
}

void GrabInfo::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;

		__super::Serialize(ar);

		ar << _grabdate;
		ar << _bellsgo;
		ar << _bellssound;
		ar << _comments;
		ar << _parenttowerKey;
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);

		__super::Serialize(ar);

		ar >> _grabdate;
		ar >> (int&)_bellsgo;
		ar >> (int&)_bellssound;
		ar >> _comments;
		ar >> _parenttowerKey;
	}
}

COleDateTime GrabInfo::getGrabDate()
{
	return _grabdate;
}
void GrabInfo::setGrabDate(COleDateTime grabdate)
{
	_grabdate = grabdate;
}

BellsGo GrabInfo::getBellsGo()
{
	return _bellsgo;
}
void GrabInfo::setBellsGo(BellsGo bellsgo)
{
	_bellsgo = bellsgo;
}

BellsSound GrabInfo::getBellsSound()
{
	return _bellssound;
}
void GrabInfo::setBellsSound(BellsSound bellssound)
{
	_bellssound = bellssound;
}

CString GrabInfo::getComments()
{
	return _comments;
}
void GrabInfo::setComments(CString comments)
{
	_comments = comments;
}

TowerInfo* GrabInfo::getParentTower()
{
	return _parenttower;
}
long GrabInfo::getParentTowerKey()
{
	return _parenttowerKey;
}
void GrabInfo::setParentTower(TowerInfo* parenttower)
{
	_parenttower = parenttower;
}
void GrabInfo::setParentTowerKey(long parenttowerKey)
{
	_parenttowerKey = parenttowerKey;
}

