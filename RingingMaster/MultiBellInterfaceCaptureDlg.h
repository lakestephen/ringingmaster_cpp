#pragma once

#include "MultiBellInterfaceManagerEventListener.h"

// MultiBellInterfaceCaptureDlg dialog

class MultiBellInterfaceCaptureDlg : public CDialog, MultiBellInterfaceManagerEventListener
{
	DECLARE_DYNAMIC(MultiBellInterfaceCaptureDlg)

public:
	MultiBellInterfaceCaptureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MultiBellInterfaceCaptureDlg();

// Dialog Data
	enum { IDD = IDD_MBI_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp) ;
	virtual void multiBellInterfaceManager_notifyMBIStatusEvent(bool portOK, bool MBIOK) ;
	virtual void multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	MultiBellResponse _multiBellResponse;

	afx_msg LRESULT OnMultiBellInterfaceEvent(WPARAM wParam, LPARAM lParam);
};
