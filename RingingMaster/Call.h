#pragma once

#include "Variance.h"
#include "ParseData.h"

class Call  
{
public:
	int getMultiplyer();
	void incrementMultiplyer();
	ParseType getPositionParseType() const;
	void setVariance(Variance variance);
	const Variance& getVariance() const;
	void setComopositionRow(int row);
#ifdef _DEBUG
	void Dump();
#endif
	Call(ParseType positionParseType, Variance variance, int compositionRow, CString call, CString position = "");
	Call(const Call& call);
							
	virtual ~Call();

	const CString& getCall() const    {return _call;}
	const CString& getPosition() const {return _position;}
	int getCompositionRow() {return _compositionRow;}
	bool operator ==(const Call &call) const;

protected:
	CString _call;
	CString _position;
	int _compositionRow;
	Variance _variance;
	ParseType _positionParseType;
	int _multiplyer; //todo only used when tidying a touch object
};

typedef CArray<Call*, Call*> Calls; 

