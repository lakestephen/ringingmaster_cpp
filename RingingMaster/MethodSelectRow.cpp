// MethodSelectRow.cpp: implementation of the MethodSelectRow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodSelectRow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MethodSelectRow::MethodSelectRow() :
MethodSelect(),
_callPositions(NULL)
{

}

MethodSelectRow::~MethodSelectRow()
{

}

void MethodSelectRow::setCallPositions(Ints* callPositions)
{
	ASSERT(callPositions != NULL);
	_callPositions = callPositions;
}

Ints* MethodSelectRow::getCallPositions()
{
	return _callPositions;

}
