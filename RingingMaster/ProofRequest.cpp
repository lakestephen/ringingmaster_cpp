#include "StdAfx.h"
#include "ProofRequest.h"
#include "ProofCalculationEngineCallsCourseBased.h"
#include "ProofCalculationEngineCallsLeadBased.h"
#include "MethodAnalysis.h"
#include "ProofAnalysisEngine.h"
#include "Method.h"
#include "ProofInput.h"

ProofRequest::ProofRequest(const ProofInput& proofInput, Method& method) :
_proofInput(&proofInput), 
_method(&method),
_calculationEngine(NULL), 
_weOwnData(false), 
_requestedEarlyTerminate(false)
{
	createCalculationEngine();
	createAnalysisEngine();
}

ProofRequest::ProofRequest(const ProofInput* proofInput, Method* method) :
_proofInput(proofInput), 
_method(method),
_calculationEngine(NULL), 
_analysisEngine(NULL),
_weOwnData(true),
_requestedEarlyTerminate(false)
{
	createCalculationEngine();
	createAnalysisEngine();
}

ProofRequest::~ProofRequest(void)
{
	if (_weOwnData)
	{
		delete _method;
		_method = NULL;
		delete _proofInput;
		_proofInput = NULL;
	}

	delete _calculationEngine;
	_calculationEngine = NULL;
}

void ProofRequest::createCalculationEngine()
{
	//decide what proof calculation engine we need.
	switch (_proofInput->getProofEngineType())
	{
	case pet_noCalls:
		_calculationEngine = new ProofCalculationEngine(*_proofInput, *_method);
		break;
	case pet_courseBased:
		_calculationEngine = new ProofCalculationEngineCallsCourseBased(*_proofInput, *_method);
		break;
	case pet_leadBased:
		_calculationEngine = new ProofCalculationEngineCallsLeadBased(*_proofInput, *_method);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void ProofRequest::createAnalysisEngine()
{
	if (_proofInput->getPerformAnalysis())
	{								   
		MethodAnalysis* methodAnalysis = _method->createAnalysis();
		_analysisEngine = new ProofAnalysisEngine(*_proofInput, *_method, *methodAnalysis);
	}  
}

void ProofRequest::requestEarlyTerminate()
{
	_requestedEarlyTerminate = true;
	_calculationEngine->requestEarlyTerminate();	
	if (_analysisEngine)
		_analysisEngine->requestEarlyTerminate();
}


bool ProofRequest::hasActuallyTerminated()
{
	return (_calculationEngine->hasActuallyTerminated() &&
			(!_analysisEngine || _analysisEngine->hasActuallyTerminated()));
}

bool ProofRequest::hasTerminatedEarly()
{
	return (hasActuallyTerminated()&& _requestedEarlyTerminate);
}
 
int ProofRequest::getUniqueId()
{
	if (_proofInput)
		return _proofInput->getUniqueId();

	ASSERT(FALSE);
	return -1;
}

Method* ProofRequest::getMethod()
{
	return _method;
}

void ProofRequest::takeOwnershipOfMethod()
{
	ASSERT(_method);
	ASSERT(_weOwnData);
	
	_method = NULL;
}

void ProofRequest::doProof()
{
	//calculate the rows
	_calculationEngine->doCalculate();

	//if there is a need for the analysis then do it.
	if (_analysisEngine && _method->hasAnalysis()) 
	{
		_analysisEngine->doCalculate();
	}
}


CString ProofRequest::toString()
{
	CString str;

	if (_proofInput)
		str.Format("%s (%d)", _proofInput->getName(), _proofInput->getUniqueId());

	return str;
}
