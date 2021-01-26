// LeadHeadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "LeadHeadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LeadHeadDlg dialog


LeadHeadDlg::LeadHeadDlg(CWnd* pParent, UINT IDD)
	: CDialog(IDD, pParent),
	_IDD(IDD)
{
	//{{AFX_DATA_INIT(LeadHeadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void LeadHeadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LeadHeadDlg)
		DDX_Control(pDX, IDC_LOWER, _left);
		DDX_Control(pDX, IDC_HIGHER, _right);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LeadHeadDlg, CDialog)
	//{{AFX_MSG_MAP(LeadHeadDlg)
	ON_BN_CLICKED(IDC_HIGHER, OnHigher)
	ON_BN_CLICKED(IDC_LOWER, OnLower)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LeadHeadDlg message handlers

void LeadHeadDlg::OnHigher() 
{
	if (_IDD == IDD_LEAD_HEAD)
		EndDialog(IDD_LEAD_HEAD_HIGH);
	else
		EndDialog(IDD_LEAD_HEAD_HIGHER);
}

void LeadHeadDlg::OnLower() 
{
	if (_IDD == IDD_LEAD_HEAD_HIGH)
		EndDialog(IDD_LEAD_HEAD);
	else
		EndDialog(IDD_LEAD_HEAD_HIGH);
}

BOOL LeadHeadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_left.m_bDrawFocus = FALSE;
	_left.SetTooltip("Show Lower Lead Head Codes");
	_left.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_right.m_bDrawFocus = FALSE;
	_right.SetTooltip("Show Higher Lead Head Codes");
	_right.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	return TRUE;  // return TRUE unless you set the focus to a control

}
