// DirectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "DirectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DirectionDlg dialog


DirectionDlg::DirectionDlg(BOOL& ascending, CWnd* pParent)
	: CDialog(DirectionDlg::IDD, pParent),
	_originalAscending(ascending),
	_ascending(ascending?0:1)
{
	//{{AFX_DATA_INIT(DirectionDlg)
	//}}AFX_DATA_INIT
}


void DirectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DirectionDlg)
	DDX_Radio(pDX, IDC_ASCENDING, _ascending);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DirectionDlg, CDialog)
	//{{AFX_MSG_MAP(DirectionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DirectionDlg message handlers

void DirectionDlg::OnOK() 
{
	UpdateData();

	_originalAscending = (_ascending == 0);
	
	CDialog::OnOK();
}
