// C:\Development\Scheduler360\Visualisation\DoubleSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DoubleSelectDlg.h"


// DoubleSelectDlg dialog

IMPLEMENT_DYNAMIC(DoubleSelectDlg, CDialog)
DoubleSelectDlg::DoubleSelectDlg(CWnd* pParent, DoubleSelectItem& doubleSelectItem, CString label, CString name) :
	CDialog(DoubleSelectDlg::IDD, pParent), 
	_doubleSelectItem(doubleSelectItem), 
	_name(name), 
	_type(0)
	, _val1(0)
	, _val2(0)
	, _typeLabel(label)
	{
}

DoubleSelectDlg::~DoubleSelectDlg()
{
}

void DoubleSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _editCtrl1);
	DDX_Control(pDX, IDC_EDIT2, _editCtrl2);
	DDX_Control(pDX, IDC_SPIN1, _spinCtrl1);
	DDX_Control(pDX, IDC_SPIN2, _spinCtrl2);
	DDX_Text(pDX, IDC_EDIT1, _val1);
	DDX_Text(pDX, IDC_EDIT2, _val2);
	DDV_MinMaxDouble(pDX, _val1, 0, 1000000.0);
	DDV_MinMaxDouble(pDX, _val2, 0, 1000000.0);
	DDX_Text(pDX, IDC_LABEL, _typeLabel);
	DDX_Radio(pDX, IDC_RADIO1, _type);
}


BEGIN_MESSAGE_MAP(DoubleSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio)
END_MESSAGE_MAP()


// DoubleSelectDlg message handlers

BOOL DoubleSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_spinCtrl1.SetRange32(0,INT_MAX);
	_spinCtrl2.SetRange32(0,INT_MAX);

	_type = _doubleSelectItem._type;
	_val1 = _doubleSelectItem._double1;
	_val2 = _doubleSelectItem._double2;

	SetWindowText(_name);

	UpdateData(FALSE);

	OnBnClickedRadio();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DoubleSelectDlg::OnBnClickedRadio()
{
	UpdateData();

	_editCtrl2.ShowWindow((_type == dst_between)?SW_SHOW:SW_HIDE);
	_spinCtrl2.ShowWindow((_type == dst_between)?SW_SHOW:SW_HIDE);
}

void DoubleSelectDlg::OnOK()
{		
	UpdateData();    	

	if (((DoubleSelectType) _type == dst_between)&&
		(_val1 >= _val2))
	{
		AfxMessageBox("Please ensure that the second value is greater than the first.");
		return;
	}

	_doubleSelectItem._type = (DoubleSelectType) _type;

	_doubleSelectItem._double1 = _val1;
	_doubleSelectItem._double2 = _val2;

	CDialog::OnOK();
}