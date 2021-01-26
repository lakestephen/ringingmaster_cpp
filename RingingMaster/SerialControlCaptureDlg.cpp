// SerialControlCaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SerialControlCaptureDlg.h"
#include ".\serialcontrolcapturedlg.h"
#include "PlayManager.h"
#include "SerialControlManager.h"
#include "WinPos.h"


// SerialControlCaptureDlg dialog

IMPLEMENT_DYNAMIC(SerialControlCaptureDlg, CDialog)
SerialControlCaptureDlg::SerialControlCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SerialControlCaptureDlg::IDD, pParent)
{
}

SerialControlCaptureDlg::~SerialControlCaptureDlg()
{
}

void SerialControlCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SerialControlCaptureDlg, CDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(SERIAL_CONTROL_INTERFACE_EVENT, OnSerialControlInterfaceEvent)	
END_MESSAGE_MAP()


// SerialControlCaptureDlg message handlers

BOOL SerialControlCaptureDlg::OnInitDialog()
{
	__super::OnInitDialog();

	WinPos::Load(this, "SerialControlCaptureDlg", false);

	serialControlManager()->addEventListener(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SerialControlCaptureDlg::OnDestroy()
{
	__super::OnDestroy();

	serialControlManager()->removeEventListener(this);

	WinPos::Save(this, "SerialControlCaptureDlg");
}

void SerialControlCaptureDlg::serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp)
{
 	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, SERIAL_CONTROL_INTERFACE_EVENT,(WPARAM)(&response), timestamp); 
}

LRESULT SerialControlCaptureDlg::OnSerialControlInterfaceEvent(WPARAM wParam, LPARAM /*lParam*/)	
{
	SerialControlResponse* item = (SerialControlResponse*)wParam;

	_serialControlResponse = *item;

	playManager()->playDefaultBell();

	EndDialog(IDOK);

	return 0;
}

void SerialControlCaptureDlg::serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& serialControlResponseSimulatorEventDataMap)
{
	for (int i=1;i<=MAX_PORTS;i++)
	{
		if (serialControlManager()->isComPortConnected(i))
		{
			for (int j=0;j<=pci_dsr;j++)
			{
				serialControlResponseSimulatorEventDataMap.SetAt(			
					SerialControlAction(i,(PortControlItem)j), new SimulatorEventData(esigt_both, est_1of1));
			}
		}
	}
}