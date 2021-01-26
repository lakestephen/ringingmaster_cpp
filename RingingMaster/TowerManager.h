// TowerManager.h: interface for the TowerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOWERMANAGER_H__4DD01DD4_C574_11D6_B60B_000255162CF0__INCLUDED_)
#define AFX_TOWERMANAGER_H__4DD01DD4_C574_11D6_B60B_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TowerManagerEventListener.h"

class Tower;


#include "ManagerBase.h"
#include "StdAfx.h"	// Added by ClassView




class TowerManager : public CCmdTarget,  public ManagerBase 
{
public:
	void resetTowers();
	bool isEditingProfile();
	void setEditingProfile(bool editingProfile);

	void decrementSpeed();
	void incrementSpeed();
	void decrementOverallFrequency();
	void incrementOverallFrequency();
	void populateComboBox(CComboBox& comboBox);
	void initialTowerSetup();

	void editTower(int index = -1, CWnd* parent = NULL);
	void deleteTower(int index);
	void cloneTower(int index, CWnd* parent);
	void newTower(CWnd* parent);
	TowerManager();
	virtual ~TowerManager(); 

	void fireUpdateTowerList();
	void addEventListener(TowerManagerEventListener* towerManagerEventListener);
	void removeEventListener(TowerManagerEventListener* towerManagerEventListener);

	void save();
	void load();

	int getTowerCount();
	Tower* getTower(int index = -1); 
	BOOL isTowerNameUnique(CString towerName);

	int getActiveTowerIndex();
	bool setActiveTowerIndex(int activeTower);

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerManager)
	public:
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(TowerManager)
	afx_msg void OnEditTowerDetails();
	afx_msg void OnTowerSoundOptions();
	afx_msg void OnUpdateEditTowerDetails(CCmdUI* pCmdUI);
	afx_msg void OnTowerCombo();
	afx_msg void OnNewTower();
	afx_msg void OnSelchangeTowerComboChange();
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()


protected:
	TowerManagerEventListeners _listenerList;
	CArray <Tower*, Tower*> _towers;  
	int _activeTower;
	bool _editingProfile;

};

#endif // !defined(AFX_TOWERMANAGER_H__4DD01DD4_C574_11D6_B60B_000255162CF0__INCLUDED_)
