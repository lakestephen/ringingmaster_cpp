#include "stdafx.h"
#include "RingingMaster.h"
#include "WeightSelectDlg.h"
#include "WeightDlg.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WeightSelectDlg dialog


WeightSelectDlg::WeightSelectDlg(CWnd* pParent, DoubleSelectItem& doubleSelectItem, CString name )
	: CDialog(WeightSelectDlg::IDD, pParent),
	_originalDoubleSelectItem(doubleSelectItem), 
	_doubleSelectItem(doubleSelectItem),
	_name(name)
{
	_logic = _originalDoubleSelectItem._type;
	//{{AFX_DATA_INIT(WeightSelectDlg)
	//}}AFX_DATA_INIT
}


void WeightSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WeightSelectDlg)
	DDX_Control(pDX, IDC_WEIGHT_LOGIC2, _weightCtrl2);
	DDX_Control(pDX, IDC_WEIGHT2, _weight2);
	DDX_Control(pDX, IDC_WEIGHT1, _weight1);
	DDX_Radio(pDX, IDC_WEIGHT_LOGIC, _logic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(WeightSelectDlg, CDialog)
	//{{AFX_MSG_MAP(WeightSelectDlg)
	ON_BN_CLICKED(IDC_WEIGHT_LOGIC, OnWeightLogic)
	ON_BN_CLICKED(IDC_WEIGHT_LOGIC1, OnWeightLogic)
	ON_BN_CLICKED(IDC_WEIGHT_LOGIC2, OnWeightLogic)
	ON_BN_CLICKED(IDC_WEIGHT1, OnWeight1)
	ON_BN_CLICKED(IDC_WEIGHT2, OnWeight2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WeightSelectDlg message handlers

BOOL WeightSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_weight1.m_bDrawFocus = FALSE;
	_weight1.SetTooltip("Press to edit weight");
	_weight1.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_weight2.m_bDrawFocus = FALSE;
	_weight2.SetTooltip("Press to edit weight");
	_weight2.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	OnWeightLogic();
	update();
	
	SetWindowText(_name);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void WeightSelectDlg::OnOK() 
{
	UpdateData(TRUE);

	if (((DoubleSelectType)_logic == dst_between)&&
		(_doubleSelectItem._double1 >= _doubleSelectItem._double2))
	{
		AfxMessageBox("The second weight must be greater the first weight.");
		return;
	}



	_originalDoubleSelectItem._double1 = _doubleSelectItem._double1;
	_originalDoubleSelectItem._double2 = _doubleSelectItem._double2;
	_originalDoubleSelectItem._type = (DoubleSelectType)_logic;	
	
	CDialog::OnOK();
}
		   

void WeightSelectDlg::OnWeightLogic() 
{
	_weight2.ShowWindow(_weightCtrl2.GetCheck()?SW_SHOW:SW_HIDE);	
}

void WeightSelectDlg::update()
{
	
	_weight1.SetWindowText(GlobalFunctions::getWeightString(_doubleSelectItem._double1, true));
	_weight2.SetWindowText(GlobalFunctions::getWeightString(_doubleSelectItem._double2, true));

	UpdateData(FALSE);

}

void WeightSelectDlg::OnWeight1() 
{
	WeightDlg dlg(_doubleSelectItem._double1, FALSE, FALSE, this);
	if (dlg.DoModal() == IDOK)
	{
		_doubleSelectItem._double1 = dlg.getWeightInKG();
		update();	
	}
}

void WeightSelectDlg::OnWeight2() 
{
	WeightDlg dlg(_doubleSelectItem._double2, FALSE, FALSE, this);

	if (dlg.DoModal() == IDOK)
	{
		_doubleSelectItem._double2 = dlg.getWeightInKG();
		update();	
	}
}
