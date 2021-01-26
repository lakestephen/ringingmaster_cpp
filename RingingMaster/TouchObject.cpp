// TouchObject.cpp: implementation of the TouchObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchObject.h"
#include "TouchDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TouchObject::TouchObject()
{

}

TouchObject::~TouchObject()
{
	for (int i=0;i<_calls.GetSize();i++)
		delete _calls.GetAt(i);

	for (int i=0;i<_splices.GetSize();i++)
		delete _splices.GetAt(i);

}
	// to2do also use this for a copy operation


void TouchObject::createTouchObject(TouchDoc *pTouchDoc)
{
	if (pTouchDoc)
	{
		//get the calls
		_calls;
		pTouchDoc->createCalls(_calls);

		//get the splices
		_splices;
		pTouchDoc->createSplices(_splices);
	}

}

void TouchObject::tidy()
{
	for (int i=1;i<_calls.GetSize();i++)
	{
		Call* lastcall = _calls.GetAt(i-1);
		Call* call = _calls.GetAt(i);

		if (*call == *lastcall)
		{
			delete _calls.GetAt(i);
			_calls.RemoveAt(i--);
			lastcall->incrementMultiplyer();
		}											
	}										
}

void TouchObject::dump()
{
#ifdef _DEBUG
	//debug
	for (int i=0;i<_calls.GetSize();i++)
	{
		Call* call = _calls.GetAt(i);
		TRACE("Call = %s, Pos = %s\r\n", call->getCall(), call->getPosition()) ;
	}
#endif //_DEBUG
}
