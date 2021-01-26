// KeyboardManagerEventListener.cpp: implementation of the KeyboardManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "KeyboardManagerEventListener.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KeyboardManagerEventListener::KeyboardManagerEventListener()
{

}

KeyboardManagerEventListener::~KeyboardManagerEventListener()
{
	clearEventMap();
}

void KeyboardManagerEventListener::clearEventMap()
{
	KeyboardAction key;
	SimulatorEventData* value;
	POSITION pos = _keyEvents.GetStartPosition();
	while (pos)
	{
		_keyEvents.GetNextAssoc(pos, key, value);
		delete value;
	}	

	_keyEvents.RemoveAll();
}

