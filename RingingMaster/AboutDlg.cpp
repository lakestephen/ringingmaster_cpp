// AboutDlg.cpp : implementation file
//
//


#include "stdafx.h"
#include "RingingMaster.h"
#include "RegisterProductDlg.h"	
#include "AboutDlg.h"
#include ".\aboutdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AboutDlg dialog



BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
	//{{AFX_MSG_MAP(AboutDlg)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_VERSION, OnStnClickedVersion)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// AboutDlg dialog used for App About


AboutDlg::AboutDlg() : CDialog(AboutDlg::IDD)
{
	//{{AFX_DATA_INIT(AboutDlg)
	//}}AFX_DATA_INIT
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AboutDlg)
	DDX_Control(pDX, IDOK, _ok);
	DDX_Control(pDX, IDC_URL, _url);
	DDX_Control(pDX, IDC_MAIL, _mail);
	DDX_Control(pDX, IDC_CREDITS, _static);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_RESOLITION, _resolution);
}


BOOL AboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    // here's where you add the items for the scrolling window
	 setCredits();

	 CString resolution;
	 resolution.Format("Res: %d", app()->_wTimerRes);
	 _resolution.SetWindowText(resolution);
	 
	 GetDlgItem(IDC_STATIC_MEM)->SetWindowText(getMemory());
	 GetDlgItem(IDC_STATIC_DISK)->SetWindowText(getDisk());

 	_mail.SetURLPrefix (_T("mailto:"));
	_mail.SetURL (_T("stephen@ringingmaster.co.uk"));
	_mail.SizeToContent ();
	_mail.SetTooltip (_T("Send mail to Stephen Lake"));

	_url.SizeToContent ();
	_mail.m_bDrawFocus = FALSE;
	_url.m_bDrawFocus = FALSE;

	return TRUE;
}

void AboutDlg::setCredits()
{	 
	_static.ResetContent();
    _static.SetColorBk(::GetSysColor(COLOR_3DFACE));
	_static.SetSpeed(Credits::SPEED_MED);

	_static.SetColorH1(RGB(0,0,0));
	_static.SetColorH2(RGB(255,255,0));
	_static.SetColorH3(RGB(0,0,0));
	_static.SetColorText(RGB(0,0,0));		
	
	_static.AddHeading3(_T("Code assistance:"));
	_static.AddNormalText(_T("Andrew Craddock"));
	_static.AddBlankLine();
	_static.AddBlankLine();
		
	_static.AddHeading3(_T("Ringing assistance:"));
	_static.AddNormalText(_T("John Illingworth"));
	_static.AddNormalText(_T("Peter Border"));
	_static.AddNormalText(_T("Bill Haynes"));
	_static.AddBlankLine();
	_static.AddBlankLine();
	
	_static.AddHeading3(_T("Testing & feedback:"));
	_static.AddNormalText(_T("Mark Banner"));
	_static.AddNormalText(_T("Ben Willetts"));
	_static.AddNormalText(_T("David Hull"));
	_static.AddNormalText(_T("Matthew Hilling"));
	_static.AddNormalText(_T("Andrew J Cairns"));
	_static.AddNormalText(_T("Adam Beer"));
	_static.AddNormalText(_T("Michael Henshaw"));
	_static.AddNormalText(_T("Dr T Simon Futers"));
	_static.AddNormalText(_T("Glenn Taylor"));
	_static.AddNormalText(_T("Paul Butler"));
	_static.AddNormalText(_T("Joan Kemp"));
	_static.AddNormalText(_T("Simon Head"));
	_static.AddNormalText(_T("Leigh Simpson"));
	
	_static.AddBlankLine();
	_static.AddBlankLine();
	_static.AddBlankLine();
	_static.AddNormalText(_T("Portions Copyright"));
	_static.AddNormalText(_T("© 1998-2002"));
	_static.AddNormalText(_T("by BCGSoft Ltd"));
	_static.AddNormalText(_T("& Cristi Posea"));
	

	
	_static.AddBlankLine();
	_static.AddBlankLine();
	_static.AddBlankLine();
	_static.AddBlankLine();
	_static.AddBlankLine();
	_static.AddBlankLine();
	_static.AddBlankLine();
	_static.AddBlankLine();

}


CString AboutDlg::getMemory()
{
	CString out;
	
	// Fill available memory
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);

	out.Format("Free memory - %lu KB", MemStat.dwAvailPhys / 1024L);

	return out;

}

CString AboutDlg::getDisk()
{
	CString out;
	struct _diskfree_t diskfree;
	int nDrive = _getdrive(); // use current default drive
	if (_getdiskfree(nDrive, &diskfree) == 0)
	{
		out.Format("Free disk on %c: - %lu KB", 
			nDrive-1 + _T('A'),
			(DWORD)diskfree.avail_clusters *
			(DWORD)diskfree.sectors_per_cluster *
			(DWORD)diskfree.bytes_per_sector / (DWORD)1024L);
	}									 	
	return out;
}





void AboutDlg::OnStnClickedVersion()
{
	
}
