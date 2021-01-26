#pragma once

#include "SaveableObjects.h"

class SaveableObject
{
public:
	SaveableObject(long key);
public:
	~SaveableObject(void);

	virtual void dummyFunction() = 0;//todo remove
	
	bool operator ==(const SaveableObject& towerInfo);
	long getKey();

protected:
	void setKey(long key);
	virtual void Serialize(CArchive &ar);

private:
	long _key;
};

