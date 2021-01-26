// OptionsCalledChanges.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsCalledChanges.h"
#include ".\optionscalledchanges.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsCalledChanges dialog

const CString OptionsCalledChanges::CallChangeSection = "Called Changes";


OptionsCalledChanges::OptionsCalledChanges()
	: CBCGPPropertyPage(OptionsCalledChanges::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsCalledChanges)
	_affect2 = FALSE;
	_affect3 = FALSE;
	_type = -1;
	_inclTenor = FALSE;
	_pullPerChange = 0;
	//}}AFX_DATA_INIT
}


void OptionsCalledChanges::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsCalledChanges)
	DDX_Control(pDX, IDC_AFFECT3, _affect3Ctrl);
	DDX_Control(pDX, IDC_AFFECT2, _affect2Ctrl);
	DDX_Control(pDX, IDC_PULL_PER_CHANGE_SPIN, _pullPerChangeSpin);
	DDX_Check(pDX, IDC_AFFECT2, _affect2);
	DDX_Check(pDX, IDC_AFFECT3, _affect3);
	DDX_Radio(pDX, IDC_CALL_TYPE, _type);
	DDX_Check(pDX, IDC_INCLUDE_TENOR, _inclTenor);
	DDX_Text(pDX, IDC_PULL_PER_CHANGE, _pullPerChange);
	DDV_MinMaxInt(pDX, _pullPerChange, 1, 100);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// OptionsCalledChanges message handlers
BEGIN_MESSAGE_MAP(OptionsCalledChanges, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsCalledChanges)
	ON_BN_CLICKED(IDC_AFFECT2, OnAffect2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsCalledChanges message handlers



BOOL OptionsCalledChanges::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_affect2 = app()->GetProfileInt(CallChangeSection, "Affect 2", FALSE);
	_affect3 = app()->GetProfileInt(CallChangeSection, "Affect 3", FALSE);
	_inclTenor = app()->GetProfileInt(CallChangeSection, "Include Tenor", FALSE);
	_pullPerChange = app()->GetProfileInt(CallChangeSection, "Pulls Per Change", 2);
	_type = app()->GetProfileInt(CallChangeSection, "Calling Type", 0);//up

	UpdateData(FALSE);

	OnAffect2();
	_pullPerChangeSpin.SetRange32(1,100);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsCalledChanges::OnAffect2() 
{
	_affect3Ctrl.EnableWindow(_affect2Ctrl.GetCheck());	
	if (_affect2Ctrl.GetCheck() == FALSE)
		_affect3Ctrl.SetCheck(FALSE);
}

BOOL OptionsCalledChanges::OnKillActive()
{
	if(CBCGPPropertyPage::OnKillActive())
	{
		app()->WriteProfileInt(CallChangeSection, "Affect 2", _affect2);	
		app()->WriteProfileInt(CallChangeSection, "Affect 3", _affect3);	
		app()->WriteProfileInt(CallChangeSection, "Include Tenor", _inclTenor);	
		app()->WriteProfileInt(CallChangeSection, "Pulls Per Change", _pullPerChange);	
		app()->WriteProfileInt(CallChangeSection, "Calling Type", _type);	
		return TRUE;
	}

	return FALSE;
}
