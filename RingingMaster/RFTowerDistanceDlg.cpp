// RFTowerDistanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RFTowerDistanceDlg.h"
#include "RecordManager.h"
#include "MapManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RFTowerDistanceDlg dialog


RFTowerDistanceDlg::RFTowerDistanceDlg(RFTowerDistanceData* rFTowerDistanceData, CWnd* /*pParent*/ )
	: CDialog(RFTowerDistanceDlg::IDD, NULL),
	_originalRFTowerDistanceData(rFTowerDistanceData)
{
	//{{AFX_DATA_INIT(RFTowerDistanceDlg)
	_distanceLogic = 0;
	_gridLoc = _T("");
	_locationType = 0;
	_distanceLabel = _T("");
	_distance = 0;
	//}}AFX_DATA_INIT
}  


void RFTowerDistanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RFTowerDistanceDlg)
	DDX_Control(pDX, IDC_GRID_LOC, _gridLocCtrl);
	DDX_Control(pDX, IDC_TOWER_COMBO, _towers);
	DDX_Control(pDX, IDC_DISTANCE_SPIN, _distanceSpin);
	DDX_Radio(pDX, IDC_DISTANCE_LOGIC, _distanceLogic);
	DDX_Text(pDX, IDC_GRID_LOC, _gridLoc);
	DDV_MaxChars(pDX, _gridLoc, 10);
	DDX_Radio(pDX, IDC_LOCATION_TYPE, _locationType);
	DDX_Text(pDX, IDC_DISTANCE_LABEL, _distanceLabel);
	DDX_Text(pDX, IDC_DISTANCE, _distance);
	DDV_MinMaxInt(pDX, _distance, 1, 99999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RFTowerDistanceDlg, CDialog)
	//{{AFX_MSG_MAP(RFTowerDistanceDlg)
	ON_BN_CLICKED(IDC_LOCATION_TYPE, OnLocationType)
	ON_BN_CLICKED(IDC_LOCATION_TYPE1, OnLocationType)
	ON_BN_CLICKED(IDC_LOCATION_TYPE2, OnLocationType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RFTowerDistanceDlg message handlers



BOOL RFTowerDistanceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_distanceLabel = 
		((app()->GetProfileInt("Tower Database", "Distance", 0) == 0)?"Miles":"Kilometers");
	
	recordManager()->populateAllTowers(_towers, _originalRFTowerDistanceData->_tower, FALSE);

	_distanceSpin.SetRange32(1,99999);
	_distance = _originalRFTowerDistanceData->_distance;
	_distanceLogic = _originalRFTowerDistanceData->_rFTowerDistanceLogic;

	_locationType = _originalRFTowerDistanceData->_rFTowerDistanceType;
	_gridLoc = _originalRFTowerDistanceData->_gridLocation.getDisplayGridRef();

	UpdateData(FALSE);
	OnLocationType();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void RFTowerDistanceDlg::OnLocationType() 
{
	UpdateData();
	_gridLocCtrl.EnableWindow((_locationType == rftdt_grid_ref)?SW_SHOW:SW_HIDE);
	_towers.EnableWindow((_locationType == rftdt_specific_tower)?SW_SHOW:SW_HIDE);

}

void RFTowerDistanceDlg::OnOK() 
{
		/*todo
	UpdateData();

	if (((RFTowerDistanceType)_locationType == rftdt_specific_tower)&&
		(_towers.GetCurSel() == CB_ERR))
	{
		AfxMessageBox("Please select a tower");
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

	_originalRFTowerDistanceData->_distance =_distance;
	_originalRFTowerDistanceData->_rFTowerDistanceLogic = (RFTowerDistanceLogic)_distanceLogic ;


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
