// WeightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "WeightDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WeightDlg dialog


WeightDlg::WeightDlg(double weightKG, bool useApprox, BOOL approx, CWnd* pParent /*=NULL*/)
	: CDialog(WeightDlg::IDD, pParent),
	_originalWeightKG(weightKG),
	_useApprox(useApprox), 
	_approx(approx)
{
	//{{AFX_DATA_INIT(WeightDlg)
	_qr = 0;
	_kg = 0.0;
	_cwt = 0;
	_lb = 0;
	//}}AFX_DATA_INIT

	_approx = approx;
}


void WeightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WeightDlg)
	DDX_Control(pDX, IDC_APPROX, _approxCtrl);
	DDX_Control(pDX, IDC_SPIN_QR, _spinQR);
	DDX_Control(pDX, IDC_SPIN_LB, _spinLB);
	DDX_Control(pDX, IDC_SPIN_CWT, _spinCWT);
	DDX_Text(pDX, IDC_QR, _qr);
	DDV_MinMaxInt(pDX, _qr, 0, 3);
	DDX_Text(pDX, IDC_KG, _kg);
	DDV_MinMaxDouble(pDX, _kg, 0., 100000.);
	DDX_Text(pDX, IDC_CWT, _cwt);
	DDV_MinMaxInt(pDX, _cwt, 0, 1000);
	DDX_Text(pDX, IDC_LB, _lb);
	DDV_MinMaxInt(pDX, _lb, 0, 27);
	DDX_Check(pDX, IDC_APPROX, _approx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(WeightDlg, CDialog)
	//{{AFX_MSG_MAP(WeightDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WeightDlg message handlers

void WeightDlg::OnOK() 
{
	int weightSwitch = app()->GetProfileInt("Tower Database", "Weight", 0);

	UpdateData();

	if (weightSwitch == 0)
	{
		GlobalFunctions::getWeightAsKG(_cwt, _qr, _lb, _originalWeightKG);		
	}
	else
	{						   
		_originalWeightKG = _kg;
	}
	
	CDialog::OnOK();
}

BOOL WeightDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int weightSwitch = app()->GetProfileInt("Tower Database", "Weight", 0);

	if (weightSwitch == 0)
	{
		//imperial - hide metric
		GetDlgItem(IDC_KG)->ShowWindow(SW_HIDE);  
		GetDlgItem(IDC_STATIC_KG)->ShowWindow(SW_HIDE);

		GlobalFunctions::getWeightAsCWT(_originalWeightKG, _cwt, _qr, _lb);

		_spinCWT.SetRange32(0,1000);
		_spinQR.SetRange32(0,3);
		_spinLB.SetRange32(0,27);
	}											   
	else
	{
		GetDlgItem(IDC_CWT)->ShowWindow(SW_HIDE);  
		GetDlgItem(IDC_STATIC_CWT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPIN_CWT)->ShowWindow(SW_HIDE);  

		GetDlgItem(IDC_QR)->ShowWindow(SW_HIDE);  
		GetDlgItem(IDC_STATIC_QR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPIN_QR)->ShowWindow(SW_HIDE);  

		GetDlgItem(IDC_LB)->ShowWindow(SW_HIDE);  
		GetDlgItem(IDC_STATIC_LB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPIN_LB)->ShowWindow(SW_HIDE);  
		
		_kg = (floor(_originalWeightKG * 100)/100);
	
	}	

	_approxCtrl.ShowWindow(_useApprox?SW_SHOW:SW_HIDE);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

double WeightDlg::getWeightInKG()
{
	return _originalWeightKG;

}



