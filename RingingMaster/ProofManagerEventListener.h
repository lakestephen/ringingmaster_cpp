// ProofManagerEventListener.h: interface for the ProofManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


class Method;

class ProofManagerEventListener  
{
public:
	ProofManagerEventListener() {};
	virtual ~ProofManagerEventListener() {};

	virtual bool proofManager_notifyProofComplete(Method* method) = 0;
};		   

typedef CArray<ProofManagerEventListener*, ProofManagerEventListener*> ProofManagerEventListeners;

