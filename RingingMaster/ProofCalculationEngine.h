#pragma once

#include "TerminateReason.h"

class ProofInput;
class Method;
class Notation;
class Row;

class ProofCalculationEngine
{
public:
	ProofCalculationEngine(const ProofInput& proofInput, Method& method);
	virtual ~ProofCalculationEngine(void);

	void doCalculate();
	
	void requestEarlyTerminate();
	bool hasRequestedTerminate();
	bool hasActuallyTerminated();  

protected:

	virtual void preCalculate();
	void calculate();
	void postCalculate();
	void requestTerminate(TerminateReason terminateReason); 

	virtual Ints* getNextNotationRow();
	virtual bool moveOn();
	virtual void moveOnSplice();

	void checkForTerminateCondition(bool checkForLeadConditions);
	void checkLeadLines();
	
	virtual bool setNotationFromName(CString name);
	
	//input parameters
	const ProofInput& _ip;
	Method& _met;

	//internal data
	bool _hasActuallyTerminated;
	bool _hasRequestedTerminate;
	Row* _endRow;  
	Row* _row;				//pointer the the current row.
	Notation* _notation;	//pointer to the currently used notation. 
	int _notationIndex;		//the row number of the current lead.
	int _blockIndex;
	int _spliceIndex;		//the index into the splice array
};
