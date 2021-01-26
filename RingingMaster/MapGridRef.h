// MapGridRef.h: interface for the MapGridRef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPGRIDREF_H__B4A917F4_8CF0_11D7_B6C2_000255162CF0__INCLUDED_)
#define AFX_MAPGRIDREF_H__B4A917F4_8CF0_11D7_B6C2_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scale.h"

enum MapGridRefType
{
	mgrt_invalid,
	mgrt_osGrid,
	mgrt_nonOSGrid
};


enum MapGridRefPrecision
{
	mgrp_1_meter,
	mgrp_10_meter,
	mgrp_100_meter,
	mgrp_1_kilometer,
	mgrp_10_kilometer,
	mgrp_100_kilometer,
};


class MapGridRef 
{
public:
	MapGridRef(int eastings, int northings, MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter);
	MapGridRef();
	virtual ~MapGridRef();

	
	BOOL isValidOSGridRef() const;
	BOOL isValidNonOSGridRef() const;

	BOOL setGridRef(CString ref, MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter);
	BOOL setGridRef(int eastings, int northings, MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter);

	int getNorthings(MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter) const;
	int getEastings(MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter) const ;

	int getEastingsDiff100M(const MapGridRef& mapGridRef) const;
	int getNorthingsDiff100M(const MapGridRef& mapGridRef) const;

	CString getNonOSGridRef() const;
	CString getDisplayGridRef() const;
	
	CString getAlphaNumeric(MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter) const;
	CString getNumeric(MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter) const;

	void Serialize(CArchive &ar);
	BOOL operator ==(const MapGridRef &mapGridRef);

	CString toString() const;

protected:
	static BOOL get100KBoxFrom2Alpha(CString alpha, int& eastings, int& northings);
	static BOOL get2Alpha100KBox(int eastings, int northings, CString& alpha);

	MapGridRefType _mapGridRefType;
	int _eastings;
	int _northings;
	CString _nonOSGridRef;

	BOOL setAlphaNumeric(CString ref, MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter);
	BOOL setNumeric(CString tenDigitRef, MapGridRefPrecision mapGridRefPrecision = mgrp_100_meter);
	void setNonOSRef(CString ref);

	friend class CDoveExtractorDlg;

};

#endif // !defined(AFX_MAPGRIDREF_H__B4A917F4_8CF0_11D7_B6C2_000255162CF0__INCLUDED_)
