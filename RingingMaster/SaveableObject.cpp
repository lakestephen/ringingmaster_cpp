#include "StdAfx.h"
#include "SaveableObject.h"

SaveableObject::SaveableObject(long key) :
_key(key)
{
}

SaveableObject::~SaveableObject(void)
{
}

long SaveableObject::getKey()
{
	return _key;
}

bool SaveableObject::operator ==(const SaveableObject& saveableObject)
{
	return _key == saveableObject._key;
}


void SaveableObject::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _key;
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);

		ar >> _key;
	}
}