// TowerDataModification.cpp: implementation of the TowerDataModification class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerDataModification.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TowerDataModification::TowerDataModification() : 
_bMapRef(FALSE),	
_bCounty(FALSE),		
_bName(FALSE),		
_bDedication(FALSE),	
_bPostCode(FALSE),	
_bGroundFloor(FALSE),
_bNumber(FALSE),		
_bWeight(FALSE),		
_bApprox(FALSE),		
_bNote(FALSE),		
_bNight(FALSE),		
_bUnringable(FALSE),	
_bWebPage(FALSE),	
_bCountry(FALSE),
_bDeleted(FALSE)
{

}

TowerDataModification::~TowerDataModification()
{

}

void TowerDataModification::Serialize(CArchive &ar)
{
	TowerDataStatic::Serialize(ar);
	
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
		ar << _bMapRef;	//NG SO387303 
		ar << _bCounty;		//FULL
		ar << _bName;		//Place
		ar << _bDedication;	//Dedicn
		ar << _bPostCode;	//Postcode
		ar << _bGroundFloor;	//GF
		ar << _bNumber;		//Bells
		ar << _bWeight;		//kg
		ar << _bApprox;		//App
		ar << _bNote;		//Note
		ar << _bNight;		//Prac
		ar << _bUnringable;	//UR
		ar << _bWebPage;		//WebPage
		ar << _bCountry;		//CountryCode
		
		ar << _bDeleted;

		ar << _comment;
	}
	else
	{
		ar >> version; //just get the version number
		ar >> _bMapRef;	//NG SO387303 
		ar >> _bCounty;		//FULL
		ar >> _bName;		//Place
		ar >> _bDedication;	//Dedicn
		ar >> _bPostCode;	//Postcode
		ar >> _bGroundFloor;	//GF
		ar >> _bNumber;		//Bells
		ar >> _bWeight;		//kg
		ar >> _bApprox;		//App
		ar >> _bNote;		//Note
		ar >> _bNight;		//Prac
		ar >> _bUnringable;	//UR
		ar >> _bWebPage;		//WebPage
		ar >> _bCountry;		//CountryCode

		ar >> _bDeleted;

		ar >> _comment;
	}
}



