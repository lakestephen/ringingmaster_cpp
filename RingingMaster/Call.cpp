// Call.cpp: implementation of the Call class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Call.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Call::Call(ParseType positionParseType, Variance variance, int compositionRow, CString call, CString position) :
_positionParseType(positionParseType),
_variance(variance),
_call(call), 
_position(position),
_multiplyer(1),
_compositionRow(compositionRow)
{
}

Call::Call(const Call& call) :
_positionParseType(call._positionParseType),
_variance(call._variance),
_call(call._call), 
_position(call._position),
_multiplyer(call._multiplyer),
_compositionRow(call._compositionRow)
{
}

Call::~Call()
{

}
#ifdef _DEBUG
void Call::Dump()
{
	TRACE ("%s %s %s (%d) \n", _call, _position, _variance.getGridString(), _compositionRow);
}
#endif

void Call::setComopositionRow(int row)
{
	_compositionRow = row;
}

const Variance& Call::getVariance() const
{
	return _variance;
}

void Call::setVariance(Variance variance)
{
	_variance = variance;
}

ParseType Call::getPositionParseType() const
{
	return _positionParseType;
}

bool Call::operator ==(const Call &call) const
{
	return ((_call == call._call)&&
			(_position == call._position)&&
			(_variance == call._variance));

}

void Call::incrementMultiplyer()
{
	_multiplyer++;
}

int Call::getMultiplyer()
{
	return _multiplyer;
}
