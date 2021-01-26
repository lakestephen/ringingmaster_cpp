#pragma once

#include "ManagerBase.h"
#include "RecordConstants.h"
#include "ListSelectItem.h"
#include "RecordManagerEventListener.h"

class TowerDoc;
class GrabDoc;
class TowerDataModification;

enum AvailableField //todo remove
{
};


class RecordManager : public CCmdTarget, public ManagerBase 
{
public:		

	void save(){};
	void load(){};

	void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	void OnFilePrint();
	void OnFilePrintToolbar();
	void OnFilePrintMenu();
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	BOOL OnPreparePrinting(CPrintInfo* pInfo, CView* view);
	RecordTower* getPrintTowerRef();
	void setPrintTowerRef(RecordTower* tower);
	void selectHomeTower();
	void deleteGrab(RecordGrab* grab);
	void deleteTower(RecordTower* recordTower = NULL);
	void editTower(RecordTower* recordTower = NULL);
	void createAllTowers();
	void addTowerDataModification(TowerDataModification* towerDataModification);
	void removeTowerDataModification(TowerDataModification* towerDataModification);
	int getNextTowerDataModificationIndex();
/*	todo void preCalculateSortDirection(RSTowerDirection*sort, RecordTowers *towers);
	void preCalculateSortDistance(RSTowerDistance* sort, RecordTowers *towers);
*/	

	void fireUpdateFilterList();
	void fireHomeTowerChange();

	int getAllTowersCount();
	RecordTower* getAllTower(int index);
	int getAllGrabsCount();
	RecordGrab* getAllGrab(int index);
	bool findTower(CString name, RecordTowers& towers);
	void editGrabDayOrder(RecordGrab *recordGrab);
	bool hasMultipleGrabsOnDay(RecordGrab* recordGrab);
	bool setNextDayOrder(RecordGrab *recordGrab);

	static CString getNoteAsString(Note note, bool verbose = false);
	static CString getNightAsString(Night night, bool verbose = false);
	static CString getDirectionAsString(Direction direction, bool fullName = false);
	static CString getCountryAsString(Country country, bool verbose = false);

	TowerDoc* getTowerDoc();
	GrabDoc* getGrabDoc();

	void editGrab(RecordGrab* grab);
	void createGrab(int index = 0);
	void deleteUserData();
	void getAllNotes(ListSelectItems &items);
	void getAllCountrys(ListSelectItems &items);
	void getAllNights(ListSelectItems &items);
	void getAllDirections(ListSelectItems &items);
	void getAllBellsGo(ListSelectItems &items);
	void getAllBellsSound(ListSelectItems &items);
	void getAllNumbers(ListSelectItems& items);
	void getAllGrabCounts(ListSelectItems &items);
	void getAllCountys(ListSelectItems& items);
	void calculateDistanceAndDirectionFromHomeTowers();
	
	void addEventListener(RecordManagerEventListener *recordManagerEventListener);
	void removeEventListener(RecordManagerEventListener *recordManagerEventListener);

	RecordTower* getTower(int index);
	RecordGrab* getGrab(int index);
	void populateAllTowers(CComboBox& comboBox, int towerIndex, BOOL includeNonOSMapRef = TRUE, RecordTowers* recordTowers = NULL);
	void populateAllGrabs(CComboBox &comboBox, RecordGrab* grab, RecordGrabs* recordGrabs);
	
	void SerializeUserData(CArchive& ar);
	void SerializeBaseData(CArchive& ar);

	BOOL isValid(bool doLoad = true);
	void loadTowers();

	void setHomeTower(int homeTowerIndex);
	int getHomeTowerIndex();
	RecordTower* getHomeTower();

	RecordManager();
	virtual ~RecordManager();

	void removeRecordTower(RecordTower* recordTower);	

	void getDistanceAndDirection(int X1, int Y1, int X2, int Y2, double& distance, Direction& direction);
	void setUpdatePoint(CString name, bool setModifyed = true, bool checkPoint = true);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RecordManager)
	public:
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(RecordManager)
	afx_msg void OnAddtower();
	afx_msg void OnDeletetower();
	afx_msg void OnTowerSethometower();
	afx_msg void OnGrabsAddtowergrab();
	afx_msg void OnEdittower();
	afx_msg void OnGrabOptions();
	afx_msg void OnTowerDatabase();
	afx_msg void OnUpdateTowerDatabase(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGrabs(CCmdUI* pCmdUI);
	afx_msg void OnGrabs();
	afx_msg void OnUserfiltermore();   
	afx_msg void OnFilterCombo();
	afx_msg void OnTowerOptions();

	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()





protected:
/* todo
	double printTower(CPage* page, double nextRow);
	void printTowerLine(CPage* page, TABLEHEADER* pTable, int index, AvailableField field);
	double printGrabs(CPage* page, double nextRow);

	 */

	//updating
	RecordManagerEventListeners _listenerList;


	static int compareGrabs( const void * arg1, const void * arg2 );

	void sortAndIndexGrabs();
	void addGrab(RecordGrab* recordGrab, BOOL doSort = TRUE);
	void addRecordTower(RecordTower* recordTower);

	//global data
	BOOL _validLoad;
	
	//the three proper data lists
	TowerDataStatics _towerDataStatics;
	TowerDataModifications _towerDataModifications;
	RecordGrabs _allGrabsList;
	CMap<int, int, RecordGrab*, RecordGrab*> _allGrabsMap; //map points to same data as list


	//the all towers list and map
	RecordTowers _allTowersList; //todo make these maps and lists a single entity
	CMap<int, int, RecordTower*, RecordTower*> _allTowersMap; //map points to same data as list
	
	//user specific data
	int _homeTowerIndex; //index on the main list

	RecordTower* _printTowerRef; 
	BOOL _showPrintSetupDlg;

public:
	afx_msg void OnUpdateAddtower(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGrabsAddtowergrab(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTowerSethometower(CCmdUI *pCmdUI);
};

