#include "StdAfx.h"
#include "MethodAnalysis.h"
#include "Method.h"
#include "FalseRow.h"

MethodAnalysis::MethodAnalysis(const Method& method) :
_method(method), 
_requestedBlockCount(1),
_completeBlocks(0),
_partialBlocks(0),
_commonRowCount(0),
_falseRowCount(0),
_callFrom(-1), 
_bellToFollow(-1)
{
}

MethodAnalysis::~MethodAnalysis(void)
{
	//clean up the music results
	for (int i=0;i<_musicResults.GetSize();i++)
		delete _musicResults.GetAt(i);
	_musicResults.RemoveAll();

	//clean false rows.
	for (int i=0;i<_falseRows.GetSize();i++)
		delete _falseRows.GetAt(i);
	_falseRows.RemoveAll();
}

bool MethodAnalysis::isTrue() const
{
	//todo set this value
	//return (_falseRowCount == 0);
	return true;
}

const CStringArray& MethodAnalysis::getNotationStrings() const
{
	return _notationStrings;
}

int MethodAnalysis::getCallFrom() const
{
	//todo set this value
	return _callFrom;	
}

const CString& MethodAnalysis::getName() const
{
	//todo set this value
	return _name;
}

const CallMadeArray& MethodAnalysis::getCallsMade() const 
{
	//todo set this value
	return _callsMade;
}

const SplicesMadeArray& MethodAnalysis::getSplicesMade() const 
{
	//todo set this value
	return _splicesMade;
}	  

const MusicResults& MethodAnalysis::getMusicResults() const 
{
	return _musicResults;
}


int MethodAnalysis::getTotalBlocksCount() const
{
	int blocks = _partialBlocks + _completeBlocks;

	if ((blocks == 0)&&(_method.getRowCount() > 0))
		blocks = 1;

	return blocks;
}

int MethodAnalysis::getCompleteBlockCount() const
{
	return _completeBlocks;
}

int MethodAnalysis::getPartialBlockCount()const
{
	return _partialBlocks;
}

const Ints& MethodAnalysis::getBlockRowCounts() const
{
	return _blockRowCounts;
}

int MethodAnalysis::getCommonRowCount() const
{
	return _commonRowCount;
}

int MethodAnalysis::getFalseRowCount()  const
{
	return _falseRowCount;
}

int MethodAnalysis::getBellToFollow()
{
	return _bellToFollow;
}

const CString& MethodAnalysis::getNotationDisplayString() const
{
	//todo set this up/
	return _notationDisplayString;
}

const FalseRowArr& MethodAnalysis::getFalseRows() const 
{
	return _falseRows;
}

int MethodAnalysis::getRequestedBlockCount() const
{
	return _requestedBlockCount;
}