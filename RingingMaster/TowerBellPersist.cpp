// TowerBellPersist.cpp: implementation of the TowerBellPersist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerBellPersist.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TowerBellPersist::TowerBellPersist() :
_posX(0),
_posY(0),
_posZ(0)
{

}

TowerBellPersist::~TowerBellPersist()
{

}


		   

bool TowerBellPersist::operator !=(const TowerBellPersist &towerBell)
{
	return !(operator == (towerBell));
}


bool TowerBellPersist::operator ==(const TowerBellPersist &towerBell)
{
	if (_hand != towerBell._hand)	return false;
	if (_back != towerBell._back)	return false;

	if (_posX != towerBell._posX)	return false;
	if (_posY != towerBell._posY)	return false;
	if (_posZ != towerBell._posZ)	return false;

	return true;
}

//started at release 3.01
void TowerBellPersist::Serialize(CArchive &ar)
{
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		_hand.Serialize(ar);
		_back.Serialize(ar);
		ar << _posX;
		ar << _posY;
		ar << _posZ;
	}
	else
	{
		ar >> version;

		TRACE("Deserialising TowerBellPersist Version %d \r\n", version);

		_hand.Serialize(ar);
		_back.Serialize(ar);
		ar >> _posX;
		ar >> _posY;
		ar >> _posZ;
	}
}




//meters from the listener
//x is left right
//y is up down
//z is front back

bool TowerBellPersist::isSettingPos() const
{
	return ((_posX != 0)||
			(_posY != 0)||
			(_posZ != 0));
}

double TowerBellPersist::getPosX() const
{
	return _posX;
}

double TowerBellPersist::getPosY() const
{
	return _posY;
}

double TowerBellPersist::getPosZ() const
{
	return _posZ;
}

void TowerBellPersist::setPosX(double x)
{
	_posX = x;
}

void TowerBellPersist::setPosY(double y)
{
	_posY = y;

}

void TowerBellPersist::setPosZ(double z)
{
	_posZ = z;
}


const TowerBellItem& TowerBellPersist::getTowerBellItem(Stroke stroke) const
{
 	if (stroke == back)
		return _back;
	else
		return _hand;
}
 
TowerBellItem& TowerBellPersist::getTowerBellItem(Stroke stroke)
{
 	if (stroke == back)
		return _back;
	else
		return _hand;
}
