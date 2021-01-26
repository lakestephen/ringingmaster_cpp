
#pragma once


#include "MapGridRef.h"
#include "RecordConstants.h"


class TowerDataStatic : public CObject  
{
public:
	TowerDataStatic();
	TowerDataStatic(TowerDataStatic & towerDataStatic);
	virtual ~TowerDataStatic();

	int		_index;			//Index
	MapGridRef _gridRef;	//ExpRef  0338702303
	CString _county;		//FULL
	CString _name;			//Place
	CString _dedication;	//Dedicn
	CString	_postCode;		//Postcode
	BOOL	_groundFloor;	//GF
	int		_number;		//Bells
	double  _weight;		//kg
	BOOL	_approx;		//App
	Note	_note;			//Note
	Night   _night;		//Prac
	BOOL	_unringable;	//UR
	CString _webPage;		//WebPage
	Country	_country;		//CountryCode



	void operator =(TowerDataStatic &towerDataStatic);
	void Serialize(CArchive &ar);

	void init();

};

