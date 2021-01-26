// MultiBellInterfaceManagerEventListener.h: interface for the MultiBellInterfaceManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SimulatorEventData.h"
#include "MultiBellResponse.h"

typedef CMap<MultiBellAction, const MultiBellAction&, SimulatorEventData*, SimulatorEventData*> MultiBellActionSimulatorEventDataMap ;
 
class MultiBellInterfaceManagerEventListener  
{
public:
	MultiBellInterfaceManagerEventListener();
	virtual ~MultiBellInterfaceManagerEventListener();

	virtual void multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp) = 0;
	virtual void multiBellInterfaceManager_notifyMBIStatusEvent(bool portOK, bool MBIOK) = 0;
	virtual void multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap) = 0;

private:
	void clearEventMap();
	MultiBellActionSimulatorEventDataMap _mbiEvents;

	friend class MultiBellInterfaceManager;
};

typedef CArray<MultiBellInterfaceManagerEventListener*, MultiBellInterfaceManagerEventListener*> MultiBellInterfaceManagerEventListeners;

