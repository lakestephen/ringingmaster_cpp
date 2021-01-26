// GrabDetailsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "GrabDetailsDlg.h"
#include "RecordManager.h"
#include "RecordGrab.h"
#include "RecordTower.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GrabDetailsDlg dialog


GrabDetailsDlg::GrabDetailsDlg(RecordGrab& recordGrab)
	: CDialog(GrabDetailsDlg::IDD, NULL), 
	_originalRecordGrab(recordGrab)

{
	_comments = _originalRecordGrab.getComments();
	_dateOle = _originalRecordGrab.getDateOle();

	//{{AFX_DATA_INIT(GrabDetailsDlg)
	_comments = _T("");
	//}}AFX_DATA_INIT
}


void GrabDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GrabDetailsDlg)
	DDX_Control(pDX, IDC_COMMENTS, _commentsCtrl);
	DDX_Control(pDX, IDC_TOWER, _towers);
	DDX_Control(pDX, IDC_BELL_SOUND, _bellSound);
	DDX_Control(pDX, IDC_BELL_GO, _bellGo);
	DDX_DateTimeCtrl(pDX, IDC_DATE, _dateOle);
	DDX_CBString(pDX, IDC_COMMENTS, _comments);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GrabDetailsDlg, CDialog)
	//{{AFX_MSG_MAP(GrabDetailsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GrabDetailsDlg message handlers

BOOL GrabDetailsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	recordManager()->populateAllTowers(_towers, _originalRecordGrab.getTowerIndex());
	_towers.EnableWindow(_originalRecordGrab.getTowerIndex() == 0);
	
	_commentsCtrl.LoadHistory("GrabDetailsDlg");
	_comments = _originalRecordGrab.getComments();

	_originalRecordGrab.bellGoFillComboBox(_bellGo);
	_originalRecordGrab.bellSoundFillComboBox(_bellSound);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void GrabDetailsDlg::OnOK() 
{
	UpdateData();

	if (_towers.GetCurSel() == CB_ERR)
	{
		AfxMessageBox("Please select a tower");
		return;
	}
	
	_commentsCtrl.SaveHistory();

	if (_towers.GetCurSel() != CB_ERR)
	{
		RecordTower* recordTower = static_cast<RecordTower*>(_towers.GetItemDataPtr(_towers.GetCurSel()));
		_originalRecordGrab.setTowerIndex(recordTower->getIndex());
	}

	_originalRecordGrab.setComments(_comments);

	
	//exclude the time component
	_originalRecordGrab.setDateOle(COleDateTime (_dateOle.GetYear(), _dateOle.GetMonth(), _dateOle.GetDay(),0,0,0));
	
	if (_bellGo.GetCurSel() != CB_ERR) 
		_originalRecordGrab.setBellsGo((BellsGo)_bellGo.GetItemData(_bellGo.GetCurSel())); 
	
	if (_bellSound.GetCurSel() != CB_ERR) 
		_originalRecordGrab.setBellsSound((BellsSound)_bellSound.GetItemData(_bellSound.GetCurSel())); 
	
	CDialog::OnOK();
}
