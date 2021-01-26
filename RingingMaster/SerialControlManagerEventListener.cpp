// MultiBellInterfaceManagerEventListener.cpp: implementation of the MultiBellInterfaceManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SerialControlManagerEventListener.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SerialControlManagerEventListener::SerialControlManagerEventListener()
{

}

SerialControlManagerEventListener::~SerialControlManagerEventListener()
{
	clearEventMap();
}

void SerialControlManagerEventListener::serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& /*item*/, DWORD /*timestamp*/) 
{
	//do nothing
}

void SerialControlManagerEventListener::serialControlManager_notifySerialControlStatusEvent()
{
	//do nothing 
}

void SerialControlManagerEventListener::clearEventMap()
{
	SerialControlAction key;
	SimulatorEventData* value;
	POSITION pos = _serialControlEvents.GetStartPosition();
	while (pos)
	{
		_serialControlEvents.GetNextAssoc(pos, key, value);
		delete value;
	}	

	_serialControlEvents.RemoveAll();
}


void SerialControlManagerEventListener::serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& /*serialControlResponseSimulatorEventDataMap*/)
{


}