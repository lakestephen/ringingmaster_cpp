// RecordTower.cpp: implementation of the RecordTower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerDataModification.h"
#include "RecordManager.h"
#include "RecordTower.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RecordTower::RecordTower() :
SaveableObject(0), //todo
_towerStatic(NULL),
_towerDataModification(NULL),
_distanceFromHomeTower(0),
_directionFromHomeTower(d_none)
{
}

RecordTower::~RecordTower()
{
	//we do not delete anything as it is all on seperate lists
}

//this static function crteates a new recordTower with a unique index.
RecordTower* RecordTower::createTower()
{
	RecordTower* recordTower = new RecordTower;
	recordTower->_towerDataModification = new TowerDataModification;
	recordTower->_towerDataModification->_index =
					recordManager()->getNextTowerDataModificationIndex();
	recordManager()->addTowerDataModification(recordTower->_towerDataModification);

	return recordTower;
}

void RecordTower::setTowerDataModification(TowerDataModification *towerDataModification)
{
	_towerDataModification = towerDataModification;
}

void RecordTower::attachGrab(RecordGrab* grab)
{
	_recordGrabs.Add(grab);
}

void RecordTower::removeGrab(RecordGrab* grab)
{
	for (int i=0;i<_recordGrabs.GetSize();i++)
	{
		if (_recordGrabs.GetAt(i) == grab)
		{
			_recordGrabs.RemoveAt(i);
			return;
		}
	}

	ASSERT(FALSE); //we have failed to remove a grab
}

void RecordTower::setTowerDataStatic(TowerDataStatic* towerDataStatic)
{
	_towerStatic = towerDataStatic;
}

CString RecordTower::getNumberAsString()
{
	CString str;
	str.Format("%d",getNumber());
	return str;
}

CString RecordTower::getNoteAsString(bool verbose)
{
	return recordManager()->getNoteAsString(getNote(), verbose);
}

CString RecordTower::getNightAsString(bool verbose)
{
	return recordManager()->getNightAsString(getNight(), verbose);
}

BOOL RecordTower::hasGrabbed()
{
	return (_recordGrabs.GetSize() > 0);
}

double RecordTower::getWeight()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bWeight))
		return _towerDataModification->_weight;
	else if (_towerStatic)
		return _towerStatic->_weight;
	else
		return 0;
}

void RecordTower::setWeight(double weight)
{
	if (checkModification())
	{
		_towerDataModification->_weight = weight;
		_towerDataModification->_bWeight = TRUE;
	}
}

BOOL RecordTower::isApproxModifyed()
{
	return ((_towerDataModification)&&
			(_towerDataModification->_bApprox));
}
//to2do add an isApproxModifyed for each item.

BOOL RecordTower::getApprox()
{
	if (isApproxModifyed())
		return _towerDataModification->_approx;
	else if (_towerStatic)
		return _towerStatic->_approx;
	else
		return FALSE;
}

void RecordTower::setApprox(BOOL approx)
{
	if (checkModification())
	{
		_towerDataModification->_approx = approx;
		_towerDataModification->_bApprox = TRUE;
	}
}


int RecordTower::getNumber()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bNumber))
		return _towerDataModification->_number;
	else if (_towerStatic)
		return _towerStatic->_number;
	else
		return 0;
}

void RecordTower::setNumber(int number)
{
	if (checkModification())
	{
		_towerDataModification->_number = number;
		_towerDataModification->_bNumber = TRUE;
	}
}

long RecordTower::getIndex() const
{
	//the order of this is important to the checkModification function.
	if (_towerStatic)
		return _towerStatic->_index;
	else if (_towerDataModification)
		return _towerDataModification->_index;

	ASSERT(FALSE);
	return 0;
}


BOOL RecordTower::isMapRefModifyed() const
{
	return ((_towerDataModification)&&
			(_towerDataModification->_bMapRef));
}

MapGridRef RecordTower::getMapRef() const
{
	if (isMapRefModifyed())
		return _towerDataModification->_gridRef;
	else if (_towerStatic)
		return _towerStatic->_gridRef;
	else
	{
		MapGridRef ref;
		return ref;
	}
}

void RecordTower::setMapRef(MapGridRef mapGridRef)
{
	if (checkModification())
	{
		_towerDataModification->_gridRef = mapGridRef;
		_towerDataModification->_bMapRef = TRUE;
	}
}

CString RecordTower::getCounty() const
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bCounty))
		return _towerDataModification->_county;
	else if (_towerStatic)
		return _towerStatic->_county;
	else
		return "";
}

void RecordTower::setCounty(CString county)
{
	if (checkModification())
	{
		_towerDataModification->_county = county;
		_towerDataModification->_bCounty = TRUE;
	}
}

CString RecordTower::getName() const
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bName))
		return _towerDataModification->_name;
	else if (_towerStatic)
		return _towerStatic->_name;
	else
		return "";
}

void RecordTower::setName(CString name)
{
	if (checkModification())
	{
		_towerDataModification->_name = name;
		_towerDataModification->_bName = TRUE;
	}
}

CString RecordTower::getDedication()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bDedication))
		return _towerDataModification->_dedication;
	else if (_towerStatic)
		return _towerStatic->_dedication;
	else
		return "";
}

void RecordTower::setDedication(CString dedication)
{
	if (checkModification())
	{
		_towerDataModification->_dedication = dedication;
		_towerDataModification->_bDedication = TRUE;
	}
}

CString RecordTower::getPostCode()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bPostCode))
		return _towerDataModification->_postCode;
	else if (_towerStatic)
		return _towerStatic->_postCode;
	else
		return "";
}

void RecordTower::setPostCode(CString postCode)
{
	if (checkModification())
	{
		_towerDataModification->_postCode = postCode;
		_towerDataModification->_bPostCode = TRUE;
	}
}

CString RecordTower::getWebAddress()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bWebPage))
		return _towerDataModification->_webPage;
	else if (_towerStatic)
		return _towerStatic->_webPage;
	else
		return "";
}

void RecordTower::setWebPage(CString webPage)
{
	if (checkModification())
	{
		_towerDataModification->_webPage = webPage;
		_towerDataModification->_bWebPage = TRUE;
	}
}

BOOL RecordTower::getGroundFloor()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bGroundFloor))
		return _towerDataModification->_groundFloor;
	else if (_towerStatic)
		return _towerStatic->_groundFloor;
	else
		return FALSE;
}

void RecordTower::setGroundFloor(BOOL groundFloor)
{
	if (checkModification())
	{
		_towerDataModification->_groundFloor = groundFloor;
		_towerDataModification->_bGroundFloor = TRUE;
	}
}

CString RecordTower::getGroundFloorAsString(bool verbose)
{
	if (getGroundFloor())
		if (verbose)
			return "Ground Floor";
		else
			return "GF";
	else
		if (verbose)
			return "Non Ground Floor";
		else
			return "";
}

int RecordTower::getGrabCount()
{
	return _recordGrabs.GetSize();
}

RecordGrab* RecordTower::getGrab(int index)
{
	return _recordGrabs.GetAt(index);
}

CString RecordTower::getGrabCountAsString(bool verbose)
{
	CString str;
	int grabCount = getGrabCount();
	if (grabCount >= ((verbose)?0:1))
		str.Format("%d", grabCount);

	return str;
}


BOOL RecordTower::getUnringable()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bUnringable))
		return _towerDataModification->_unringable;
	else if (_towerStatic)
		return _towerStatic->_unringable;
	else
		return FALSE;
}

void RecordTower::setUnringable(BOOL unringable)
{
	if (checkModification())
	{
		_towerDataModification->_unringable = unringable;
		_towerDataModification->_bUnringable = TRUE;
	}
}

CString RecordTower::getUnringableAsString(bool verbose)
{
	if (getUnringable())
		if (verbose)
			return "Unringable";
		else
			return "Unringable";
	else
		if (verbose)
			return "Ringable";
		else
			return "";
}

Note RecordTower::getNote()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bNote))
		return _towerDataModification->_note;
	else if (_towerStatic)
		return _towerStatic->_note;
	else
		return n_none;
}

void RecordTower::setNote(Note note)
{
	if (checkModification())
	{
		_towerDataModification->_note = note;
		_towerDataModification->_bNote = TRUE;
	}
}

Night RecordTower::getNight()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bNight))
		return _towerDataModification->_night;
	else if (_towerStatic)
		return _towerStatic->_night;
	else
		return now_none;
}

void RecordTower::setNight(Night night)
{
	if (checkModification())
	{
		_towerDataModification->_night = night;
		_towerDataModification->_bNight = TRUE;
	}
}

Country RecordTower::getCountry()
{
	if ((_towerDataModification)&&
		(_towerDataModification->_bCountry))
		return _towerDataModification->_country;
	else if (_towerStatic)
		return _towerStatic->_country;
	else
		return c_none;
}

void RecordTower::setCountry(Country country)
{
	if (checkModification())
	{
		_towerDataModification->_country = country;
		_towerDataModification->_bCountry = TRUE;
	}
}

CString RecordTower::getCountryAsString(bool verbose)
{
	return recordManager()->getCountryAsString(getCountry(), verbose);
}	

CString RecordTower::getDisplayName() const 
{
	return getName() + " , " + getCounty();
}

double RecordTower::getDistanceFromHomeTower()
{
	return _distanceFromHomeTower;
}

void RecordTower::setDistanceFromHomeTower(double distanceFromHomeTower)
{
	_distanceFromHomeTower = distanceFromHomeTower;
}

CString RecordTower::getDistanceFromHomeTowerAsString()
{
	return GlobalFunctions::getDistanceString(_distanceFromHomeTower);
}

Direction RecordTower::getDirectionFromHomeTower()
{
	return _directionFromHomeTower;
}

void RecordTower::setDirectionFromHomeTower(Direction directionFromHomeTower)
{
	_directionFromHomeTower = directionFromHomeTower;
}

CString RecordTower::getDirectionFromHomeTowerAsString()
{
	return recordManager()->getDirectionAsString(_directionFromHomeTower, false);
}

double RecordTower::getTempDistance()
{
	return _tempDistance;
}

void RecordTower::setTempDistance(double tempDistance)
{
	_tempDistance = tempDistance;
}

Direction RecordTower::getTempDirection()
{
	return _tempDirection;
}

void RecordTower::setTempDirection(Direction tempDirection)
{
	_tempDirection = tempDirection;
}

void RecordTower::setComment(CString comment)
{
	if (checkModification())
	{
		_towerDataModification->_comment = comment;
	}
}

CString RecordTower::getCommentAsSingleLine()
{
	//comment is not available in the base data
	if (_towerDataModification)
		return _towerDataModification->_comment;
	else
		return "";
}

bool RecordTower::checkModification()
{
	if (!_towerDataModification)
	{
		_towerDataModification = new TowerDataModification;
		_towerDataModification->_index = getIndex();
		recordManager()->addTowerDataModification(_towerDataModification);
	}


	return (_towerDataModification != NULL);
}

void RecordTower::deleteTower() //these sorts of calls should be generic. 
{
	//to delete a tower we must have a modification, and then flag that modification as deleted
	if (_towerStatic == NULL)
	{
		//this is a user created tower
		if (_towerDataModification)
		{
			recordManager()->removeTowerDataModification(_towerDataModification);
			delete _towerDataModification;
			_towerDataModification = NULL;
		}
	}
	//we are a static tower
	else if (checkModification())
	{
		_towerDataModification->_bDeleted = TRUE;
	}
}
	   
void RecordTower::sortAndIndexSecondGrabs(int index)
{
	if (_recordGrabs.GetSize() <= 1)
	{
		for (int i=0;i<_recordGrabs.GetSize();i++)
		{
			_recordGrabs.GetAt(i)->setGrabIndex(index);
			_recordGrabs.GetAt(i)->setSecondGrabIndex(-1);
		}
		return;
	}

	//sort 
	qsort((void*)_recordGrabs.GetData(), 
		  (size_t)_recordGrabs.GetSize(), 
		  sizeof(RecordGrab *), 
		  compareGrabs);


	//set the index of the grabs 
	for (int i=0;i<_recordGrabs.GetSize();i++)
	{
		_recordGrabs.GetAt(i)->setGrabIndex(index);
		_recordGrabs.GetAt(i)->setSecondGrabIndex(i+1);
	}
}

int RecordTower::compareGrabs( const void * arg1, const void * arg2 )
{
	RecordGrab* grab1 = (*(RecordGrab**)arg1);
	RecordGrab* grab2 = (*(RecordGrab**)arg2);

	int val = (int)(grab1->getDateOle() - grab2->getDateOle()).GetTotalDays();
	if (val != 0)
		return val;
	else
		return (grab1->getDayOrder() - grab2->getDayOrder());
}



