// Key CreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Key Create.h"
#include "Key CreateDlg.h"
#include "Key.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyCreateDlg dialog

CKeyCreateDlg::CKeyCreateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyCreateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyCreateDlg)
	_key = _T("");
	_user = _T("");
	_version = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyCreateDlg)
	DDX_Control(pDX, IDC_VERSION, _versionCtrl);
	DDX_Text(pDX, IDC_KEY, _key);
	DDX_Text(pDX, IDC_USER, _user);
	DDX_Text(pDX, IDC_VERSION, _version);
	DDV_MinMaxInt(pDX, _version, 1000, 9999);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyCreateDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyCreateDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GENERATE, OnGenerate)
	ON_BN_CLICKED(IDC_VERIFY, OnVerify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyCreateDlg message handlers

BOOL CKeyCreateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	_version = AfxGetApp()->GetProfileInt("Settings", "Version", 2010);	
	_user = AfxGetApp()->GetProfileString("Settings", "User", "Stephen J Lake");	
	UpdateData(FALSE);

	_versionCtrl.LoadHistory("Version");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyCreateDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeyCreateDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CKeyCreateDlg::OnGenerate() 
{
	UpdateData();

	_key = Key::calculateKey(_user, _version);
	KeyRetType res = Key::verifyKey(_user,_version, _key);
	if (res != KR_OK)
		AfxMessageBox(Key::getErrorString(res));

	UpdateData(FALSE);

	_versionCtrl.SaveHistory();
	AfxGetApp()->WriteProfileInt("Settings", "Version", _version);	
	AfxGetApp()->WriteProfileString("Settings", "User", _user);	

	//need to copy key 

}

void CKeyCreateDlg::OnVerify() 
{
	UpdateData();

	KeyRetType error = Key::verifyKey(_user, _version, _key);
	if (error != KR_OK)
		AfxMessageBox(Key::getErrorString(error));
}
