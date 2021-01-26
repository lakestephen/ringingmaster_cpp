#include "StdAfx.h"
#include "DateTimeSelectDlg.h"



// DateTimeSelectDlg dialog

IMPLEMENT_DYNAMIC(DateTimeSelectDlg, CDialog)
DateTimeSelectDlg::DateTimeSelectDlg(CWnd* pParent, DateTimeSelectItem& dateTimeSelectItem, CString name)
	: CDialog(DateTimeSelectDlg::IDD, pParent), 
	_dateTimeSelectItem(dateTimeSelectItem), 
	_name(name), 
	_type(0)
{
	
}		   

DateTimeSelectDlg::~DateTimeSelectDlg()
{
}

void DateTimeSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, _dateCtrl1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, _dateCtrl2);
	DDX_Radio(pDX, IDC_RADIO1, _type);
}


BEGIN_MESSAGE_MAP(DateTimeSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio)
END_MESSAGE_MAP()

// DateTimeSelectDlg message handlers



BOOL DateTimeSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
   	UINT state = 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE | 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR;

	if ((_dateTimeSelectItem._format == dtsft_date_time)||
		(_dateTimeSelectItem._format == dtsft_date))
		state |= CBCGPDateTimeCtrl::DTM_DATE;

	if ((_dateTimeSelectItem._format == dtsft_date_time)||
		(_dateTimeSelectItem._format == dtsft_time))
		state |= CBCGPDateTimeCtrl::DTM_TIME;

	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE;	  

	_dateCtrl1.SetState(state, stateMask);
	if (_dateTimeSelectItem._dateTime1.GetStatus() == COleDateTime::valid)
		_dateCtrl1.SetDate(_dateTimeSelectItem._dateTime1);

	_dateCtrl2.SetState(state, stateMask);
	if (_dateTimeSelectItem._dateTime2.GetStatus() == COleDateTime::valid)
		_dateCtrl2.SetDate(_dateTimeSelectItem._dateTime2);

	_type = _dateTimeSelectItem._type;

	SetWindowText(_name);

	UpdateData(FALSE);

	OnBnClickedRadio();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void DateTimeSelectDlg::OnBnClickedRadio()
{
	UpdateData();

	_dateCtrl2.ShowWindow((_type == dtst_between)?SW_SHOW:SW_HIDE);
}

void DateTimeSelectDlg::OnOK()
{
	UpdateData();    	

	if (((DateTimeSelectType) _type == dtst_between)&&
		(_dateCtrl1.GetDate() >= _dateCtrl2.GetDate()))
	{
		AfxMessageBox("Please ensure that the second date is after the first.");
		return;
	}

	_dateTimeSelectItem._type = (DateTimeSelectType) _type;
	_dateTimeSelectItem._dateTime1 = _dateCtrl1.GetDate();
	_dateTimeSelectItem._dateTime2 = _dateCtrl2.GetDate();

	CDialog::OnOK();
}
