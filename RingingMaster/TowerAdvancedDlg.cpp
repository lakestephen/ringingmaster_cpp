// TowerAdvancedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerAdvancedDlg.h"
#include "TowerBellItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerAdvancedDlg dialog


TowerAdvancedDlg::TowerAdvancedDlg(CWnd* pParent,TowerBellItem& towerBellItem)
	: CDialog(TowerAdvancedDlg::IDD, pParent),
	_towerBellItem(towerBellItem), 
	_inited(false)
{
	//{{AFX_DATA_INIT(TowerAdvancedDlg)
	//}}AFX_DATA_INIT
}


void TowerAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TowerAdvancedDlg)
	DDX_Control(pDX, IDC_VOLUME_RESET, _btnVolumeReset);
	DDX_Control(pDX, IDC_OVERALL_FREQUENCY_RESET, _btnOverallFrequencyReset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TowerAdvancedDlg, CDialog)
	//{{AFX_MSG_MAP(TowerAdvancedDlg)
	ON_BN_CLICKED(IDC_OVERALL_FREQUENCY_RESET, OnOverallFrequencyReset)
	ON_BN_CLICKED(IDC_VOLUME_RESET, OnVolumeReset)
	//}}AFX_MSG_MAP
  ON_REGISTERED_MESSAGE(NEN_CHANGED,OnNumericEditChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerAdvancedDlg message handlers

BOOL TowerAdvancedDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_btnVolumeReset.SetImage(IDB_RESET);
	_btnVolumeReset.m_bDrawFocus = FALSE;
	_btnVolumeReset.SetTooltip("Reset volume override");
	_btnVolumeReset.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_btnOverallFrequencyReset.SetImage(IDB_RESET);
	_btnOverallFrequencyReset.m_bDrawFocus = FALSE;
	_btnOverallFrequencyReset.SetTooltip("Reset pitch override");
	_btnOverallFrequencyReset.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	CRect rcRect;
	GetDlgItem(IDC_VOL)->GetWindowRect(&rcRect);
	ScreenToClient(&rcRect);
	_vol.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,rcRect,this,IDC_VOL);
	_vol.SetRange(-10,10);//-10Db through 10Db
	_vol.SetValue((float)_towerBellItem.getVolumeOverride());

	GetDlgItem(IDC_FREQ)->GetWindowRect(&rcRect);
	ScreenToClient(&rcRect);
	_freq.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,rcRect,this,IDC_FREQ);
	_freq.SetRange(-100, 100);//-100% through 100% 
	_freq.SetValue((float)_towerBellItem.getFrequencyOverride());
	
	_inited = true;

	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void TowerAdvancedDlg::OnCancel() 
{		
	UpdateData();
 	 _towerBellItem.setFrequencyOverride(_freq.GetValue());
	 _towerBellItem.setVolumeOverride(_vol.GetValue());

	CDialog::OnCancel();
}


void TowerAdvancedDlg::OnOverallFrequencyReset() 
{
	_towerBellItem.resetFrequencyOverride();
	
	_freq.SetValue((float)_towerBellItem.getFrequencyOverride());
}

void TowerAdvancedDlg::OnVolumeReset() 
{
	_towerBellItem.resetVolumeOverride();	

	_vol.SetValue((float)_towerBellItem.getVolumeOverride());
}

LRESULT TowerAdvancedDlg::OnNumericEditChange(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
	if (_inited)
	{
		_towerBellItem.setFrequencyOverride(_freq.GetValue());
		_towerBellItem.setVolumeOverride(_vol.GetValue());
	}

  return 0;
}
