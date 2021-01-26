// TowerSoundsGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsGeneral.h"
#include "Tower.h"
#include "TowerManager.h"
#include "TowerSoundsDlg.h"

#include "TowerSoundsFIles.h"
#include "TowerSoundsDlg.h"
#include "UserMessages.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsGeneral dialog


TowerSoundsGeneral::TowerSoundsGeneral( Tower& internalTower)
	: CBCGPPropertyPage	(TowerSoundsGeneral::IDD),
	_internalTower(internalTower)
{
	//{{AFX_DATA_INIT(TowerSoundsGeneral)
	_name = _T("");
	_number = 0;

	//}}AFX_DATA_INIT
}


void TowerSoundsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TowerSoundsGeneral)
	DDX_Control(pDX, IDC_TEMPLATE_BUTT, _templateBtn);
	DDX_Control(pDX, IDC_TOWER_NUMBER, _numberCtrl);
	DDX_Text(pDX, IDC_TOWER_NAME, _name);
	DDX_CBIndex(pDX, IDC_TOWER_NUMBER, _number);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TowerSoundsGeneral, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(TowerSoundsGeneral)
	ON_EN_KILLFOCUS(IDC_TOWER_NAME, OnKillfocusTowerName)
	ON_CBN_SELCHANGE(IDC_TOWER_NUMBER, OnSelchangeTowerNumber)
	ON_BN_CLICKED(IDC_TEMPLATE_BUTT, OnTemplateButt)
	ON_MESSAGE(PSM_QUERYSIBLINGS, OnQuerySiblings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsGeneral message handlers

BOOL TowerSoundsGeneral::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	CString str;	   	

	for (int i=MINBELLS; i<= MAXBELLS; i++)
	{
		str.Format("%d", i);
		_numberCtrl.AddString(str);
	}

	_templateBtn.m_bDrawFocus = FALSE;
	_templateBtn.SetTooltip("Select the treble, and the program will automatically fill in the other files.");
	_templateBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_templateBtn.SetImage(IDB_TOWER_TREBLE);

	if (_internalTower.isExistingPath(_internalTower.getNumber()))
	{
		_templateBtn.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CREATE_TOWER_STATIC_LINE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CREATE_TOWER_STATIC)->ShowWindow(SW_HIDE);
	}

	update();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void TowerSoundsGeneral::OnOK()
{
	_internalTower.setName(_name);
	_internalTower.setNumber( _number +3, false);

	CBCGPPropertyPage::OnOK();
}

void TowerSoundsGeneral::OnKillfocusTowerName() 
{
	UpdateData();
	((TowerSoundsDlg*)GetParent())->setConstantText(_name);
	_internalTower.setName(_name);
}

void TowerSoundsGeneral::OnSelchangeTowerNumber() 
{
	UpdateData();	
	_internalTower.setNumber( _number +3, false);	
}

void TowerSoundsGeneral::update()
{
   	_name = _internalTower.getName();
	_number = _internalTower.getNumber() - 3;

	UpdateData(FALSE);
}

void TowerSoundsGeneral::OnTemplateButt() 
{
	UpdateData();

	if (_internalTower.createFromTemplate())
	{
		update();
		//the number may have changed, so update siblings
		VERIFY(QuerySiblings(TOWER_SOUNDS_UPDATE,0) == 0);
	}

	((TowerSoundsDlg*)GetParent())->showTowerSoundsFilesPage();
}

LRESULT TowerSoundsGeneral::OnQuerySiblings( WPARAM wParam, LPARAM /*lParam */)
{
	if (wParam == TOWER_SOUNDS_UPDATE)
		update();

	return 0;
}




