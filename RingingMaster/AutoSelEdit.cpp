// AutoSelEdit.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "AutoSelEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AutoSelEdit

AutoSelEdit::AutoSelEdit()
{
}

AutoSelEdit::~AutoSelEdit()
{
}


BEGIN_MESSAGE_MAP(AutoSelEdit, CEdit)
	//{{AFX_MSG_MAP(AutoSelEdit)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AutoSelEdit message handlers


void AutoSelEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	 SetSel(0,1000);	
}
