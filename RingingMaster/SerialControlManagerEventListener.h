
#pragma once

#include "SerialControlAction.h"
#include "SimulatorEventData.h"

class SerialControlResponse;

typedef CMap<SerialControlAction, const SerialControlAction&, SimulatorEventData*, SimulatorEventData*> SerialControlResponseSimulatorEventDataMap ;


class SerialControlManagerEventListener  
{
public:
	SerialControlManagerEventListener();
	virtual ~SerialControlManagerEventListener();

	virtual void serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp);
	virtual void serialControlManager_notifySerialControlStatusEvent();
	virtual void serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& serialControlResponseSimulatorEventDataMap);
private:
	void clearEventMap();
	SerialControlResponseSimulatorEventDataMap _serialControlEvents;

	friend class SerialControlManager;
};

typedef CArray<SerialControlManagerEventListener*, SerialControlManagerEventListener*> SerialControlManagerEventListeners;
