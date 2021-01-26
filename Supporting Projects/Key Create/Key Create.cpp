// Key Create.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Key Create.h"
#include "Key CreateDlg.h"
#include "PasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyCreateApp

BEGIN_MESSAGE_MAP(CKeyCreateApp, CWinApp)
	//{{AFX_MSG_MAP(CKeyCreateApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyCreateApp construction

CKeyCreateApp::CKeyCreateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKeyCreateApp object

CKeyCreateApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKeyCreateApp initialization

BOOL CKeyCreateApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


	SetRegistryKey(_T("Sensation Software"));

	PasswordDlg passwordDlg;

	if ((passwordDlg.DoModal() == IDOK)&&
		(verifyPassword(passwordDlg._password)))
	{
		CKeyCreateDlg dlg;
		m_pMainWnd = &dlg;
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}
	else
	{
		Beep(100,100);
	}



	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

bool CKeyCreateApp::verifyPassword(CString password)
{
	int size = 0;
	for (int i=0;i<password.GetLength();i++)
	{
		size += password.GetAt(i);
	}

	return (size == 874);
}
