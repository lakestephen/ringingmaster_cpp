// OptionsScales.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsScales.h"
#include ".\optionsscales.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsScales dialog

				   
OptionsScales::OptionsScales()
	: CBCGPPropertyPage(OptionsScales::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsScales)
		_weight = 0;
		_distance = 0;
	//}}AFX_DATA_INIT
}


void OptionsScales::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsScales)
	DDX_Radio(pDX, IDC_CWT, _weight);
	DDX_Radio(pDX, IDC_MILES, _distance);  

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsScales, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsScales)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsScales message handlers

BOOL OptionsScales::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_distance = app()->GetProfileInt("Tower Database", 
									"Distance", 0);

	_weight = app()->GetProfileInt("Tower Database", 
									"Weight", 0);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL OptionsScales::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		app()->WriteProfileInt("Tower Database", 
									"Distance", _distance);	

		app()->WriteProfileInt("Tower Database", 
									"Weight", _weight);	 	   

		return TRUE;
	}

	return FALSE;

}
