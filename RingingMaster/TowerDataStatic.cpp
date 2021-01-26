// TowerDataStatic.cpp: implementation of the TowerDataStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TowerDataStatic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TowerDataStatic::TowerDataStatic() :
_groundFloor(FALSE)	,
_number(0),		
_weight(0.0),			
_approx(FALSE),		
_unringable(FALSE),
_note(n_none),
_night(now_none),
_country(c_england),
_index(0)
{

}

TowerDataStatic::TowerDataStatic(TowerDataStatic & towerDataStatic) :
_index(towerDataStatic._index),	
_county(towerDataStatic._county),	
_name(towerDataStatic._name),		
_dedication(towerDataStatic._dedication),
_postCode(towerDataStatic._postCode),	
_groundFloor(towerDataStatic._groundFloor),
_number(towerDataStatic._number),	
_weight(towerDataStatic._weight),		
_approx(towerDataStatic._approx),	
_note(towerDataStatic._note),		
_night(towerDataStatic._night),
_unringable(towerDataStatic._unringable),
_webPage(towerDataStatic._webPage),	
_country(towerDataStatic._country)
{
}


TowerDataStatic::~TowerDataStatic()
{

}

//set back to the default state
void TowerDataStatic::init()
{
	_index = 0;			
	_county = "";		
	_name = "";			
	_dedication = "";		
	_postCode = "";		
	_groundFloor = FALSE;	
	_number = 0;		
	_weight = 0.0;			
	_approx = FALSE;		
	_note = n_none;			
	_night = now_none;	
	_unringable = FALSE;	
	_webPage = "";		
	_country = c_england;	

}

void TowerDataStatic::operator =(TowerDataStatic &towerDataStatic)
{
	_index = towerDataStatic._index;	
	_gridRef = towerDataStatic._gridRef;
	_county = towerDataStatic._county;	
	_name = towerDataStatic._name;		
	_dedication = towerDataStatic._dedication;
	_postCode = towerDataStatic._postCode;	
	_groundFloor = towerDataStatic._groundFloor;
	_number = towerDataStatic._number;	
	_weight = towerDataStatic._weight;		
	_approx = towerDataStatic._approx;	
	_note = towerDataStatic._note;		
	_night = towerDataStatic._night;
	_unringable = towerDataStatic._unringable;
	_webPage = towerDataStatic._webPage;	
	_country = towerDataStatic._country;
}


void TowerDataStatic::Serialize(CArchive &ar)
{
	int serialize_version = 1;
	int data_version = 1;
	int value = 0;
	if (ar.IsStoring())
	{
		ar << serialize_version;  
		ar << data_version;
		ar << _index;			
		_gridRef.Serialize(ar);	
		ar << _county;		
		ar << _name;			
		ar << _dedication;		
		ar << _postCode;		
		ar << _groundFloor;	
		ar << _number;		
		ar << _weight;			
		ar << _approx;		
		ar << _note;			
		ar << _night;	
		ar << _unringable;	
		ar << _webPage;		
		ar << _country;	

	}
	else
	{
		ar >> serialize_version; //just get the version number
		ar >> data_version;
		ar >> _index;			
		_gridRef.Serialize(ar);	
		ar >> _county;		
		ar >> _name;			
		ar >> _dedication;		
		ar >> _postCode;		
		ar >> _groundFloor;	
		ar >> _number;		
		ar >> _weight;			
		ar >> _approx;		
		ar >> value;
		_note = (Note)value;	   
		ar >> value;
		_night = (Night)value;	
		ar >> _unringable;	
		ar >> _webPage;		
		ar >> value;
		_country = (Country)value;
	}
}

