// CallPosMethodSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "CallPosMethodSelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CallPosMethodSelDlg dialog


CallPosMethodSelDlg::CallPosMethodSelDlg(CStringArray * strArr, int passedIndex)
	: CDialog(CallPosMethodSelDlg::IDD, NULL),
	_stringArray(strArr),
	_passedIndex(passedIndex)
{
	//{{AFX_DATA_INIT(CallPosMethodSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CallPosMethodSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CallPosMethodSelDlg)
	DDX_Control(pDX, IDC_CALL_POS_SEL, _selectionCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CallPosMethodSelDlg, CDialog)
	//{{AFX_MSG_MAP(CallPosMethodSelDlg)
	ON_CBN_SELCHANGE(IDC_CALL_POS_SEL, OnSelchangeCallPosSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CallPosMethodSelDlg message handlers

BOOL CallPosMethodSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for (int i=0;i<_stringArray->GetSize();i++)
	{
		_selectionCombo.AddString(_stringArray->GetAt(i));
	}

	//now select the first item
	_selectionCombo.SetCurSel(_passedIndex);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CallPosMethodSelDlg::OnSelchangeCallPosSel() 
{
	_selectionCombo.GetLBText(_selectionCombo.GetCurSel(), _selection);	
	EndDialog(IDOK);
}
