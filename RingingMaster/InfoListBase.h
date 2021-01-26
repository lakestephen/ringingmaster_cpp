#pragma once


template <class T>
	class InfoListBase
{
public:
	InfoListBase(int nBlockSize = 10):
	_map(nBlockSize)
	{
	}

	~InfoListBase(void)
	{
	}

	int Count()
	{
		return _map.GetCount();
	}

	bool Add(T infoItem)
	{
		ASSERT(infoItem);
		if (infoItem)
		{
			//the index must be greater than 0
			ASSERT(infoItem->getKey() > 0);
			if (infoItem->getKey() <= 0)
				return false;

			T value = NULL;
			if (_map.Lookup(infoItem->getKey(), value))
			{
				//we already have one
				ASSERT(FALSE);
				return false;
			}

			_map.SetAt(infoItem->getKey(), infoItem);

			return true;
		}

	return false;
	}

	T Find(long key)
	{
		T value = NULL;
		_map.Lookup(key, value);

		return value;
	}

	POSITION GetStartPosition( ) const
	{
		_map.GetStartPosition();
	}

	T GetNextAssoc(POSITION& rNextPosition) const
	{
		int key;
		T value;
		_map.GetNextAssoc(rNextPosition, key, value);
		return value;
	}



private:
	CMap<long, long, T, T> _map;
};	
