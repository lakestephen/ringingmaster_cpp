// C:\Development\Scheduler360\Visualisation\OptionSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionSelectDlg.h"


// OptionSelectDlg dialog

IMPLEMENT_DYNAMIC(OptionSelectDlg, CDialog)
OptionSelectDlg::OptionSelectDlg(CWnd* pParent, CString trueString, CString falseString, BOOL& optionItem, CString title)
	: CDialog(OptionSelectDlg::IDD, pParent), 
	_trueString(trueString),
	_falseString(falseString), 
	_originalOptionItem(optionItem), 
	_title(title), 
	_option(optionItem)
{
}

OptionSelectDlg::~OptionSelectDlg()
{
}

void OptionSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, _falseCtrl);
	DDX_Control(pDX, IDC_RADIO2, _trueCtrl);
	DDX_Radio(pDX, IDC_RADIO1, _option);
}


BEGIN_MESSAGE_MAP(OptionSelectDlg, CDialog)
END_MESSAGE_MAP()


// OptionSelectDlg message handlers

BOOL OptionSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_trueCtrl.SetWindowText(_trueString);
	_falseCtrl.SetWindowText(_falseString);

	SetWindowText(_title);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void OptionSelectDlg::OnOK()
{
	UpdateData();

	_originalOptionItem = _option;

	CDialog::OnOK();
}
