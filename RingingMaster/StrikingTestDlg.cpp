// StrikingTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "StrikingTestDlg.h"
#include ".\strikingtestdlg.h"


// StrikingTestDlg dialog

IMPLEMENT_DYNAMIC(StrikingTestDlg, CDialog)
StrikingTestDlg::StrikingTestDlg(CWnd* pParent, int maxNumber)
	: CDialog(StrikingTestDlg::IDD, pParent)
	, _bellsAffected(1)
	, _allowFllowOn(FALSE)
	, _hideVisualFeedback(FALSE)
	, _maxNumber(maxNumber)
	, _lockHandBack(FALSE)
{
	_bellsAffected = app()->GetProfileInt("Simulator", "Listen Test Bells Affected", 1);
	_allowFllowOn = app()->GetProfileInt("Simulator", "Listen Test Allow Follow On", FALSE);
	_hideVisualFeedback = app()->GetProfileInt("Simulator", "Listen Test Hide Feedback", TRUE);
	_lockHandBack = app()->GetProfileInt("Simulator", "Listen Test Lock Hand Back", TRUE);
	CString maxError = app()->GetProfileString("Simulator", "Listen Test Max Error", "10.0");
	_maxErrorValue = atof(maxError);

}

StrikingTestDlg::~StrikingTestDlg()
{
}

void StrikingTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BELLS_AFFECTED, _bellsAffected);
	DDV_MinMaxInt(pDX, _bellsAffected, 1, _maxNumber);
	DDX_Control(pDX, IDC_BELLS_AFFECTED, _bellsAffectedCtrl);
	DDX_Control(pDX, IDC_BELLS_AFFECTED_SPIN, _bellsAffectedSpinCtrl);
	DDX_Check(pDX, IDC_FOLLOW_PREVIOUS_BELL, _allowFllowOn);
	DDX_Check(pDX, IDC_HIDE_VISUAL_FEEDBACK, _hideVisualFeedback);
	DDX_Check(pDX, IDC_LOCKHANDBACK, _lockHandBack);
}


BEGIN_MESSAGE_MAP(StrikingTestDlg, CDialog)
END_MESSAGE_MAP()


// StrikingTestDlg message handlers

BOOL StrikingTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_bellsAffectedSpinCtrl.SetRange32(1,_maxNumber);
	
		CRect rcRect;
	GetDlgItem(IDC_MAX_ERROR)->GetWindowRect(&rcRect);
	ScreenToClient(&rcRect);
	_maxError.Create(WS_VISIBLE | WS_TABSTOP | NES_SPIN | NES_POPUP | NES_LIVEUPDATE | NES_METER,rcRect,this,IDC_VOL);
	_maxError.SetRange(0,100);//-10Db through 10Db
	_maxError.SetValue((float)_maxErrorValue);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void StrikingTestDlg::OnOK()
{
	UpdateData();

	app()->WriteProfileInt("Simulator", "Listen Test Bells Affected", _bellsAffected );
	app()->WriteProfileInt("Simulator", "Listen Test Allow Follow On", _allowFllowOn);
	app()->WriteProfileInt("Simulator", "Listen Test Hide Feedback", _hideVisualFeedback);
	app()->WriteProfileInt("Simulator", "Listen Test Lock Hand Back", _lockHandBack);

	_maxErrorValue =  _maxError.GetValue();
	CString maxError;
	maxError.Format("%.3f", _maxErrorValue);
	app()->WriteProfileString("Simulator", "Listen Test Max Error", maxError);


	CDialog::OnOK();
}
