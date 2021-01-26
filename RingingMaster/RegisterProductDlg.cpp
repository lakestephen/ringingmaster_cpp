// RegisterProductDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RegisterProductDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RegisterProductDlg dialog


RegisterProductDlg::RegisterProductDlg(CWnd* pParent)
	: CDialog(RegisterProductDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RegisterProductDlg)
	_key = _T("");
	_name = _T("");
	//}}AFX_DATA_INIT
}


void RegisterProductDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RegisterProductDlg)
	DDX_Text(pDX, IDC_KEY, _key);
	DDX_Text(pDX, IDC_NAME, _name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RegisterProductDlg, CDialog)
	//{{AFX_MSG_MAP(RegisterProductDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RegisterProductDlg message handlers

void RegisterProductDlg::OnOK() 
{
	UpdateData();	

	if (_name.IsEmpty()||_key.IsEmpty())
	{
		AfxMessageBox("Please enter User Name && Key");
		return;
	}		   

	BOOL result = mainFrame()->verifyKey(_name, _key);
	if (result == TRUE)
	{
		app()->setupTitle();
		AfxMessageBox("Product successfully registered");
		CDialog::OnOK();
	}
}
