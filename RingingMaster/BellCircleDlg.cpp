// BellCircleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "BellCircleDlg.h"
#include "WinPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BellCircleDlg dialog


BellCircleDlg::BellCircleDlg(CWnd* pParent, int numberOfBells)
	: CDialog(BellCircleDlg::IDD, pParent),
	_numberOfBells(numberOfBells)
{
	//{{AFX_DATA_INIT(BellCircleDlg)
	_clockwise = TRUE;
	_userInCenter = TRUE;
	_number = 1;
	_meters = 2.5;
	//}}AFX_DATA_INIT
}


void BellCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BellCircleDlg)
	DDX_Control(pDX, IDC_NUMBER_SPIN, _numberSpin);
	DDX_Check(pDX, IDC_CLOCKWISE, _clockwise);
	DDX_Check(pDX, IDC_USER_IN_CENTER, _userInCenter);
	DDX_Text(pDX, IDC_NUMBER, _number);
	DDX_Text(pDX, IDC_METERS, _meters);
	DDV_MinMaxDouble(pDX, _meters, 0., 1000.);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _number, 1, _numberOfBells);
}


BEGIN_MESSAGE_MAP(BellCircleDlg, CDialog)
	//{{AFX_MSG_MAP(BellCircleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BellCircleDlg message handlers

BOOL BellCircleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_numberSpin.SetRange32(1,_numberOfBells);
	
	WinPos::Load(this, "BellCircleDlg", false);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL BellCircleDlg::DestroyWindow() 
{
	WinPos::Save(this, "BellCircleDlg");
	
	return CDialog::DestroyWindow();
}
