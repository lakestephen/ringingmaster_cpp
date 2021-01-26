// RFTowerDistanceData.cpp: implementation of the RFTowerDistanceData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "RFTowerDistanceData.h"
#include "RecordTower.h"
#include "RecordManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RFTowerDistanceData::RFTowerDistanceData() :
_rFTowerDistanceLogic(rftdl_less_than),
_rFTowerDistanceType(rftdt_home_tower),
_distance(10),
_tower(1)
{

}

RFTowerDistanceData::~RFTowerDistanceData()
{

}

void RFTowerDistanceData::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _rFTowerDistanceType;
		ar << _rFTowerDistanceLogic;
		ar << _distance;
		ar << _tower;
		_gridLocation.Serialize(ar);;
	}
	else
	{
		ar >> version;
		int value;
		ar >> value;
		_rFTowerDistanceType = (RFTowerDistanceType)value;;
		ar >> value;
		_rFTowerDistanceLogic = (RFTowerDistanceLogic)value;
		ar >> _distance;
		ar >> _tower;
		_gridLocation.Serialize(ar);;
	}
}



double RFTowerDistanceData::getDistanceAsKM() const
{
	int distanceType = app()->GetProfileInt("Tower Database", "Distance", 0);
	if (distanceType == 0)
		return GlobalFunctions::milesToKM(_distance);
	else
		return _distance; //leave as KM	  
}
