#include "StdAfx.h"
#include "ProofInput.h"
#include "Method.h"
#include "UniqueIDGenerator.h"

ProofInput::ProofInput(int number) : 
_number(number),
_plainLeadLetter("p"), 
_rowLimit(10000),
_leadLimit(1000),
_partLimit(-1),
_circularCourseLimit(2),
_wrapCalls(true),
_startStroke(back), 
_startAtRow(0), 
_blockCount(1), 
_callFrom(number),
_performAnalysis(false), 
_uniqueId(UniqueIDGenerator::getNextId())
{

}


ProofInput::~ProofInput(void)
{
	for (int i=0;i<_notations.GetSize();i++)
		delete _notations.GetAt(i);

	for (int i=0;i<_calls.GetSize();i++)
		delete _calls.GetAt(i);

	for (int i=0;i<_splices.GetSize();i++)
		delete _splices.GetAt(i);

	for (int i=0;i<_musicRules.GetSize();i++)
		delete _musicRules.GetAt(i);
}

int ProofInput::getUniqueId() const
{
	return _uniqueId;
}

int ProofInput::getNumber() const
{
	return _number;
}

void ProofInput::setName(CString name)
{
	_name = name;
}

CString ProofInput::getName() const
{
	return _name;
}

void ProofInput::setPlainLeadLetter(const CString& plainLeadLetter)
{
	_plainLeadLetter = plainLeadLetter;
}

const CString& ProofInput::getPlainLeadLetter()
{
	return _plainLeadLetter;
}

void ProofInput::setStartRow(const CString& startRow)
{
	_startRow = startRow;
}

const CString& ProofInput::getStartRow() const
{
	return _startRow;
}

void ProofInput::setEndRow(const CString& endRow)
{
	_endRow = endRow;
}

const CString& ProofInput::getEndRow() const
{
	return _endRow;
}

ProofEngineType ProofInput::getProofEngineType() const
{
	
	if (_calls.GetSize() == 0)
		return pet_noCalls;

	Call* call = _calls.GetAt(0);

	if (call->getPositionParseType() == PT_NONE)
		return pet_leadBased;
	else
		return pet_courseBased;

	ASSERT(FALSE);
	return pet_noCalls;
}

void ProofInput::setStartStroke(Stroke startStroke)
{
	_startStroke = startStroke;
}

Stroke ProofInput::getStartStroke() const
{
	return _startStroke;
}
	
void ProofInput::setStartAtRow(int startAtRow)
{
	_startAtRow = startAtRow;
}

int ProofInput::getStartAtRow() const
{
	return _startAtRow;
}

void ProofInput::setBlockCount(int blockCount) 
{
	_blockCount = blockCount;
}

int ProofInput::getBlockCount() const
{
	return _blockCount;
}

void ProofInput::setCallFrom(int callFrom)
{
	ASSERT(_callFrom <= _number);
	_callFrom = callFrom;
}

int ProofInput::getCallFrom() const
{
	return _callFrom;
}

void ProofInput::setStartNotation(const Notation& startNotation)
{
//todo put back in	ASSERT(startNotation.getNumber() == _number);
	_startNotation = startNotation;
}

Notation& ProofInput::getStartNotation() //todo this should be a const method
{
	return _startNotation;
}

void ProofInput::setPerformAnalysis()
{
	_performAnalysis = true;
}

bool ProofInput::getPerformAnalysis() const
{
	return _performAnalysis;
}
	
void ProofInput::addNotation(const Notation& notation)
{
	//take a copy of the notation. it makes it thread safe
	ASSERT(notation.getNumber() == _number);

	if (notation.getNumber() == _number)
	{
		Notation* notationCopy = new Notation(notation);
		notationCopy->prepare();		
		_notations.Add(notationCopy);
	}
}

void ProofInput::addNotation(const Notations& notations)
{
	//take a copy of the notations array  makes it thread safe
	for (int i=0;i<notations.GetSize();i++)
		addNotation(*notations.GetAt(i));

	ASSERT(_notations.GetSize() >0);	
}

const Notations& ProofInput::getNotations() const  //todo change to a getNotation and getNotationCount
{
	return _notations ;
}

void ProofInput::addCall(const Calls& calls)
{
	for (int i=0;i<calls.GetSize();i++)
		_calls.Add(calls.GetAt(i));

}
void ProofInput::addCall(const Call& call)
{
	_calls.Add(new Call(call));
}

int ProofInput::getCallCount() const
{
	return _calls.GetSize();
}

const Call* ProofInput::getCall(int index) const
{
	if (index > _calls.GetUpperBound())
	{
		ASSERT(_calls.GetSize() == 0 || getWrapCalls() == FALSE );
		return NULL;
	}

	return _calls.GetAt(index);
}

void ProofInput::addSplice(const Splices& splices) //todo do checks that we have the notation, and / or connect with notation
{
	for (int i=0;i<splices.GetSize();i++)
		_splices.Add(splices.GetAt(i));
}

void ProofInput::addSplice(const Splice& splice) //todo do checks that we have the notation, and / or connect with notation
{
	_splices.Add(new Splice(splice));
}

int ProofInput::getSpliceCount() const
{
	return _splices.GetSize();
}

const Splice* ProofInput::getSplice(int index) const
{
	if (index > _splices.GetUpperBound())
		return NULL;

	return _splices.GetAt(index);
}			

const MusicRules& ProofInput::getMusicRules() const
{
	return _musicRules;
}

void ProofInput::addMusicRule(const MusicRules& musicRules)
{
	for (int i=0;i<musicRules.GetSize();i++)
		_musicRules.Add(musicRules.GetAt(i));

}

void ProofInput::setRowLimit(int rowLimit)
{
	_rowLimit = rowLimit;
}

int ProofInput::getRowLimit() const
{
	return _rowLimit;
}

void ProofInput::setLeadLimit(int leadLimit)
{
	_leadLimit = leadLimit;
}

int ProofInput::getLeadLimit() const
{
	return _leadLimit;
}

void ProofInput::setPartLimit(int partLimit)
{
	_partLimit = partLimit;
}

int ProofInput::getPartLimit() const
{
	return _partLimit;
}

void ProofInput::setCircularCourseLimit(int circularCourseLimit)
{
	_circularCourseLimit = circularCourseLimit;
}

int ProofInput::getCircularCourseLimit() const
{
	return _circularCourseLimit;
}
	
void ProofInput::setWrapCalls(bool wrapCalls)
{
	_wrapCalls = wrapCalls;
}

bool ProofInput::getWrapCalls() const
{
	return _wrapCalls;
}

