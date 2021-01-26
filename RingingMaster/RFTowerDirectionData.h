// RFTowerDirectionData.h: interface for the RFTowerDirectionData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFTOWERDIRECTIONDATA_H__D2A27C34_5614_11D7_B694_000255162CF0__INCLUDED_)
#define AFX_RFTOWERDIRECTIONDATA_H__D2A27C34_5614_11D7_B694_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TowerDataStatic.h"


enum RFTowerDirectionType 
{
	rftdit_home_tower, 
	rftdit_specific_tower,
	rftdit_grid_ref
};



class RFTowerDirectionData  
{
public:
	RFTowerDirectionData();
	virtual ~RFTowerDirectionData();
	RFTowerDirectionData(RFTowerDirectionData& rFTowerDirectionData);
	int _tower;
	MapGridRef _gridLocation;
	RFTowerDirectionType _rFTowerDirectionType;
	Directions _directions;

	void Serialize(CArchive& ar);
};

#endif // !defined(AFX_RFTOWERDIRECTIONDATA_H__D2A27C34_5614_11D7_B694_000255162CF0__INCLUDED_)
