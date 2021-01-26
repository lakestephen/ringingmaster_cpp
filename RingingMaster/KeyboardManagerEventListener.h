// KeyboardManagerEventListener.h: interface for the KeyboardManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "KeyboardAction.h"
#include "SimulatorEventData.h"

class KeyboardResponse;

typedef CMap<KeyboardAction, const KeyboardAction&, SimulatorEventData*, SimulatorEventData*> KeyboardActionSimulatorEventDataMap ;

class KeyboardManagerEventListener  
{
public:
	KeyboardManagerEventListener();
	virtual ~KeyboardManagerEventListener();


	virtual void keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp) = 0;
	virtual void keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) = 0;

private:
	void clearEventMap();
	KeyboardActionSimulatorEventDataMap _keyEvents;

	friend class KeyboardManager;
};

typedef CArray<KeyboardManagerEventListener*, KeyboardManagerEventListener*> KeyboardManagerEventListeners;

