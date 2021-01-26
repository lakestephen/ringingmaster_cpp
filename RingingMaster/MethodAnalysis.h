#pragma once

#include "MusicResults.h"

class Method;
class FalseRow;
class MultiBlockFalseRow;

typedef CArray<CStringArray*, CStringArray*> CallMadeArray;
typedef CArray<CStringArray*, CStringArray*> SplicesMadeArray;
typedef CArray <FalseRow*, FalseRow*> FalseRowArr;
typedef CArray <MultiBlockFalseRow*, MultiBlockFalseRow*> MultiBlockFalseRows;

class MethodAnalysis
{
public:
	MethodAnalysis(const Method& method);
	~MethodAnalysis(void);

	bool isTrue() const;
	const CStringArray& getNotationStrings() const ;
	int getCallFrom() const;
	const CString& getName() const;
	const CallMadeArray& getCallsMade() const;
	const SplicesMadeArray& getSplicesMade() const;
	const MusicResults& getMusicResults()const;
	int getRequestedBlockCount() const;
	int getPartialBlockCount() const;
	int getCompleteBlockCount() const;
	int getTotalBlocksCount() const;
	const Ints& getBlockRowCounts() const;
	int getCommonRowCount() const;
	int getFalseRowCount() const;
	int getBellToFollow();
	const CString& getNotationDisplayString() const;
	const FalseRowArr& getFalseRows() const;

protected:

	//our parent method
	const Method& _method;

	CStringArray _notationStrings;//todo populate this 
	int _bellToFollow;
	int _callFrom;
	CString _name;
	CString _notationDisplayString;
	CallMadeArray _callsMade;
	SplicesMadeArray _splicesMade;

	//for reporting
	FalseRowArr _falseRows;
	int _falseRowCount;

	Ints _blockRowCounts;	 //number of rows in each block i.e. 3 rows appear in two blocks 
	int _requestedBlockCount;
	int _completeBlocks;
	int _partialBlocks;
	int _commonRowCount;	

	MusicResults _musicResults;


	friend class ProofAnalysisEngine; 
};
