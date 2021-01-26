// GenericEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"

#include "GenericEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GenericEditDlg dialog


GenericEditDlg::GenericEditDlg(CWnd* pParent, CString strTitle, CString strPrompt, CString strStartText, bool allowZeroLength )
	: CDialog(GenericEditDlg::IDD, pParent),
	_allowZeroLength(allowZeroLength)
{
	//{{AFX_DATA_INIT(GenericEditDlg)
	_strText = strPrompt;
	_strGenericEdit = strStartText;
	//}}AFX_DATA_INIT
	_strTitle = strTitle;
}


void GenericEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GenericEditDlg)
	DDX_Control(pDX, IDC_GENERIC_EDIT, _autoSelEdit);
	DDX_Text(pDX, IDC_STATIC_TEXT, _strText);
	DDX_Text(pDX, IDC_GENERIC_EDIT, _strGenericEdit);
	DDV_MaxChars(pDX, _strGenericEdit, 200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GenericEditDlg, CDialog)
	//{{AFX_MSG_MAP(GenericEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GenericEditDlg message handlers

BOOL GenericEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_GENERIC_EDIT)->SetFocus();
	SetWindowText(_strTitle);

	return FALSE;  // return TRUE unless you set the focus to a control

}

void GenericEditDlg::OnOK()
{
	UpdateData();
	if ((_strGenericEdit.GetLength() > 0)||
		(_allowZeroLength))
		CDialog::OnOK();
}
