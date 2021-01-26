#pragma once
#include "proofcalculationenginecalls.h"

class CallPositionMap;


class ProofCalculationEngineCallsCourseBased :
	public ProofCalculationEngineCalls
{
public:
	ProofCalculationEngineCallsCourseBased(const ProofInput& proofInput, Method& method);
	virtual ~ProofCalculationEngineCallsCourseBased(void);

protected:
	void preCalculate();
	void canStartCall();

	void createCallPositionMaps();

	CArray <CallPositionMap*, CallPositionMap*> _callPositionMaps;
	CallPositionMap* _curCallPosMap;

};
