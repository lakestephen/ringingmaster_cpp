// MultiBellInterfaceManagerEventListener.cpp: implementation of the MultiBellInterfaceManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MultiBellInterfaceManagerEventListener.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MultiBellInterfaceManagerEventListener::MultiBellInterfaceManagerEventListener()
{

}

MultiBellInterfaceManagerEventListener::~MultiBellInterfaceManagerEventListener()
{       
	clearEventMap();
}

void MultiBellInterfaceManagerEventListener::clearEventMap()
{
	MultiBellAction key;
	SimulatorEventData* value;
	POSITION pos = _mbiEvents.GetStartPosition();
	while (pos)
	{
		_mbiEvents.GetNextAssoc(pos, key, value);
		delete value;
	}	

	_mbiEvents.RemoveAll();
}
