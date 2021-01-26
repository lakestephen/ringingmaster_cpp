// DoveTowerPersistExtra.cpp: implementation of the DoveTowerPersistExtra class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoveTowerPersistExtra.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DoveTowerPersistExtra::DoveTowerPersistExtra() :
_cwt(0),
_quarters(0),
_lb(0)
{

}

DoveTowerPersistExtra::~DoveTowerPersistExtra()
{

}

void DoveTowerPersistExtra::calculateWeight()
{
	int pounds = (112 * _cwt) + (_quarters * 28) + _lb;
	_weight = (pounds / 2.205);
}
