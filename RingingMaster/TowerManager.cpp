// TowerManager.cpp: implementation of the TowerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerManager.h"
#include "RegArchive.h"
#include "Tower.h"
#include "TowerSoundsDlg.h"
#include "OptionsDlg.h"
#include "PlayManager.h"
#include "TowerPopup.h"
#include "NewTowerWizard.h"
#include "SerialControlManager.h"
#include "MultiBellInterfaceManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TowerManager::TowerManager() :
_editingProfile(false)
{

}



TowerManager::~TowerManager()
{
	//destroy the tower objects
	for (int i=0;i<_towers.GetSize();i++)
		delete _towers.GetAt(i);
	_towers.RemoveAll();
}

BEGIN_MESSAGE_MAP(TowerManager, CCmdTarget)
	//{{AFX_MSG_MAP(TowerManager)
		ON_COMMAND(ID_EDIT_TOWER_DETAILS, OnEditTowerDetails)
		ON_UPDATE_COMMAND_UI(ID_EDIT_TOWER_DETAILS, OnUpdateEditTowerDetails)
		ON_UPDATE_COMMAND_UI(ID_TOWER_COMBO, OnUpdateEditTowerDetails)
	  	ON_COMMAND(ID_TOWER_COMBO, OnTowerCombo)
		ON_COMMAND(ID_TOWER_SOUND_OPTIONS, OnTowerSoundOptions)
		ON_COMMAND(ID_NEW_TOWER_PROFILE, OnNewTower)
		ON_CBN_SELCHANGE(ID_TOWER_COMBO, OnSelchangeTowerComboChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
   




void TowerManager::addEventListener(TowerManagerEventListener* towerManagerEventListener)
{
	ASSERT(towerManagerEventListener != NULL);

	_listenerList.Add(towerManagerEventListener);
	// bring this one up to date
	towerManagerEventListener->towerManager_notifyUpdateTowerList();	

}

void TowerManager::removeEventListener(TowerManagerEventListener* towerManagerEventListener)
{
	ASSERT(towerManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == towerManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	ASSERT(hasRemoved == 1);
}

void TowerManager::fireUpdateTowerList()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->towerManager_notifyUpdateTowerList(); 
	}
}
	   

void TowerManager::save()
{
	CString strToken, strEntry ;	

	int count = _towers.GetSize();
	
	app()->WriteProfileInt("Towers", "Count", count);
	app()->WriteProfileInt("Towers", "Active", _activeTower);
	
	for (int i=0;i<count;i++)
	{
		strEntry.Format("Tower %d", i);
		RegArchive ar(CArchive::store,"Towers",strEntry);   
		_towers[i]->Serialize(ar);
	}
}

void TowerManager::load()
{
	CString strToken, strEntry ;	
	
	int count = app()->GetProfileInt("Towers", "Count", 0);
	_activeTower = app()->GetProfileInt("Towers", "Active", 0);
	
	if (_activeTower >= count) _activeTower = 0;

	for (int i=0;i<count;i++)
	{
		strEntry.Format("Tower %d", i);
		RegArchive ar (CArchive::load, "Towers", strEntry);
		Tower* tower = new Tower;
		if (ar.isValid()) tower->Serialize(ar);
		_towers.Add(tower);
	}

	fireUpdateTowerList();
}


int TowerManager::getTowerCount()
{
	return _towers.GetSize();
}				  

//returns true if a change has been made
bool TowerManager::setActiveTowerIndex(int activeTower)
{
	if (_activeTower == activeTower)
	{
		fireUpdateTowerList();
		return false;
	}

	_activeTower = activeTower;
	fireUpdateTowerList();
	return true;
}

int TowerManager::getActiveTowerIndex()
{
	return _activeTower;
}

Tower* TowerManager::getTower(int index)
{
	if (index == -1) index = _activeTower;
	if ((index >= _towers.GetSize())||(index < 0)) return NULL;
		
	return _towers.GetAt(index);
}								


BOOL TowerManager::isTowerNameUnique(CString towerName)
{
	//check for unique name
	for (int i=0;i<_towers.GetSize();i++)
	{
		CString name = _towers.GetAt(i)->getName();
		if (towerName == name)
			return false;
	}
	
	return true;
}

void TowerManager::OnNewTower()
{
	newTower(mainFrame());
}

void TowerManager::newTower(CWnd* parent)
{
	Tower* tower = new Tower;

	NewTowerWizard wizard;
	UINT result = wizard.DoModal();

	if (result != IDCANCEL)
	{   
		if (result == IDOK)
		{
			SimulatorSetupType type = sst_home;

			//do wizard setup
			CString msg;
			Tower::CreateTower(wizard._treblePath, *tower, msg);

			if (wizard._usingSimulator)
			{
				switch (wizard._simulatorUseType)
				{
				case 0:
					type = sst_home;
					break;
				case 1:
					if (wizard._reflectorNumber == 0)
						type = sst_tower_serial_one_reflector;
					else
						type = sst_tower_serial_two_reflector;
					break;
				case 2:
					if (wizard._reflectorNumber == 0)
						type = sst_tower_mbi_one_reflector;
					else
						type = sst_tower_mbi_two_reflector;
					break;
				}
				tower->setSimulatorDefaults(type, wizard._comPort+1, (wizard._addSpeechCalls == TRUE));
			}
		}

		TowerSoundsDlg dlg(parent, *tower, false);

		if (dlg.DoModal() != IDCANCEL)
		{
			setActiveTowerIndex(_towers.Add(tower));

			//do checks for bits working
			if (wizard._simulatorUseType == 1 && !serialControlManager()->getUsingSerialControl())
			{
				AfxMessageBox("You have selected the use of the Serial Control. \n You now need to set this up.");
				mainFrame()->doSimulatorOptions(false, parent);
			}
			else if (wizard._simulatorUseType == 2 && !multiBellInterfaceManager()->getUsingMBI())
			{
				AfxMessageBox("You have selected the use of the Multi Bell Interface. \n You now need to set this up.");
				mainFrame()->doSimulatorOptions(false, parent);
			}

			return;
		}
	}

	//we did not add the tower
	delete tower;
}

void TowerManager::deleteTower(int index)
{
	ASSERT(index >=0);

	Tower* tower = getTower(index);
	if (tower)
	{
		delete tower;
		_towers.RemoveAt(index);
		setActiveTowerIndex((_towers.GetSize() >0)? 0:-1);		
	}
}

void TowerManager::cloneTower(int index, CWnd* parent)
{
	ASSERT(index >=0);

	Tower* tower = getTower(index);
	if (tower)
	{
		Tower* newTower = new Tower();
		*newTower = *tower;

		newTower->setName("Copy of " + newTower->getName());

		TowerSoundsDlg dlg(parent, *newTower, false);

		if (dlg.DoModal() != IDCANCEL)
		{
			setActiveTowerIndex(_towers.Add(newTower));
		}
		else
		{
			delete newTower;
		}
	}
}

void TowerManager::editTower(int index, CWnd* parent)
{
	Tower* tower = getTower(index);
	if (tower)
	{
		TowerSoundsDlg dlg(parent, *tower, true);
		dlg.DoModal();
	}		
	else if (getTowerCount() == 0)
	{
		if (AfxMessageBox("No profiles have been set up. Would you like to set up a profile now?", MB_YESNO|MB_ICONQUESTION) == IDYES)
			newTower(parent);
	}	
}
	   
void TowerManager::initialTowerSetup()
{
	if (app()->GetProfileInt("Towers", "Initial Setup", FALSE))
		return;	

	//flag that we have done this before
	app()->WriteProfileInt("Towers", "Initial Setup", TRUE);
	
	if (getTowerCount() == 0)
	{	//attempt tower setup

		//get the directory
		CString bellsDir = app()->_rootPath + "Bells\\*.*";
		
		//get a list of files in the bells dir
		CFileFind fileFind;

		if (fileFind.FindFile(bellsDir))
		{

			CString currFile;
			BOOL caryOn = TRUE;

			while (caryOn)
			{
				caryOn = fileFind.FindNextFile();

				currFile = fileFind.GetFilePath();

				TRACE(currFile + "\r\n");
				if (currFile.Find("01") != -1)	//to2do this causes a problem the 01 is in the path (i.e. Ringing Master 3.01)
				{
					Tower* tower = new Tower;
					CString msg;					
					Tower::CreateTower(currFile, *tower, msg);
					_activeTower = 0;
					_towers.Add(tower);					
				}			   
			}
		}
	}

	fireUpdateTowerList();

}

void TowerManager::populateComboBox(CComboBox &comboBox)
{
	comboBox.ResetContent();
	for (int i=0;i<_towers.GetSize();i++)
	{
		comboBox.AddString(_towers.GetAt(i)->getDisplayName());
	}
	comboBox.SetCurSel(_activeTower);
}

void TowerManager::incrementSpeed()
{
	Tower* activeTower = getTower();
	if (activeTower)
	{
		activeTower->incrementSpeed();
		fireUpdateTowerList();
	}
}

void TowerManager::decrementSpeed()
{
	Tower* activeTower = getTower();
	if (activeTower)
	{
		activeTower->decrementSpeed();
		fireUpdateTowerList();
	}
}

void TowerManager::incrementOverallFrequency()
{
	Tower* activeTower = getTower();
	if (activeTower)
	{
		activeTower->incrementOverallFrequency();
		fireUpdateTowerList();
	}
}

void TowerManager::decrementOverallFrequency()
{
	Tower* activeTower = getTower();
	if (activeTower)
	{
		activeTower->decrementOverallFrequency();
		fireUpdateTowerList();
	}
}


void TowerManager::OnEditTowerDetails() 
{
	editTower(-1, mainFrame());	
}

void TowerManager::OnTowerSoundOptions() 
{		 
	OptionsDisplayFilter filter(false);
	filter._towerSounds = true;
	OptionsDlg propSheet(mainFrame(), filter);
	propSheet.DoModal();	
}

void TowerManager::OnUpdateEditTowerDetails(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(playManager()->isPlayAvailable());
}

void TowerManager::OnTowerCombo() 
{
	CPoint point;
	::GetCursorPos(&point);

	TowerPopup* towerPopup = new TowerPopup();
	towerPopup->Create(mainFrame());
	towerPopup->Set(point);
}



void TowerManager::OnSelchangeTowerComboChange()
{
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	CString towerName("");

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_TOWER_COMBO, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); 
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo = 
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL)
			{
				if (setActiveTowerIndex(pCombo->GetCurSel()))
					break;
			}
		}
	}
}

bool TowerManager::isEditingProfile()
{
	return _editingProfile ;
}    

void TowerManager::setEditingProfile(bool editingProfile)
{
	ASSERT(_editingProfile != editingProfile);
	_editingProfile = editingProfile;
}

void TowerManager::resetTowers()
{
	if (AfxMessageBox("This action will remove all your towers and reset to the defaults. Do you wish to continue?",
		MB_ICONHAND|MB_YESNO) == IDYES)
	{
		while(_towers.GetSize() > 0)
			deleteTower(0);

		_towers.RemoveAll();
		
		app()->WriteProfileInt("Towers", "Initial Setup", FALSE);

		initialTowerSetup();
	}
}
