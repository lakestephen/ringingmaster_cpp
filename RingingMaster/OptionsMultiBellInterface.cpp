// OptionsMultiBellInterface.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsMultiBellInterface.h"
#include "MultiBellInterfaceManager.h"
#include "UserMessages.h"
#include "PlayManager.h"
#include "OptionsDlg.h"
#include "SerialControlManager.h"
#include ".\optionsmultibellinterface.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsMultiBellInterface dialog


OptionsMultiBellInterface::OptionsMultiBellInterface()   // standard constructor
	: CBCGPPropertyPage(OptionsMultiBellInterface::IDD, NULL),
	_inited(false)
{
	//{{AFX_DATA_INIT(OptionsMultiBellInterface)
	//}}AFX_DATA_INIT

	multiBellInterfaceManager()->addEventListener(this);	
}

OptionsMultiBellInterface::~OptionsMultiBellInterface()
{
	multiBellInterfaceManager()->removeEventListener(this);
}


void OptionsMultiBellInterface::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsMultiBellInterface)
	DDX_Control(pDX, IDC_CONNECT_MSG, _connectMsgCtrl);
	DDX_Control(pDX, IDC_CONNECT_ICON, _connectIcon);
	DDX_Control(pDX, IDC_COM_PORT, _comPortCtrl);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TEST, _test);
	DDX_Control(pDX, IDC_RECONNECT, _reconnect);
}


BEGIN_MESSAGE_MAP(OptionsMultiBellInterface, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsMultiBellInterface)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_UPDATE_COMMAND_UI(IDC_ENABLE, OnUpdateEnable)		   	
	ON_CBN_SELCHANGE(IDC_COM_PORT, OnSelchangeComPort)
	ON_UPDATE_COMMAND_UI(IDC_CONNECT_MSG, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_CONNECT_ICON, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATUS_STATIC, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_DELAYS_STATIC, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_COM_PORT, OnUpdateGeneral)		 
	ON_UPDATE_COMMAND_UI(IDC_STATIC_DELAY, OnUpdateGeneral)		 
	ON_UPDATE_COMMAND_UI(IDC_STATIC_TRIG, OnUpdateGeneral)		 
	ON_UPDATE_COMMAND_UI(IDC_RECONNECT, OnUpdateGeneral)		 
	//}}AFX_MSG_MAP
	ON_MESSAGE(MULTI_BELL_INTERFACE_EVENT, OnMultiBellInterfaceEvent)

	ON_UPDATE_COMMAND_UI(IDC_TEST, OnUpdateTest)		 
	ON_BN_CLICKED(IDC_TEST, OnBnClickedTest)

	ON_BN_CLICKED(IDC_RECONNECT, OnBnClickedReconnect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsMultiBellInterface message handlers

BOOL OptionsMultiBellInterface::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	update();
	
	_connectIcon.m_bDrawFocus = FALSE;
	_connectIcon.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_NOBORDERS;

	_test.SetImage(IDB_MBI);
	_test.m_bDrawFocus = FALSE;
	_test.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_test.SetTooltip("Launch the multi bell interface test dialog.");

	_reconnect.SetImage(IDB_RESET);
	_reconnect.m_bDrawFocus = FALSE;
	_reconnect.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_reconnect.SetTooltip("Reconnect to the Multi Bell Interface.");

	_inited = true;

	multiBellInterfaceManager()->reconnect();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsMultiBellInterface::update() 
{
	_comPortCtrl.ResetContent();

	PortItems ports;
	serialPortManager()->enumPorts(ports);

	int MBIPort = multiBellInterfaceManager()->getPortID();
	CString name;
	
	for (int i=0;i<ports.GetSize();i++)
	{
		name = ports.GetAt(i)._name;
		if ((serialControlManager()->isSerialControlAvailable())&&
			(serialControlManager()->isComPortActive(ports.GetAt(i)._number)))
			name += " (SPC)";

		int index = _comPortCtrl.AddString(name);

		_comPortCtrl.SetItemData(index, ports.GetAt(i)._number);

		if (MBIPort == ports.GetAt(i)._number)
		{
			_comPortCtrl.SetCurSel(index);
		}
	}	
}

void OptionsMultiBellInterface::OnEnable() 
{
	multiBellInterfaceManager()->setUsingMBI(!multiBellInterfaceManager()->getUsingMBI());	
}


void OptionsMultiBellInterface::OnUpdateEnable(CCmdUI* pCmdUI)  
{
	pCmdUI->SetCheck(multiBellInterfaceManager()->getUsingMBI()?1:0);
}

void OptionsMultiBellInterface::OnUpdateGeneral(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(multiBellInterfaceManager()->isMBIAvailable());
}		  

void OptionsMultiBellInterface::OnUpdateTest(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(multiBellInterfaceManager()->isMBIConnected());
}		  

void OptionsMultiBellInterface::OnSelchangeComPort() 
{
	CWaitCursor wait;
	int comPort = _comPortCtrl.GetItemData(_comPortCtrl.GetCurSel());
	if (comPort >0)
		multiBellInterfaceManager()->setPortID(comPort);
}

void OptionsMultiBellInterface::OnBnClickedReconnect()
{
	multiBellInterfaceManager()->reconnect();
}

void OptionsMultiBellInterface::multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap)
{
	for (int i=0;i<MultiBellInterfaceManager::_MaxChannel;i++)
		multiBellActionSimulatorEventDataMap.SetAt(MultiBellAction(i),new SimulatorEventData(esigt_both));
}

void OptionsMultiBellInterface::multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, MULTI_BELL_INTERFACE_EVENT,(WPARAM)(&multiBellResponse), timestamp); 
}

LRESULT OptionsMultiBellInterface::OnMultiBellInterfaceEvent(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
//	MultiBellResponse* multiBellResponse = (MultiBellResponse*)wParam;

//	if ((_inited)&&(((OptionsDlg*)GetParent())->GetActivePage() == this))
//	{
//		playManager()->playDefaultBell();
//	}

	return 0;
}

void OptionsMultiBellInterface::multiBellInterfaceManager_notifyMBIStatusEvent(bool /*portOK*/, bool MBIOK)
{
	if (_inited)
	{
		if (MBIOK)
		{
			_connectMsgCtrl.SetWindowText("Connection OK to Multi Bell Interface.");
			_connectIcon.SetImage(IDB_TICK);
		}
		else
		{
			_connectMsgCtrl.SetWindowText("Can't connect to the Multi Bell Interface. You can still set up the system, but the system will not work. This is usefull when a laptop is disconnected from the system.");
			_connectIcon.SetImage(IDB_DELETE);
		}	

		_connectMsgCtrl.Invalidate();
		_connectIcon.Invalidate();
	}
}

void OptionsMultiBellInterface::OnBnClickedTest()
{
	multiBellInterfaceManager()->testMBI(this);
}

BOOL OptionsMultiBellInterface::OnKillActive()
{
	multiBellInterfaceManager()->isPortClash();

	return __super::OnKillActive();
}

BOOL OptionsMultiBellInterface::OnSetActive()
{
	update();

	return __super::OnSetActive();
}


