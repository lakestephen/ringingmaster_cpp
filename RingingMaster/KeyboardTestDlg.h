#if !defined(AFX_KEYBOARDTESTDLG_H__53888B3C_3811_4E8C_906F_BABA08ADF90B__INCLUDED_)
#define AFX_KEYBOARDTESTDLG_H__53888B3C_3811_4E8C_906F_BABA08ADF90B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyboardTestDlg.h : header file
//

#include "ResizeDialog.h"
#include "KeyboardManagerEventListener.h"

/////////////////////////////////////////////////////////////////////////////
// KeyboardTestDlg dialog


class KeyboardTestDlg : public ResizeDialog, KeyboardManagerEventListener
{
// Construction
public:
	KeyboardTestDlg(CWnd* pParent );   // standard constructor

	void keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp);
	void keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) ;


// Dialog Data
	//{{AFX_DATA(KeyboardTestDlg)
	enum { IDD = IDD_KEYBOARD_TEST };
	CListBox	_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KeyboardTestDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	afx_msg LRESULT OnKeyboardInterfaceEvent(WPARAM wParam, LPARAM lParam);
	int _timestampOrigin;


	// Generated message map functions
	//{{AFX_MSG(KeyboardTestDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDTESTDLG_H__53888B3C_3811_4E8C_906F_BABA08ADF90B__INCLUDED_)
