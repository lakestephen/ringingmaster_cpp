// RSTowerDirectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RSTowerDirectionDlg.h"
#include "RecordManager.h"
#include "RecordTower.h"
#include "MapManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RSTowerDirectionDlg dialog

 RSTowerDirectionDlg::RSTowerDirectionDlg(RFTowerDirectionData* rFTowerDirectionData,  BOOL& ascending, CWnd* pParent)
	: CDialog(RSTowerDirectionDlg::IDD, pParent),
	_originalRFTowerDirectionData(rFTowerDirectionData),
	_originalAscending(ascending),
	_ascending(ascending?0:1)

{
	//{{AFX_DATA_INIT(RSTowerDirectionDlg)
	_gridLoc = _T("");
	_locationType = 0;
	//}}AFX_DATA_INIT
}


void RSTowerDirectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RSTowerDirectionDlg)
	DDX_Control(pDX, IDC_GRID_LOC, _gridLocCtrl);
	DDX_Control(pDX, IDC_TOWER_COMBO, _towers);
	DDX_Text(pDX, IDC_GRID_LOC, _gridLoc);
	DDV_MaxChars(pDX, _gridLoc, 10);
	DDX_Radio(pDX, IDC_ASCENDING, _ascending);
	DDX_Radio(pDX, IDC_LOCATION_TYPE, _locationType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RSTowerDirectionDlg, CDialog)
	//{{AFX_MSG_MAP(RSTowerDirectionDlg)
	ON_BN_CLICKED(IDC_LOCATION_TYPE, OnLocationType)
	ON_BN_CLICKED(IDC_LOCATION_TYPE1, OnLocationType)
	ON_BN_CLICKED(IDC_LOCATION_TYPE2, OnLocationType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RSTowerDirectionDlg message handlers

BOOL RSTowerDirectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	recordManager()->populateAllTowers(_towers, _originalRFTowerDirectionData->_tower, FALSE);

	_locationType = _originalRFTowerDirectionData->_rFTowerDirectionType;
	_gridLoc = _originalRFTowerDirectionData->_gridLocation.getDisplayGridRef();

	UpdateData(FALSE);
	OnLocationType();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void RSTowerDirectionDlg::OnLocationType() 
{
	UpdateData();
	_gridLocCtrl.EnableWindow((_locationType == rftdit_grid_ref)?SW_SHOW:SW_HIDE);
	_towers.EnableWindow((_locationType == rftdit_specific_tower)?SW_SHOW:SW_HIDE);

}

void RSTowerDirectionDlg::OnOK() 
{

	UpdateData();  

	_originalAscending = (_ascending == 0);


	if (((RFTowerDirectionType)_locationType == rftdit_specific_tower)&&
		(_towers.GetCurSel() == CB_ERR))
	{
		AfxMessageBox("Please select a home tower");
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

	_originalRFTowerDirectionData->_rFTowerDirectionType = (RFTowerDirectionType)_locationType ;


	_originalRFTowerDirectionData->_gridLocation =  mapGridRef; 
	
	_originalRFTowerDirectionData->_tower = 0;
	if (_towers.GetCurSel() != CB_ERR)
	{
		RecordTower* recordTower = static_cast<RecordTower*>(_towers.GetItemDataPtr(_towers.GetCurSel()));
		_originalRFTowerDirectionData->_tower = recordTower->getIndex();
	}
	
	CDialog::OnOK();
}
