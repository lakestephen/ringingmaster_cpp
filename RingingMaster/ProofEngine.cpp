// ProofEngine.cpp: implementation of the ProofEngine class.
//
//////////////////////////////////////////////////////////////////////
//todo remove this class.
#include "stdafx.h"
#include "ProofEngine.h"
/*#include "Lead.h"
#include "FalseRow.h"
#include "MethodPosition.h"
#include "CallNotationMap.h"
#include "CallPosition.h"
#include "RingingMaster.h"
#include "AllTheWorkBell.h"
#include "ProofManager.h" //todo remove this include


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////






//constructor for simple storage
ProofEngine::ProofEngine(int number) :
_proofInput(NULL),
_number(number),
_courseHeads(1,1,NULL),
_leadEnds(1,1,NULL),
_methdodRequestId(proofManager()->getNextProofRequestId())
{
	init();
	Add(new Lead(this));
	TRACE("METHOD: ****************ONLY USED FOR CALLED CHANGES*********************\r\n");
}
			   
ProofEngine::ProofEngine(): 
_number(-1),
_courseHeads(1,1,NULL),
_leadEnds(1,1,NULL),
_methdodRequestId(proofManager()->getNextProofRequestId())
{
}


void ProofEngine::init()
{	
	_number = 0;
	_requestTerminateReason = TR_NOT_REQUESTED;
	_circleCheckNeeded = true;
	_spliceCount = 0;
	_callSubIndex = 0;
	_partIndex = 0;
	_callIndex = 0;
	_currCall = NULL;
	_curNotation = NULL;
	_notationRow = 0;
	_callCount = 0;
	_callRow = 0;
	_astheticPartIndex = 0;
	_canChangeAstheticPartIndex = false;
	_lastCompositionRow = 1;
	_lastPartIndex = 0;
	_proofTime = 0;
	_leadEndIndex = 0;
	_calculatedInCourse = false;
}

void ProofEngine::canIncrementAstheticPartIndex( bool force)
{
	//has the calling bell arrived home?
	if ((_dynamicStartRow.getBellInPlace(_proofInput->getCallFrom()-1) == _number)||
		(force)||
		(!_proofInput->getCourseBased())) //i.e. it is part based
	{																
		if ((_canChangeAstheticPartIndex)&&(!hasRequestedTerminate()))

		{
//			TRACE("Incrementing Asthetic Part Index: %s\r\n", (force)?"forced":"Not Forced" );
			_canChangeAstheticPartIndex = false;
			_astheticPartIndex++; //this should be the only place that _astheticPartIndex is changed
			if (GetUpperBound() >= 0)
			{
				CString str, str2;
				str.Format("Part %d", _astheticPartIndex+1);
				str2.Format("%d", _astheticPartIndex+1);
				RowComment* rowComment = new RowComment;
				rowComment->setRow(GetUpperBound(), GetAt(GetUpperBound())->GetUpperBound());
				rowComment->setPart(str, str2);
				addComment(rowComment);
			}

			//add the circle touch row.
			_circleCheckRows.Add(new Row(_dynamicStartRow));
			_circleCheckNeeded = true;

			//wrap around the calls made
			if (force) 
				wrapOverForcedCallMadeMatrix();

		}
	}		
}

bool ProofEngine::checkTerminateCircle()
{				 
	if (!_circleCheckNeeded)
		return false;

	_circleCheckNeeded = false;

	int count = _circleCheckRows.GetSize();

	Row** rows = new Row*[count]; 

	for (int i=0;i<count;i++)
	{
		rows[i] = _circleCheckRows.GetAt(i);
	}

	//sort them
  	qsort((void*)rows, 
		  (size_t)count, 
		  sizeof(Row *), 
		  compareRows );

	int identicalCount = 0;

	for (int i=0;i<count-1;i++)
	{
		if (*rows[i]== *rows[i+1])
		{
			identicalCount++;
			if(identicalCount >= _proofInput->getCircularCourseLimit())
			{
				delete [] rows;
				return true;
			}
		}
		else
		{	//we have come to the end of a sequence of identical rows
			identicalCount = 0;
		}		 		
	}

	delete [] rows;
	return false;
}





ProofEngine::~ProofEngine()
{



	//clean up course heads
	for (int i=0;i<_courseHeads.GetWidth();i++)
	{
		for (int j=0;j<_courseHeads.GetHeight();j++)
		{
			delete _courseHeads.GetAt(i,j);	
			_courseHeads.SetAt(NULL, i,j);	
		}
	}

	//clean up lead ends
	for (int i=0;i<_leadEnds.GetWidth();i++)
	{
		for (int j=0;j<_leadEnds.GetHeight();j++)
		{
			delete _leadEnds.GetAt(i,j);	
			_leadEnds.SetAt(NULL, i,j);	
		}
	}

	//clean up the calls made
	for (int i=0;i<_callsMade.GetSize();i++)
	{
		delete _callsMade.GetAt(i);	
		_callsMade.SetAt(i, NULL);	
	}

	//clean up the splices made
	for (int i=0;i<_splicesMade.GetSize();i++)
	{
		delete _splicesMade.GetAt(i);	
		_splicesMade.SetAt(i, NULL);	
	}

	//clean up the circle rows
	for(int i=0;i<_circleCheckRows.GetSize();i++)
		delete _circleCheckRows.GetAt(i);
	_circleCheckRows.RemoveAll();

	//sort the pending comments
	for (int i=0;i<_pendingComments.GetSize();i++)
		delete _pendingComments.GetAt(i);
	_pendingComments.RemoveAll();
}


void ProofEngine::addComment(RowComment* rowComment)
{
	ASSERT(rowComment->isValid(true));
	_pendingComments.Add(rowComment);
}

Row* ProofEngine::getRow(const RowPosition &rowPosition)
{
	if (rowPosition.isValid())
	{
		if (rowPosition.getLead() >=0 && rowPosition.getLead() < GetSize())
		{
			Lead* lead = GetAt(rowPosition.getLead());
			
			if (rowPosition.getLeadRow() >=0 && rowPosition.getLeadRow() < lead->GetSize())
				return lead->GetAt(rowPosition.getLeadRow());

		}
	}
	return NULL;
}


FalseRow * ProofEngine::getFalseRow(Row *row)
{
	if (row)
	{
		for (int i=0;i<_falseRows.GetSize();i++)
		{
			if (*row == *_falseRows.GetAt(i))
				return _falseRows.GetAt(i);
		}
	}
	return NULL;
}




void ProofEngine::addCallMade(CString call, int partIndex)
{
	if (call.GetLength() == 0)
		call = _proofInput->getPlainLeadLetter();

	//TRACE ("[addCallMade] %s, %d\r\n", call, partIndex);
	//ensure it is big enough
	while (_callsMade.GetSize() <= partIndex)
		_callsMade.Add(new CStringArray());

	//get the String Array
	CStringArray* calls = _callsMade.GetAt(partIndex);
	
	//add call to the end
	calls->Add(call);
}

void ProofEngine::wrapOverForcedCallMadeMatrix()
{
	//the idea of this is to drag the last call in each part to the start of the following part.
	CStringArray* callMade = _callsMade.GetAt(_callsMade.GetUpperBound());
	if ((callMade)&&(callMade->GetSize() > 0))
	{
		//get the last string
		CString str = callMade->GetAt(callMade->GetUpperBound());
		//delete from here
		callMade->RemoveAt(callMade->GetUpperBound());
		//now insert in the next part
		addCallMade(str, _callsMade.GetSize());
	}	
}

void ProofEngine::addSpliceMade(CString splice, int partIndex)
{

	//TRACE ("[addSpliceMade] %s, %d\r\n", splice, partIndex);
	//ensure it is big enough
	while (_splicesMade.GetSize() <= partIndex)
		_splicesMade.Add(new CStringArray());

	//get the String Array
	CStringArray* splices = _splicesMade.GetAt(partIndex);
	
	//add call to the end
	splices->Add(splice);
}

void ProofEngine::wrapOverForcedSpliceMadeMatrix()
{
	//the idea of this is to drag the last call in each part to the start of the following part.
	CStringArray* spliceMade = _splicesMade.GetAt(_splicesMade.GetUpperBound());
	if ((spliceMade)&&(spliceMade->GetSize() > 0))
	{
		//get the last string
		CString str = spliceMade->GetAt(spliceMade->GetUpperBound());
		//delete from here
		spliceMade->RemoveAt(spliceMade->GetUpperBound());
		//now insert in the next part
		addSpliceMade(str, _splicesMade.GetSize());
	}	
}

void ProofEngine::addCourseHead(Row* row, int partIndex, int compositionRow)
{
	//TRACE("addCourseHead: %s, Part %d, row %d\n",row->getAsText(), partIndex, compositionRow);
	
	//ensure it is big enough
	while (_courseHeads.GetWidth() <= partIndex)
		_courseHeads.AddColumn(NULL);
	while (_courseHeads.GetHeight() <= compositionRow)
		_courseHeads.AddRow(NULL);

	Row* oldRow = _courseHeads.GetAt(partIndex, compositionRow);
	delete oldRow;
	_courseHeads.SetAt(new Row(*row), partIndex, compositionRow);
}


void ProofEngine::completeCourseHeadMatrix()
{
	int lastLine = _courseHeads.GetHeight() -1;
	if (lastLine <= 0) 
		return;
	for (int i=1;i<_courseHeads.GetWidth();i++)
	{
		if (_courseHeads.GetAt(i,0) == NULL)
		{
			for (int j=lastLine;j>0;j--) 
			{
				if (_courseHeads.GetAt(i-1, j) != NULL)
				{
					_courseHeads.SetAt(new Row(*_courseHeads.GetAt(i-1, j)), i, 0);
					break;
				}
			}
		}

	}
}
  

void ProofEngine::addLeadEnd(Row* row, int partIndex, int position)
{


	//ensure it is big enough
	while (_leadEnds.GetWidth() <= partIndex) 
	{
		_leadEnds.AddColumn(NULL);
		_leadEndIndex = 0 ;
	}
	
	if (position == -1)
		position = ++_leadEndIndex;

//	TRACE("size:%d, partIndex:%d, position:%d \r\n", GetSize(), partIndex, position);

	while (_leadEnds.GetHeight() <= position)
		_leadEnds.AddRow(NULL);

	Row* oldRow = _leadEnds.GetAt(partIndex, position);
	ASSERT(oldRow == NULL);
	delete oldRow;
	_leadEnds.SetAt(new Row(*row), partIndex, position);
}


void ProofEngine::completeLeadEndMatrix()
{
	int lastLine = _leadEnds.GetHeight() -1;
	if (lastLine <= 0) 
		return;
	for (int i=1;i<_leadEnds.GetWidth();i++)
	{
		if (_leadEnds.GetAt(i,0) == NULL)
		{
			for (int j=lastLine;j>0;j--) 
			{
				if (_leadEnds.GetAt(i-1, j) != NULL)
				{
					_leadEnds.SetAt(new Row(*_leadEnds.GetAt(i-1, j)), i, 0);
					break;
				}
			}
		}

	}
}

	 


void ProofEngine::incrementSpliceIndex()
{
	//get the string of the current splice
	if (_spliceIndex < getSplicesSize())
	{
		_lastSpliceShorthand = getNextSplice()->_spliceShorthand;	
	}
	
	//increment splice
	_spliceIndex++;
	if (_spliceIndex > getSplicesSize()-1)
		_spliceIndex = 0;
}

int ProofEngine::getSplicesSize()
{
	return _proofInput->getSplices().GetSize();
}

bool ProofEngine::shouldForceSetupNotationsCallsAndPositions()
{
	return false;
}

void ProofEngine::setupNotationsCallsAndPositions(int number)
{
	if (hasDonePrepWork())
		return;

	//simulator needs maps when no calls present
	if (_proofInput->getCalls().GetSize() >0 || shouldForceSetupNotationsCallsAndPositions())
	{
		createCallNotationMaps();
		createCallPositionMaps(number);
	}
	setNotation(_proofInput->getInitialNotationLonghand());
}

Notation* ProofEngine::getCurNot()
{
	TRACE((_curNotation)?"":"***********Warning Current Notation is NULL. OK for called changes*************");
	if (_proofInput->getStartNotation().getChangesPerPlainLead() != 0)
		return &_proofInput->getStartNotation();
	return _curNotation;
}





void ProofEngine::getSpliceCountPairs(SpliceCountPairs &spliceCountPairs)
{
	for (int i=0;i<getNotations().GetSize();i++)
	{
		if (getNotations().GetAt(i)->_spliceRowCount != 0)
			spliceCountPairs.Add(new SpliceCountPair(
				getNotations().GetAt(i)->getName(),getNotations().GetAt(i)->_spliceRowCount));
	}
}

void ProofEngine::createAllTheWorkStruct()
{
	for (int i=0;i<getNotations().GetSize();i++)
	{
		Notation* not = getNotations().GetAt(i);
		not->_allTheWork = new AllTheWorkBell(not->getNumber(), 
			max(not->getSplicePositionCount(), 1)); // the 1 is for no splice pos, and use lead end
	}

}

void ProofEngine::addAllTheWork(Row *row, int splicePos)
{
	if (getCurNot()->_allTheWork)
		getCurNot()->_allTheWork->addRow(row, splicePos);
}







bool ProofEngine::isSpliced()
{	
	return _proofInput->isSpliced(); 
}

void ProofEngine::createSpliceTransitions()
{
	//look to the all work analysis and record all the transitions

	if (!isSpliced())
		return;

	CStringArray allSplices;

	for (int i=0;i<_splicesMade.GetSize();i++)
	{	
		CStringArray* spliceLead = _splicesMade.GetAt(0);

		for (int j=0;j<spliceLead->GetSize();j++)
		{
			allSplices.Add(spliceLead->GetAt(j));
		}
	}

	if (allSplices.GetSize() == 0)
		return;


	CString from = allSplices.GetAt(0);
	CString to;
	
	for (int i=0;i<allSplices.GetSize();i++) 
	{
		for (int j=(i==0?1:0);j<allSplices.GetSize();j++)	
		{
			to = allSplices.GetAt(j);

			if (from != to) //only after real transitions
			{
		
				int count = 0;
				CString token = getSpliceTransitionToken(from, to);
				_spliceTransitions.Lookup(token, count);

				//increment and re set
				_spliceTransitions.SetAt(token, ++count);
			//	TRACE(token + " : %d \r\n", count);
			}

			from = to;
		}				
	}
}

CString ProofEngine::getSpliceTransitionToken(const CString &from, const CString &to)
{
	return from + ":-:" + to;
}

void ProofEngine::calculateInCourse()
{
	if (_calculatedInCourse)
		return;

	_calculatedInCourse = true;

	for (int i=0;i<GetSize();i++)
	{
		Lead* lead = GetAt(i);
		for (int j=0;j<lead->GetSize();j++)
		{
			lead->GetAt(j)->calculateInCourse();
		}
	}
}

CString ProofEngine::getCopyDataString()
{
	CString output;

	for (int l = 0;l<GetSize();l++)
	{
		Lead * lead = GetAt(l);
		//index through all rows
		for (int j=(l>0)?1:0;j<lead->GetSize();j++)
		{
			output += lead->GetAt(j)->getAsText();
			output += "\r\n";
		}
	}

	return output;

}


void ProofEngine::distributePendingComments()
{	
	CString spliceLongHand;

	for (int i=0;i<_pendingComments.GetSize();i++)
	{
		RowComment* pendCom = _pendingComments.GetAt(i);

		//verify
		if (pendCom)
		{
			VERIFY(pendCom->verify(this));

			if (!pendCom->isValid())
			{
				delete pendCom;
				pendCom = NULL;
				_pendingComments.RemoveAt(i--);
			}
		}

		//remove duplicated splices.
		if (pendCom)
		{
			if (pendCom->getRowCommentType() == rct_splice)
			{
				if (pendCom->getStr2().CompareNoCase(spliceLongHand) != 0)
				{
					spliceLongHand = pendCom->getStr2();
				}
				else
				{
					delete pendCom;
					pendCom = NULL;
					_pendingComments.RemoveAt(i--);
				}
			}
		}

		//display comments - gets a copy of the original
		if (pendCom)
		{
			// do not allow go type when not spliced
			// do not allow thats all
			// all others go without a problem
			if ((pendCom->getRowCommentType() == rct_thatsAll)||
				(pendCom->getRowCommentType() == rct_go&&!isSpliced()))
			{
				//do nothing
			}
			else
			{
				RowComment* pendComCopy = new RowComment(*_pendingComments.GetAt(i));
				GetAt(pendCom->getLead())->addComment(pendComCopy, rcut_display);	
			}
		}

		//the sound comments get the original
		if (pendCom)
		{
			if ((pendCom->getRowCommentType() == rct_splice)||
				(pendCom->getRowCommentType() == rct_thatsAll))
			{
				pendCom->decrementRow(this);
			}

			if (pendCom->isValid())
			{
				GetAt(pendCom->getLead())->addComment(pendCom, rcut_sound);
			}
			else 
			{
				ASSERT(FALSE);
				delete pendCom;
				pendCom = NULL;
				_pendingComments.RemoveAt(i--);
			}
		}
	}

	//cear the list down
	_pendingComments.RemoveAll();

	//verify each comment set
	for (int i=0;i<GetSize();i++)
	{
		Lead* lead = GetAt(i);			
		lead->verifyComments(rcut_display);
		lead->verifyComments(rcut_sound);
	}
		 
}



int ProofEngine::getAstheticPartIndex()
{
	return _astheticPartIndex;
}

int ProofEngine::getCallCount()
{
	return _callCount;
}

int ProofEngine::getProofTime()
{							
	return _proofTime;
}

int ProofEngine::getNotationRow()
{
	return _notationRow;
}

void ProofEngine::setNotationRow(int notationRow)
{
	_notationRow = notationRow;
}

WaitCallbackAction ProofEngine::waitCallback()
{
	//do nothing here
	return wca_continue;
}

Call* ProofEngine::getNextCall()
{
	if(_proofInput->getCalls().GetSize() == 0)
		return NULL;	//i.e. a plain course 

	if (_callIndex > _proofInput->getCalls().GetUpperBound())
	{
		ASSERT(_proofInput->getWrapCalls() == FALSE);
		return NULL;
	}

	return _proofInput->getCalls().GetAt(_callIndex);
}

Splice* ProofEngine::getNextSplice()
{
	return _proofInput->getSplices().GetAt(_spliceIndex);
}

int ProofEngine::getCallsSize()
{
	return _proofInput->getCalls().GetSize();
}

bool ProofEngine::hasDonePrepWork()
{
	return false;
}

NotationCall* ProofEngine::getNotationCall(Call *call)
{
	if (call == NULL)
		return NULL;

	if (call->getCall() == DEFAULT_CALL_TOKEN )
		return getCurNot()->getStandardCall();
	else
	{
		for (int i=0;i<getCurNot()->getCallCount();i++)
		{
			NotationCall* thisCall = getCurNot()->getCall(i);
			if ((call->getCall() == thisCall->_nameShorthand)||
				(call->getCall() == thisCall->_nameLonghand))
			{
				return thisCall;
			}		  
		}
	}

	return NULL;
}

void ProofEngine::notifySplicePosPending()
{
	//do nothing in base class	
}

void ProofEngine::notifyCallPosPending()
{
	//do nothing in base class	
}





























































































Lead::Lead(Method* method) :
_method(method)
{
	ASSERT(_method);
}

void Lead::create()
{
	ASSERT(_method);
	Ints* nextNotRow = NULL;

	_method->setNotationRow(getInitialNotationRow());
	checkLeadLines(); //to catch row 0

	//insert a copy of the passed row.
	Row* row = new Row(_method->_dynamicStartRow);
	Add(row);

	checkAndAddCourseHead(row);
 	
	WaitCallbackAction action ;

	while (!_method->hasRequestedTerminate())
	{
		checkLeadLines();

		action = _method->waitCallback();
		if (action == wca_continue)
		{
			//cary on as normal	
			//get the next notation
			nextNotRow = getNextNotation(row);
			if (nextNotRow == 0)
				break; 
			row = new Row(*row, nextNotRow);
		}
		else if (action == wca_startNewLead)
		{
			//get out - like getNextNotation returning NULL
			break;
		}   
		else if (action == wca_addStartRow)
		{
			//add a copy of teh start row.
			ASSERT(row);
			Row* oldRow = row;
			row = new Row(_method->_proofInput->getStartRow());
			row->setRowIndex(oldRow->getRowIndex() + 1);
		}

//		TRACE("Row Index = %d\r\n", row->getRowIndex());
		Add(row);		
		_method->getCurNot()->_spliceRowCount++;
		checkTerminate(row);
		//process splices AFTER check checkTerminate()
		processSplice(row); 
		checkAndAddCourseHead(row);
	}

	//catch last row
	checkLeadLines();

	//pass info back to the method 
	_method->	_dynamicStartRow = *(row);
}


void Lead::checkAndAddCourseHead(Row* row)
{
	
	//are we on a reportable row?
	if (_method->getCurNot()->getCourseHeadPos() == _method->getNotationRow())
	{
		_method->addLeadEnd(row, _method->_astheticPartIndex);

		//has the calling bell arrived home?

		if (row->getBellInPlace(_method->_number-1) == _method->_proofInput->getCallFrom())
		{
			_method->addCourseHead(row, _method->_lastPartIndex, _method->_lastCompositionRow);
		//	TRACE("%d, %d \r\n", _method->GetSize(), GetSize());
		}
	}
}

Ints * Lead::getNextNotation(const Row * row)
{
	//have we gone past the end of the notation size?
	if (_method->getNotationRow()>=_method->getCurNot()->getChangesPerPlainLead())
		return NULL;

	//are we in a call?
	if (_method->_currCall)
	{
		//do we have any more of the call to process?
		if (_method->_callRow < _method->_currCall->getChangesPerPlainLead()) 
			return getCallRow();	
		else //drop out of the call
			_method->_currCall = NULL;
	}

	//get the next valid call
	Call* call = NULL;
	int enteringPart = _method->_partIndex;
	while (((call = _method->getNextCall()) != NULL)&&
			(!call->getVariance().isInPart(_method->_partIndex)))
	{
		//this code just handles variances
		incrementCallIndex();

		if (_method->_partIndex > enteringPart+2) //stop potential lock up
		{
			_method->requestTerminate(TR_EMPTY_PARTS);
			
			RowComment* rowComment = new RowComment;
			rowComment->setRow(_method->GetUpperBound()-1, GetUpperBound());
			rowComment->setAborted();
			_method->addComment(rowComment);
			return NULL;
		}				
	}

	//can we start calling the call?
	if((_method->_currCall = isConditionMet(call, row)) != NULL)
	{
		//if we have got to the start of the next part without crossing a course end, force change
		_method->canIncrementAstheticPartIndex(true);

		_method->_callRow = 0;
		incrementCallIndex();
		_method->_callCount++; //overall call count
		NotationCall* notationCall = _method->getNotationCall(call);

		if (notationCall)
		{
			RowComment* rowComment = new RowComment;
			rowComment->setRow(_method->GetSize()-1, max(0,GetSize() - 2)) ;
			rowComment->setCall(notationCall->_nameShorthand, notationCall->_nameLonghand, call->getPosition());
			_method->addComment(rowComment);
		}

		_method->_lastCompositionRow = call->getCompositionRow();
		_method->_lastPartIndex = _method->_astheticPartIndex;

		return getCallRow();
	}

	//must be a row from the plain lead
	return getNotationRow();
}



Notation* Lead::isConditionMet(Call *call, const Row* row)
{

	Notation* notation = _method->getCurNot();
	//could we possably be at a call juncture
	for (int i=0;i<notation->getCallPosLeadCount();i++)
	{
		//notify for the simulator
		if (_method->getNotationRow() == notation->getCallPosLead(i) - EARLY_NOTIFY_SIZE_CALL)
		{
			_method->notifyCallPosPending();
		}

		if (_method->getNotationRow() == notation->getCallPosLead(i))
		{
			if (call == NULL)
			{
				addCallMade();
				return NULL; //proberbly in a plain course / or spliced with no calls
			}  


			CString callStr = call->getCall();
			if ((callStr == DEFAULT_CALL_TOKEN)&&
				(notation->hasStandardCall()))
				callStr = notation->getStandardCall()->_nameShorthand;

			
			//find the call
			Notation* callNotation = NULL;
			if (!_method->getCurrCallNotationMap()->Lookup(callStr, callNotation))
			{
				if (call->getPositionParseType() == PT_NONE)
					incrementCallIndex();	//if NULL, then a plain lead in lead based method
													// bypass that plain lead!!
				addCallMade(); 
				return NULL;
			}

			//have we got a call without a position? (i.e. Lead Based)
			//when lead based, return by default
			if (call->getPositionParseType() == PT_NONE)
			{
				addCallMade(callStr); 
				return callNotation;	  
			}

			//find the position
			CallPosition* callPosition = NULL;
			CString strCallPosition = call->getPosition();

			if(call->getPositionParseType() == PT_CALL_POS_AGREGATE)
			{
				NotationCallPosAgregate* agregate = NULL;
				for (int j=0;j<_method->getCurNot()->getCallPosAgregateCount();j++)
				{
					agregate = _method->getCurNot()->getCallPosAgregate(j);
					if(agregate->_name == strCallPosition)
					{
						strCallPosition = agregate->_callPosMethods.GetAt(_method->_callSubIndex);
						break;
					}
				}
			}

			if (!_method->getCurrCallPositionMap()->Lookup(strCallPosition, callPosition))
			{
				TRACE("The requested call position %s has not been found in method %s\r\n", strCallPosition, _method->getCurNot()->getName());
				ASSERT(FALSE);
				return NULL;
			}
				
			//are we on the right row?
			if (callPosition->getAfterRow() != _method->getNotationRow())
			{
				addCallMade();
				return NULL; //wrong row (when call pos lead has more than one row
			}
			
			//is our call bell in the correct place?
			if (callPosition->getPlace() ==   row->getPositionOfBell(_method->_proofInput->getCallFrom()))
			{
				addCallMade(callStr); 
				return callNotation;
			}
			//plainn lead
			else 
			{			 
				addCallMade(); 
				return NULL;
			}


			break;
		}
	}

	return NULL;
}


void Lead::incrementCallIndex( )
{
	//are we in an agregate call?
	Call* call = _method->getNextCall();
	if (call->getPositionParseType() == PT_CALL_POS_AGREGATE)
	{
		//find the agreagate
		NotationCallPosAgregate* agregate = NULL;
		for (int i=0;i<_method->getCurNot()->getCallPosAgregateCount();i++)
		{
			agregate = _method->getCurNot()->getCallPosAgregate(i);
			if(agregate->_name == call->getPosition())
			{
				if (_method->_callSubIndex < agregate->_callPosMethods.GetUpperBound())
				{
					_method->_callSubIndex++;
				}
				else
				{
					_method->_callIndex++; 
					_method->_callSubIndex = 0; 
				}
				break;
			}
		}		
	}
	else
	{
		_method->_callIndex++; 
		_method->_callSubIndex = 0;
	}

	if(_method->_proofInput->getWrapCalls() &&
		_method->_callIndex >= _method->getCallsSize())
	{
		//changing parts
		_method->_callSubIndex = 0;
		_method->_callIndex = 0;
		_method->_partIndex++;
		_method->_canChangeAstheticPartIndex = true;
	}
}




void Lead::getHuntBells(Ints& bells)
{
	if (GetSize() < 2) return;

	for (int i=0;i<GetAt(0)->getNumber();i++)
	{
		if (GetAt(0)->getBellInPlace(i) == GetAt(GetUpperBound())->getBellInPlace(i))
			bells.Add(i);
	}
}

int Lead::getWorkingBellCount()
{
	return GetAt(0)->getNumber() - getHuntCount();
}

int Lead::getWorkingBell(int index)
{
	if (index >= getWorkingBellCount())
		index = getWorkingBellCount()-1;

	if (index < 0)
		index = 0;

	int count = -1;
	for (int i=0;i<GetAt(0)->getNumber();i++)
	{
		if (GetAt(0)->getBellInPlace(i) != GetAt(GetUpperBound())->getBellInPlace(i))
			count++;
		if (count == index)
			return GetAt(0)->getBellInPlace(i);
	} 
	return -1;
}


int Lead::getInitialNotationRow( )
{
		//make sure that are not starting past the end of the lead
	if (_method->getCurNot()->getChangesPerPlainLead() >0)
	{
		while (_method->_proofInput->getStartAtRow() >= _method->getCurNot()->getChangesPerPlainLead())
		{
			_method->_proofInput->setStartAtRow(
				_method->_proofInput->getStartAtRow() -_method->getCurNot()->getChangesPerPlainLead()); //todo startatrow should be an engine variable as well as an input variable.
		}
	}
	if (_method->_proofInput->getStartAtRow() < 0)
		_method->_proofInput->setStartAtRow(0);

	return _method->_proofInput->getStartAtRow();//usually 0
}

void Lead::addCallMade(CString call)
{
	_method->addCallMade(call, _method->_astheticPartIndex);
}

void Lead::addSpliceMade(CString splice)
{
	_method->addSpliceMade(splice, _method->_astheticPartIndex);
}

bool Lead::isCommentRow(int row, RowCommentUseType type)
{
	RowComments& rowComments = getCommentArrayFromType(type);

	for (int i=0;i<rowComments.GetSize();i++)
	{
		if (rowComments.GetAt(i)->getLeadRow() == row)
			return true;
	}
		
	return false;
}

void Lead::removeAllComments()
{
	for (int i=0;i<_displayComments.GetSize();i++)
		delete _displayComments.GetAt(i);
	_displayComments.RemoveAll();

	for (int i=0;i<_soundComments.GetSize();i++)
		delete _soundComments.GetAt(i);
	_soundComments.RemoveAll();

}	   

RowComment* Lead::getComment(int index, RowCommentUseType type)
{
	RowComments& rowComments = getCommentArrayFromType(type);

	if (index < rowComments.GetSize())
		return rowComments.GetAt(index);

	return NULL;
}

void Lead::addComment(RowComment* rowComment, RowCommentUseType type)
{	  
	getCommentArrayFromType(type).Add(rowComment);
}

void Lead::verifyComments(RowCommentUseType type)
{
	switch (type)
	{
	case rcut_display:
		{
			verifyDisplayComments();
		}
		break;
	case rcut_sound:
		break;
	default:
		ASSERT(FALSE);
	}
}

void Lead::verifyDisplayComments()
{
	for (int i=0;i<GetSize()+_displayComments.GetSize();i++)  //you can have the all teh comments on the last row. thus they would spread well after the lead
	{
		//go down each row
		RowComments tempCommentsList;
		if (getCommentList(tempCommentsList, i, rcut_display) > 1)
		{
			//we have a clash, move the lowest priority ones onto the next row.
			while (tempCommentsList.GetSize() > 1)
			{
				RowComment* comment0 = tempCommentsList.GetAt(0);
				RowComment* comment1 = tempCommentsList.GetAt(1);

				if (comment0 > comment1)
				{
					//bump comment 0
					comment0->incrementRow(_method);
					tempCommentsList.RemoveAt(0);
				}							
				else
				{
					//bump comment 1
					comment1->incrementRow(_method);
					tempCommentsList.RemoveAt(1);
				}
			}
		}		
	}	
}

int Lead::getCommentList(RowComments& commentsList, int row, RowCommentUseType type)
{
	RowComments& rowComments = getCommentArrayFromType(type);

	for (int i=0;i<rowComments.GetSize();i++)
	{
		if (rowComments.GetAt(i)->getLeadRow() == row)
			commentsList.Add(rowComments.GetAt(i));
	}

	return commentsList.GetSize();  
}

int Lead::getCommentCount(RowCommentUseType type)
{
	return getCommentArrayFromType(type).GetSize();
}


RowComments& Lead::getCommentArrayFromType(RowCommentUseType type)
{
	switch (type)
	{
	case rcut_display:
		return _displayComments;
	case rcut_sound:
		return _soundComments;
	default:
		ASSERT(FALSE);
		return _displayComments;
	}

}


  */