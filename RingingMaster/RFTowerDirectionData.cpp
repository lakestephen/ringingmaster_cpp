// RFTowerDirectionData.cpp: implementation of the RFTowerDirectionData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "RFTowerDirectionData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RFTowerDirectionData::RFTowerDirectionData() :
_rFTowerDirectionType(rftdit_home_tower),
_tower(1)
{

}

RFTowerDirectionData::RFTowerDirectionData(RFTowerDirectionData& rFTowerDirectionData) :
_tower(rFTowerDirectionData._tower),
_gridLocation(rFTowerDirectionData._gridLocation),
_rFTowerDirectionType(rFTowerDirectionData._rFTowerDirectionType)

{
	for (int i=0;i<rFTowerDirectionData._directions.GetSize();i++)
	{
		_directions.Add(rFTowerDirectionData._directions.GetAt(i));
	}	
}


RFTowerDirectionData::~RFTowerDirectionData()
{

}

void RFTowerDirectionData::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _directions.GetSize();
		for (int i=0;i<_directions.GetSize();i++)
			ar << _directions.GetAt(i);

		ar << _tower;
		_gridLocation.Serialize(ar);
		ar << _rFTowerDirectionType;

	}
	else
	{
		_directions.RemoveAll();
		ar >> version;
		int size;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			int value;
			ar >> value;
			_directions.Add((Direction)value);
		}
		ar >> _tower;
		_gridLocation.Serialize(ar);
		int value;
		ar >> value;
		_rFTowerDirectionType = (RFTowerDirectionType)value;
	}
}


