#pragma once

#include "ResizeDialog.h"
#include "MultiBellInterfaceManagerEventListener.h"


// MultiBellInterfaceTestDlg dialog

class MultiBellInterfaceTestDlg : public ResizeDialog, MultiBellInterfaceManagerEventListener
{
	DECLARE_DYNAMIC(MultiBellInterfaceTestDlg)

public:
	MultiBellInterfaceTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MultiBellInterfaceTestDlg();

// Dialog Data
	enum { IDD = IDD_MBI_TEST };
	CListBox	_list;
	int _timestampOrigin;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp);
	virtual void multiBellInterfaceManager_notifyMBIStatusEvent(bool portOK, bool MBIOK);
	afx_msg LRESULT OnMultiBellInterfaceEvent(WPARAM wParam, LPARAM lParam);

	void multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
