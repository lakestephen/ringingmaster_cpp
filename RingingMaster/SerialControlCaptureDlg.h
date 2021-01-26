#pragma once

#include "SerialControlManagerEventListener.h"
#include "SerialControlResponse.h"

// SerialControlCaptureDlg dialog

class SerialControlCaptureDlg : public CDialog, SerialControlManagerEventListener
{
	DECLARE_DYNAMIC(SerialControlCaptureDlg)

public:
	SerialControlCaptureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SerialControlCaptureDlg();

// Dialog Data
	enum { IDD = IDD_SERIAL_CONTROL_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp);
	void serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& /*serialControlResponseSimulatorEventDataMap*/);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnSerialControlInterfaceEvent(WPARAM wParam, LPARAM lParam);

	SerialControlResponse _serialControlResponse;
};
