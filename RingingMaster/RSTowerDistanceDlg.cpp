// RSTowerDistanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RSTowerDistanceDlg.h"
#include "RecordManager.h"
#include "MapManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RSTowerDistanceDlg dialog


RSTowerDistanceDlg::RSTowerDistanceDlg(RFTowerDistanceData* rFTowerDistanceData,  BOOL& ascending, CWnd* pParent)
	: CDialog(RSTowerDistanceDlg::IDD, pParent),
	_originalRFTowerDistanceData(rFTowerDistanceData),
	_originalAscending(ascending),
	_ascending(ascending?0:1)

{
	//{{AFX_DATA_INIT(RSTowerDistanceDlg)
	_gridLoc = _T("");
	_locationType = 0;
	//}}AFX_DATA_INIT
}


void RSTowerDistanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RSTowerDistanceDlg)
	DDX_Control(pDX, IDC_GRID_LOC, _gridLocCtrl);
	DDX_Control(pDX, IDC_TOWER_COMBO, _towers);
	DDX_Text(pDX, IDC_GRID_LOC, _gridLoc);
	DDV_MaxChars(pDX, _gridLoc, 10);
	DDX_Radio(pDX, IDC_ASCENDING, _ascending);
	DDX_Radio(pDX, IDC_LOCATION_TYPE, _locationType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RSTowerDistanceDlg, CDialog)
	//{{AFX_MSG_MAP(RSTowerDistanceDlg)
	ON_BN_CLICKED(IDC_LOCATION_TYPE, OnLocationType)
	ON_BN_CLICKED(IDC_LOCATION_TYPE1, OnLocationType)
	ON_BN_CLICKED(IDC_LOCATION_TYPE2, OnLocationType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RSTowerDistanceDlg message handlers

BOOL RSTowerDistanceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	recordManager()->populateAllTowers(_towers, _originalRFTowerDistanceData->_tower, FALSE);

	_locationType = _originalRFTowerDistanceData->_rFTowerDistanceType;
	_gridLoc = _originalRFTowerDistanceData->_gridLocation.getDisplayGridRef();

	UpdateData(FALSE);
	OnLocationType();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void RSTowerDistanceDlg::OnLocationType() 
{
	UpdateData();
	_gridLocCtrl.EnableWindow((_locationType == rftdt_grid_ref)?SW_SHOW:SW_HIDE);
	_towers.EnableWindow((_locationType == rftdt_specific_tower)?SW_SHOW:SW_HIDE);

}

void RSTowerDistanceDlg::OnOK() 
{
	   /*  todo
	UpdateData();  

	_originalAscending = (_ascending == 0);


	if (((RFTowerDistanceType)_locationType == rftdt_specific_tower)&&
		(_towers.GetCurSel() == CB_ERR))
	{
		AfxMessageBox("Please select a home tower");
		return;
	}

	MapGridRef mapGridRef;
	mapGridRef.setGridRef(_gridLoc);

	if (((RFTowerDirectionType)_locationType == rftdt_grid_ref)&&
		(!mapGridRef.isValidOSGridRef()))
	{
		AfxMessageBox("Please enter a valid OS grid Ref in the form NN123123");
		return;
	}
	_originalRFTowerDistanceData->_rFTowerDistanceType = (RFTowerDistanceType)_locationType ;

	_originalRFTowerDistanceData->_gridLocation =  mapGridRef; 

	_originalRFTowerDistanceData->_tower = 0;
	if (_towers.GetCurSel() != CB_ERR)
	{
		RecordTower* recordTower = static_cast<RecordTower*>(_towers.GetItemDataPtr(_towers.GetCurSel()));
		_originalRFTowerDistanceData->_tower = recordTower->getIndex();
	}
			 */
	CDialog::OnOK();
}
