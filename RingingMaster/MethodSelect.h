
#pragma once


#include "Method.h"
#include "MethodPosition.h"

class ProofInput;

class MethodSelect : public Method  
{
public:
	virtual void create(ProofInput* proofInput);
	MethodSelect();
	virtual ~MethodSelect();

	MethodPosition _selectedPosition;


};

