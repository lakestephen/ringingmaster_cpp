// GenericNumberSpinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "GenericNumberSpinDlg.h"
#include ".\genericnumberspindlg.h"


// GenericNumberSpinDlg dialog

IMPLEMENT_DYNAMIC(GenericNumberSpinDlg, CDialog)
GenericNumberSpinDlg::GenericNumberSpinDlg(CWnd* pParent, int& val, int min, int max, CString dlgName, CString hint)
	: CDialog(GenericNumberSpinDlg::IDD, pParent), 
	_val(val), 
	_valEdit(val), 
	_min(min), 
	_max(max), 
	_dlgName(dlgName), 
	_hint(hint)
{
}

GenericNumberSpinDlg::~GenericNumberSpinDlg()
{
}

void GenericNumberSpinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VAL_SPIN, _valSpin);
	DDX_Text(pDX, IDC_VAL, _valEdit);
	DDV_MinMaxInt(pDX, _valEdit, _min, _max);
	DDX_Control(pDX, IDC_STATIC_TEXT, _hintStatic);
}


BEGIN_MESSAGE_MAP(GenericNumberSpinDlg, CDialog)
END_MESSAGE_MAP()


// GenericNumberSpinDlg message handlers

BOOL GenericNumberSpinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_valSpin.SetRange32(_min, _max);

	SetWindowText(_dlgName);
	_hintStatic.SetWindowText(_hint);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void GenericNumberSpinDlg::OnOK()
{
	UpdateData();
	_val = _valEdit;

	CDialog::OnOK();
}
