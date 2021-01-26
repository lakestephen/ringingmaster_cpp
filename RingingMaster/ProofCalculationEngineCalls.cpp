#include "StdAfx.h"
#include "ProofCalculationEngineCalls.h"
#include "ProofInput.h"
#include "CallNotationMap.h"


ProofCalculationEngineCalls::ProofCalculationEngineCalls(const ProofInput& proofInput, Method& method) :
ProofCalculationEngine(proofInput, method),
_curCallNotMap(NULL),
_callNotation(NULL), 
_callNotationIndex(0),
_call(NULL),
_callIndex(-1), 
_callSubIndex(0),
_partIndex(0)
{
}

ProofCalculationEngineCalls::~ProofCalculationEngineCalls(void)
{
	//delete the created call notation maps
	for (int i=0;i<_callNotationMaps.GetSize();i++)
		delete _callNotationMaps.GetAt(i);
}

void ProofCalculationEngineCalls::preCalculate()
{
	//set up the calls
	createCallNotationMaps();
	moveOnCall();

	__super::preCalculate();
}


void ProofCalculationEngineCalls::createCallNotationMaps()
{
	for (int i=0;i<_ip.getNotations().GetSize();i++)
	{
		_callNotationMaps.Add(new CallNotationMap(_ip.getNotations().GetAt(i)));
	}
	ASSERT(_callNotationMaps.GetSize()>0);
}

bool ProofCalculationEngineCalls::moveOn()
{
	bool leadEnded = ProofCalculationEngine::moveOn();	

	//move the call on and check for call end
	if (_callNotation)
	{
		_callNotationIndex++;
		if (_callNotationIndex >= _callNotation->getChangesPerPlainLead()) 
		{
			_callNotation = NULL;
			_callNotationIndex = 0;
			//move on to the next logical  call
			moveOnCall();
		}
	}

	//can we start ringing a new call?
	if (!_callNotation && _call)
	{
		canStartCall();
	}

	return leadEnded;
}

void ProofCalculationEngineCalls::moveOnCall()
{ 
	//get the next valid  call
	incrementCallIndex();

	//this code just skips the calls that are not relevent to this 
	// part due to a variance
	int enteringPart = _partIndex;
	while (((_call = _ip.getCall(_callIndex)) != NULL)&&
			(!_call->getVariance().isInPart(_partIndex)))
	{
		incrementCallIndex();

		if (_partIndex > enteringPart+2) //stop potential lock up
		{
			requestTerminate(TR_EMPTY_PARTS);
				 /* todo
			RowComment* rowComment = new RowComment;
			rowComment->setRow(_method->GetUpperBound()-1, GetUpperBound());
			rowComment->setAborted();
			_method->addComment(rowComment);  */
			_call = NULL;
			return ;
		}				
	}
}	


Ints* ProofCalculationEngineCalls::getNextNotationRow()
{
	if(_callNotation)
		return _callNotation->getRow(_callNotationIndex);
	else	
		return  ProofCalculationEngine::getNextNotationRow();
}

void ProofCalculationEngineCalls::incrementCallIndex( )
{
	//are we in an agregate call?
	if (_call && _call->getPositionParseType() == PT_CALL_POS_AGREGATE)
	{
		//find the agreagate
		NotationCallPosAgregate* agregate = NULL;
		for (int i=0;i<_notation->getCallPosAgregateCount();i++)
		{
			agregate = _notation->getCallPosAgregate(i);
			if(agregate->_name == _call->getPosition())
			{
				if (_callSubIndex < agregate->_callPosMethods.GetUpperBound())
				{
					_callSubIndex++;
				}
				else
				{
					_callIndex++; 
					_callSubIndex = 0; 
					_call = NULL;
				}
				break;
			}
		}		
	}
	else
	{
		_callIndex++; 
		_callSubIndex = 0;	
		_call = NULL;
	}

	if(_ip.getWrapCalls() &&
		_callIndex >= _ip.getCallCount())
	{
		//changing parts
		_callSubIndex = 0;
		_callIndex = 0;
		_partIndex++;
//todo		_canChangeAstheticPartIndex = true;
	}  
}

bool ProofCalculationEngineCalls::setNotationFromName(CString name)
{		
	bool changed = __super::setNotationFromName(name);

	//todo we haev to find a way of linking the maps with the actual Notation* _notation.

	/*	 todo
		_curCallNotMap = _callNotationMaps.GetAt(i);
		_curCallPosMap = _callPositionMaps.GetAt(i);
		*/

	return changed;
}