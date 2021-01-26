// OptionsTowerSounds.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsTowerSounds.h"
#include "Tower.h"
#include "TowerManager.h"
#include "PlayManager.h"
#include ".\optionstowersounds.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsTowerSounds dialog


OptionsTowerSounds::OptionsTowerSounds()
	: CBCGPPropertyPage(OptionsTowerSounds::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsTowerSounds)
	_tower = -1;
	//}}AFX_DATA_INIT

	towerManager()->addEventListener(this);	

}

OptionsTowerSounds::~OptionsTowerSounds()
{
	towerManager()->removeEventListener(this);
}

void OptionsTowerSounds::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsTowerSounds)
	DDX_Control(pDX, IDC_OPTIONS_TOWER_TEST, _testBtn);
	DDX_Control(pDX, IDC_OPTIONS_TOWER_CLONE, _cloneBtn);
	DDX_Control(pDX, IDC_OPTIONS_TOWER_EDIT, _editBtn);
	DDX_Control(pDX, IDC_OPTIONS_TOWER_DEL, _delBtn);
	DDX_Control(pDX, IDC_OPTIONS_TOWER_ADD, _addBtn);
	DDX_Control(pDX, IDC_OPTIONS_TOWER_COMBO, _comboTower);
	DDX_CBIndex(pDX, IDC_OPTIONS_TOWER_COMBO, _tower);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RESET_TOWERS, _resetTowers);
}


BEGIN_MESSAGE_MAP(OptionsTowerSounds, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsTowerSounds)
	ON_BN_CLICKED(IDC_OPTIONS_TOWER_ADD, OnOptionsTowerSoundsAdd)
	ON_BN_CLICKED(IDC_OPTIONS_TOWER_DEL, OnOptionsTowerSoundsDel)
	ON_BN_CLICKED(IDC_OPTIONS_TOWER_EDIT, OnOptionsTowerSoundsEdit)
	ON_BN_CLICKED(IDC_OPTIONS_TOWER_TEST, OnOptionsTowerTest)
	ON_BN_CLICKED(IDC_OPTIONS_TOWER_CLONE, OnOptionsTowerSoundsClone)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_CBN_SELCHANGE(IDC_OPTIONS_TOWER_COMBO, OnSelchangeOptionsTowerSoundsCombo)
	ON_WM_CLOSE()
	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_TOWER, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_ENABLE, OnUpdateEnable)
	ON_UPDATE_COMMAND_UI(IDC_OPTIONS_TOWER_COMBO, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_OPTIONS_TOWER_CLONE, OnUpdateGeneralSelected)		   	
	ON_UPDATE_COMMAND_UI(IDC_OPTIONS_TOWER_EDIT, OnUpdateGeneralSelected)		   	
	ON_UPDATE_COMMAND_UI(IDC_OPTIONS_TOWER_ADD, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_OPTIONS_TOWER_DEL, OnUpdateGeneralSelected)		   	
	ON_UPDATE_COMMAND_UI(IDC_OPTIONS_TOWER_TEST, OnUpdateGeneralSelected)		   	
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RESET_TOWERS, OnBnClickedResetTowers)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsTowerSounds message handlers


BOOL OptionsTowerSounds::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	_addBtn.SetImage(IDB_NEW);
	_addBtn.m_bDrawFocus = FALSE;
	_addBtn.SetTooltip("Add Tower Profile");
	_addBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_delBtn.SetImage(IDB_DELETE);
	_delBtn.m_bDrawFocus = FALSE;
	_delBtn.SetTooltip("Delete Tower Profile");
	_delBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_editBtn.SetImage(IDB_DOT_DOT_DOT);
	_editBtn.m_bDrawFocus = FALSE;
	_editBtn.SetTooltip("Edit Tower Profile");
	_editBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_cloneBtn.SetImage(IDB_CLONE);
	_cloneBtn.m_bDrawFocus = FALSE;
	_cloneBtn.SetTooltip("Clone Selected Tower Profile");
	_cloneBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_testBtn.SetImage(IDB_BELL);
	_testBtn.m_bDrawFocus = FALSE;
	_testBtn.SetTooltip("Test the Tower Profile Sound.");
	_testBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_resetTowers.m_bDrawFocus = FALSE;
	_resetTowers.SetTooltip("Restore default towers.");
	_resetTowers.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	

	towerManager_notifyUpdateTowerList();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}
			    

void OptionsTowerSounds::OnOptionsTowerSoundsAdd() 
{
	towerManager()->newTower(this);
}

void OptionsTowerSounds::OnOptionsTowerSoundsDel() 
{
	UpdateData();

	towerManager()->deleteTower(_tower);
}

void OptionsTowerSounds::OnOptionsTowerSoundsEdit() 
{
	UpdateData();

	towerManager()->editTower(_tower, this);
}

void OptionsTowerSounds::OnOptionsTowerSoundsClone() 
{
	UpdateData();

	towerManager()->cloneTower(_tower, this);
}

void OptionsTowerSounds::OnSelchangeOptionsTowerSoundsCombo() 
{
	towerManager()->setActiveTowerIndex(_comboTower.GetCurSel());	
}

void OptionsTowerSounds::towerManager_notifyUpdateTowerList()
{
//	TRACE("Listener OptionsTowerSounds::towerManager_notifyUpdateTowerList() \r\n");

	if(IsWindow(_comboTower))
		towerManager()->populateComboBox(_comboTower);
}



void OptionsTowerSounds::OnEnable() 
{
	playManager()->setUsingSound(!playManager()->getUsingSound());	
}

void OptionsTowerSounds::OnUpdateGeneral(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(playManager()->isPlayAvailable());
}	

void OptionsTowerSounds::OnUpdateGeneralSelected(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(playManager()->isPlayAvailable() && towerManager()->getTower());
}	

void OptionsTowerSounds::OnUpdateEnable(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(playManager()->isDLLAvailable());
	pCmdUI->SetCheck(playManager()->getUsingSound()?1:0);
}		  
	 
void OptionsTowerSounds::OnOptionsTowerTest() 
{
	playManager()->test(false);	
}

void OptionsTowerSounds::OnBnClickedResetTowers()
{
	towerManager()->resetTowers();
}
