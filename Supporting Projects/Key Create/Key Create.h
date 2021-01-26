// Key Create.h : main header file for the KEY CREATE application
//

#if !defined(AFX_KEYCREATE_H__D6385BD8_F35E_11D4_B571_009027BB3286__INCLUDED_)
#define AFX_KEYCREATE_H__D6385BD8_F35E_11D4_B571_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeyCreateApp:
// See Key Create.cpp for the implementation of this class
//

class CKeyCreateApp : public CWinApp
{
public:
	bool verifyPassword(CString password);
	CKeyCreateApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyCreateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKeyCreateApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCREATE_H__D6385BD8_F35E_11D4_B571_009027BB3286__INCLUDED_)
