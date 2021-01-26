// CallNotationMap.cpp: implementation of the CallNotationMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "CallNotationMap.h"
#include "Notation.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CallNotationMap::CallNotationMap(Notation* notation,  bool onlyLonghand)
{
	for (int i=0;i<notation->getCallCount();i++)
	{
		NotationCall * call = notation->getCall(i);
		
		//NOTE 	_nameLonghand can be empty
		bool addLonghand = ((call->_nameLonghand != call->_nameShorthand)&&
			                (!call->_nameLonghand.IsEmpty()));
		
		if (addLonghand)
		{
			//add the longhand
			Notation * notLonghand= new Notation();
			notLonghand->create(true, call->_nameShorthand, notation->getNumber(), false, call->_notation, "");
			notLonghand->prepare();
			
			SetAt(call->_nameLonghand,notLonghand);
		}

		//add in the shorthand
		if ((!onlyLonghand)||(!addLonghand))
		{
			Notation * notShorthand = new Notation();
			notShorthand->create(true, call->_nameShorthand, notation->getNumber(), false, call->_notation, "");
			notShorthand->prepare();
			
			ASSERT(!call->_nameShorthand.IsEmpty());
			SetAt(call->_nameShorthand,notShorthand);
		}

	}
}

CallNotationMap::~CallNotationMap()
{	
	//tidy up the calls definitions
	CString call;
	Notation* callNotation;
	POSITION pos = GetStartPosition();

	while (pos)
	{
		GetNextAssoc(pos, call, callNotation);
		delete callNotation;
	}
	RemoveAll(); 
}


