// SerialControlTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SerialControlTestDlg.h"
#include "SerialControlManager.h"
#include "WinPos.h"
#include "UserMessages.h"
#include "PLayManager.h"
#include "SerialControlResponse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SerialControlTestDlg dialog
							    

SerialControlTestDlg::SerialControlTestDlg(CWnd* pParent /*=NULL*/)
: ResizeDialog(SerialControlTestDlg::IDD, pParent),
_timestampOrigin(0)
{
	//{{AFX_DATA_INIT(SerialControlTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void SerialControlTestDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SerialControlTestDlg)
	DDX_Control(pDX, IDC_LIST, _list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SerialControlTestDlg, ResizeDialog)
	//{{AFX_MSG_MAP(SerialControlTestDlg)
	ON_MESSAGE(SERIAL_CONTROL_INTERFACE_EVENT, OnSerialControlInterfaceEvent)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SerialControlTestDlg message handlers

BOOL SerialControlTestDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
	
	AddControl(IDC_LIST, resize, resize);

	WinPos::Load(this, "SerialControlTestDlg", true);

	serialControlManager()->addEventListener(this);

	_list.SetTabStops(10);
	_list.SetHorizontalExtent(150);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL SerialControlTestDlg::DestroyWindow() 
{
	serialControlManager()->removeEventListener(this);

	WinPos::Save(this, "SerialControlTestDlg");
	
	return ResizeDialog::DestroyWindow();
}

void SerialControlTestDlg::serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, SERIAL_CONTROL_INTERFACE_EVENT ,(WPARAM)(&response), timestamp); 
  
}

LRESULT SerialControlTestDlg::OnSerialControlInterfaceEvent(WPARAM wParam, LPARAM lParam)	
{
	SerialControlResponse* item = (SerialControlResponse*)wParam;

	int time = lParam - _timestampOrigin;

	time = min(time, 99999);
	
	CString str;
	str.Format("%05d ms\t%s",time , item->getDescription());

	int nCount = _list.AddString(str);

	if (nCount >= 0)
		_list.SetCurSel(nCount);
	
	playManager()->playDefaultBell();

	_timestampOrigin = lParam;

	return 0;
}

void SerialControlTestDlg::serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& serialControlResponseSimulatorEventDataMap)
{
	for (int i=1;i<=MAX_PORTS;i++)
	{
		if (serialControlManager()->isComPortConnected(i))
		{
			for (int j=0;j<=pci_dsr;j++)
			{
				serialControlResponseSimulatorEventDataMap.SetAt(			
					SerialControlAction(i,(PortControlItem)j), new SimulatorEventData(esigt_both,est_1of1));
			}
		}
	}
}