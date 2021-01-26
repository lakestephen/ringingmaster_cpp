#pragma once

class Method;
class ProofInput;
class ProofCalculationEngine;
class ProofAnalysisEngine;

class ProofRequest
{
public:
	ProofRequest(const ProofInput* proofInput, Method* method);
	ProofRequest(const ProofInput& proofInput, Method& method);
public:
	~ProofRequest(void);

	void requestEarlyTerminate();
	bool hasActuallyTerminated();
	bool hasTerminatedEarly();
	void takeOwnershipOfMethod();
	Method* getMethod();
	void doProof();
	CString toString();
	int getUniqueId();

protected:
	void createCalculationEngine();
	void createAnalysisEngine();


	const ProofInput* _proofInput;
	Method* _method;
	ProofCalculationEngine* _calculationEngine; 
	ProofAnalysisEngine* _analysisEngine;
	bool _weOwnData;
	bool _requestedEarlyTerminate;

};
