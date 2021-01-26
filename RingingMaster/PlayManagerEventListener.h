// PlayManagerEventListener.h: interface for the PlayManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "RowPosition.h"
#include "Stroke.h"

class RowPositionChangedPackage;

class PlayManagerEventListener  
{
public:
	PlayManagerEventListener() {};
	virtual ~PlayManagerEventListener() {};						    

	virtual void playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& /*rowPositionChangedPackage*/) {};
	virtual void playManager_notifyBellPlayedFromSeperateThread(int /*place*/, int /*bell*/, Stroke /*stroke*/, DWORD /*perfectTimestamp*/, DWORD /*actualTimestamp*/, int /*rowIndex*/){};
	virtual void playManager_notifyStartPlayingFromSeperateThread(int /*numberIncludingCover*/, Stroke /*stroke*/, CString /*startString*/, DWORD /*startTimestamp*/) {};
	virtual void playManager_notifyStopPlaying(){};
};

typedef CArray<PlayManagerEventListener*, PlayManagerEventListener*> PlayManagerEventListeners;

