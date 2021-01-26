#if !defined(AFX_KEYBOARDTESTDLG_H__53888B3C_3811_4E8C_906F_BABA08ADF90B__INCLUDED_)
#define AFX_KEYBOARDTESTDLG_H__53888B3C_3811_4E8C_906F_BABA08ADF90B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerialControlTestDlg.h : header file
//

#include "ResizeDialog.h"
#include "SerialControlManagerEventListener.h"

/////////////////////////////////////////////////////////////////////////////
// SerialControlTestDlg dialog


class SerialControlTestDlg : public ResizeDialog, SerialControlManagerEventListener
{
// Construction
public:
	SerialControlTestDlg(CWnd* pParent );   // standard constructor


// Dialog Data
	//{{AFX_DATA(SerialControlTestDlg)
	enum { IDD = IDD_SERIAL_CONTROL_TEST};
	CListBox	_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SerialControlTestDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp);
	void serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& /*serialControlResponseSimulatorEventDataMap*/);
	afx_msg LRESULT OnSerialControlInterfaceEvent(WPARAM wParam, LPARAM lParam);
	int _timestampOrigin;


	// Generated message map functions
	//{{AFX_MSG(SerialControlTestDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDTESTDLG_H__53888B3C_3811_4E8C_906F_BABA08ADF90B__INCLUDED_)
