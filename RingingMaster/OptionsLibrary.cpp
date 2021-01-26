// OptionsLibrary.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsLibrary.h"
#include "MethodManager.h"
#include ".\optionslibrary.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsLibrary dialog


OptionsLibrary::OptionsLibrary()
	: CBCGPPropertyPage(OptionsLibrary::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsLibrary)
	_CCLibLoad = TRUE;
	_nearPlaceM = FALSE;
	//}}AFX_DATA_INIT
}

void OptionsLibrary::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsLibrary)
	DDX_Check(pDX, IDC_CC_LIB_LOAD, _CCLibLoad);
	DDX_Check(pDX, IDC_NEAR_PLACE_M, _nearPlaceM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsLibrary, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsLibrary)
	ON_BN_CLICKED(IDC_CC_LIB_LOAD, OnCcLibLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsLibrary message handlers



BOOL OptionsLibrary::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	

	_CCLibLoad = app()->GetProfileInt("MainFrame", 
									"Include CC Library", TRUE);

	_nearPlaceM = app()->GetProfileInt("MainFrame", 
									"Near Place Type M", TRUE);
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsLibrary::OnCcLibLoad() 
{
	methodManager()->saveExplorerData();	
}
		 
BOOL OptionsLibrary::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		app()->WriteProfileInt("MainFrame", 
									"Include CC Library", _CCLibLoad);	
		app()->WriteProfileInt("MainFrame", 
									"Near Place Type M", _nearPlaceM);	

		return TRUE;
	}

	return FALSE;
}
