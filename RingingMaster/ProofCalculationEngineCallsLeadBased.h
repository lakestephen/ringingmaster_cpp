#pragma once
#include "proofcalculationenginecalls.h"

class ProofCalculationEngineCallsLeadBased :
	public ProofCalculationEngineCalls
{
public:
	ProofCalculationEngineCallsLeadBased(const ProofInput& proofInput, Method& method);
	virtual ~ProofCalculationEngineCallsLeadBased(void);

protected:
	void canStartCall();
	virtual bool setNotationFromName(CString name);
};
