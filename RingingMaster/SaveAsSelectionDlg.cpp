// SaveAsSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SaveAsSelectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SaveAsSelectionDlg dialog


SaveAsSelectionDlg::SaveAsSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SaveAsSelectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SaveAsSelectionDlg)
	_touchWindow = 0;
	//}}AFX_DATA_INIT
}


void SaveAsSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SaveAsSelectionDlg)
	DDX_Radio(pDX, IDC_TOUCH_WINDOW, _touchWindow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SaveAsSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(SaveAsSelectionDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SaveAsSelectionDlg message handlers

bool SaveAsSelectionDlg::isTouchWindow()
{
	return (_touchWindow == 0);
}
