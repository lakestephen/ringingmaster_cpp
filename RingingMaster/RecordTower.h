// RecordTower.h: interface for the RecordTower class.

#pragma once


#include "TowerDataStatic.h"
#include "RecordGrab.h"
#include "MapGridRef.h"	// Added by ClassView
#include "SaveableObject.h"

class TowerDataModification;

class RecordTower : public SaveableObject
{
public:
	void dummyFunction(){};//todo remove

	MapGridRef getMapRef() const;
	void sortAndIndexSecondGrabs(int index);
	static RecordTower* createTower();
	void deleteTower();
	bool checkModification();
	CString getCommentAsSingleLine();
	CString getCountryAsString(bool verbose);
	CString getUnringableAsString(bool verbose);
	CString getGroundFloorAsString(bool verbose);
	void attachGrab(RecordGrab* RecordGrab);
	void removeGrab(RecordGrab* grab);

	void setDirectionFromHomeTower(Direction directionFromHomeTower);
	Direction getDirectionFromHomeTower();

	void setDistanceFromHomeTower(double distanceFromHomeTower);
	double getDistanceFromHomeTower();
	void setTempDistance(double tempDistance);
	double getTempDistance();

	Direction getTempDirection();
	void setTempDirection(Direction tempDirection);

	CString getDisplayName() const ;
	
	//basic data accessors
	double getWeight();
	int RecordTower::getNumber();
	long getIndex() const;//todo change to uniqueid
	CString getCounty() const;

	CString getName() const;
	CString getDedication();
	CString getPostCode();
	CString getWebAddress();
	BOOL getGroundFloor();
	BOOL getApprox();
	BOOL getUnringable();
	Note getNote();
	Night getNight();
	Country getCountry();

	BOOL isApproxModifyed();
	BOOL isMapRefModifyed() const;

	void setTowerDataModification(TowerDataModification* towerDataModification);
	void setTowerDataStatic(TowerDataStatic* base);
	BOOL hasGrabbed();
	RecordTower();
	virtual ~RecordTower();
	

	//todo migrate these to Global Functions
	CString getNightAsString(bool verbose);
	CString getNoteAsString(bool verbose);
	CString getNumberAsString();
	CString getDistanceFromHomeTowerAsString();
	CString getDirectionFromHomeTowerAsString();		

	int getGrabCount();
	RecordGrab* getGrab(int index);
	CString getGrabCountAsString(bool verbose);		 


	void setName(CString name);
	void setCounty(CString county);
	void setDedication(CString dedication);
	void setWebPage(CString webPage);
	void setNumber(int number);
	void setGroundFloor(BOOL groundFloor);
	void setWeight(double weight);
	void setUnringable(BOOL unringable);
	void setNote(Note note);
	void setNight(Night night);
	void setPostCode(CString postCode);
	void setMapRef(MapGridRef mapGridRef);
	void setCountry(Country country);
	void setApprox(BOOL approx);


	void setComment(CString comment);
	
	static int compareGrabs( const void * arg1, const void * arg2 );

protected:
	TowerDataStatic* _towerStatic;
	TowerDataModification * _towerDataModification;
	RecordGrabs _recordGrabs;

	
private:	
	//non serialised data
	double _distanceFromHomeTower; //KM
	Direction _directionFromHomeTower;
	double _tempDistance; //KM
	Direction _tempDirection;

};


