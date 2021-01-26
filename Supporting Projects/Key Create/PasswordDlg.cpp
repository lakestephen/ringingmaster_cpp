// PasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "key create.h"
#include "PasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PasswordDlg dialog


PasswordDlg::PasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PasswordDlg)
	_password = _T("");
	//}}AFX_DATA_INIT
}


void PasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PasswordDlg)
	DDX_Control(pDX, IDC_PASSWORD, _passwordCtrl);
	DDX_Text(pDX, IDC_PASSWORD, _password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PasswordDlg, CDialog)
	//{{AFX_MSG_MAP(PasswordDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PasswordDlg message handlers

BOOL PasswordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_passwordCtrl.SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
