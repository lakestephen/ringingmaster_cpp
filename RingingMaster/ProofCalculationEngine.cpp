#include "StdAfx.h"
#include "ProofCalculationEngine.h"
#include "ProofInput.h"
#include "Method.h"
#include "Row.h"

ProofCalculationEngine::ProofCalculationEngine(const ProofInput& proofInput, Method& method) :
_ip(proofInput),
_met(method),
_hasRequestedTerminate(false),
_hasActuallyTerminated(false), 
_row(NULL), 
_endRow(0), 
_notation(NULL), 
_notationIndex(0),
_blockIndex(0),
_spliceIndex(0)
{

}

ProofCalculationEngine::~ProofCalculationEngine(void)
{
	delete _endRow;
	_endRow = NULL;
}

void ProofCalculationEngine::doCalculate()
{
	DWORD start = timeGetTime();

	preCalculate();
	calculate();
	postCalculate();

	_met.setProofTime( timeGetTime() - start );	//todo split proof and analysis time out

	_hasActuallyTerminated = true;
}

void ProofCalculationEngine::requestEarlyTerminate()
{
	requestTerminate(TR_EARLY);
	TRACE("[ProofCalculationEngine::requestEarlyTerminate] %s (%d)\r\n", _ip.getName(), _ip.getUniqueId() );
}

void ProofCalculationEngine::requestTerminate(TerminateReason terminateReason)
{
	//we only want to record the first terminate request.
	if (!_hasRequestedTerminate)
		_met.setTerminateReason(terminateReason); 
	_hasRequestedTerminate = true;
}

bool ProofCalculationEngine::hasRequestedTerminate()
{
	return _hasRequestedTerminate;
}

bool ProofCalculationEngine::hasActuallyTerminated()
{
	return _hasActuallyTerminated;
}

void ProofCalculationEngine::preCalculate()
{
	//copy stuff to Method
	_met.setUniqueId(_ip.getUniqueId());

	//set up the variables
	_notation = _ip.getNotations().GetAt(0); //todo set up for spliced.
	_notationIndex = _ip.getStartAtRow();
	
	//create and add the first row
	_row = new Row(_ip.getNumber(), _ip.getStartStroke(), _ip.getStartRow());
	_met.addRow(_row);
	checkLeadLines();

	//create the last row - only used for checking terminate condition.
	_endRow = new Row(_ip.getNumber(), _ip.getStartStroke(), _ip.getEndRow());


	   /*


	createAllTheWorkStruct();	 

	//prep work-
	setupNotationsCallsAndPositions(_number);
	
	//make the course head calculations
	int rowForFirstCourseHead;
	if (getCurNot()->getCourseHeadPos() == 
		getCurNot()->getChangesPerPlainLead())
		rowForFirstCourseHead = 0;
	else
		rowForFirstCourseHead = getCurNot()->getCourseHeadPos();
	
	//this is needed as we automatically set set the first method
	if (isSpliced())
	{
		incrementSpliceIndex();
		addSpliceMade(_proofInput->getInitialNotationShorthand(),0);
	}

	//set the go comment
	RowComment* rowComment = new RowComment;
	rowComment->setRow(0,0); 
	rowComment->setGo(getCurNot());
	addComment(rowComment);

	if (_proofInput->getCalls().GetSize() >0)
	{
		RowComment* rowComment = new RowComment;
		rowComment->setRow(0,0);
		rowComment->setPart("Part 1", "1");
		addComment(rowComment);
	}

	//when in spliced the first row needs to be checked for all teh work
	if (isSpliced()) //spliced
	{
		int spliceCount = getCurNot()->getSplicePositionCount();
		for (int i=0;i<spliceCount;i++)
		{
			int splicePos = getCurNot()->getSplicePosition(i);
			if ((splicePos - getCurNot()->getChangesPerPlainLead() == _proofInput->getStartAtRow())||
				(splicePos == _proofInput->getStartAtRow()))
			{
				addAllTheWork(&_dynamicStartRow, i);
				break;
			}
		}				
	}

	//start notation check 
	_proofInput->getStartNotation().prepare();

	*/
}


void ProofCalculationEngine::calculate()
{		 	
	//each run round this loop will create a Row.
	while (!hasRequestedTerminate())
	{	 		
		//get the notation to use 
		Ints * nextNotation = getNextNotationRow();

		if (nextNotation == NULL) 
		{
			requestTerminate(TR_EARLY);
		}
		else
		{  
			//apply the notation
			_row = new Row(*_row, nextNotation);
			_met.addRow(_row);
			
			//move on  the indexes
			bool leadEnded = moveOn();

			//check to see if we need to change method
			moveOnSplice();//todo is this in the correct place?

			//check for analysis stuff
			checkLeadLines();

			//can we finish?
			checkForTerminateCondition(leadEnded);
		}


/*	todo	if (!isSpliced()) //not spliced
			addAllTheWork(&_dynamicStartRow);
		
		if (_proofInput->getStartNotation().getChangesPerPlainLead() >0)
	 		_proofInput->getStartNotation().init(true);

		canIncrementAstheticPartIndex();
		*/
	};
		 
}

void ProofCalculationEngine::postCalculate()
{
	 /*
	//we must have a terminate reason here
	ASSERT(getTerminateReason() != TR_NOT_REQUESTED);

	if (getTerminateReason() == TR_CHANGE ||
		getTerminateReason() == TR_SIMULATOR_CHANGE)
	{
		RowComment* rowComment = new RowComment;
		rowComment->setRow(GetUpperBound(), GetAt(GetUpperBound())->GetUpperBound());
		rowComment->setThatsAll();
		addComment(rowComment);	
	}

	if (getTerminateReason() != TR_SIMULATOR && 
		getTerminateReason() != TR_SIMULATOR_CHANGE) 
	{

		if (getTerminateReason() != TR_EARLY) 
		{
			//add the first row (Rounds) and part
			if ((GetSize() >0)&&(GetAt(0)->GetSize() > rowForFirstCourseHead))
			{
				addCourseHead(GetAt(0)->GetAt(rowForFirstCourseHead), 0,0);
				addLeadEnd(GetAt(0)->GetAt(rowForFirstCourseHead), 0, 0);
			}
		}
	}

	ASSERT(getTerminateReason() != TR_NOT_REQUESTED);		
	*/


}


Ints* ProofCalculationEngine::getNextNotationRow()
{
	return _notation->getRow(_notationIndex); 
}

//returns true if a lead has ended.
bool ProofCalculationEngine::moveOn()
{
	bool leadEnded = false;

	//move the notation row on and check for lead end.
	_notationIndex++;	
	if (_notationIndex >= _notation->getChangesPerPlainLead())
	{
		_met.leadFinished();
		_notationIndex = 0;
		leadEnded = true;
	}

	return leadEnded;
}

void ProofCalculationEngine::moveOnSplice()
{			  
	for (int i=0;i<_notation->getSplicePositionCount();i++)
	{
		//with splices we move on every splice juncture. Regardless!!
		if (_notationIndex == _notation->getSplicePosition(i))
		{
			//get the next splice from the touch
			const Splice* splice = _ip.getSplice(_spliceIndex);

			//find the new notation
			Notation* oldNotation = _notation;
			VERIFY(setNotationFromName(splice->_spliceShorthand));
			ASSERT(_notation);

			//now find the same lead position in the new method, and ajust index			
			int newMethodSplicePosition = i;
			if ((_notation->getSplicePositionCount() < oldNotation->getSplicePositionCount()) &&
				(i > _notation->getSplicePositionCount()-1))
			{
				//new notation has less positions and we are higher than the number 
				//the new notation positions, then use the highest available
				newMethodSplicePosition = _notation->getSplicePositionCount()-1;
			}		
	  /*  todo
			//use the matching position top calculate the notation row
			_method->setNotationRow(_notation->getSplicePosition(newMethodSplicePosition));

			//use the method version to ensure proper management of overlapping comments
			RowComment* rowComment = new RowComment;
			rowComment->setRow(_method->GetSize()-1, GetUpperBound());
			rowComment->setSplice(splice);
			_method->addComment(rowComment);
			
			//sort out the splice count
			if (splice->_spliceShorthand != _method->_lastSpliceShorthand)
				_method->_spliceCount++;

			addSpliceMade(splice->_spliceShorthand);

			_method->incrementSpliceIndex();

			//do the all the work stuff (with the calculated position 
			_method->addAllTheWork(row, newMethodSplicePosition);
			*/
		}
	}				 
}

bool ProofCalculationEngine::setNotationFromName(CString name) //todo splices should be mapped directly to the notations.
{   
	for (int i=0;i<_ip.getNotations().GetSize();i++)
	{
		if (_ip.getNotations().GetAt(i)->isSpliceLetter(name))
		{
//			TRACE("Changing Notation: %s", name);
			_notation = _ip.getNotations().GetAt(i);
			return true;
		}
	}	
	return false;
}



/*	todo //increment splice
	_spliceIndex++;
	if (_spliceIndex > getSplicesSize()-1)
		_spliceIndex = 0;
  */

void ProofCalculationEngine::checkForTerminateCondition(bool checkForLeadConditions)
{


	//Terminate on change 
	if (*_row == *_endRow)
	{						
		//do the multi block stuff
		_blockIndex++;

		if (_blockIndex >= _ip.getBlockCount())
		{
			requestTerminate(TR_CHANGE);
		}
	}

	//Terminate on number of rows.
	if (_met.getRowCount() >= _ip.getRowLimit()) 
		requestTerminate(TR_ROWS);

	if (checkForLeadConditions)
	{	 
		//Terminate on Lead Linit
		if ((_ip.getLeadLimit() != -1)&&
			(_met.getLeadCount() >= _ip.getLeadLimit()))
			requestTerminate(TR_LEADS);

		
		/* todo
		//Terminate on part limit
		if ((_proofInput->getPartLimit() != -1)&&
			(_astheticPartIndex +(_canChangeAstheticPartIndex?1:0)>= _proofInput->getPartLimit()))
		{
			requestTerminate(TR_PARTS);
		}	  
		
		//circle touch
		if ((_ip.getCircularCourseLimit() != -1)&&
			(checkTerminateCircle()))
		{
			requestTerminate(TR_CIRCLE);
			  
			RowComment* rowComment = new RowComment;
			rowComment->setRow(GetUpperBound(), GetAt(GetUpperBound())->GetUpperBound());
			rowComment->setAborted();
			addComment(rowComment);
		
		}	  */
	}
}


void ProofCalculationEngine::checkLeadLines( )
{
	if (!_met.hasAnalysis())
		return;

	//fill in the lead lines array
	for (int i=0;i<_notation->getLeadLineCount();i++)
	{
		if (_notation->getLeadLine(i) == _notationIndex)
			_row->setLeadLine();
	}
}