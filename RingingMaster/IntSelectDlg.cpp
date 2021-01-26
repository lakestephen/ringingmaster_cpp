// C:\Development\Scheduler360\Visualisation\IntSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IntSelectDlg.h"


// IntSelectDlg dialog

IMPLEMENT_DYNAMIC(IntSelectDlg, CDialog)
IntSelectDlg::IntSelectDlg(CWnd* pParent, IntSelectItem& intSelectItem, CString label, CString name) :
	CDialog(IntSelectDlg::IDD, pParent), 
	_intSelectItem(intSelectItem), 
	_name(name), 
	_type(0)
	, _val1(0)
	, _val2(0)
	, _typeLabel(label)
	{
}

IntSelectDlg::~IntSelectDlg()
{
}

void IntSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _editCtrl1);
	DDX_Control(pDX, IDC_EDIT2, _editCtrl2);
	DDX_Control(pDX, IDC_SPIN1, _spinCtrl1);
	DDX_Control(pDX, IDC_SPIN2, _spinCtrl2);
	DDX_Text(pDX, IDC_EDIT1, _val1);
	DDX_Text(pDX, IDC_EDIT2, _val2);
	DDV_MinMaxInt(pDX, _val1, 0, INT_MAX);
	DDV_MinMaxInt(pDX, _val2, 0, INT_MAX);
	DDX_Text(pDX, IDC_LABEL, _typeLabel);
	DDX_Radio(pDX, IDC_RADIO1, _type);
}


BEGIN_MESSAGE_MAP(IntSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio)
END_MESSAGE_MAP()


// IntSelectDlg message handlers

BOOL IntSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_spinCtrl1.SetRange32(0,INT_MAX);
	_spinCtrl2.SetRange32(0,INT_MAX);

	_type = _intSelectItem._type;
	_val1 = _intSelectItem._int1;
	_val2 = _intSelectItem._int2;

	SetWindowText(_name);

	UpdateData(FALSE);

	OnBnClickedRadio();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void IntSelectDlg::OnBnClickedRadio()
{
	UpdateData();

	_editCtrl2.ShowWindow((_type == ist_between)?SW_SHOW:SW_HIDE);
	_spinCtrl2.ShowWindow((_type == ist_between)?SW_SHOW:SW_HIDE);
}

void IntSelectDlg::OnOK()
{		
	UpdateData();    	

	if (((IntSelectType) _type == ist_between)&&
		(_val1 >= _val2))
	{
		AfxMessageBox("Please ensure that the second value is greater than the first.");
		return;
	}

	_intSelectItem._type = (IntSelectType) _type;

	_intSelectItem._int1 = _val1;
	_intSelectItem._int2 = _val2;

	CDialog::OnOK();
}