#pragma once

#include "TowerDataStatic.h"

class TowerDataModification : public TowerDataStatic
{
public:
	TowerDataModification();
	virtual ~TowerDataModification();
	void Serialize(CArchive &ar);

	BOOL _bMapRef;		//grid ref
	BOOL _bCounty;		//FULL
	BOOL _bName;		//Place
	BOOL _bDedication;	//Dedicn
	BOOL _bPostCode;	//Postcode
	BOOL _bGroundFloor;	//GF
	BOOL _bNumber;		//Bells
	BOOL _bWeight;		//kg
	BOOL _bApprox;		//App
	BOOL _bNote;		//Note
	BOOL _bNight;		//Prac
	BOOL _bUnringable;	//UR
	BOOL _bWebPage;		//WebPage
	BOOL _bCountry;		//CountryCode
	
	BOOL _bDeleted;		//are we deleted?
	CString _comment;
};

