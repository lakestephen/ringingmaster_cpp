// TowerSoundsFilesAdvanced.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsFilesAdvanced.h"
#include "Tower.h"
#include "Bell3DDlg.h"
#include ".\towersoundsfilesadvanced.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsFilesAdvanced dialog


TowerSoundsFilesAdvanced::TowerSoundsFilesAdvanced(Tower& internalTower)
	: CBCGPPropertyPage(TowerSoundsFilesAdvanced::IDD),
	_internalTower(internalTower)
{
	//{{AFX_DATA_INIT(TowerSoundsFilesAdvanced)
	_speed = 0;

	//}}AFX_DATA_INIT
}


void TowerSoundsFilesAdvanced::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TowerSoundsFilesAdvanced)
	DDX_Control(pDX, IDC_RESET_VOL, _resetVol);
	DDX_Control(pDX, IDC_RESET_FREQ, _resetFreq);
	DDX_Control(pDX, IDC_3D_ENABLE, _enableCtrl);
	DDX_Control(pDX, IDC_EDIT_POS, _editPos);
	DDX_Control(pDX, IDC_PEAL_SPEED, _pealSpeed);
	DDX_Control(pDX, IDC_TOWER_SPEED, _speedCtrl);
	DDX_Slider(pDX, IDC_TOWER_SPEED, _speed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TowerSoundsFilesAdvanced, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(TowerSoundsFilesAdvanced)
	ON_BN_CLICKED(IDC_EDIT_POS, OnEditPos)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_POS, OnUpdateEditPos)
	ON_BN_CLICKED(IDC_3D_ENABLE, On3dEnable)
	ON_BN_CLICKED(IDC_RESET_VOL, OnResetVol)
	ON_BN_CLICKED(IDC_RESET_FREQ, OnResetFreq)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(PSM_QUERYSIBLINGS, OnQuerySiblings)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsFilesAdvanced message handlers


BOOL TowerSoundsFilesAdvanced::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_editPos.m_bDrawFocus = FALSE;
	_editPos.SetTooltip("Edit the three dimensional position of the bells.");
	_editPos.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_editPos.SetImage(IDB_BELL_AND_DOTS);

	_resetFreq.m_bDrawFocus = FALSE;
	_resetFreq.SetTooltip("Reset pitch");
	_resetFreq.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_resetFreq.SetImage(IDB_RESET);

	_resetVol.m_bDrawFocus = FALSE;
	_resetVol.SetTooltip("Reset volume");
	_resetVol.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_resetVol.SetImage(IDB_RESET);

	CRect rcRect;
	GetDlgItem(IDC_VOL)->GetWindowRect(&rcRect);
	ScreenToClient(&rcRect);
	_vol.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,rcRect,this,IDC_VOL);
	_vol.SetRange(-40,0);//-40Db through 0Db
	_vol.SetValue((float)_internalTower.getOverallVolume());

	GetDlgItem(IDC_FREQ)->GetWindowRect(&rcRect);
	ScreenToClient(&rcRect);
	_freq.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,rcRect,this,IDC_FREQ);
	_freq.SetRange((float)Tower::_minFreq,(float)Tower::_maxFreq);//1% through 1000%
	_freq.SetValue((float)_internalTower.getOverallFrequency());
	
	_speedCtrl.SetRange(Tower::_minSpeed, Tower::_maxSpeed);
	_speed = _internalTower.getSpeed();

	
	UpdateData(FALSE);
	
	_enableCtrl.SetCheck(_internalTower.getUse3D())	;

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}

void TowerSoundsFilesAdvanced::OnEditPos() 
{
	Bell3DDlg dlg(reinterpret_cast<TowerSoundsDlg*>(GetParent()), _internalTower);
	dlg.DoModal();
}	 

void TowerSoundsFilesAdvanced::OnUpdateEditPos(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_internalTower.getUse3D());
}	

void TowerSoundsFilesAdvanced::On3dEnable() 
{
	_internalTower.setUse3D(_enableCtrl.GetCheck()); 	
}													 

void TowerSoundsFilesAdvanced::OnResetVol() 
{
	_internalTower.resetOverallVolume();	

	_vol.SetValue((float)_internalTower.getOverallVolume());
}

void TowerSoundsFilesAdvanced::OnResetFreq() 
{
	_internalTower.resetOverallFrequency();	

	_freq.SetValue((float)_internalTower.getOverallFrequency());
}

BOOL TowerSoundsFilesAdvanced::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		if (!_internalTower.setOverallVolume(_vol.GetValue()))
		{
			AfxMessageBox("Set the volume between -40Db and 0Db");
			return FALSE;
		}
		if (!_internalTower.setOverallFrequency(_freq.GetValue()))
		{
			CString str;
			str.Format("Set the frequency between %.1f% and %.1f%", Tower::_minFreq, Tower::_maxFreq);
			AfxMessageBox(str);
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
	
}

void TowerSoundsFilesAdvanced::OnOK()
{
	_internalTower.setSpeed(_speed);

	CBCGPPropertyPage::OnOK();
}

void TowerSoundsFilesAdvanced::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd ==  _speedCtrl.m_hWnd)
		update();
	
	CBCGPPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}


void TowerSoundsFilesAdvanced::update()
{
	//update the peal time
	CString msg = GlobalFunctions::getPealTimeText(_internalTower.getNumber(), 
											_speedCtrl.GetPos(),
											_internalTower.getOpenHandstrokeGap(),
											true);

	_pealSpeed.SetWindowText("Peal Time on " + msg + " h:m:s"); 
}

LRESULT TowerSoundsFilesAdvanced::OnQuerySiblings( WPARAM wParam, LPARAM /*lParam */)
{
	if (wParam == TOWER_SOUNDS_UPDATE)
		update();

	return 0;
}