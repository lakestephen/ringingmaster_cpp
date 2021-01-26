#pragma once

template<class TYPE, class ARG_TYPE = const TYPE&>
class CListTS : private CList<TYPE, ARG_TYPE>
{
public:
	CListTS() {}

	~CListTS(void) {};

	POSITION AddHead(ARG_TYPE newElement)
	{
		CSingleLock lock(&_criticalSection, TRUE);
		return CList<TYPE, ARG_TYPE>::AddHead(newElement);
	}    

	POSITION AddTail(ARG_TYPE newElement)
	{
		CSingleLock lock(&_criticalSection, TRUE);
		return CList<TYPE, ARG_TYPE>::AddTail(newElement);
	}    

	void RemoveAll()
	{
		CSingleLock lock(&_criticalSection, TRUE);
		CList<TYPE, ARG_TYPE>::RemoveAll();
	}
	
	const TYPE& GetAt(POSITION position) const
	{
		CSingleLock lock(&_criticalSection, TRUE);
		return CList<TYPE, ARG_TYPE>::GetAt(position);
	}
	
	POSITION GetHeadPosition() const
	{
		CSingleLock lock(&_criticalSection, TRUE);
		return CList<TYPE, ARG_TYPE>::GetHeadPosition();
	}

	TYPE RemoveHead()
	{
		CSingleLock lock(&_criticalSection, TRUE);
		return CList<TYPE, ARG_TYPE>::RemoveHead();
	}

	INT_PTR GetSize() const
	{
		CSingleLock lock(&_criticalSection, TRUE);
		return CList<TYPE, ARG_TYPE>::GetSize();
	}

	const TYPE& GetNext(POSITION& rPosition) const 
	{
		CSingleLock lock(&_criticalSection, TRUE);
		return CList<TYPE, ARG_TYPE>::GetNext(rPosition);
	}

	mutable CCriticalSection _criticalSection;
};
