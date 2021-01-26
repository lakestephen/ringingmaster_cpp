#include "StdAfx.h"
#include "ProofCalculationEngineCallsLeadBased.h"
#include "Notation.h"
#include "Call.h"
#include "CallNotationMap.h"
#include "Call.h"

//todo have a proper mechanicsam to identify lead based, and have different Call objects that do not include the position (WBMH) stuff

ProofCalculationEngineCallsLeadBased::ProofCalculationEngineCallsLeadBased(const ProofInput& proofInput, Method& method) :
ProofCalculationEngineCalls(proofInput, method)
{
}

ProofCalculationEngineCallsLeadBased::~ProofCalculationEngineCallsLeadBased(void)
{
}



void ProofCalculationEngineCallsLeadBased::canStartCall()
{
	ASSERT(_callNotation == NULL);

	for (int i=0;i<_notation->getCallPosLeadCount();i++)
	{
		//are we on the a row in the notation that is capable of starting a call?
		if (_notationIndex == _notation->getCallPosLead(i))
		{
			//find the string for the call
			CString callStr =  _call->getCall();
			if ((callStr == DEFAULT_CALL_TOKEN)&&
				(_notation->hasStandardCall()))
				callStr = _notation->getStandardCall()->_nameShorthand;

			//find the call string in the call notation map.
			Notation* callNotation = NULL;
			if (callStr.IsEmpty() || !_curCallNotMap->Lookup(callStr, callNotation))
			{	
				//if we cant find the call then call a plain lead !!
				ASSERT(_call->getPositionParseType() == PT_NONE); //todo do a proper check for lead based call
				moveOnCall();	
				//todo addCallMade(); 
				return;
			}
			else
			{
				//todo addCallMade(); 
				_callNotation = callNotation;
				return;
			}
		}
	}
}

bool ProofCalculationEngineCallsLeadBased::setNotationFromName(CString name)
{		
	bool changed = __super::setNotationFromName(name);

	//todo we haev to find a way of linking the maps with the actual Notation* _notation.

	/*	 todo
		_curCallNotMap = _callNotationMaps.GetAt(i);
		_curCallPosMap = _callPositionMaps.GetAt(i);
		*/

	return changed;
}
