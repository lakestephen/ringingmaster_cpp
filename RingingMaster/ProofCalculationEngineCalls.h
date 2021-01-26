#pragma once
#include "proofcalculationengine.h"

class Call;
class CallNotationMap;

class ProofCalculationEngineCalls :
	public ProofCalculationEngine
{
public:
	ProofCalculationEngineCalls(const ProofInput& proofInput, Method& method);
	virtual ~ProofCalculationEngineCalls(void);

protected:
	void preCalculate();
	bool moveOn();
	virtual void canStartCall() = 0;
	void moveOnCall();
	Ints* getNextNotationRow();
	void incrementCallIndex( );
	void createCallNotationMaps();
	virtual bool setNotationFromName(CString name);

	CArray <CallNotationMap*, CallNotationMap*> _callNotationMaps;
	CallNotationMap* _curCallNotMap;							  

	Notation* _callNotation;//pointer to the current call Notation if any	
	int _callNotationIndex;	//index into the call notation

	const Call* _call;
	int _callIndex;
	int _callSubIndex;	  
	int _partIndex;		
};
