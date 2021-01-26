#pragma once

class ManagerBase
{
public:
	ManagerBase(void);
	virtual ~ManagerBase(void);

	virtual void save() = 0;
	virtual void load() = 0;
	virtual void closeDown() {};
	virtual void openUp() {};
};

typedef CArray<ManagerBase*, ManagerBase*> ManagerBases;
