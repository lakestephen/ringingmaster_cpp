// SpeechManagerEventListener.h: interface for the SpeechManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SimulatorEventData.h"
#include "SpeechAction.h"

class SpeechResponse;


typedef CMap<SpeechAction, const SpeechAction&, SimulatorEventData*, SimulatorEventData*> SpeechActionEventDataMap ;

class SpeechManagerEventListener  
{
public:
	SpeechManagerEventListener();
	virtual ~SpeechManagerEventListener();

	virtual void speechManager_notifySpeechRecognisedFromSeperateThread(const SpeechResponse& response, DWORD timestamp) = 0;
	virtual void speechManager_notifyGetSpeechList(SpeechActionEventDataMap& speechActionEventDataMap) = 0;

private:
	void clearEventMap();
	SpeechActionEventDataMap _speechEvents;

	friend class SpeechManager;
};		


typedef CArray<SpeechManagerEventListener*, SpeechManagerEventListener*> SpeechManagerEventListeners;


