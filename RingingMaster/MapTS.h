#pragma once

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CMapTS : private CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>
{
public:
	CMapTS() {}

	~CMapTS(void) {};

	void SetAt(ARG_KEY key, ARG_VALUE newValue)
	{
		CSingleLock lock(&_criticalSection, TRUE);
		CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(key, newValue);
	}

	void RemoveAll()
	{
		CSingleLock lock(&_criticalSection, TRUE);
		CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveAll();
	}

	BOOL Lookup(ARG_KEY key, VALUE& rValue) const
	{
		CSingleLock lock(&_criticalSection, TRUE);
		BOOL val = CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(key, rValue);

		return val;
	}

	void GetNextAssoc(POSITION& rNextPosition,KEY& rKey, VALUE& rValue) const
	{
		CSingleLock lock(&_criticalSection, TRUE);
		CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(rNextPosition, rKey, rValue);
	}

	POSITION GetStartPosition() const
	{
		CSingleLock lock(&_criticalSection, TRUE);
		return CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetStartPosition()	;	
	}

	mutable CCriticalSection _criticalSection;
};
