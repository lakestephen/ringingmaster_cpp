// TowerTTSItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerTTSItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerTTSItemDlg dialog


TowerTTSItemDlg::TowerTTSItemDlg(CWnd* parent, TowerTTSItem& itemTowerTTSItem)
	: CDialog(TowerTTSItemDlg::IDD, parent),
	_originalTowerTTSItem(itemTowerTTSItem)
{
	//{{AFX_DATA_INIT(TowerTTSItemDlg)
	//}}AFX_DATA_INIT
	
	_enable = _originalTowerTTSItem._active;
	_text = _originalTowerTTSItem._string;

}


void TowerTTSItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TowerTTSItemDlg)
	DDX_Control(pDX, IDC_RESET, _reserCtrl);
	DDX_Control(pDX, IDC_TEXT, _textCtrl);
	DDX_Control(pDX, IDC_LABEL, _labelCtrl);
	DDX_Control(pDX, IDC_ENABLE, _enableWnd);
	DDX_Check(pDX, IDC_ENABLE, _enable);
	DDX_Text(pDX, IDC_TEXT, _text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TowerTTSItemDlg, CDialog)
	//{{AFX_MSG_MAP(TowerTTSItemDlg)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerTTSItemDlg message handlers


BOOL TowerTTSItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_reserCtrl.SetImage(IDB_RESET);
	_reserCtrl.m_bDrawFocus = FALSE;
	_reserCtrl.SetTooltip("Set to original settings");
	_reserCtrl.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	CString strEnable;
	strEnable.Format("Enable '%s' Type", TowerTTSItem::getItemTypeText(_originalTowerTTSItem._type));
	_enableWnd.SetWindowText(strEnable);		

	strEnable.Format("Edit '%s' Type", TowerTTSItem::getItemTypeText(_originalTowerTTSItem._type));
	SetWindowText(strEnable);

	enable();

	return TRUE;  // return TRUE unless you set the focus to a control

}


void TowerTTSItemDlg::OnOK() 
{
	UpdateData();

	_originalTowerTTSItem._active = _enable;
	_originalTowerTTSItem._string = _text;
	
	if (_originalTowerTTSItem._active == FALSE)
		_originalTowerTTSItem._string.Empty();

	CDialog::OnOK();
}

void TowerTTSItemDlg::enable()
{
	_labelCtrl.EnableWindow(_enableWnd.GetCheck() == TRUE);
	_textCtrl.EnableWindow(_enableWnd.GetCheck() == TRUE);
}


void TowerTTSItemDlg::OnEnable() 
{
	enable();

}

void TowerTTSItemDlg::OnReset() 
{
	TowerTTSItem item(_originalTowerTTSItem._type);

	_enable = item._active;
	_text = item._string;

	UpdateData(FALSE);	

	enable();
}
