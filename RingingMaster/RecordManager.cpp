// RecordManager.cpp: implementation of the RecordManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "RecordManager.h"
#include "MapManager.h"
#include "RecordTower.h"
#include "Usermanager.h"
#include "TowerDataStatic.h"
#include "TowerDataModification.h"
#include "PropItem.h"
#include "OptionsDlg.h"

#include "ListSelectItem.h"
#include "GrabDetailsDlg.h"
#include "GrabOrderDlg.h"
#include "MapDoc.h"

#include "RecordChooseTowerDlg.h"
#include "RecordTowerEditDlg.h"

#include "PrintData.h"
#include "CPage.h"
#include "PrintUtils.h"

#include "MoreDlg.h"

#include "TowerDoc.h"
#include "GrabDoc.h"

#include "FilterPopup.h"
#include "recordmanager.h"
#include "Matrixmanager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//todo in the new matrix world, give the option of adding all towers before filtering

RecordManager::RecordManager() :
_validLoad(FALSE),
_homeTowerIndex(1), //ABBEY DORE
_printTowerRef(NULL),
_showPrintSetupDlg(TRUE)
{
	deleteUserData();
}

BEGIN_MESSAGE_MAP(RecordManager, CCmdTarget)
	//{{AFX_MSG_MAP(RecordManager)
	ON_COMMAND(ID_ADDTOWER, OnAddtower)
	ON_COMMAND(ID_DELETETOWER, OnDeletetower)
	ON_COMMAND(ID_TOWER_SETHOMETOWER, OnTowerSethometower)
	ON_COMMAND(ID_GRABS_ADDTOWERGRAB, OnGrabsAddtowergrab)
	ON_COMMAND(ID_EDITTOWER, OnEdittower) 
	ON_COMMAND(ID_TOWER_DATABASE, OnTowerDatabase)
	ON_COMMAND(ID_GRAB_OPTIONS, OnGrabOptions)
	ON_UPDATE_COMMAND_UI(ID_TOWER_DATABASE, OnUpdateTowerDatabase)
	ON_COMMAND(ID_GRABS, OnGrabs)
	ON_UPDATE_COMMAND_UI(ID_GRABS, OnUpdateGrabs)
	ON_COMMAND(ID_USERFILTERMORE, OnUserfiltermore)
	ON_COMMAND(ID_FILTER_COMBO, OnFilterCombo)
	ON_COMMAND(ID_TOWER_OPTIONS, OnTowerOptions)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_ADDTOWER, OnUpdateAddtower)
	ON_UPDATE_COMMAND_UI(ID_GRABS_ADDTOWERGRAB, OnUpdateGrabsAddtowergrab)
	ON_UPDATE_COMMAND_UI(ID_TOWER_SETHOMETOWER, OnUpdateTowerSethometower)
END_MESSAGE_MAP()
	


RecordManager::~RecordManager()
{
	//user Data
	deleteUserData();
	
	//data
	for (int i=0;i<_towerDataStatics.GetSize();i++)
		delete _towerDataStatics.GetAt(i);
	
}	  

void RecordManager::deleteUserData()
{
	for (int i=0;i<_allGrabsList.GetSize();i++)
		delete _allGrabsList.GetAt(i);
	_allGrabsList.RemoveAll();
	_allGrabsMap.RemoveAll();


	for (int i=0;i<_allTowersList.GetSize();i++)
		delete _allTowersList.GetAt(i);
	_allTowersList.RemoveAll();
	_allTowersMap.RemoveAll();

	for (int i=0;i<_towerDataModifications.GetSize();i++)
		delete _towerDataModifications.GetAt(i);
	_towerDataModifications.RemoveAll();

	//set defaults (do not use the methods for this as they trigger updates on shutdown, and then a crash
	_homeTowerIndex = 1; //abby dore

}

void RecordManager::SerializeBaseData(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ASSERT(FALSE);
		//do nothing: only load.
	}
	else
	{
		int size;
		ar >> size;
		TowerDataStatic* towerDataStatic;
		for (int i=0;i<size;i++)
		{
			towerDataStatic = new TowerDataStatic;
			towerDataStatic->Serialize(ar);
			_towerDataStatics.Add(towerDataStatic);
		}
	}
}

void RecordManager::SerializeUserData(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;

		//archive the modifications	
		ar << _towerDataModifications.GetSize();
		for (int i=0;i<_towerDataModifications.GetSize();i++)
		{
			_towerDataModifications.GetAt(i)->Serialize(ar);
		}

		ar << _homeTowerIndex;

		//archive the user data
		ar << _allGrabsList.GetSize();
		for (int i=0;i<_allGrabsList.GetSize();i++)
		{
			_allGrabsList.GetAt(i)->Serialize(ar);
		}

	}
	else
	{
		deleteUserData();

		int size;

		ar >> version;

		//archive the modifications
		ar >> size;
		for (int i=0;i<size;i++)
		{
			TowerDataModification* towerDataModification = new TowerDataModification;
			towerDataModification->Serialize(ar);
			_towerDataModifications.Add(towerDataModification);
		}	

		//this must be done early, as other fields need it
		createAllTowers();

		int homeTower;
		ar >> homeTower;
						
		
		//archive the user data
		ar >> size;
		for (int i=0;i<size;i++)
		{
			RecordGrab* grab = new RecordGrab;
			grab->Serialize(ar);
			addGrab(grab, FALSE);  //we do our index at the end
		}

		sortAndIndexGrabs();
		
		//do the setup
		setHomeTower(homeTower);

		//reset
//todo		resetFilter(vt_tower);
//todo 		resetFilter(vt_grab);

		//filter list
		fireUpdateFilterList();
	}
}


void RecordManager::createAllTowers()
{
	//Now create and _allTowers data
	for (int i=0;i<_towerDataStatics.GetSize();i++)
	{
		RecordTower* recordTower = new RecordTower;
		recordTower->setTowerDataStatic(_towerDataStatics.GetAt(i));
		addRecordTower(recordTower);
	}	

	//match up the modification data
	for (int i=0;i<_towerDataModifications.GetSize();i++)
	{
		TowerDataModification* towerDataModification = _towerDataModifications.GetAt(i);
		RecordTower* tower = getTower(towerDataModification->_index);
		if (tower)
		{
			if (towerDataModification->_bDeleted)
			{
				//remove and delete
				removeRecordTower(tower);
				delete tower;
			}
			else
			{
				//match up
				tower->setTowerDataModification(towerDataModification);
			}
		}
		else
		{
			//thos is an added tower 
			if (towerDataModification->_bDeleted)
			{
				//A user added tower that ios deleted is wrong. 
				// The delete code should remove the added code
				ASSERT(FALSE);
			}
			else
			{
				//create new TowerData;
				RecordTower* recordTower = new RecordTower;
				recordTower->setTowerDataModification(towerDataModification);
				addRecordTower(recordTower);
			}
		}
	}
}
	   
BOOL RecordManager::isValid(bool doLoad)
{
	if ((!_validLoad)&&(doLoad))
		loadTowers();
	return _validLoad;
}  

void RecordManager::loadTowers()
{	 
	//load in the base data
	CFile file;
	CString path = app()->_rootPath + "towers.dat";

	if(!file.Open(path,CFile::modeRead))
	{
		TRACE("********LOAD: Tower Base Data not found********\n");
		return;
	}

	//load the towers into memory
	CArchive ar(&file, CArchive::load);
	
	SerializeBaseData(ar);

	//close 
	ar.Flush();
	file.Close();

	//flag the success
	_validLoad = TRUE;
}

//make sure that we add to both the list and the map
void RecordManager::addRecordTower(RecordTower *recordTower)
{
	_allTowersList.Add(recordTower);
	_allTowersMap.SetAt(recordTower->getIndex(),recordTower);
}

void RecordManager::removeRecordTower(RecordTower *recordTower)
{
	for (int i=0;i<_allTowersList.GetSize();i++)
	if (recordTower == _allTowersList.GetAt(i))
		_allTowersList.RemoveAt(i--);

	//remove from map
	_allTowersMap.RemoveKey(recordTower->getIndex());
}

//make sure that we add to both and to the parent tower
void RecordManager::addGrab(RecordGrab* recordGrab, BOOL doSort)
{
	//find the tower
	RecordTower* tower = recordGrab->getTower();
	if (tower)
	{
		tower->attachGrab(recordGrab);
		_allGrabsList.Add(recordGrab);
		_allGrabsMap.SetAt(recordGrab->getGrabIndex(),recordGrab);
		setNextDayOrder(recordGrab);
		if (doSort)											   
			sortAndIndexGrabs();
		//todo update matrix filter _filteredGrabs.Add(recordGrab);

		matrixManager()->fireRefresh();

		setUpdatePoint("Add Grab");
	}
	else
	{
		ASSERT(FALSE);
		delete recordGrab;
	}
}

//used in future undo processing. See TouchDoc
void RecordManager::setUpdatePoint(CString /*name*/, bool /*setModifyed*/, bool /*checkPoint*/) 
{

}

void RecordManager::sortAndIndexGrabs()
{
	//index
	_allGrabsMap.RemoveAll();
	for (int i=0;i<_allGrabsList.GetSize();i++)
	{
		_allGrabsList.GetAt(i)->setGrabIndex(-1);
		_allGrabsList.GetAt(i)->setSecondGrabIndex(-1);
	}

	//sort
	qsort((void*)_allGrabsList.GetData(), 
		(size_t)_allGrabsList.GetSize(), 
		sizeof(RecordGrab *), 
		compareGrabs);

	//index
	int index = 1;
	for (int i=0;i<_allGrabsList.GetSize();i++)
	{
		if (_allGrabsList.GetAt(i)->getGrabIndex() == -1)
		{
			//get the tower and set all grabs to the same index
			RecordTower* tower = _allGrabsList.GetAt(i)->getTower();
			if (tower) 
				tower->sortAndIndexSecondGrabs(index);
			_allGrabsMap.SetAt(_allGrabsList.GetAt(i)->getGrabIndex(),_allGrabsList.GetAt(i));
			
			index++;
		}
	}
}

int RecordManager::compareGrabs( const void * arg1, const void * arg2 )
{
	RecordGrab* grab1 = (*(RecordGrab**)arg1);
	RecordGrab* grab2 = (*(RecordGrab**)arg2);

	if (grab1->getDateOle() == grab2->getDateOle())
	{
		if (grab1->getDayOrder() == grab2->getDayOrder())
			return 0;
		else
			return (grab1->getDayOrder() > grab2->getDayOrder());
	}
	else 
		return (grab1->getDateOle() > grab2->getDateOle())?1:(-1);
}



void RecordManager::setHomeTower(int homeTowerIndex)
{  
	_homeTowerIndex = homeTowerIndex;
	calculateDistanceAndDirectionFromHomeTowers();

	fireHomeTowerChange();

	setUpdatePoint("Set Home Tower");

}

int RecordManager::getHomeTowerIndex()
{
	return _homeTowerIndex;
}

RecordTower* RecordManager::getHomeTower()
{
	return getTower(_homeTowerIndex);
}
/* todo 
void RecordManager::preCalculateSortDistance(RSTowerDistance *sort, RecordTowers *towers)
{
	//get the distance from the tower
	int homeX = 0;
	int homeY = 0;
	int localX1 = 0;
	int localY1 = 0;

	bool homeOK = (sort->getHomeGridRef(homeX, homeY));

	for (int i=0;i<towers->GetSize();i++)
	{
		RecordTower* tower = towers->GetAt(i);
		if (homeOK)
		{
			if (tower->getMapRef().isValidOSGridRef())
			{
				localX1 = tower->getMapRef().getEastings();
				localY1 = tower->getMapRef().getNorthings();
				double distance = sqrt((double)(((homeX-localX1)*(homeX-localX1)) + ((homeY-localY1)*(homeY-localY1))));
				tower->setTempDistance(distance);
			}
			else
				tower->setTempDistance(INT_MAX);
		}
		else
			tower->setTempDistance(0);
	}				 
}

void RecordManager::preCalculateSortDirection(RSTowerDirection*sort, RecordTowers *towers)
{
	//get the distance from the tower
	int homeX = 0;
	int homeY = 0;
	int localX1 = 0;
	int localY1 = 0;
	double distance;
	Direction direction;

	bool homeOK = (sort->getHomeGridRef(homeX, homeY));

	for (int i=0;i<towers->GetSize();i++)
	{
		RecordTower* tower = towers->GetAt(i);
		if (homeOK)
		{
			if (tower->getMapRef().isValidOSGridRef())
			{
				localX1 = tower->getMapRef().getEastings();
				localY1 = tower->getMapRef().getNorthings();
				getDistanceAndDirection(homeX, homeY, localX1, localY1, distance, direction);
				tower->setTempDirection(direction);
			}
			else
				tower->setTempDirection(d_none);
		}
		else
			tower->setTempDistance(d_home);
	}				 
}
			*/

void RecordManager::populateAllTowers(CComboBox &comboBox, int towerIndex, BOOL includeNonOSMapRef, RecordTowers* recordTowers)
{
	if (recordTowers == NULL)
		recordTowers = &_allTowersList;

	comboBox.ResetContent();
	for (int i=0;i<recordTowers->GetSize();i++)
	{
		RecordTower* recordTower = recordTowers->GetAt(i);
		if ((includeNonOSMapRef)||(recordTower->getMapRef().isValidOSGridRef()))
		{
			int index = comboBox.AddString(recordTower->getDisplayName());
			comboBox.SetItemDataPtr(index, recordTower);
			if (towerIndex == recordTower->getIndex())
				comboBox.SetCurSel(index);
		}
	}
}

void RecordManager::populateAllGrabs(CComboBox &comboBox, RecordGrab* grab, RecordGrabs* recordGrabs)
{
	if (recordGrabs == NULL)
		recordGrabs = &_allGrabsList;

	comboBox.ResetContent();
	for (int i=0;i<recordGrabs->GetSize();i++)
	{
		RecordGrab* recordGrab = recordGrabs->GetAt(i);
		int index = comboBox.AddString(recordGrab->getDisplayName());
		comboBox.SetItemDataPtr(index, recordGrab);
		if (grab == recordGrab)
			comboBox.SetCurSel(i);
	}
}

RecordTower* RecordManager::getTower(int index)
{
	RecordTower* recordTower = NULL;
	if (_allTowersMap.Lookup(index, recordTower))
		return recordTower;
	else
		return NULL;
}

RecordGrab* RecordManager::getGrab(int index)
{
	RecordGrab* recordGrab = NULL;
	if (_allGrabsMap.Lookup(index, recordGrab))
		return recordGrab;
	else
		return NULL;
}

void RecordManager::addEventListener(RecordManagerEventListener *recordManagerEventListener)
{
	ASSERT(recordManagerEventListener != NULL);

	_listenerList.Add(recordManagerEventListener);

}

void RecordManager::removeEventListener(RecordManagerEventListener *recordManagerEventListener)
{
	ASSERT(recordManagerEventListener!= NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == recordManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	ASSERT(hasRemoved == 1);
}

void RecordManager::fireUpdateFilterList()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->recordManager_updateFilterList(); 
	}  
}

void RecordManager::fireHomeTowerChange() 
{
	RecordTower* tower = getTower(_homeTowerIndex);
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->recordManager_notifyHomeTowerChange(tower); 
	}  
}

void RecordManager::calculateDistanceAndDirectionFromHomeTowers()
{
	int homeX = 0;
	int homeY = 0;
	int localX = 0;
	int localY = 0;
	double distance;
	Direction direction;

	RecordTower* homeTower = getHomeTower();
	if ((homeTower)&&(homeTower->getMapRef().isValidOSGridRef()))
	{
		homeX = homeTower->getMapRef().getEastings();
		homeY = homeTower->getMapRef().getNorthings();
	}
	else
		homeTower = NULL;

	

	for (int i=0;i<_allTowersList.GetSize();i++)
	{					 
		RecordTower* recordTower = _allTowersList.GetAt(i);
		if ((homeTower)&&(recordTower->getMapRef().isValidOSGridRef()))
		{	   
			localX = recordTower->getMapRef().getEastings();
			localY = recordTower->getMapRef().getNorthings();
			getDistanceAndDirection(homeX, homeY, localX, localY, distance, direction);
			recordTower->setDistanceFromHomeTower(distance/10); 		
			recordTower->setDirectionFromHomeTower(direction);
		}
		else
		{
			recordTower->setDistanceFromHomeTower(0); 		
			recordTower->setDirectionFromHomeTower(d_none); 		
		}
	}
}

void RecordManager::getDistanceAndDirection(int X1, int Y1, int X2, int Y2, double& distance, Direction& direction)
{
	const int DX = X2 - X1;
	const int DY = Y2 - Y1;
	distance = sqrt((double)((DX*DX) + (DY*DY)));

	if ((DX == 0)&&(DY == 0))
	{
		direction = d_home; //for when checking home tower against home tower
		return;
	}

	//calculate the quadrent
	double theta = 0;

	if (DY >= 0)
	{
		if (DX >= 0)
			// top right (Q1)
			theta = asin(abs(DX)/distance);
		else
			//top left	(Q4)
			theta = PI2 - asin(abs(DX)/distance);

	}
	else
	{
		if (DX >= 0)
			// bottom right (Q2)
			theta = PI - asin(abs(DX)/distance);
		else
			// bottom left (Q3)
			theta = PI + asin(abs(DX)/distance);
	}


	//get the section 
	theta += segment32;
	
	while (theta > PI2) theta -= PI2;

	direction = (Direction)((int)(theta / segment16));

	ASSERT(((direction >=d_n)&&(direction <=d_nnw))||
		   (direction == d_none)||
		   (direction == d_home));

}



void RecordManager::getAllNumbers(ListSelectItems &items)
{
	for (int i=0;i<_allTowersList.GetSize();i++)
	{
		RecordTower* tower = _allTowersList.GetAt(i);
		bool onList = false;

		for (int j=0;j<items.GetSize();j++)
			if (items.GetAt(j)->_ref == tower->getNumber())
				onList = true;

		if (!onList)
		{
			CString str;
			str.Format("%d", tower->getNumber());
			items.Add(new ListSelectItem(tower->getNumber(), str));
		}
	}

	//now sort
		//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<items.GetUpperBound();i++)
		{
			if (items.GetAt(i)->_ref >  items.GetAt(i+1)->_ref)
			{
				changed = true;
				ListSelectItem* temp    = items.GetAt(i);
				items.SetAt(i,items.GetAt(i+1));
				items.SetAt(i+1, temp);
			}
		}
	}
}

void RecordManager::getAllGrabCounts(ListSelectItems &items)
{
	for (int i=0;i<_allTowersList.GetSize();i++)
	{
		RecordTower* tower = _allTowersList.GetAt(i);
		bool onList = false;

		for (int j=0;j<items.GetSize();j++)
			if (items.GetAt(j)->_ref == tower->getGrabCount())
				onList = true;

		if (!onList)
			items.Add(new ListSelectItem(tower->getGrabCount(), ((tower->getGrabCount()==0)?"[none]":tower->getGrabCountAsString(false))));
	}

	//now sort
		//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<items.GetUpperBound();i++)
		{
			if (items.GetAt(i)->_ref >  items.GetAt(i+1)->_ref)
			{
				changed = true;
				ListSelectItem* temp    = items.GetAt(i);
				items.SetAt(i,items.GetAt(i+1));
				items.SetAt(i+1, temp);
			}
		}
	}
}

void RecordManager::getAllCountrys(ListSelectItems &items)
{
	for (int i=c_england;i<=c_india;i++)
		items.Add(new ListSelectItem(i, getCountryAsString((Country)i)));
}

void RecordManager::getAllNotes(ListSelectItems &items)
{
	for (int i=n_a_flat;i<=n_g_sharp;i++)
		items.Add(new ListSelectItem(i, getNoteAsString((Note)i)));
}

void RecordManager::getAllNights(ListSelectItems &items)
{
	for (int i=now_sun;i<=now_sat_minus;i++)
		items.Add(new ListSelectItem(i, getNightAsString((Night)i)));
}

void RecordManager::getAllBellsGo(ListSelectItems &items)
{
	for (int i=bgt_wow;i<=bgt_unringable;i++)
		items.Add(new ListSelectItem(i, RecordGrab::getBellsGoName((BellsGo)i)));
}

void RecordManager::getAllBellsSound(ListSelectItems &items)
{
	for (int i=bst_sweet;i<=bst_tinCans;i++)
		items.Add(new ListSelectItem(i, RecordGrab::getBellsSoundName((BellsSound)i)));
}

void RecordManager::getAllDirections(ListSelectItems &items)
{
	for (int i=d_n;i<=d_nnw;i++)
		items.Add(new ListSelectItem(i, getDirectionAsString((Direction)i)));
}

void RecordManager::getAllCountys(ListSelectItems &items) //todo cache these values.
{
	for (int i=0;i<_allTowersList.GetSize();i++)
	{
		RecordTower* tower = _allTowersList.GetAt(i);
		bool onList = false;

		for (int j=0;j<items.GetSize();j++)
			if (items.GetAt(j)->_text == tower->getCounty())
				onList = true;

		if (!onList)
		{
			items.Add(new ListSelectItem(-1, tower->getCounty()));
		}
	}

	//now sort
		//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<items.GetUpperBound();i++)
		{
			if (items.GetAt(i)->_text>  items.GetAt(i+1)->_text)
			{
				changed = true;
				ListSelectItem* temp    = items.GetAt(i);
				items.SetAt(i,items.GetAt(i+1));
				items.SetAt(i+1, temp);
			}
		}
	}
}

void RecordManager::createGrab(int index)
{
	RecordGrab* grab = new RecordGrab;
	grab->setTowerIndex(index);
	
	GrabDetailsDlg dlg(*grab);

	if (dlg.DoModal() == IDOK)
	{
		addGrab(grab);
		//todo add to matrix filter _filteredGrabsMemory.Add(grab);
	}
	else
	{
		delete grab;
	}
	setUpdatePoint("New Grab");
}

void RecordManager::editGrab(RecordGrab* grab)
{
	GrabDetailsDlg dlg(*grab);

	if (dlg.DoModal() == IDOK)
	{
		sortAndIndexGrabs();
	}
	setUpdatePoint("Edit Grab");
}
			/* todo	 make sure this is handled in the matrix cod
CString RecordManager::getAvailableFieldsPropList(AvailableField availableFields)
{
	CString str;
	switch (availableFields)								  
	{
		//tower fields
	case af_tower_groundFloor:	
		return "Ground Floor|Non Ground Floor|";
	case af_tower_number:		
		{
			for (int i=MINBELLS;i<=MAXBELLS;i++)
			{
				CString tempStr;
				tempStr.Format("%d|", i);
				str += tempStr;
			}
		}
		return str;
	case af_tower_note:			
		{
			for (int i=n_a_flat;i<=n_g_sharp;i++)
				str += (getNoteAsString((Note)i) + "|");
			str += "[None]|";
			return str;
		}
	case af_tower_night:	
		{
			for (int i=now_sun;i<=now_sat_minus;i++)
				str += (getNightAsString((Night)i) + "|");
			str += "[None]|";
			return str;
		}
	case af_tower_unringable:	
		return "Ringable|Unringable|";
	case af_tower_country:	
		{
			for (int i=c_england;i<=c_india;i++)
				str += (getCountryAsString((Country)i) + "|");
			str += "[None]|";
			return str;
		}

	default:
		return "";

	}
}			 */


CString RecordManager::getCountryAsString(Country country, bool verbose)
{
	switch (country)
	{
	case c_england:			 return "England";		       
	case c_wales:			 return "Wales";		       
	case c_scotland:		 return "Scotland";	 
	case c_usa:	   			 return "USA";	   		       
	case c_australia:		 return "Australia";	       
	case c_chanel_isl:		 return "Chanel Isl";	       
	case c_new_zealand:		 return "New Zealand";	       
	case c_ireland:			 return "Ireland";	       
	case c_south_africa:	 return "South Africa";       
	case c_kenya:			 return "Kenya";		       
	case c_pakistan:		 return "Pakistan";	       
	case c_zimbabwe:		 return "Zimbabwe";	       
	case c_canada:			 return "Canada";		       
	case c_isle_of_man:		 return "Isle of Man";	       
	case c_spain:			 return "Spain";		       
	case c_india:			 return "India";		       
	case c_none:	
		if (verbose) return "[None]";
	default:
		return "";              
	}	
}	




CString RecordManager::getNoteAsString(Note note, bool verbose)
{
	switch (note)
	{
	case n_a_flat:	return "Ab";
	case n_a:		return "A";
	case n_a_sharp: return "A#";

	case n_b_flat:  return "Bb";
	case n_b:		return "B";
	case n_b_sharp: return "B#";

	case n_c_flat:  return "Cb";
	case n_c:		return "C";
	case n_c_sharp: return "C#";

	case n_d_flat:  return "Db";
	case n_d:		return "D";
	case n_d_sharp: return "D#";

	case n_e_flat:  return "Eb";
	case n_e:		return "E";
	case n_e_sharp: return "E#";

	case n_f_flat:  return "Fb";
	case n_f:		return "F";
	case n_f_sharp: return "F#";

	case n_g_flat:  return "Gb";
	case n_g:		return "G";
	case n_g_sharp: return "G#";

	case n_none:  
		if (verbose) return "[None]";
	default:
					return "";
	} 
}

CString RecordManager::getDirectionAsString(Direction direction, bool /*fullName*/)
{
	switch (direction)
	{
	case d_n:		return "N";
	case d_nne:		return "NNE";
	case d_ne:		return "NE";
	case d_ene:		return "ENE";

	case d_e:		return "E";
	case d_ese:		return "ESE";
	case d_se:		return "SE";
	case d_sse:		return "SSE";

	case d_s:		return "S";
	case d_ssw:		return "SSW";
	case d_sw:		return "SW";
	case d_wsw:		return "WSW";

	case d_w:		return "W";
	case d_wnw:		return "WNW";
	case d_nw:		return "NW";
	case d_nnw:		return "NNW";

	case d_none:
	default:		return "";

	}
}

CString RecordManager::getNightAsString(Night night, bool verbose)
{
	switch(night)
	{
	case now_mon:  return "Mon";
	case now_tue:  return "Tues";
	case now_wed:  return "Wed";
	case now_thu:  return "Thurs";
	case now_fri:  return "Fri";
	case now_sat:  return "Sat";
	case now_sun:  return "Sun";
	
	case now_mon_plus:  return "Mon (and other nights)";
	case now_tue_plus:  return "Tues (and other nights)";
	case now_wed_plus:  return "Wed (and other nights)";
	case now_thu_plus:  return "Thurs (and other nights)";
	case now_fri_plus:  return "Fri (and other nights)";
	case now_sat_plus:  return "Sat (and other nights)";
	case now_sun_plus:  return "Sun (and other nights)";
	
	case now_mon_minus:  return "Mon (Usually)";
	case now_tue_minus:  return "Tues (Usually)";
	case now_wed_minus:  return "Wed (Usually)";
	case now_thu_minus:  return "Thurs (Usually)";
	case now_fri_minus:  return "Fri (Usually)";
	case now_sat_minus:  return "Sat (Usually)";
	case now_sun_minus:  return "Sun (Usually)";

	case now_none:  
		if (verbose) return "[None]";
	default:
					return "";
	}

}


bool RecordManager::hasMultipleGrabsOnDay(RecordGrab *recordGrab)
{
	if (!recordGrab)
		return false;
	int count = 0;

	for (int i=0;i<_allGrabsList.GetSize();i++)
	{
		RecordGrab* item = _allGrabsList.GetAt(i);
		if (item->getDateOle() == recordGrab->getDateOle())
		{
			count++;
			if (count > 1)
				return true;
		}
	}
	return false;
}

bool RecordManager::setNextDayOrder(RecordGrab *recordGrab)
{
	if (recordGrab->getDayOrder() > 0)
		return false;

	int index = 0;

	for (int i=0;i<_allGrabsList.GetSize();i++)
	{
		RecordGrab* item = _allGrabsList.GetAt(i);
		if (item->getDateOle() == recordGrab->getDateOle())
		{
			if (item->getDayOrder() > index)
				index = item->getDayOrder();
		}
	}
	
	//minimum will be 1
	recordGrab->setDayOrder(++index);

	setUpdatePoint("", false, false);

	return true;
}

void RecordManager::editGrabDayOrder(RecordGrab *recordGrab)
{

	RecordGrabs singleDay;

	for (int i=0;i<_allGrabsList.GetSize();i++)
	{
		RecordGrab* item = _allGrabsList.GetAt(i);
		if (item->getDateOle() == recordGrab->getDateOle())
		{
			singleDay.Add(item);
		}
	}

	if (singleDay.GetSize() <= 1) 
	{		
		AfxMessageBox("There is only one grab on " + recordGrab->getDateAsString(false));
		return;
	}

	GrabOrderDlg dlg(singleDay);

	if (dlg.DoModal() == IDOK)
	{
		sortAndIndexGrabs();
		setUpdatePoint("Edit Day Order");
	}
}

bool RecordManager::findTower(CString name, RecordTowers &towers)
{	  
	towers.RemoveAll();
	for (int i=0;i<_allTowersList.GetSize();i++)
	{
		if (_allTowersList.GetAt(i)->getName().Find(name) != -1)
			towers.Add(_allTowersList.GetAt(i));
	}
	return (towers.GetSize() >0);
}

int RecordManager::getAllTowersCount()
{
	return _allTowersList.GetSize();
}

RecordTower* RecordManager::getAllTower(int index)
{
	return _allTowersList.GetAt(index);
}

int RecordManager::getAllGrabsCount()
{
	return _allGrabsList.GetSize();
}

RecordGrab* RecordManager::getAllGrab(int index)
{
	return _allGrabsList.GetAt(index);
}

int RecordManager::getNextTowerDataModificationIndex()
{
	int index =-1;
	for (int i=0;i<_towerDataModifications.GetSize();i++)
	{
		if (_towerDataModifications.GetAt(i)->_index < index)
			index--;
	}

	index--;

	TRACE("Next _towerDataModifications Index = %d \r\n", index);

	return index;
}

void RecordManager::addTowerDataModification(TowerDataModification *towerDataModification)
{
	ASSERT(towerDataModification);

	_towerDataModifications.Add(towerDataModification);
}

void RecordManager::removeTowerDataModification(TowerDataModification *towerDataModification)
{
	ASSERT(towerDataModification);

	for (int i=0;i<_towerDataModifications.GetSize();i++)
		if (_towerDataModifications.GetAt(i) == towerDataModification)
			_towerDataModifications.RemoveAt(i--);
}


void RecordManager::editTower(RecordTower *recordTower)
{
	//do we have a tower?
	if (recordTower == NULL)
	{
		CPoint point;
		GetCursorPos(&point);
		RecordChooseTowerDlg dlg(mainFrame(), point);
		dlg.DoModal();

		recordTower = dlg.getRecordTower();
	}

	if (recordTower)
	{
		RecordTowerEditDlg dlg(mainFrame(), recordTower);
		dlg.DoModal();
	}
	
	matrixManager()->fireRefresh();
}

void RecordManager::deleteTower(RecordTower *recordTower)
{
	//do we have a tower?
	if (recordTower == NULL)
	{
		CPoint point;
		GetCursorPos(&point);
		RecordChooseTowerDlg dlg(mainFrame(), point);
		dlg.DoModal();

		recordTower = dlg.getRecordTower();
		 
	}

	if ((recordTower)&&
		(AfxMessageBox("Do you want to delete " + recordTower->getName() + ".", MB_ICONQUESTION|MB_YESNO) == IDYES)&&
		(AfxMessageBox("This action will permanently remove " + recordTower->getName() + " and all assocated data.\n If you want to simply remove from the filter, use Records->Apply Logic->Remove Tower From Filter.", MB_ICONEXCLAMATION|MB_OKCANCEL) == IDOK))
	{	

		//notify the matrix to update filters.
		matrixManager()->fireEntityRemoved(met_tower, recordTower->getIndex() );		
		
		//remove from filtered grab list
		for (int i=0;i<recordTower->getGrabCount();i++)
		{
			matrixManager()->fireEntityRemoved(met_grab, recordTower->getGrab(i)->getGrabIndex());		

			//todo update matrix filter removeGrabFromFilteredList(recordTower->getGrab(i));
			//todo update matrix filter removeGrabFromFilteredMemoryList(recordTower->getGrab(i));
		}  

		//remove from list
		removeRecordTower(recordTower);

		//are we the home tower?
		if ((recordTower->getIndex() == _homeTowerIndex)&&
			(_allTowersList.GetSize() > 0))
			_homeTowerIndex = _allTowersList.GetAt(0)->getIndex();

		//remove the grabs from the grab list
		for (int i=0;i<_allGrabsList.GetSize();i++)
			if (_allGrabsList.GetAt(i)->getTowerIndex() == recordTower->getIndex())
			{
				_allGrabsMap.RemoveKey(_allGrabsList.GetAt(i)->getGrabIndex());
				_allGrabsList.RemoveAt(i--);
			}

		//update
		setUpdatePoint("Delete " + recordTower->getName());

		//flag as deleted in modification and delete object
		recordTower->deleteTower();
		delete recordTower;

		//get the grabs back in order
		sortAndIndexGrabs();



	}	
}


void RecordManager::OnAddtower() 
{
	RecordTower* recordTower = RecordTower::createTower();

	addRecordTower(recordTower);

	RecordTowerEditDlg dlg(mainFrame(), recordTower);
	dlg.DoModal();

	//update the filters
	matrixManager()->fireEntityAdded(met_tower, recordTower->getIndex());

	setUpdatePoint("New Tower");
}

void RecordManager::OnUpdateAddtower(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(getTowerDoc() != NULL || getGrabDoc() != NULL);
}

void RecordManager::deleteGrab(RecordGrab* grab)
{
	ASSERT(grab);

	if ((grab)&&
		(AfxMessageBox("Do you want to delete grab of " + grab->getDisplayName() + ".", MB_ICONQUESTION|MB_YESNO) == IDYES)&&
		(AfxMessageBox("This action will permanently remove the grab of " + grab->getDisplayName() + ".\n If you want to simply remove from the filter, use Records->Apply Logic->Remove Grab From Filter.", MB_ICONEXCLAMATION|MB_OKCANCEL) == IDOK))
	{	


		//first remove the grab from the tower
		RecordTower* tower = grab->getTower();

		if (tower)
			tower->removeGrab(grab);

		//now remove from the filters
		//todo update matrix filter removeGrabFromFilteredList(grab);
		//todo update matrix filter removeGrabFromFilteredMemoryList(grab);

		//now remove from the grabs master list.
		for (int i=0;i<_allGrabsList.GetSize();i++)
		{
			if (_allGrabsList.GetAt(i) == grab)
			{
				_allGrabsMap.RemoveKey(_allGrabsList.GetAt(i)->getGrabIndex());
				_allGrabsList.RemoveAt(i);
			}
		}	 

		//rebuild the index
		sortAndIndexGrabs();

		//update 
		setUpdatePoint("Delete Grab");	
	}
}

void RecordManager::selectHomeTower()  //todo make this use selection from matrix.
{
	CPoint point;
	GetCursorPos(&point);
	RecordChooseTowerDlg dlg(mainFrame(), point, getHomeTower());
	dlg.DoModal();

	RecordTower* recordTower = dlg.getRecordTower();
	if (recordTower)
		setHomeTower(recordTower->getIndex());
}
/* todo move to matrix
CString RecordManager::getCopyData(RecordTower *tower) //todo remove
{
	const AvailableFields& selected = recordManager()->getSelectedFields(vt_tower); //these MUST be tower as they are displaying tower tables.
	const AvailableFields& available = recordManager()->getAvailableFields(vt_tower);
	AvailableFields others;

	for (int i=0;i<available.GetSize();i++)
	{
		bool needToAdd = true;
		for (int j=0;j<selected.GetSize();j++)
		{
			if (selected.GetAt(j) == available.GetAt(i))
			{
				needToAdd = false;
				break;
			}
		}

		if (needToAdd)
			others.Add(available.GetAt(i));
	}

	//sort the others #
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<others.GetUpperBound();i++)
		{
			if (recordManager()->getAvailableFieldsName(others[i]) > recordManager()->getAvailableFieldsName(others[i+1]))
			{
				changed = true;
				AvailableField temp    = others.GetAt(i);
				others.SetAt(i,others.GetAt(i+1));
				others.SetAt(i+1, temp);
			}
		}
	}


	CString output;

	for (int i=0;i<selected.GetSize();i++)
		output += getCopyDataLine(selected.GetAt(i), tower);    

	for (int i=0;i<others.GetSize();i++)
		output += getCopyDataLine(others.GetAt(i), tower);    


	//now do the grabs
	if (tower->getGrabCount() >0)
	{
		output += "\r\nGrabs\r\n";
	}
	for (int i=0;i<tower->getGrabCount();i++)
	{
		RecordGrab* grab = tower->getGrab(i);

		output += grab->getGrabIndexAsString() + "\t";
		output += grab->getDateAsString(false) + "\t";
		output += grab->getBellsGoAsString() + "\t";
		output += grab->getBellsSoundAsString() + "\t";
		output += grab->getCommentAsSingleLine() + "\t";
		output += "\r\n";
	}	

	return output;
}


CString RecordManager::getCopyDataLine(AvailableField field, RecordTower* tower)
{
	return recordManager()->getAvailableFieldsName(field) + "\t" +  
		   recordManager()->getAvailableFieldsString(field, tower, NULL, true) + "\r\n";
}
 */

void RecordManager::setPrintTowerRef(RecordTower *tower)
{
	_printTowerRef = tower;
}

RecordTower* RecordManager::getPrintTowerRef()
{ 
	return _printTowerRef;

}

BOOL RecordManager::OnPreparePrinting(CPrintInfo *pInfo, CView *view)
{
	TRACE("OnPreparePrinting\r\n");
	
	
	//a bit of jiggery to force the standard print dialog not to show.
	// see DoPreparePrinting for information
	BOOL holdDirectState = pInfo->m_bDirect;
	if (!_showPrintSetupDlg)
	{
		pInfo->m_bDirect = TRUE;
	}	

	BOOL ret = view->DoPreparePrinting(pInfo); //this is where we get the print dialog

	//go back st the previous state
	pInfo->m_bDirect = holdDirectState ;

	return ret;

}

void RecordManager::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo *pInfo)
{
	TRACE("OnBeginPrinting\r\n");

	PrintData* printData = new PrintData;
	pInfo->m_lpUserData = printData;
}

void RecordManager::OnPrepareDC(CDC *pDC, CPrintInfo *pInfo)
{
	//only do stuff if we are printing / preview
	if ((pInfo)&&(pDC->IsPrinting()))
	{
		PrintData* printData = (PrintData*)pInfo->m_lpUserData;

		pInfo->m_bContinuePrinting = (pInfo->m_nCurPage <= printData->_numPages );
		TRACE("TouchDoc::OnPrepareDC  curPage = %d  numPages = %d pInfo->m_bContinuePrinting = %s\r\n",pInfo->m_nCurPage, printData->_numPages,  pInfo->m_bContinuePrinting?"TRUE":"FALSE");
	}
}

void RecordManager::OnEndPrinting(CDC* /*pDC*/, CPrintInfo *pInfo)
{
	TRACE("OnEndPrinting\r\n");

	PrintData* printData = (PrintData*)pInfo->m_lpUserData;
	delete printData;
	
	pInfo->m_lpUserData = NULL;
}

void RecordManager::OnFilePrint()
{
	_showPrintSetupDlg = TRUE;
}

void RecordManager::OnFilePrintToolbar()
{
  	_showPrintSetupDlg = FALSE;
}

void RecordManager::OnFilePrintMenu()
{
	_showPrintSetupDlg = TRUE;
}
		   
void RecordManager::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{		  /* todo 
		//this gets called for each page in turn
	CPage*	page= new CPage(pInfo->m_rectDraw,pDC,MM_TEXT);

	//apply the appropriate offset;
	double vertSize = page->getLengthInches() - PrintUtils::getFootnoteHeight();
	page->setOffset(-(vertSize  * (pInfo->m_nCurPage -1)), 0.0);

	
	//leave the bottom clear
	//clear the area that has been printed on;
	page->ExcludeClipRect(page->getLengthInches() - PrintUtils::getFootnoteHeight(), 0.0,
							page->getLengthInches() + 0.1, 
							page->getWidthinches());


	double nextRow = (double)((double)(app()->GetProfileInt("Print", "Touch Top", 1000))/2540.0);		


	if (_printTowerRef)
	{
		nextRow = PrintUtils::printTitle(page, _printTowerRef->getDisplayName(), nextRow, true);
		nextRow = printTower(page, nextRow);
		nextRow = printGrabs(page, nextRow);
	}
	else
	{
		nextRow = PrintUtils::printTitle(page, "Tower: error" , nextRow, true);
	}

  
	//calculate the number of pages
	PrintData* printData = (PrintData*)pInfo->m_lpUserData;	   
	printData->_numPages = (int)(nextRow/vertSize) + 1;

	//before printing footnote and shareware stuff, remove the page offset / clip 
	page->setOffset(0.0,0.0);
	page->ResetClipRect();

	CString pageText;

	if (printData->_numPages > 1)
		pageText.Format("Page %d of %d",pInfo->m_nCurPage ,printData->_numPages);
		
	PrintUtils::printFootnote(page, pageText, PrintUtils::getFootnoteHeight());

	TRACE("Document has %d pages \r\n", printData->_numPages );

	if (!pInfo->m_bPreview)
		PrintUtils::printShareware(page);	

	delete page;
	 */
}   
	  /*

double RecordManager::printTower(CPage* page, double nextRow)
{
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	
	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont=page->SetFont("Arial");

	const AvailableFields& selected = recordManager()->getSelectedFields(vt_tower); //these MUST be tower as they are displaying tower tables.
	const AvailableFields& available = recordManager()->getAvailableFields(vt_tower);
	AvailableFields others;

	for (int i=0;i<available.GetSize();i++)
	{
		bool needToAdd = true;
		for (int j=0;j<selected.GetSize();j++)
		{
			if (selected.GetAt(j) == available.GetAt(i))
			{
				needToAdd = false;
				break;
			}
		}

		if (needToAdd)
			others.Add(available.GetAt(i));
	}

	//sort the others #
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<others.GetUpperBound();i++)
		{
			if (recordManager()->getAvailableFieldsName(others[i]) > recordManager()->getAvailableFieldsName(others[i+1]))
			{
				changed = true;
				AvailableField temp    = others.GetAt(i);
				others.SetAt(i,others.GetAt(i+1));
				others.SetAt(i+1, temp);
			}
		}
	}

	nextRow = PrintUtils::printTitle(page, "Tower", nextRow, false);

	TABLEHEADER* pTable =new TABLEHEADER;        
	pTable->PointSize=	8;
	pTable->UseInches=	TRUE;
	pTable->AutoSize=	FALSE;
	pTable->Border=		FALSE;
	pTable->FillFlag=	FILL_NONE;
	pTable->NumColumns=	2;
	pTable->NumRows=	available.GetSize();
	pTable->StartRow=	nextRow;
	pTable->StartCol=	column;
	pTable->EndCol=		page->getWidthinches();
	pTable->HLines =	FALSE;
	pTable->VLines =	FALSE;
	pTable->NoHeader =	TRUE;

	pTable->ColDesc[0].Init(PrintUtils::_AnalysisMargin,"");
	pTable->ColDesc[1].Init(3.0,"");

	pTable->_leftVLinesLimit = 10000;
	pTable->_topHLinesLimit = 10000;

	page->Table(pTable);
	


	int index;
	for (index=0;index<selected.GetSize();index++)
		printTowerLine(page, pTable, index, selected.GetAt(index));

	for (int i=0;i<others.GetSize();i++, index++)
		printTowerLine(page, pTable, index, others.GetAt(i));


	page->SetFont(oldFont);
	page->SetColor(oldColor);
	
	nextRow = pTable->EndRow + .1;

	delete pTable;
	pTable = NULL;

	return nextRow;
}


void RecordManager::printTowerLine(CPage* page, TABLEHEADER* pTable, int index, AvailableField field)
{
		
	CString fieldName = getAvailableFieldsName(field);
	CString fieldString = getAvailableFieldsString(field, _printTowerRef, NULL, true);

	//number of parts.
	page->Print(pTable,index,0, 8, TEXT_LEFT, 0.0, 0.0, fieldName.GetBuffer(fieldName.GetLength()));
	fieldName.ReleaseBuffer();

	page->Print(pTable,index,1, 8, TEXT_LEFT, 0.0, 0.0, fieldString.GetBuffer(fieldString.GetLength()));
	fieldString.ReleaseBuffer();	
}						

double RecordManager::printGrabs(CPage* page, double nextRow)
{		 
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	
	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont=page->SetFont("Arial");

	nextRow = PrintUtils::printTitle(page, "Grabs", nextRow, false);

	if(_printTowerRef->getGrabCount() != 0)
	{

		AvailableFields fields;
		fields.Add(af_grab_number);
		fields.Add(af_grab_date);
		fields.Add(af_grab_bellsGo);
		fields.Add(af_grab_bellsSound);
		fields.Add(af_grab_comments);


		TABLEHEADER* pTable =new TABLEHEADER;        
		pTable->PointSize=	8;
		pTable->UseInches=	TRUE;
		pTable->AutoSize=	FALSE;
		pTable->Border=		FALSE;
		pTable->FillFlag=	FILL_NONE;
		pTable->NumColumns=	fields.GetSize();
		pTable->NumRows=	_printTowerRef->getGrabCount() +1;
		pTable->StartRow=	nextRow;
		pTable->StartCol=	column;
		pTable->EndCol=		page->getWidthinches();
		pTable->HLines =	FALSE;
		pTable->VLines =	FALSE;
		pTable->NoHeader =	TRUE;

		pTable->ColDesc[0].Init(.75,"kufyg");
		pTable->ColDesc[1].Init(.75,"");
		pTable->ColDesc[2].Init(.75,"");
		pTable->ColDesc[3].Init(.75,"");
		pTable->ColDesc[4].Init(2.75,"");

		pTable->_leftVLinesLimit = 10000;
		pTable->_topHLinesLimit = 10000;

		page->Table(pTable);
		

		for (int i=0;i<fields.GetSize();i++)
		{
			CString fieldName = getAvailableFieldsName(fields.GetAt(i));
			page->Print(pTable,0,i, 8, TEXT_LEFT|TEXT_BOLD, 0.0, 0.0, fieldName.GetBuffer(fieldName.GetLength()));
			fieldName.ReleaseBuffer();
		}


		for (int i=0;i<_printTowerRef->getGrabCount();i++)
		{
			RecordGrab* grab = _printTowerRef->getGrab(i);

			for (int j=0;j<fields.GetSize();j++)
			{
				CString fieldString = getAvailableFieldsString(fields.GetAt(j), _printTowerRef, grab, true);
				page->Print(pTable,i + 1,j, 8, TEXT_LEFT, 0.0, 0.0, fieldString.GetBuffer(fieldString.GetLength()));
				fieldString.ReleaseBuffer();
			}

		}

		nextRow = pTable->EndRow + .1;

		delete pTable;
		pTable = NULL;

	}
	else
	{
		page->Print(nextRow ,column, TEXT_LEFT, 8, "No Grabs");		
	}

	page->SetFont(oldFont);
	page->SetColor(oldColor);
	
					
	return nextRow;


}
			   */
void RecordManager::OnUserfiltermore()
{ 
  /*
	MoreDlg dlg(NULL, strings, "Filter");
	if (dlg.DoModal() == IDOK)
		applyUserFilter(viewType, dlg.getIndex());
		*///todo move to the matrix and test
}




void RecordManager::OnDeletetower() 
{
	deleteTower();	
}

void RecordManager::OnTowerSethometower() 
{
	selectHomeTower();	
}

void RecordManager::OnUpdateTowerSethometower(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(getTowerDoc() != NULL||getGrabDoc() != NULL);
}


void RecordManager::OnGrabsAddtowergrab() 
{
	createGrab();	
}

void RecordManager::OnUpdateGrabsAddtowergrab(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(getTowerDoc() != NULL||getGrabDoc() != NULL);
}



void RecordManager::OnEdittower() 
{
	editTower();		
}

void RecordManager::OnGrabOptions() 
{		 
	OptionsDisplayFilter filter(false);
	filter._grabs = true;
	filter._scales = true;
	filter._map = true;
	filter._users = true;
	filter._displayScreenType = os_Grabs;
	OptionsDlg propSheet(mainFrame(), filter);
	propSheet.DoModal();	
}

void RecordManager::OnTowerDatabase() 
{
	CWaitCursor wait;
	TowerDoc * pTowerDoc = getTowerDoc();
	if (pTowerDoc)
		pTowerDoc->OnCloseDocument();
	else
		app()->openDocument(vt_tower);	
}

void RecordManager::OnGrabs() 
{
	CWaitCursor wait;
	GrabDoc* pGrabDoc = getGrabDoc();
	if (pGrabDoc)
		pGrabDoc->OnCloseDocument();
	else
		app()->openDocument(vt_grab);	
}

void RecordManager::OnUpdateTowerDatabase(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(getTowerDoc() != NULL);		
}

TowerDoc* RecordManager::getTowerDoc() //should only ever be one of these
{
	if (app()->m_pDocManager != NULL)
	{
		POSITION TemplatePos = app()->GetFirstDocTemplatePosition();
		while(TemplatePos)//end itterate doc templates
		{
			CDocTemplate * pTemplate = app()->GetNextDocTemplate(TemplatePos);
		
			POSITION pos2 = pTemplate->GetFirstDocPosition();
			while (pos2)
			{
				CDocument* doc = pTemplate->GetNextDoc(pos2);
				TowerDoc* pTowerDoc = dynamic_cast<TowerDoc*>(doc);
				if (pTowerDoc)
					return pTowerDoc;
			}
		}
	}
	return NULL;
}

void RecordManager::OnUpdateGrabs(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(getGrabDoc() != NULL);		
}


GrabDoc* RecordManager::getGrabDoc() //should only ever be one of these
{
	if (app()->m_pDocManager != NULL)
	{
		POSITION TemplatePos = app()->GetFirstDocTemplatePosition();
		while(TemplatePos)//end itterate doc templates
		{
			CDocTemplate * pTemplate = app()->GetNextDocTemplate(TemplatePos);
		
			POSITION pos2 = pTemplate->GetFirstDocPosition();
			while (pos2)
			{
				CDocument* doc = pTemplate->GetNextDoc(pos2);
				GrabDoc* pGrabDoc = dynamic_cast<GrabDoc*>(doc);
				if (pGrabDoc)
					return pGrabDoc;
			}
		}
	}
	return NULL;
}



void RecordManager::OnFilterCombo() 
{
	CPoint point;
	::GetCursorPos(&point);

	FilterPopup* filterPopup = new FilterPopup();
	filterPopup->Create(mainFrame());
	filterPopup->Set(point);
}


void RecordManager::OnTowerOptions() 
{		 
	OptionsDisplayFilter filter(false);
	filter._tower = true;
	filter._scales = true;
	filter._map = true;
	filter._internet = true;
	filter._users = true;
	filter._displayScreenType = os_Tower;
	OptionsDlg propSheet(mainFrame(), filter);
	propSheet.DoModal();	
}

