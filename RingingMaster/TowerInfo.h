#pragma once

#include "SaveableObject.h"
#include "InfoListDefs.h"
#include "MapGridRef.h"

enum Note;
enum Night;

class TowerInfo : SaveableObject
{
public:
	TowerInfo(long key);
	TowerInfo(CArchive& ar);
	TowerInfo(const TowerInfo& towerInfo);
	~TowerInfo();

	TowerInfo& operator =(const TowerInfo& towerInfo);
public:
	void Serialize(CArchive &ar);

public:
	MapGridRef getGridRef();
	void setGridRef(MapGridRef gridref);

	CString getCounty();
	void setCounty(CString county);

	CString getName();
	void setName(CString name);

	CString getDedication();
	void setDedication(CString dedication);

	CString getPostCode();
	void setPostCode(CString postcode);

	BOOL getGroundFloor();
	void setGroundFloor(BOOL groundfloor);

	int getNumber();
	void setNumber(int number);

	double getWeight();
	void setWeight(double weight);

	BOOL getApprox();
	void setApprox(BOOL approx);

	Note getNote();
	void setNote(Note note);

	Night getNight();
	void setNight(Night night);

	BOOL getUnringable();
	void setUnringable(BOOL unringable);

	CString getWebPage();
	void setWebPage(CString webpage);

	CString getCountry();
	void setCountry(CString country);


private:
	MapGridRef _gridref;
	CString _county;
	CString _name;
	CString _dedication;
	CString _postcode;
	BOOL _groundfloor;
	int _number;
	double _weight;
	BOOL _approx;
	Note _note;
	Night _night;
	BOOL _unringable;
	CString _webpage;
	CString _country;
};
