// ToolbarCombo.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "ToolbarCombo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ToolbarCombo

ToolbarCombo::ToolbarCombo()
{
}


BEGIN_MESSAGE_MAP(ToolbarCombo, CComboBox)
	//{{AFX_MSG_MAP(ToolbarCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ToolbarCombo message handlers

BOOL ToolbarCombo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CFrameWnd* pMainFrame = GetTopLevelFrame ();
		if (pMainFrame != NULL)
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				pMainFrame->SetFocus ();
				return TRUE;
			}
		}

		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

