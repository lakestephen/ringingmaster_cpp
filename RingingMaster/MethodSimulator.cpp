#include "StdAfx.h"
#include ".\methodsimulator.h"
#include "Lead.h"
#include "SimulatorManager.h"
#include "PlayManager.h"

//to2do allow this to show the cover bells.

MethodSimulator::MethodSimulator() :
Method(), 
_simulatorRowCreated(false), 
_simulatorWaitCallbackAction(wca_continue), 
_simulatorTransitionState(msts_none), 
_simulatorInThread(false), 
_simulatorDonePrepWork(false), 
_simulatorCurrentCallIndex(-1),
_simulatorCurrentSpliceIndex(-1)
{

}

MethodSimulator::~MethodSimulator(void)
{
	POSITION pos = _simulatorCallMap.GetStartPosition();
	SimulatorCallType key;
	Call* call = NULL;

	while (pos)
	{
		_simulatorCallMap.GetNextAssoc(pos, key, call);
		delete call;
	}	
	_simulatorCallMap.RemoveAll();

	for (int i=0;i<_simulatorSpliceArray.GetSize();i++)
	{
		delete _simulatorSpliceArray.GetAt(i);
	}

	_simulatorSpliceArray.RemoveAll();
}

bool MethodSimulator::isInThread()
{
	return _simulatorInThread;
}   

void MethodSimulator::requestEarlyTerminate()
{
	/*   todo 

	TRACE("[MethodSimulator::requestEarlyTerminate] \r\n");
	requestTerminate(TR_SIMULATOR);
	
	//to pulse 
	while (!hasTerminated())
	{
//		TRACE("[MethodSimulator::requestEarlyTerminate] waiting\r\n");	
		_simulatorRowCreated = false;
		Sleep(1);
	}  

	*/
}

void MethodSimulator::requestTerminate(TerminateReason terminateReason)
{
/* todo
	TRACE("[MethodSimulator::requestTerminate] %d\r\n", terminateReason);

	//we only allow the terminate on the end change from the method
	if (terminateReason == TR_CHANGE)
	{
		if ((simulatorManager()->getSimulatorStateType() != sst_connectedRounds) &&
			(simulatorManager()->getSimulatorStateType() != sst_connectedMethodPendingHandFromRounds) &&
			(simulatorManager()->getSimulatorStateType() != sst_connectedMethodPendingBackFromRounds) && 
			(simulatorManager()->getSimulatorStateType() != sst_connectedSilentPendingFromMethod)&&
			(simulatorManager()->getSimulatorStateType() != sst_connectedSilentPendingFromRounds))
		{
			if (simulatorManager()->getComeRoundNextTime())
			{
				simulatorManager()->addComment(tttst_thatsAll);
				simulatorManager()->setSimulatorStateType(sst_connectedRounds);
				
				TRACE("[MethodSimulator::requestTerminate] requesting TR_SIMULATOR_CHANGE\r\n");

				Method::requestTerminate(TR_SIMULATOR_CHANGE);
				return;
			}
		}
	}
	else if (terminateReason == TR_SIMULATOR)
	{
		Method::requestTerminate(TR_SIMULATOR);			
		return;
	}
	else
	{
		ASSERT(FALSE);
	}	  */
}

WaitCallbackAction MethodSimulator::waitCallback()
{					
/* todo
if (!Method::hasRequestedTerminate())
	{
		_simulatorRowCreated = true;

		while (_simulatorRowCreated)
		{
//			TRACE("[MethodSimulator::waitCallback] waiting\r\n");	
			Sleep(1);
		}
	}
		   */
	return _simulatorWaitCallbackAction;
}

Row* MethodSimulator::getAnotherRow(Stroke stroke)
{
	//wait untill the previous create has happened
	//normally go straight through this 
/*	while (!_simulatorRowCreated && !Method::hasRequestedTerminate())
	{
		Sleep(1);
		ASSERT(FALSE);
	}

	//set the correct status. 
	SimulatorStateType simulatorStateType = simulatorManager()->getSimulatorStateType();
	switch (simulatorStateType)
	{
	case sst_connectedRounds:
		//as we have come round, and are restarting we need to 
		// reset the terminate flag to allow more rows to be created.
		if (getTerminateReason() == TR_SIMULATOR_CHANGE)
		{
			_requestTerminateReason = TR_NOT_REQUESTED;
			TRACE("[MethodSimulator::getAnotherRow] setting TR_NOT_REQUESTED\r\n");
		}
		_simulatorWaitCallbackAction = wca_addStartRow;
		break;

	case sst_connectedMethodPendingHandFromRounds:
	case sst_connectedMethodPendingBackFromRounds:  
		if ((stroke == hand && simulatorStateType == sst_connectedMethodPendingHandFromRounds)||
			(stroke == back && simulatorStateType == sst_connectedMethodPendingBackFromRounds))
		{
			if (_simulatorTransitionState == msts_madeCall)
			{
				_simulatorTransitionState = msts_none;
				//start a new lead.
				_simulatorWaitCallbackAction = wca_startNewLead;			
				waitNextRow();
				//now set to going properly, and allow to continue
				simulatorManager()->setSimulatorStateType(sst_connectedMethod);
				_simulatorWaitCallbackAction = wca_continue;			
				break;
			}
			else if (_simulatorTransitionState == msts_none)
			{
				RowComment rowComment;
				rowComment.setGo(getCurNot());
				CString str;
				playManager()->extractCommentForSpeech(&rowComment, str);
				simulatorManager()->addComment(str);
				_simulatorTransitionState = msts_madeCall;
			}		
		}

		//act as if still sst_connectedRounds
		_simulatorWaitCallbackAction = wca_addStartRow;			
		break;

	case sst_connectedMethod:
		_simulatorWaitCallbackAction = wca_continue;
		break;

	case sst_connectedRoundsPendingFromMethod: 
		if (stroke == hand)
		{
			if (_simulatorTransitionState == msts_madeCall)
			{
				_simulatorTransitionState = msts_none;

				//set the start row
				_simulatorWaitCallbackAction = wca_addStartRow;			
				waitNextRow();
				//start a new lead.
				_simulatorWaitCallbackAction = wca_startNewLead;			
				waitNextRow();
				//nor repeat the row
				simulatorManager()->setSimulatorStateType(sst_connectedRounds);
				_simulatorWaitCallbackAction = wca_addStartRow;		
				break;
			}
			else if (_simulatorTransitionState == msts_none)
			{
				simulatorManager()->addComment(tttst_rounds);
				_simulatorTransitionState = msts_madeCall;
				//take default action
			}		

		}

		//act as if still in sst_connectedMethod
	 	_simulatorWaitCallbackAction = wca_continue;
		break;

	case sst_connectedSilentPendingFromMethod:
	case sst_connectedSilentPendingFromRounds:
		if (stroke == hand)
		{
			if (_simulatorTransitionState == msts_madeCall)
			{
				_simulatorTransitionState = msts_none;

				requestEarlyTerminate();
		        
				return NULL;
			}
			else if (_simulatorTransitionState == msts_none)
			{
				simulatorManager()->addComment(tttst_stand);
				_simulatorTransitionState = msts_madeCall;
				//take default action
			}		
		}
		if (simulatorStateType == sst_connectedSilentPendingFromMethod)
			//act as if still in sst_connectedMethod
	 		_simulatorWaitCallbackAction = wca_continue;
		else if (simulatorStateType == sst_connectedSilentPendingFromRounds)
			//act as if still sst_connectedRounds
			_simulatorWaitCallbackAction = wca_addStartRow;			
		else
			ASSERT(FALSE);
		break;

	default:
		ASSERT(FALSE);
		break;
	}


	//wait for the calculation of the next row
	waitNextRow();

	//we do not use the first row of a leads other than the first
	if (GetSize() >= 1)
	{	
		Lead* lead = GetAt(GetUpperBound());

		if (lead->GetSize() == 1)
		{
			waitNextRow();
		}
	}
	//return a pointer to the latest Row
	Row* row = NULL;
	if (GetSize() >0)
	{	
		Lead* lead = GetAt(GetUpperBound());

		if (lead->GetSize() > 0)
		{
			row = lead->GetAt(lead->GetUpperBound());
		}
	}

	simulatorManager()->fireNotifyMethodStructureChangedFromSeperateThread();
			  
	return row;


	*/

return NULL;
}

void MethodSimulator::waitNextRow()
{
	//startr the creation of the next row.
	_simulatorRowCreated = false;

//	TRACE("[MethodSimulator::waitNextRow] %d\r\n", _simulatorWaitCallbackAction);
/* todo
	//wait for the row to be created
	DWORD start = timeGetTime();
	while (!_simulatorRowCreated && !Method::hasRequestedTerminate())
	{
		Sleep(1);	
		if ((timeGetTime() - start) >10000)
		{
			ASSERT(FALSE);
			TRACE("[MethodSimulator::waitNextRow] Waited too long\r\n");	
			requestEarlyTerminate();
			break;
		}
	}  */
}

void MethodSimulator::create(ProofInput* proofInput)
{
	//todo we are doing things with proof input before it has been set in the Method::create() method. 


	//get the last row
/* todo	
	if (GetSize() > 0)
	{
		Lead* lead = GetAt(GetUpperBound());
		if (lead && lead->GetSize() > 0)
		{
			_proofInput->getStartRow().setRowIndex(_dynamicStartRow.getRowIndex());
		}
		else
			ASSERT(FALSE);
	}

	
	Method::create(proofInput);		 */
	
}

void MethodSimulator::startCreateMethodThread()
{
	CWinThread* thread = 
		AfxBeginThread(	createMethodThread, this, 
					THREAD_PRIORITY_TIME_CRITICAL, 
					0,		//stack size
					CREATE_SUSPENDED, 
					NULL );	//security attributes


	thread->ResumeThread();
}

UINT MethodSimulator::createMethodThread( LPVOID pParam )
{	
	MethodSimulator* method = reinterpret_cast<MethodSimulator*>(pParam);

	if (method)
		method->createMethodFremSeperateThread();
    
	return 0;
}

void MethodSimulator::createMethodFremSeperateThread()
{
	/*todo
	ASSERT(!_simulatorInThread);

	if (_simulatorInThread)
		return;

	_simulatorInThread = true;
	
	while(_requestTerminateReason != TR_SIMULATOR)
	{
		if (_requestTerminateReason != TR_SIMULATOR_CHANGE)
			_requestTerminateReason = TR_NOT_REQUESTED;
		
		//we have to reset the method here so it can start again each time
		_hasActuallyTerminated = false;
		_simulatorLastSpliceShorthand = _proofInput->getInitialNotationShorthand();
		setNotation(_proofInput->getInitialNotationShorthand());
		
		_spliceIndex = 0;
		_callIndex = 0;
		
		_simulatorCurrentCallType = sct_plain;
		_simulatorCurrentSpliceType = 0;
		for (int i=0;i<getNotations().GetSize();i++)
		{
			if (getNotations().GetAt(i)->getSpliceLetter() == _proofInput->getInitialNotationShorthand())
				_simulatorCurrentSpliceType = i;
		}
		
		_simulatorCallTransitionState = mscts_system;
		_simulatorSpliceTransitionState = mscts_system;
		
		_simulatorCurrentCallIndex = -1;
		_simulatorCurrentSpliceIndex = -1;

		//do the calculation
		TRACE("[MethodSimulator::createMethod] pre create\r\n");
		create();
		TRACE("[MethodSimulator::createMethod] post create\r\n");
		
		_simulatorDonePrepWork = true;
		
		//this is to allow the play thread the ability to get in and request another row.
		//NOTE: they are both time critical
		Sleep(10);	
	}

	simulatorManager()->setSimulatorStateType(sst_connectedSilent);

	_simulatorInThread = false;
	*/

}

void MethodSimulator::addComment(RowComment* rowComment)
{
	//all comments are managed by other means

	delete rowComment;
}

void MethodSimulator::addSimulatorComment(CString msg)
{
/* todo
int leadNo = GetUpperBound();

	if (leadNo>= 0)
	{
		Lead* lead = GetAt(leadNo);

		if (lead)
		{	
			int rowNo = lead->GetUpperBound();

			RowComment* rowComment = new RowComment();
			rowComment->setRow(this, leadNo, rowNo, false);
			rowComment->setComment(msg);

			lead->addComment(rowComment, rcut_display);
		}
	}  */
}

bool MethodSimulator::isPossibleCall(SimulatorCallType type)
{
	static Call* call = NULL;

	return (_simulatorCallMap.Lookup(type, call) == TRUE);	
}

Call* MethodSimulator::getCall(SimulatorCallType type)
{
	Call* call = NULL;

	if (!_simulatorCallMap.Lookup(type, call))
	{
		ASSERT(FALSE);

		VERIFY(_simulatorCallMap.Lookup(sct_plain, call));
	}


	ASSERT(call != NULL);

	return call;
		
}

bool MethodSimulator::setSpliceType(SimulatorActionType /*type*/)
{
	CSingleLock lock(&_simulatorCallsCriticalSection, TRUE);
/* todo

	//extract the method name
	int index = type - SIMULATOR_ACTION_FIRST_METHOD;

	ASSERT(index >= 0 && index < _notations.GetSize());
	
	if (index >= 0 && index < getNotations().GetSize())
	{
		_simulatorCurrentSpliceType = index;

		if (!extractAndAddSplice(true))
			ASSERT(FALSE);
		
		_simulatorSpliceTransitionState = mscts_user;

		return true;
	}   	
*/
	return false;
}

bool MethodSimulator::setCallType(SimulatorCallType type)
{
	CSingleLock lock(&_simulatorCallsCriticalSection, TRUE);

	if (isPossibleCall(type))
	{
		_simulatorCurrentCallType = type;

		//this will call the call
		if (!extractAndAddCall())
			simulatorManager()->addComment("Plain");

		_simulatorCallTransitionState = mscts_user;

		return true;
	}
	
	return false;
}

Splice* MethodSimulator::getNextSplice()
{
	Splice* splice = doGetNextSplice();
	
	//this is from the method, therefore we must reset this to be able to detect if the user has pressed the next one.
	_simulatorSpliceTransitionState = mscts_system;

	return splice;
}

Splice* MethodSimulator::doGetNextSplice()
{
	ASSERT(isSpliced());

/* todo	if (_simulatorCurrentSpliceIndex != _spliceIndex)
	{

		//we must calculate a new one
		CSingleLock lock(&_simulatorCallsCriticalSection, TRUE);

		_simulatorCurrentSpliceIndex = _spliceIndex;

		if (_simulatorSpliceTransitionState != mscts_user)
		{

			switch (simulatorManager()->getSimulatorCallingType())
			{
			case sct_random:
				{
					if (_simulatorSpliceArray.GetSize()> 0)
					{
						//sead the number generator
						srand(GetTickCount());
						int index = (rand() * (_simulatorSpliceArray.GetSize()))/RAND_MAX; 

						//this includes the plain lead
						if (index >=0 && index <_simulatorSpliceArray.GetSize())
						{
							_simulatorCurrentSpliceType = index;
						}
					}  
				}	 
				break;
			case sct_plainCourse:
				//use the previous one
				break;
			case sct_touchCalls:
				{	
					if(_simulatorSplices.GetSize() == 0)
						return NULL;	//i.e. a plain course 

					int index = _spliceIndex%_simulatorSplices.GetSize();

					ASSERT(index >=0 && index < _simulatorSplices.GetSize());

					_simulatorCurrentSpliceType = _simulatorSplices.GetAt(index);
				}
				break;
			default:
				ASSERT(FALSE);
			}
		}

		//TRACE("[MethodSimulator::getNextSplice] got new splice\r\n");
	}
	   */
	if (_simulatorCurrentSpliceType >= 0 || 
		_simulatorCurrentSpliceType < _simulatorSpliceArray.GetSize())
		return _simulatorSpliceArray.GetAt(_simulatorCurrentSpliceType);
	else
	{
		ASSERT(FALSE);
		return _simulatorSpliceArray.GetAt(0);
	}
	
}




Call* MethodSimulator::getNextCall()
{
/* todo
if (_simulatorCurrentCallIndex != _callIndex)
	{
		//we must calculate a new one
		CSingleLock lock(&_simulatorCallsCriticalSection, TRUE);

		_simulatorCurrentCallIndex = _callIndex;
		_simulatorCurrentCallType = sct_plain;
		_simulatorCallTransitionState = mscts_system;

		switch (simulatorManager()->getSimulatorCallingType())
		{
		case sct_random:
			{
				if (_simulatorCallMap.GetSize()> 0)
				{
					//sead the number generator
					srand(GetTickCount());
					int index = (rand() * (_simulatorCallMap.GetSize()))/RAND_MAX; 

					//this includes the plain lead
					if (index >=0 && index <_simulatorCallMap.GetSize())
					{
						//iterate through the map
						POSITION pos = _simulatorCallMap.GetStartPosition();
						SimulatorCallType key;
						Call* call = NULL;
						
						int i=0;
						bool gotOne = false;
						while (pos)
						{
							_simulatorCallMap.GetNextAssoc(pos, key, call);
							if (i == index)
							{
								_simulatorCurrentCallType = key;	
								gotOne = true;
							}
							i++;
						}	

						ASSERT(gotOne == true);
					
					}
				}
			}
			break;
		case sct_plainCourse:
			//use the default plain lead
			break;
		case sct_touchCalls:
			{
				if(_simulatorCalls.GetSize() == 0)
					return NULL;	//i.e. a plain course 

				int index = _callIndex%_simulatorCalls.GetSize();

				ASSERT(index >=0 && index < _simulatorCalls.GetSize());

				_simulatorCurrentCallType = _simulatorCalls.GetAt(index);;
			}
			break;
		default:
			ASSERT(FALSE);
		}

		//TRACE("[MethodSimulator::getNextCall] got new call\r\n");
	}
				 */
	return getCall(_simulatorCurrentCallType);

}

bool MethodSimulator::shouldForceSetupNotationsCallsAndPositions()
{
	return true;
}

int MethodSimulator::getCallsSize()
{
	return INT_MAX;
}

int MethodSimulator::getSplicesSize()
{
	return INT_MAX;
}

bool MethodSimulator::hasDonePrepWork()
{
	return _simulatorDonePrepWork;
}

bool MethodSimulator::addCall(SimulatorCallType type, const CString shorthand)
{
	Call* call = NULL;
	if (_simulatorCallMap.Lookup(type, call))
		return false; //did not add as one exists

/* todo
	//create a new one
	static Variance blankVariance;	

	call = new Call(PT_NONE,blankVariance,0,shorthand);
	_simulatorCallMap.SetAt(type, call);
		 */
	return true;
}

bool MethodSimulator::addPossibleCalls(CStringArray& callShorthands, const CString plainLead)
{
	//add the plain lead first
	addCall(sct_plain, plainLead);

	//remove duplicates
	for (int i=0;i<callShorthands.GetSize();i++)
	{
		for (int j=i+1;j<callShorthands.GetSize();j++)
		{
			if (callShorthands.GetAt(i) == callShorthands.GetAt(j))
			{
				callShorthands.RemoveAt(j--);
			}
		}    		
	}

	//now look for bob and single
	for (int i=0;i<callShorthands.GetSize();i++)
	{
		if (callShorthands.GetAt(i).CompareNoCase("-") == 0)
		{
			addCall(sct_bob, callShorthands.GetAt(i));			
			callShorthands.RemoveAt(i--);
		}
		else if (callShorthands.GetAt(i).CompareNoCase("s") == 0)
		{
			addCall(sct_single, callShorthands.GetAt(i));			
			callShorthands.RemoveAt(i--);
		}
	}

	//now add all other calls in the next available slot
	for (int i=0;i<callShorthands.GetSize();i++)
	{
		Call* call = NULL;

		for (int type = sct_plain; type <=sct_callZ; type++)
		{
			//is there is not one in that slot, then add
			if (!_simulatorCallMap.Lookup((SimulatorCallType)type, call))
			{
				addCall((SimulatorCallType)i, callShorthands.GetAt(i));			
			}
		}

	}

	return true;
}


SimulatorCallType MethodSimulator::getSimulatorCallTypeFromString(const CString& string)
{
	/* todo
	POSITION pos = _simulatorCallMap.GetStartPosition();
	SimulatorCallType key;
	Call* call = NULL;

	while (pos)
	{
		_simulatorCallMap.GetNextAssoc(pos, key, call);

		if (call->getCall() == string)
			return key;
	}	
			   */
	ASSERT(FALSE);
	return sct_plain;
}

SimulatorCallType MethodSimulator::getSimulatorCurrentCallType()
{
	return _simulatorCurrentCallType;
}

void MethodSimulator::notifySplicePosPending()
{
	if (_simulatorSpliceTransitionState == mscts_system)
	{
		doGetNextSplice();   //this forces the calculation of the next splice before we extract it.

		extractAndAddSplice();
	}
}

void MethodSimulator::notifyCallPosPending()
{
	if (_simulatorCallTransitionState == mscts_system)
		extractAndAddCall();
}

bool MethodSimulator::extractAndAddSplice(bool forceSpeak)
{
/* todo
if (_simulatorCurrentSpliceType >= 0 && _simulatorCurrentSpliceType < getNotations().GetSize())
	{
		Notation* notation = getNotations().GetAt(_simulatorCurrentSpliceType);

		TRACE(" _simulatorLastSpliceShorthand = %s,  notation->getSpliceLetter() = %s\r\n", _simulatorLastSpliceShorthand, notation->getSpliceLetter());
		if (forceSpeak || _simulatorLastSpliceShorthand != notation->getSpliceLetter())
			simulatorManager()->addComment(notation->getName());

		_simulatorLastSpliceShorthand = notation->getSpliceLetter();
		return true;
	}
	else
	{
		ASSERT(FALSE);
	}
		 */
	return false;
}

bool MethodSimulator::extractAndAddCall()
{
/* todo
Call* call = getCall(_simulatorCurrentCallType);
	NotationCall* notationCall = getNotationCall(call);
	if (notationCall)
	{
		simulatorManager()->addComment(notationCall->_nameLonghand);
		return true;
	}
		*/
	return false;
}

void MethodSimulator::forceRecalculationOfCurrentCallAndSplice()
{
    _simulatorCurrentCallIndex = -1; //force recalculation
	_simulatorCurrentSpliceIndex = -1; 
}


bool MethodSimulator::isSpliced()
{
	return _simulatorSplices.GetSize() > 0;
}