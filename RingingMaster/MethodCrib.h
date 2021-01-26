// MethodCrib.h: interface for the MethodCrib class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Method.h"

class Crib;
class ProofInput;  //todo remove

class MethodCrib : public Method  
{
public:
	CribDoc* getDocument();
	void setDocument(CribDoc* cribDoc);
	Crib* getCrib();
	void create(ProofInput* proofInput);
	MethodCrib();
	virtual ~MethodCrib();

protected:
	Crib* _crib;
	CribDoc* _cribDoc;
};

