#include "stdafx.h"
#include "TowerInfo.h"



TowerInfo::TowerInfo(long key) :
SaveableObject(key)
{
}

TowerInfo::TowerInfo(CArchive& ar) :
SaveableObject(-1)
{
	ASSERT(!ar.IsStoring());
	Serialize(ar);
	ASSERT(getKey() != -1);
}

TowerInfo::TowerInfo(const TowerInfo& towerInfo) : 
SaveableObject(towerInfo)
{
	operator = (towerInfo);
}

TowerInfo::~TowerInfo()
{
}

TowerInfo& TowerInfo::operator =(const TowerInfo& towerInfo)
{
	_gridref = towerInfo._gridref;
	_county = towerInfo._county;
	_name = towerInfo._name;
	_dedication = towerInfo._dedication;
	_postcode = towerInfo._postcode;
	_groundfloor = towerInfo._groundfloor;
	_number = towerInfo._number;
	_weight = towerInfo._weight;
	_approx = towerInfo._approx;
	_note = towerInfo._note;
	_night = towerInfo._night;
	_unringable = towerInfo._unringable;
	_webpage = towerInfo._webpage;
	_country = towerInfo._country;

	return *this;
}

void TowerInfo::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;

		__super::Serialize(ar);

		_gridref.Serialize(ar);
		ar << _county;
		ar << _name;
		ar << _dedication;
		ar << _postcode;
		ar << _groundfloor;
		ar << _number;
		ar << _weight;
		ar << _approx;
		ar << _note;
		ar << _night;
		ar << _unringable;
		ar << _webpage;
		ar << _country;
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);

		__super::Serialize(ar);

		_gridref.Serialize(ar);
		ar >> _county;
		ar >> _name;
		ar >> _dedication;
		ar >> _postcode;
		ar >> _groundfloor;
		ar >> _number;
		ar >> _weight;
		ar >> _approx;
		ar >> (int&)_note;
		ar >> (int&)_night;
		ar >> _unringable;
		ar >> _webpage;
		ar >> _country;
	}
}

MapGridRef TowerInfo::getGridRef()
{
	return _gridref;
}
void TowerInfo::setGridRef(MapGridRef gridref)
{
	_gridref = gridref;
}

CString TowerInfo::getCounty()
{
	return _county;
}
void TowerInfo::setCounty(CString county)
{
	_county = county;
}

CString TowerInfo::getName()
{
	return _name;
}
void TowerInfo::setName(CString name)
{
	_name = name;
}

CString TowerInfo::getDedication()
{
	return _dedication;
}
void TowerInfo::setDedication(CString dedication)
{
	_dedication = dedication;
}

CString TowerInfo::getPostCode()
{
	return _postcode;
}
void TowerInfo::setPostCode(CString postcode)
{
	_postcode = postcode;
}

BOOL TowerInfo::getGroundFloor()
{
	return _groundfloor;
}
void TowerInfo::setGroundFloor(BOOL groundfloor)
{
	_groundfloor = groundfloor;
}

int TowerInfo::getNumber()
{
	return _number;
}
void TowerInfo::setNumber(int number)
{
	_number = number;
}

double TowerInfo::getWeight()
{
	return _weight;
}
void TowerInfo::setWeight(double weight)
{
	_weight = weight;
}

BOOL TowerInfo::getApprox()
{
	return _approx;
}
void TowerInfo::setApprox(BOOL approx)
{
	_approx = approx;
}

Note TowerInfo::getNote()
{
	return _note;
}
void TowerInfo::setNote(Note note)
{
	_note = note;
}

Night TowerInfo::getNight()
{
	return _night;
}
void TowerInfo::setNight(Night night)
{
	_night = night;
}

BOOL TowerInfo::getUnringable()
{
	return _unringable;
}
void TowerInfo::setUnringable(BOOL unringable)
{
	_unringable = unringable;
}

CString TowerInfo::getWebPage()
{
	return _webpage;
}
void TowerInfo::setWebPage(CString webpage)
{
	_webpage = webpage;
}

CString TowerInfo::getCountry()
{
	return _country;
}
void TowerInfo::setCountry(CString country)
{
	_country = country;
}

