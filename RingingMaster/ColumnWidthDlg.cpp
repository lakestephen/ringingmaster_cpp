// ColumnWidthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "ColumnWidthDlg.h"

//todo can we implement this with a generic dialog. 
//todo needs some resource information

// ColumnWidthDlg dialog

IMPLEMENT_DYNAMIC(ColumnWidthDlg, CDialog)
ColumnWidthDlg::ColumnWidthDlg(int& width, CWnd* pParent, CString name)
	: CDialog(ColumnWidthDlg::IDD, pParent), 
	_originalWidth(width),
	_width(max(width, 15)), 
	_name(name)
{
}

ColumnWidthDlg::~ColumnWidthDlg()
{
}

void ColumnWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, _widthSpin);
	DDX_Text(pDX, IDC_EDIT1, _width);
	DDV_MinMaxInt(pDX, _width, 15, 500);
}


BEGIN_MESSAGE_MAP(ColumnWidthDlg, CDialog)
END_MESSAGE_MAP()


// ColumnWidthDlg message handlers

void ColumnWidthDlg::OnOK()
{
	UpdateData();

	_originalWidth = _width;

	CDialog::OnOK();
}

BOOL ColumnWidthDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_widthSpin.SetRange32(15, 500); 	

	if (!_name.IsEmpty())
		SetWindowText(_name);


	return TRUE;  	
}
