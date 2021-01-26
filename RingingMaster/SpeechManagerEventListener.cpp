// SpeechManagerEventListener.cpp: implementation of the SpeechManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SpeechManagerEventListener.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SpeechManagerEventListener::SpeechManagerEventListener()
{

}

SpeechManagerEventListener::~SpeechManagerEventListener()
{
	clearEventMap();
}

void SpeechManagerEventListener::clearEventMap()
{		
	SpeechAction key;
	SimulatorEventData* value;
	POSITION pos = _speechEvents.GetStartPosition();
	while (pos)
	{
		_speechEvents.GetNextAssoc(pos, key, value);
		delete value;
	}	

	_speechEvents.RemoveAll();
}

