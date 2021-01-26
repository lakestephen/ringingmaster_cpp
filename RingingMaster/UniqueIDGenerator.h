#pragma once

class UniqueIDGenerator
{
public:
	UniqueIDGenerator(void);

	static int getNextId();
public:
	~UniqueIDGenerator(void);

private:
	static int _NextId;
	static CCriticalSection* _criticalSection;
};
