// RFTowerDirectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RFTowerDirectionDlg.h"
#include "RecordManager.h"
#include "MapManager.h"
#include "RecordTower.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RFTowerDirectionDlg dialog


RFTowerDirectionDlg::RFTowerDirectionDlg(CWnd* /*pParent*/, ListSelectItems& available, ListSelectItems& selected, RFTowerDirectionData& rFTowerDirectionData) :
	ListSelectDlg(NULL, "Filter on directions", available, selected, RFTowerDirectionDlg::IDD),
	_originalRFTowerDirectionData(rFTowerDirectionData)
{
	//{{AFX_DATA_INIT(RFTowerDirectionDlg)
	_gridLoc = _T("");
	_locationType = 0;
	//}}AFX_DATA_INIT
}


void RFTowerDirectionDlg::DoDataExchange(CDataExchange* pDX)
{
	ListSelectDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RFTowerDirectionDlg)
	DDX_Control(pDX, IDC_GRID_LOC, _gridLocCtrl);
	DDX_Control(pDX, IDC_TOWER_COMBO, _towers);
	DDX_Text(pDX, IDC_GRID_LOC, _gridLoc);
	DDV_MaxChars(pDX, _gridLoc, 10);
	DDX_Radio(pDX, IDC_LOCATION_TYPE, _locationType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RFTowerDirectionDlg, ListSelectDlg)
	//{{AFX_MSG_MAP(RFTowerDirectionDlg)
	ON_BN_CLICKED(IDC_LOCATION_TYPE, OnLocationType)
	ON_BN_CLICKED(IDC_LOCATION_TYPE1, OnLocationType)
	ON_BN_CLICKED(IDC_LOCATION_TYPE2, OnLocationType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RFTowerDirectionDlg message handlers

void RFTowerDirectionDlg::OnOK() 
{

	UpdateData();

	if (((RFTowerDirectionType)_locationType == rftdit_specific_tower)&&
		(_towers.GetCurSel() == CB_ERR))
	{
		AfxMessageBox("Please select a tower");
		return;
	}

	MapGridRef mapGridRef;
	mapGridRef.setGridRef(_gridLoc);

	if (((RFTowerDirectionType)_locationType == rftdit_grid_ref)&&
		(!mapGridRef.isValidOSGridRef()))
	{
		AfxMessageBox("Please enter a valid OS grid Ref in the form NN123123");
		return;
	}

	_originalRFTowerDirectionData._rFTowerDirectionType = (RFTowerDirectionType)_locationType ;
	
	_originalRFTowerDirectionData._gridLocation =  mapGridRef; 
	
	_originalRFTowerDirectionData._tower = 0;
	if (_towers.GetCurSel() != CB_ERR)
	{
		RecordTower* recordTower = static_cast<RecordTower*>(_towers.GetItemDataPtr(_towers.GetCurSel()));
		_originalRFTowerDirectionData._tower = recordTower->getIndex();
	}

	ListSelectDlg::OnOK();
	
}

BOOL RFTowerDirectionDlg::OnInitDialog() 
{
	ListSelectDlg::OnInitDialog();
	
	recordManager()->populateAllTowers(_towers, _originalRFTowerDirectionData._tower, FALSE);

	_gridLoc = _originalRFTowerDirectionData._gridLocation.getDisplayGridRef();
	_locationType = _originalRFTowerDirectionData._rFTowerDirectionType;

	UpdateData(FALSE);
	OnLocationType();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}


void RFTowerDirectionDlg::OnLocationType() 
{
	UpdateData();
	_gridLocCtrl.EnableWindow((_locationType == rftdit_grid_ref)?SW_SHOW:SW_HIDE);
	_towers.EnableWindow((_locationType == rftdit_specific_tower)?SW_SHOW:SW_HIDE);

}

