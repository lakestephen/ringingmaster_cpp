#include "StdAfx.h"
#include "ProofCalculationEngineCallsCourseBased.h"
#include "ProofInput.h"
#include "CallPosition.h"
#include "CallNotationMap.h"
#include "Row.h"



ProofCalculationEngineCallsCourseBased::ProofCalculationEngineCallsCourseBased(const ProofInput& proofInput, Method& method) :
ProofCalculationEngineCalls(proofInput, method),
_curCallPosMap(NULL)
{
}

ProofCalculationEngineCallsCourseBased::~ProofCalculationEngineCallsCourseBased(void)
{										  	
	//delete the created call position maps
	for (int i=0;i<_callPositionMaps.GetSize();i++)
		delete _callPositionMaps.GetAt(i);
}

void ProofCalculationEngineCallsCourseBased::preCalculate()
{
	createCallPositionMaps();

	__super::preCalculate();
}

void ProofCalculationEngineCallsCourseBased::createCallPositionMaps()
{
	for (int i=0;i<_ip.getNotations().GetSize();i++)
	{
		_callPositionMaps.Add(new CallPositionMap(_ip.getNotations().GetAt(i), _ip.getNumber()));
	}

	ASSERT(_callPositionMaps.GetSize()>0);
}


void ProofCalculationEngineCallsCourseBased::canStartCall()
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
			if (!_curCallNotMap->Lookup(callStr, callNotation))
			{	
				ASSERT(FALSE);
				return;
			}

			// now find the calling position
			CallPosition* callPosition = NULL;
			CString strCallPosition =  _call->getPosition();

			if(_call->getPositionParseType() == PT_CALL_POS_AGREGATE)
			{
				NotationCallPosAgregate* agregate = NULL;
				for (int j=0;j<_notation->getCallPosAgregateCount();j++)
				{
					agregate = _notation->getCallPosAgregate(j);
					if(agregate->_name == strCallPosition)
					{
						strCallPosition = agregate->_callPosMethods.GetAt(_callSubIndex);
						break;
					}
				}
			}

			if (!_curCallPosMap->Lookup(strCallPosition, callPosition))
			{	  
				TRACE("The requested  _call position %s has not been found in method %s\r\n", strCallPosition, _notation->getName());
				ASSERT(FALSE);
/*todo				addCallMade(); */

				return;
			}			 
				
			//are we on the right row when multiple call pos lead calling positions?
			if (callPosition->getAfterRow() != _notationIndex)
			{	 
/*todo				addCallMade(); */
				return;
			}
			
			//is our call bell in the correct place?
			if (callPosition->getPlace() ==   _row->getPositionOfBell(_ip.getCallFrom()))
			{
/*todo				addCallMade(callStr); */
				_callNotation = callNotation;
				return;
			}

			return;
		}
	}
}  