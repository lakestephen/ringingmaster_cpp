// EventWaitDlg.cpp : implementation file
//

#include "stdafx.h"

#include "EventWaitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EventWaitDlg dialog


EventWaitDlg::EventWaitDlg(CWnd* pParent )
	: CDialog(EventWaitDlg::IDD, pParent)
{
	Create(EventWaitDlg::IDD, NULL);
	//{{AFX_DATA_INIT(EventWaitDlg)
	//}}AFX_DATA_INIT
}


void EventWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EventWaitDlg)
	DDX_Control(pDX, IDC_ANIMATE1, _animate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EventWaitDlg, CDialog)
	//{{AFX_MSG_MAP(EventWaitDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EventWaitDlg message handlers

BOOL EventWaitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//start the animation
	_animate.Open(IDR_WAIT_AVI);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}
