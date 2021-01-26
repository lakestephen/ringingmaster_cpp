// RowPositionition.cpp: implementation of the RowPositionition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodPosition.h"
#include "TouchDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MethodPosition::MethodPosition() :
_place(-1),
_bell(-1),
_leadLine(-1),
_stroke(hand)
{
}


void MethodPosition::setInvalid()
{
	RowPosition::setInvalid();

	_place = -1;
	_bell = -1;
	_leadLine = -1;
	_stroke = hand;
}

bool MethodPosition::setRow(Method* method, int lead, int leadRow, bool rollOver) 
{
	ASSERT(method);
	//have we rolled over?
	bool res = RowPosition::setRow(method, lead, leadRow, rollOver);
	if ((res)&&
		(_lead == lead +1)&&
		(_leadRow == 0))
	{
		_leadLine = 0;
	}

	return res;
}

int MethodPosition::getPlace()
{
	return _place;
}

int MethodPosition::getBell()
{
	return _bell;
}

Stroke MethodPosition::getStroke()
{
	return _stroke;
}

void MethodPosition::setLeadLine(int leadLine)
{
	_leadLine = leadLine;
}

void MethodPosition::setPlace(int place)
{
	_place = place;
}

void MethodPosition::setBell(int bell)
{
	_bell = bell;
}

void MethodPosition::setStroke(Stroke stroke)
{
	_stroke = stroke;
}

int MethodPosition::getLeadLine()
{
	return _leadLine;
}

CString MethodPosition::format(RowPositionFormatType type)
{
	CString display;
	switch (type)
	{
	case rpft_mp_place:
		display.Format("Place %d", getPlace() + 1);
		break;
	case rpft_mp_bell:
		display.Format("Bell %c", GlobalFunctions::bellNumbersToChar(getBell()));
		break;
	case rpft_mp_stroke:
		display = (getStroke()== hand)?Handstroke:Backstroke;
		break;
	default:
		display = RowPosition::format(type);
	}
	return display;
}