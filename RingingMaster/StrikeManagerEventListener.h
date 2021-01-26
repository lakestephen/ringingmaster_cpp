// StrikeManagerEventListener.h: interface for the StrikeManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class StrikeManagerEventListener
{
public:
	StrikeManagerEventListener(){};
	virtual ~StrikeManagerEventListener(){};

	virtual void strikeManager_notifyUpdateStrikeList() = 0;


};

typedef CArray<StrikeManagerEventListener*, StrikeManagerEventListener*> StrikeManagerEventListeners;

