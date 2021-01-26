// OptionsPrint.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsPrint.h"
#include ".\optionsprint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsPrint dialog


OptionsPrint::OptionsPrint()
	: CBCGPPropertyPage(OptionsPrint::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsPrint)
	_printColumns = FALSE;
	//}}AFX_DATA_INIT
}


void OptionsPrint::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsPrint)
	DDX_Check(pDX, IDC_PRINT_COLUMNS, _printColumns);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsPrint, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsPrint message handlers

BOOL OptionsPrint::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_printColumns = app()->GetProfileInt("Print", 
									"Method Columns", TRUE);
	UpdateData(FALSE);
	
	return TRUE;  
	
}


BOOL OptionsPrint::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		app()->WriteProfileInt("Print", 
									"Method Columns", _printColumns);	

		return TRUE;
	}

	return FALSE;
}
