// RecordChooseTowerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RecordChooseTowerDlg.h"
#include "RecordManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//todo replace this class with generic matrix class 
/////////////////////////////////////////////////////////////////////////////
// RecordChooseTowerDlg dialog


RecordChooseTowerDlg::RecordChooseTowerDlg(CWnd* pParent, CPoint requestPosition, RecordTower* initialTower, RecordTowers* recordTowers)
	: CDialog(RecordChooseTowerDlg::IDD, pParent),
	_requestPosition(requestPosition), 
	_recordTowers(recordTowers),
	_recordTower(NULL),
	_initialTower(initialTower)
{
	//{{AFX_DATA_INIT(RecordChooseTowerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void RecordChooseTowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RecordChooseTowerDlg)
	DDX_Control(pDX, IDC_TOWER_COMBO, _towers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RecordChooseTowerDlg, CDialog)
	//{{AFX_MSG_MAP(RecordChooseTowerDlg)
	ON_CBN_SELCHANGE(IDC_TOWER_COMBO, OnSelchangeTowerCombo)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RecordChooseTowerDlg message handlers

BOOL RecordChooseTowerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	/* todo
	if (_initialTower == NULL)
		_initialTower = recordManager()->recordManager_notifyGetSelectedTower();

	if (_initialTower == NULL)
		_initialTower = recordManager()->getHomeTower();

	int index = ((_initialTower == NULL)?-1:_initialTower->getIndex());

	recordManager()->populateAllTowers(_towers, index, TRUE, _recordTowers);

	if ((_requestPosition.x != -1)&&(_requestPosition.y != -1))
		GlobalFunctions::negotiateSpace(this, _requestPosition); 

					*/
	return TRUE;  // return TRUE unless you set the focus to a control

}

void RecordChooseTowerDlg::OnSelchangeTowerCombo() 
{
	if (_towers.GetCurSel() != CB_ERR)
	{
		_recordTower = static_cast<RecordTower*>(_towers.GetItemDataPtr(_towers.GetCurSel()));
		EndDialog(IDOK);
	}	
}

RecordTower* RecordChooseTowerDlg::getRecordTower()
{		 
	return _recordTower;
}

void RecordChooseTowerDlg::OnClose() 
{
	if (_towers.GetCurSel() != CB_ERR)
	{
		_recordTower = static_cast<RecordTower*>(_towers.GetItemDataPtr(_towers.GetCurSel()));
	}
	
	CDialog::OnClose();
}
