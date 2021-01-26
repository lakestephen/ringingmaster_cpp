// TowerSoundsOptions.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsOptions.h"
#include "Tower.h"
#include ".\towersoundsoptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsOptions dialog


TowerSoundsOptions::TowerSoundsOptions( Tower& internalTower)
	: CBCGPPropertyPage(TowerSoundsOptions::IDD),
	_internalTower(internalTower)
{
	//{{AFX_DATA_INIT(TowerSoundsOptions)
	_smaller = -1;
	_repeatStart = 0;
	_coverCount = 0;
	_repeatEnd = 0;
	//}}AFX_DATA_INIT
}


void TowerSoundsOptions::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TowerSoundsOptions)
	DDX_Control(pDX, IDC_TOWER_EXTRA_COVERFOR_ODD_BELLS, _extraCoverForOddBells);
	DDX_Control(pDX, IDC_COVER_COUNT_SPIN, _coverCountSpin);
	DDX_Control(pDX, IDC_REPEAT_SPIN_START, _repeatSpinStart);
	DDX_Control(pDX, IDC_REPEAT_SPIN_END, _repeatSpinEnd);
	DDX_Radio(pDX, IDC_TOWER_SMALLER_FRONT, _smaller);
	DDX_Text(pDX, IDC_REPEAT_EDIT_START, _repeatStart);
	DDV_MinMaxInt(pDX, _repeatStart, 0, 200);
	DDX_Text(pDX, IDC_COVER_COUNT_EDIT, _coverCount);	
	DDV_MinMaxInt(pDX, _coverCount, 0, MAXBELLS);
	DDX_Text(pDX, IDC_REPEAT_EDIT_END, _repeatEnd);
	DDV_MinMaxInt(pDX, _repeatEnd, 0, 200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TowerSoundsOptions, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(TowerSoundsOptions)
	ON_UPDATE_COMMAND_UI(IDC_TOWER_EXTRA_COVERFOR_ODD_BELLS, OnUpdateTowerCover)
	ON_UPDATE_COMMAND_UI(IDC_STATIC_COVER, OnUpdateTowerCover)
	ON_BN_CLICKED(IDC_TOWER_EXTRA_COVERFOR_ODD_BELLS, OnTowerCover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsOptions message handlers

BOOL TowerSoundsOptions::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_repeatSpinStart.SetRange32(0,200);
	_repeatSpinEnd.SetRange32(0,200);

	_coverCountSpin.SetRange32(0,MAXBELLS);

	_smaller = (int)_internalTower.getSmallerRingType();

	CRect rcRect;
	GetDlgItem(IDC_HAND_LEAD)->GetWindowRect(&rcRect);
	ScreenToClient(&rcRect);
	_handLeadGap.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,rcRect,this,IDC_VOL);
	_handLeadGap.SetRange(0,2);//-10Db through 10Db
	_handLeadGap.SetValue((float)_internalTower.getOpenHandstrokeGap());

	_repeatStart = _internalTower.getStartChangeRepeat();
	_coverCount = _internalTower.getCoverCount();
	_repeatEnd = _internalTower.getEndChangeRepeat();

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control

}

void TowerSoundsOptions::OnUpdateTowerCover(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GlobalFunctions::isEven(_internalTower.getNumber()));	
	pCmdUI->SetCheck((!GlobalFunctions::isEven(_internalTower.getNumber()))?FALSE:_internalTower.getExtraCoverForOddBells());
}

void TowerSoundsOptions::OnTowerCover() 
{
	_internalTower.setExtraCoverForOddBells(_extraCoverForOddBells.GetCheck());	
}

void TowerSoundsOptions::OnOK()
{
	_internalTower.setSmallerRingType(static_cast<SmallerRingType>(_smaller));
	_internalTower.setOpenHandstrokeGap(_handLeadGap.GetValue());
	_internalTower.setEndChangeRepeat(_repeatEnd);
	_internalTower.setStartChangeRepeat(_repeatStart);
	_internalTower.setCoverCount(_coverCount);

	CBCGPPropertyPage::OnOK();
}
