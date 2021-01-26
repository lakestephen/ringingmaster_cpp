// LogicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "LogicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LogicDlg dialog


LogicDlg::LogicDlg(CWnd* pParent, LogicType& logicType)
	: CDialog(LogicDlg::IDD, pParent),
	_originalLogicType(logicType),
	_logicType((logicType == lt_and)?0:1)
{
	//{{AFX_DATA_INIT(LogicDlg)
		
	//}}AFX_DATA_INIT
}


void LogicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LogicDlg)
	DDX_Radio(pDX, IDC_AND, _logicType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LogicDlg, CDialog)
	//{{AFX_MSG_MAP(LogicDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LogicDlg message handlers
 
void LogicDlg::OnOK() 
{
	UpdateData();

	_originalLogicType = (LogicType)_logicType;
	
	CDialog::OnOK();
}

