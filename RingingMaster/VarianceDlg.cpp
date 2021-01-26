// VarianceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "VarianceDlg.h"
#include "Variance.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// VarianceDlg dialog


VarianceDlg::VarianceDlg(Variance* variance)
	: CDialog(VarianceDlg::IDD, NULL),
	_variance(variance)
{
	//{{AFX_DATA_INIT(VarianceDlg)
	_parts = _T("");
	_type = -1;
	//}}AFX_DATA_INIT
}


void VarianceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(VarianceDlg)
	DDX_Control(pDX, IDC_PARTS, _partsEdit);
	DDX_Control(pDX, IDC_OMIT_INC, _omitInc);
	DDX_Text(pDX, IDC_PARTS, _parts);
	DDX_Radio(pDX, IDC_SPECIFY, _type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(VarianceDlg, CDialog)
	//{{AFX_MSG_MAP(VarianceDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SPECIFY, OnSpecify)
	ON_BN_CLICKED(IDC_EVEN, OnSpecify)
	ON_BN_CLICKED(IDC_ODD, OnSpecify)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// VarianceDlg message handlers

BOOL VarianceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (_variance->isOmit())
		_omitInc.SetCurSel(0);
	else
		_omitInc.SetCurSel(1);

	_type = (int) _variance->getVarianceType();
	
	if (_variance->getVarianceType() == vt_parts)
		_parts = _variance->getVarianceStr();

	UpdateData(FALSE);

	WinPos::Load(this, "NewVarianceDlg", true); 
	
	enablePartsWindow();

	_partsEdit.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control

}

void VarianceDlg::OnOK() 
{
	UpdateData();

	if (_type == -1)
	{
		AfxMessageBox("Please Select a Variance Type");
		return;
	}

	Variance varTemp;
	if ((_type != vt_parts)||(varTemp.setParts(_parts)))
	{			
		_variance->setParts((_type == vt_parts)?_parts:"");
		_variance->setOmit(_omitInc.GetCurSel() == 0);
		_variance->setVarianceType((VarianceType)_type);
		CDialog::OnOK();
	}
	else
		AfxMessageBox("Please enter one or more parts, or select odd or even.");
}

void VarianceDlg::OnDestroy() 
{
	WinPos::Save(this, "NewVarianceDlg");

	CDialog::OnDestroy();	
}



void VarianceDlg::enablePartsWindow()
{
	UpdateData();
	_partsEdit.EnableWindow(_type == vt_parts);

}

void VarianceDlg::OnSpecify() 
{
	enablePartsWindow();	
}
