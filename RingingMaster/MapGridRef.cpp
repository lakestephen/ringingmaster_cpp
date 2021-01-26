// MapGridRef.cpp: implementation of the MapGridRef class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapGridRef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MapGridRef::MapGridRef() :
_mapGridRefType(mgrt_invalid), 
_eastings(0), 
_northings(0)
{
}

MapGridRef::MapGridRef(int eastings, int northings, MapGridRefPrecision mapGridRefPrecision) :
_mapGridRefType(mgrt_invalid), 
_eastings(0), 
_northings(0)
{

	setGridRef(eastings, northings, mapGridRefPrecision);
}

MapGridRef::~MapGridRef()
{

}


BOOL MapGridRef::setNumeric(CString tenNumeric, MapGridRefPrecision /*mapGridRefPrecision*/)
{
//	ASSERT(mapGridRefPrecision == mgrp_100_meter);

	// Monks Kirby 0446302832

	tenNumeric.TrimLeft();                                                                                                                       
	tenNumeric.TrimRight(); 
	while (tenNumeric.Replace(" ", ""));

	if (tenNumeric.GetLength() != 10)
		return FALSE;

	CString eastings = tenNumeric.Left(5);
	CString northings = tenNumeric.Right(5);

	int iEastings = atoi(eastings);
	int iNorthings = atoi(northings);

	return setGridRef(iEastings, iNorthings);
}

BOOL MapGridRef::setAlphaNumeric(CString ref, MapGridRefPrecision /*mapGridRefPrecision*/)
{
//	ASSERT(mapGridRefPrecision == mgrp_100_meter);

	//Monks Kirby SP463862

	ref.TrimLeft();                                                                                                                       
	ref.TrimRight(); 
	while (ref.Replace(" ", ""));

	if (ref.GetLength() != 8)
		return FALSE;

	CString alpha = ref.Left(2);
	alpha.MakeUpper();

	int iEastings, iNorthings;

	if (!get100KBoxFrom2Alpha(alpha, iEastings, iNorthings))
		return FALSE;

	CString eastings = ref.Mid(2,3);
	CString northings = ref.Right(3);

	iEastings += atoi(eastings);
	iNorthings += atoi(northings);

	return setGridRef(iEastings, iNorthings);                                                                                                                                 
	
}                                                                                                                                       

CString MapGridRef::getNumeric(MapGridRefPrecision /*mapGridRefPrecision*/) const
{
//	ASSERT(mapGridRefPrecision == mgrp_100_meter);

	CString str;
	str.Format("%5d%5d", _eastings, _northings);

	while(str.Replace(" ", "0")); 

	return str;
}

BOOL MapGridRef::isValidOSGridRef() const
{
	return (_mapGridRefType == mgrt_osGrid);
}

BOOL MapGridRef::isValidNonOSGridRef() const
{
	return (_mapGridRefType == mgrt_nonOSGrid);
}

void MapGridRef::setNonOSRef(CString ref)
{
	_nonOSGridRef = ref;
	_mapGridRefType = mgrt_nonOSGrid;
}

void MapGridRef::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
		ar << _eastings;
		ar << _northings;
		ar << _mapGridRefType;
		ar << _nonOSGridRef;
	}
	else
	{
		ar >> version;  
		ar >> _eastings;
		ar >> _northings;
		int value;
		ar >> value;
		_mapGridRefType = (MapGridRefType)value;
		ar >> _nonOSGridRef;
	}
}

BOOL MapGridRef::setGridRef(int eastings, int northings, MapGridRefPrecision /*mapGridRefPrecision */)
{
//	ASSERT(mapGridRefPrecision == mgrp_100_meter);

	if ((eastings <= 0)||(northings <=0))
		return FALSE;

	_eastings = eastings;
	_northings = northings;

	_mapGridRefType = mgrt_osGrid;

	return TRUE;

}

int MapGridRef::getEastings(MapGridRefPrecision /*mapGridRefPrecision*/) const
{
//	ASSERT(mapGridRefPrecision == mgrp_100_meter);
	ASSERT(isValidOSGridRef());
	
	if (isValidOSGridRef())
		return _eastings;
	else
		return 0;
}

int MapGridRef::getNorthings(MapGridRefPrecision /*mapGridRefPrecision*/) const
{
//	ASSERT(mapGridRefPrecision == mgrp_100_meter);
	ASSERT(isValidOSGridRef());
	
	if (isValidOSGridRef())
		return _northings;
	else
		return 0;
}

int MapGridRef::getEastingsDiff100M(const MapGridRef &mapGridRef) const
{
	if ((isValidOSGridRef())&&(mapGridRef.isValidOSGridRef()))
	{
		return getEastings() - mapGridRef.getEastings();
	}

	return 0;
}

int MapGridRef::getNorthingsDiff100M(const MapGridRef &mapGridRef) const
{
	if ((isValidOSGridRef())&&(mapGridRef.isValidOSGridRef()))
	{
		return getNorthings() - mapGridRef.getNorthings();
	}

	return 0;
}

CString MapGridRef::getAlphaNumeric(MapGridRefPrecision /*mapGridRefPrecision*/)	const
{
//	ASSERT(mapGridRefPrecision == mgrp_100_meter);

	
	CString alpha;
	if (get2Alpha100KBox(_eastings, _northings, alpha))
	{
		int eastings = ((_eastings/1000)*1000);
		if (eastings == 0)
			eastings = _eastings;
		else 
			eastings = _eastings % eastings;

		int northings = ((_northings/1000)*1000);
		if (northings == 0)
			northings = _northings;
		else 
			northings = _northings % northings;

	
		CString str;
		str.Format("%s%3d%3d", alpha, eastings, northings);

		while(str.Replace(" ", "0")); 

		return str;

	}

	return "";
}

BOOL MapGridRef::get100KBoxFrom2Alpha(CString alpha, int & eastings, int & northings)
{
	if (alpha.GetLength() != 2)
		return FALSE;

	eastings = -1;
	northings = -1; 

	alpha.MakeUpper();
	if      (alpha == "HL") {eastings = 00; northings = 12;}
	else if (alpha == "HM") {eastings = 01; northings = 12;}
	else if (alpha == "HN") {eastings = 02; northings = 12;}
	else if (alpha == "HO") {eastings = 03; northings = 12;}
	else if (alpha == "HP") {eastings = 04; northings = 12;}
	else if (alpha == "JL") {eastings = 05; northings = 12;}
	else if (alpha == "JM") {eastings = 06; northings = 12;}

	else if (alpha == "HQ") {eastings = 00; northings = 11;}
	else if (alpha == "HR") {eastings = 01; northings = 11;}
	else if (alpha == "HS") {eastings = 02; northings = 11;}
	else if (alpha == "HT") {eastings = 03; northings = 11;}
	else if (alpha == "HU") {eastings = 04; northings = 11;}
	else if (alpha == "JQ") {eastings = 05; northings = 11;}
	else if (alpha == "JR") {eastings = 06; northings = 11;}

	else if (alpha == "HV") {eastings = 00; northings = 10;}
	else if (alpha == "HW") {eastings = 01; northings = 10;}
	else if (alpha == "HX") {eastings = 02; northings = 10;}
	else if (alpha == "HY") {eastings = 03; northings = 10;}
	else if (alpha == "HZ") {eastings = 04; northings = 10;}
	else if (alpha == "JV") {eastings = 05; northings = 10;}
	else if (alpha == "JW") {eastings = 06; northings = 10;}

	else if (alpha == "NA") {eastings = 00; northings = 9;}
	else if (alpha == "NB") {eastings = 01; northings = 9;}
	else if (alpha == "NC") {eastings = 02; northings = 9;}
	else if (alpha == "ND") {eastings = 03; northings = 9;}
	else if (alpha == "NE") {eastings = 04; northings = 9;}
	else if (alpha == "OA") {eastings = 05; northings = 9;}
	else if (alpha == "OB") {eastings = 06; northings = 9;}

	else if (alpha == "NF") {eastings = 00; northings = 8;}
	else if (alpha == "NG") {eastings = 01; northings = 8;}
	else if (alpha == "NH") {eastings = 02; northings = 8;}
	else if (alpha == "NJ") {eastings = 03; northings = 8;}
	else if (alpha == "NK") {eastings = 04; northings = 8;}
	else if (alpha == "OF") {eastings = 05; northings = 8;}
	else if (alpha == "OG") {eastings = 06; northings = 8;}

	else if (alpha == "NL") {eastings = 00; northings = 07;}
	else if (alpha == "NM") {eastings = 01; northings = 07;}
	else if (alpha == "NN") {eastings = 02; northings = 07;}
	else if (alpha == "NO") {eastings = 03; northings = 07;}
	else if (alpha == "NP") {eastings = 04; northings = 07;}
	else if (alpha == "OL") {eastings = 05; northings = 07;}
	else if (alpha == "OM") {eastings = 06; northings = 07;}

	else if (alpha == "NQ") {eastings = 00; northings = 06;}
	else if (alpha == "NR") {eastings = 01; northings = 06;}
	else if (alpha == "NS") {eastings = 02; northings = 06;}
	else if (alpha == "NT") {eastings = 03; northings = 06;}
	else if (alpha == "NU") {eastings = 04; northings = 06;}
	else if (alpha == "OQ") {eastings = 05; northings = 06;}
	else if (alpha == "OR") {eastings = 06; northings = 06;}

	else if (alpha == "NV") {eastings = 00; northings = 05;}
	else if (alpha == "NW") {eastings = 01; northings = 05;}
	else if (alpha == "NX") {eastings = 02; northings = 05;}
	else if (alpha == "NY") {eastings = 03; northings = 05;}
	else if (alpha == "NZ") {eastings = 04; northings = 05;}
	else if (alpha == "OV") {eastings = 05; northings = 05;}
	else if (alpha == "OW") {eastings = 06; northings = 05;}

	else if (alpha == "SA") {eastings = 00; northings = 04;}
	else if (alpha == "SB") {eastings = 01; northings = 04;}
	else if (alpha == "SC") {eastings = 02; northings = 04;}
	else if (alpha == "SD") {eastings = 03; northings = 04;}
	else if (alpha == "SE") {eastings = 04; northings = 04;}
	else if (alpha == "TA") {eastings = 05; northings = 04;}
	else if (alpha == "TB") {eastings = 06; northings = 04;}

	else if (alpha == "SF") {eastings = 00; northings = 03;}
	else if (alpha == "SG") {eastings = 01; northings = 03;}
	else if (alpha == "SH") {eastings = 02; northings = 03;}
	else if (alpha == "SJ") {eastings = 03; northings = 03;}
	else if (alpha == "SK") {eastings = 04; northings = 03;}
	else if (alpha == "TF") {eastings = 05; northings = 03;}
	else if (alpha == "TG") {eastings = 06; northings = 03;}

	else if (alpha == "SL") {eastings = 00; northings = 02;}
	else if (alpha == "SM") {eastings = 01; northings = 02;}
	else if (alpha == "SN") {eastings = 02; northings = 02;}
	else if (alpha == "SO") {eastings = 03; northings = 02;}
	else if (alpha == "SP") {eastings = 04; northings = 02;}
	else if (alpha == "TL") {eastings = 05; northings = 02;}
	else if (alpha == "TM") {eastings = 06; northings = 02;}

	else if (alpha == "SQ") {eastings = 00; northings = 01;}
	else if (alpha == "SR") {eastings = 01; northings = 01;}
	else if (alpha == "SS") {eastings = 02; northings = 01;}
	else if (alpha == "ST") {eastings = 03; northings = 01;}
	else if (alpha == "SU") {eastings = 04; northings = 01;}
	else if (alpha == "TQ") {eastings = 05; northings = 01;}
	else if (alpha == "TR") {eastings = 06; northings = 01;}

	else if (alpha == "SV") {eastings = 00; northings = 00;}
	else if (alpha == "SW") {eastings = 01; northings = 00;}
	else if (alpha == "SX") {eastings = 02; northings = 00;}
	else if (alpha == "SY") {eastings = 03; northings = 00;}
	else if (alpha == "SZ") {eastings = 04; northings = 00;}
	else if (alpha == "TV") {eastings = 05; northings = 00;}
	else if (alpha == "TW") {eastings = 06; northings = 00;}



	if ((eastings == -1)||(northings == -1))
		return FALSE;

	eastings *= 1000;
	northings *= 1000;

	return TRUE;
}


//pass in the resolution down to 100M
BOOL MapGridRef::get2Alpha100KBox(int eastings, int northings, CString& alpha)
{
	//strip down do the 100 k values (ROUNDING DOWN
	eastings /=1000;
	northings/=1000;

	alpha = "";

	// 00,00 is bottom left

	switch (northings)
	{
	case 12:
		switch (eastings)
		{
		case 00:	alpha = "HL"	;break;
		case 01:	alpha = "HM"	;break;
		case 02:	alpha = "HN"	;break;
		case 03:	alpha = "HO"	;break;
		case 04:	alpha = "HP"	;break;
		case 05:	alpha = "JL"	;break;
		case 06:	alpha = "JM"	;break;
		}
		break;
	case 11:
		switch (eastings)
		{
		case 00:	alpha = "HQ"	;break;
		case 01:	alpha = "HR"	;break;
		case 02:	alpha = "HS"	;break;
		case 03:	alpha = "HT"	;break;
		case 04:	alpha = "HU"	;break;
		case 05:	alpha = "JQ"	;break;
		case 06:	alpha = "JR"	;break;
		}
		break;
	case 10:
		switch (eastings)
		{
		case 00:	alpha = "HV"	;break;
		case 01:	alpha = "HW"	;break;
		case 02:	alpha = "HX"	;break;
		case 03:	alpha = "HY"	;break;
		case 04:	alpha = "HZ"	;break;
		case 05:	alpha = "JV"	;break;
		case 06:	alpha = "JW"	;break;
		}
		break;
	case 9:
		switch (eastings)
		{
		case 00:	alpha = "NA"	;break;
		case 01:	alpha = "NB"	;break;
		case 02:	alpha = "NC"	;break;
		case 03:	alpha = "ND"	;break;
		case 04:	alpha = "NE"	;break;
		case 05:	alpha = "OA"	;break;
		case 06:	alpha = "OB"	;break;
		}
		break;
	case 8:
		switch (eastings)
		{
		case 00:	alpha = "NF"	;break;
		case 01:	alpha = "NG"	;break;
		case 02:	alpha = "NH"	;break;
		case 03:	alpha = "NJ"	;break;
		case 04:	alpha = "NK"	;break;
		case 05:	alpha = "OF"	;break;
		case 06:	alpha = "OG"	;break;
		}
		break;
	case 07:
		switch (eastings)
		{
		case 00:	alpha = "NL"	;break;
		case 01:	alpha = "NM"	;break;
		case 02:	alpha = "NN"	;break;
		case 03:	alpha = "NO"	;break;
		case 04:	alpha = "NP"	;break;
		case 05:	alpha = "OL"	;break;
		case 06:	alpha = "OM"	;break;
		}
		break;
	case 06:
		switch (eastings)
		{
		case 00:	alpha = "NQ"	;break;
		case 01:	alpha = "NR"	;break;
		case 02:	alpha = "NS"	;break;
		case 03:	alpha = "NT"	;break;
		case 04:	alpha = "NU"	;break;
		case 05:	alpha = "OQ"	;break;
		case 06:	alpha = "OR"	;break;
		}
		break;
	case 05:
		switch (eastings)
		{
		case 00:	alpha = "NV"	;break;
		case 01:	alpha = "NW"	;break;
		case 02:	alpha = "NX"	;break;
		case 03:	alpha = "NY"	;break;
		case 04:	alpha = "NZ"	;break;
		case 05:	alpha = "OV"	;break;
		case 06:	alpha = "OW"	;break;
		}
		break;
	case 04:
		switch (eastings)
		{
		case 00:	alpha = "SA"	;break;
		case 01:	alpha = "SB"	;break;
		case 02:	alpha = "SC"	;break;
		case 03:	alpha = "SD"	;break;
		case 04:	alpha = "SE"	;break;
		case 05:	alpha = "TA"	;break;
		case 06:	alpha = "TB"	;break;
		}
		break;
	case 03:
		switch (eastings)
		{
		case 00:	alpha = "SF"	;break;
		case 01:	alpha = "SG"	;break;
		case 02:	alpha = "SH"	;break;
		case 03:	alpha = "SJ"	;break;
		case 04:	alpha = "SK"	;break;
		case 05:	alpha = "TF"	;break;
		case 06:	alpha = "TG"	;break;
		}
		break;
	case 02:
		switch (eastings)
		{
		case 00:	alpha = "SL"	;break;
		case 01:	alpha = "SM"	;break;
		case 02:	alpha = "SN"	;break;
		case 03:	alpha = "SO"	;break;
		case 04:	alpha = "SP"	;break;
		case 05:	alpha = "TL"	;break;
		case 06:	alpha = "TM"	;break;
		}
		break;
	case 01:
		switch (eastings)
		{
		case 00:	alpha = "SQ"	;break;
		case 01:	alpha = "SR"	;break;
		case 02:	alpha = "SS"	;break;
		case 03:	alpha = "ST"	;break;
		case 04:	alpha = "SU"	;break;
		case 05:	alpha = "TQ"	;break;
		case 06:	alpha = "TR"	;break;
		}
		break;
	case 00:
		switch (eastings)
		{
		case 00:	alpha = "SV"	;break;
		case 01:	alpha = "SW"	;break;
		case 02:	alpha = "SX"	;break;
		case 03:	alpha = "SY"	;break;
		case 04:	alpha = "SZ"	;break;
		case 05:	alpha = "TV"	;break;
		case 06:    alpha = "TW"	;break;
		}
		break;
	}

	return (alpha.GetLength()==2);
}

CString MapGridRef::getDisplayGridRef() const
{
	if (isValidOSGridRef())
	{
		return getAlphaNumeric();
	}
	else if (isValidNonOSGridRef())
	{
		return getNonOSGridRef();
	}
	else		
	{
		return "";
	}
}

CString MapGridRef::getNonOSGridRef() const
{
	ASSERT(isValidNonOSGridRef());

	if (isValidNonOSGridRef())
		return _nonOSGridRef;
	else
		return "";
}

BOOL MapGridRef::setGridRef(CString ref, MapGridRefPrecision mapGridRefPrecision )
{
	ASSERT(mapGridRefPrecision == mgrp_100_meter);
	if (ref.IsEmpty())
		return FALSE;
	else if (setAlphaNumeric(ref, mapGridRefPrecision))
		return TRUE;
	else if (setNumeric(ref, mapGridRefPrecision))
		return TRUE;
	{
		setNonOSRef(ref);
		return TRUE;
	}
}
 
CString MapGridRef::toString() const
{
	CString str;

	str.Format("eastings: %d, northings %d", _eastings, _northings);

	return str;
}

BOOL MapGridRef::operator ==(const MapGridRef &mapGridRef)
{
	if (_mapGridRefType == mapGridRef._mapGridRefType)
	{
		switch (_mapGridRefType)
		{
		case mgrt_invalid:
			return TRUE;
			
		case mgrt_osGrid:
			return ((_eastings  == mapGridRef._eastings)&&(_northings == mapGridRef._northings));

		case mgrt_nonOSGrid:
			return (_nonOSGridRef == mapGridRef._nonOSGridRef);
		}
	}

	return FALSE;
}
