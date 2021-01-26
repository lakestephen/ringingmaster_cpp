// RecordTowerEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RecordTowerEditDlg.h"
#include "RecordTower.h"
#include "RecordAnalysisTowerTab.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RecordTowerEditDlg dialog


RecordTowerEditDlg::RecordTowerEditDlg(CWnd* pParent, RecordTower* tower )
	: ResizeDialog(RecordTowerEditDlg::IDD, pParent),
	_tower(tower)
{
	//{{AFX_DATA_INIT(RecordTowerEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void RecordTowerEditDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RecordTowerEditDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RecordTowerEditDlg, ResizeDialog)
	//{{AFX_MSG_MAP(RecordTowerEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RecordTowerEditDlg message handlers

BOOL RecordTowerEditDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
	
	CRect drawRect;
	GetDlgItem(IDC_TOWER_DUMMY)->GetWindowRect(&drawRect);
	ScreenToClient(&drawRect);

	//create the method window
	_analysisTowerTower.Create(this, drawRect);
	_analysisTowerTower.doUpdate(_tower);

	AddControl(AAA_ANALYSIS_TOWER_TOWER, resize, resize);
	AddControl(IDOK, repos_center, repos);

	WinPos::Load(this, "RecordTowerEditDlg", true);

	
	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL RecordTowerEditDlg::DestroyWindow() 
{
	WinPos::Save(this, "RecordTowerEditDlg");
	
	_analysisTowerTower.DestroyWindow();
	
	return __super::DestroyWindow();
}
