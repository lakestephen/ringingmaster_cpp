#include "StdAfx.h"
#include "UniqueIDGenerator.h"

int UniqueIDGenerator::_NextId = 1;
CCriticalSection* UniqueIDGenerator::_criticalSection = NULL;

UniqueIDGenerator::UniqueIDGenerator(void)
{
}

UniqueIDGenerator::~UniqueIDGenerator(void)
{
}

int UniqueIDGenerator::getNextId() 
{
	if (_criticalSection == NULL)
		_criticalSection = new CCriticalSection;
	CSingleLock lock(_criticalSection, TRUE);

	_NextId++;

	return _NextId;
}
