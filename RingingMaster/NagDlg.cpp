// NagDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RegisterProductDlg.h"
#include "NagDlg.h"
#include "UserMessages.h"
#include "SpeechManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NagDlg dialog


bool NagDlg::_firstTime = true;

NagDlg::NagDlg(CWnd* pParent)
	: CDialog(NagDlg::IDD, pParent),
	_timingCount(0)
{
	//{{AFX_DATA_INIT(NagDlg)
	_message = _T("");
	//}}AFX_DATA_INIT
}


void NagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NagDlg)
	DDX_Control(pDX, IDC_REGISTER, _register);
	DDX_Control(pDX, IDOK, _ok);
	DDX_Control(pDX, IDC_PROGRESS, _progress);
	DDX_Control(pDX, IDC_URL, _url);
	DDX_Control(pDX, IDC_MAIL, _mail);
	DDX_Text(pDX, IDC_MESSAGE, _message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NagDlg, CDialog)
	//{{AFX_MSG_MAP(NagDlg)
	ON_BN_CLICKED(IDC_REGISTER, OnRegister)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NagDlg message handlers

BOOL NagDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();   

	speechManager()->speak("This is an unregistered copy of Ringing Master.");

	Beep(100,100);	//this beep is wanted!!
	
 	_mail.SetURLPrefix (_T("mailto:"));
	_mail.SetURL (_T("Stephen@ringingmaster.co.uk"));
	_mail.SizeToContent ();
	_mail.SetTooltip (_T("Send mail to Stephen Lake"));

	_url.SizeToContent ();

	_mail.m_bDrawFocus = FALSE;
	_url.m_bDrawFocus = FALSE;

	//set the message
	if (mainFrame()->_trialState == MainFrame::ts_ok)
	{
		_message.Format("%d days of trial left", mainFrame()->_trialDaysLeft);
		speechManager()->speak("You have " + _message);
	}
	else if (mainFrame()->_trialState == MainFrame::ts_expired)
	{
		_message = "Trial has expired ";
		speechManager()->speak("Your trial has expired. Please register or remove from your machine" );
	}
	else if (mainFrame()->_trialState == MainFrame::ts_tampered)
	{
		_message = "Trial terminated due to date change";
		speechManager()->speak("You appear to have changed the dates. Please register or remove from your machine");
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void NagDlg::OnOK() 
{
	if (_firstTime)
	{
		_firstTime = false;
		CDialog::OnOK();	
	}

	//check for correct key, we may have registered it from here
	_ok.ShowWindow(SW_HIDE);
	_progress.ShowWindow(SW_SHOW);
	_progress.SetRange32(0,100);
	_progress.SetStep(1);
	_timingCount = 0;
	
	commonSetTimer();

}

void NagDlg::OnRegister() 
{
	KillTimer(AAA_TIMER_NAG_PROGRESS);

	if (mainFrame()->verifyKey() == TRUE)
	{
		CDialog::OnOK();	
		return ;
	}

	RegisterProductDlg dlg(this);
	dlg.DoModal();		

	if (mainFrame()->verifyKey() == TRUE)
	{
		CString user = app()->GetProfileString("MainFrame","User Name", "");

		speechManager()->speak(" Thank you for registering. I hope you enjoy the program");
		
		CDialog::OnOK();	
		return ;
	}

	if (_progress.IsWindowVisible())
		commonSetTimer();
}

void NagDlg::OnCancel() 
{
	//do nothing to disable escape key
}

void NagDlg::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

	if (nIDEvent == AAA_TIMER_NAG_PROGRESS)
	{
		_progress.StepIt();
		_timingCount ++;

		if (_timingCount == 100)
			CDialog::OnOK();
	}	
}

void NagDlg::commonSetTimer()
{
#ifdef _DEBUG		
	SetTimer(AAA_TIMER_NAG_PROGRESS, 10, NULL);
#else
	SetTimer(AAA_TIMER_NAG_PROGRESS, 200, NULL);
#endif
}
