// RFTowerDistanceData.h: interface for the RFTowerDistanceData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFTowerDistanceData_H__8382538D_265A_44A6_BEAC_0EC398478BAA__INCLUDED_)
#define AFX_RFTowerDistanceData_H__8382538D_265A_44A6_BEAC_0EC398478BAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapGridRef.h"

enum RFTowerDistanceType 
{
	rftdt_home_tower, 
	rftdt_specific_tower,
	rftdt_grid_ref
};

enum RFTowerDistanceLogic 
{
	rftdl_greater_than,
	rftdl_less_than,
};

class RFTowerDistanceData  
{
public:
	double getDistanceAsKM() const;
	RFTowerDistanceData();
	virtual ~RFTowerDistanceData();
	void Serialize(CArchive& ar);

	RFTowerDistanceType _rFTowerDistanceType; 
	RFTowerDistanceLogic _rFTowerDistanceLogic;
	int _distance;
	int _tower;
	MapGridRef _gridLocation;
};

#endif // !defined(AFX_RFTowerDistanceData_H__8382538D_265A_44A6_BEAC_0EC398478BAA__INCLUDED_)
