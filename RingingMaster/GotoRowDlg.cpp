// GotoRowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "GotoRowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GotoRowDlg dialog


int GotoRowDlg::_row  = 1;

GotoRowDlg::GotoRowDlg(CWnd* pParent, int maxRows)
: CDialog(GotoRowDlg::IDD, pParent),
_maxRows(maxRows)
{
	//{{AFX_DATA_INIT(GotoRowDlg)
	//}}AFX_DATA_INIT

	if (_row <=1)
		_row = 1;
	if (_row > maxRows)
		_row = maxRows;
}


void GotoRowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GotoRowDlg)
	DDX_Control(pDX, IDC_ROW, _rowCtrl);
	DDX_Control(pDX, IDC_ROW_SPIN, _spin);
	DDX_Text(pDX, IDC_ROW, _row);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _row, 1, _maxRows+1);
}


BEGIN_MESSAGE_MAP(GotoRowDlg, CDialog)
	//{{AFX_MSG_MAP(GotoRowDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GotoRowDlg message handlers

BOOL GotoRowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_spin.SetRange32(1,_maxRows+1);
	_rowCtrl.SetFocus();

	return FALSE;
}
