// OptionsSerialControl.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsSerialControl.h"
#include "SerialControlManager.h"
#include "SerialPortManager.h"
#include "MultiBellInterfaceManager.h"
#include ".\optionsserialcontrol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsSerialControl dialog


OptionsSerialControl::OptionsSerialControl()
	: CBCGPPropertyPage(OptionsSerialControl::IDD, NULL)
	, _interval(serialControlManager()->getPollInterval())
	, _debounce(serialControlManager()->getDebounce())
{
	serialControlManager()->addEventListener(this);

	//{{AFX_DATA_INIT(OptionsSerialControl)
	//}}AFX_DATA_INIT
}

OptionsSerialControl::~OptionsSerialControl()
{
	serialControlManager()->removeEventListener(this);
}

void OptionsSerialControl::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsSerialControl)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TEST, _test);
	DDX_Control(pDX, IDC_PORTS, _ports);

	DDX_Text(pDX, IDC_DEBOUNCE, _debounce);
	DDV_MinMaxInt(pDX, _debounce, 0, 10);
	DDX_Control(pDX, IDC_DEBOUNCE_SPIN, _debounceSpin);

	DDX_Text(pDX, IDC_POLL_INTERVAL, _interval);
	DDV_MinMaxInt(pDX, _interval, 0, 100);
	DDX_Control(pDX, IDC_POLL_INTERVAL_SPIN, _intervalSpin);

	DDX_Control(pDX, IDC_RECONNECT, _reconnect);
}


BEGIN_MESSAGE_MAP(OptionsSerialControl, CBCGPPropertyPage)
	ON_UPDATE_COMMAND_UI(IDC_TEST, OnUpdateTest)		 
	ON_BN_CLICKED(IDC_TEST, OnBnClickedTest)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_UPDATE_COMMAND_UI(IDC_ENABLE, OnUpdateEnable)		   	
	ON_UPDATE_COMMAND_UI(IDC_PORTS, OnUpdateGeneral)
	ON_UPDATE_COMMAND_UI(IDC_STATIC_PORTS, OnUpdateGeneral)
	ON_UPDATE_COMMAND_UI(IDC_DEBOUNCE_SPIN, OnUpdateGeneral)
	ON_UPDATE_COMMAND_UI(IDC_DEBOUNCE, OnUpdateGeneral)
	ON_UPDATE_COMMAND_UI(IDC_STATIC_MS, OnUpdateGeneral)
	ON_UPDATE_COMMAND_UI(IDC_STATIC_NOTE, OnUpdateGeneral)
	ON_UPDATE_COMMAND_UI(IDC_STATIC_DEBOUNCE, OnUpdateGeneral)
	ON_CLBN_CHKCHANGE(IDC_PORTS, OnClbnChkChangePorts)
	ON_BN_CLICKED(IDC_RECONNECT, OnBnClickedReconnect)
	ON_UPDATE_COMMAND_UI(IDC_RECONNECT, OnUpdateGeneral)		 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsSerialControl message handlers

void OptionsSerialControl::OnOK() 
{
	UpdateData();
	//check a valid file	   
	
	serialControlManager()->setDebounce(_debounce);
	serialControlManager()->setPollInterval(_interval);
	

	CBCGPPropertyPage::OnOK();
}

BOOL OptionsSerialControl::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	_test.SetImage(IDB_SERIAL);
	_test.m_bDrawFocus = FALSE;
	_test.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_test.SetTooltip("Launch the serial port test dialog.");

	_reconnect.SetImage(IDB_RESET);
	_reconnect.m_bDrawFocus = FALSE;
	_reconnect.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_reconnect.SetTooltip("Reconnect to the serial port.");


	_debounceSpin.SetRange32(0,10);
	_intervalSpin.SetRange32(0,100);

	update();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsSerialControl::OnEnable() 
{
	serialControlManager()->setUsingSerialControl(!serialControlManager()->getUsingSerialControl());	
	update();
}

			   
void OptionsSerialControl::OnUpdateEnable(CCmdUI* pCmdUI)  
{
	pCmdUI->SetCheck(serialControlManager()->getUsingSerialControl()?1:0);
}

void OptionsSerialControl::OnUpdateGeneral(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(serialControlManager()->isSerialControlAvailable());
}

void OptionsSerialControl::OnBnClickedTest()
{
	serialControlManager()->testSerialControl(this);
}

void OptionsSerialControl::OnUpdateTest(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(serialControlManager()->isComPortConnected(-1));
}		  

void OptionsSerialControl::update()
{
	if (::IsWindow(_ports.m_hWnd))
	{
		_ports.ResetContent();

		PortItems ports;
		serialPortManager()->enumPorts(ports);

		int MBIPort = multiBellInterfaceManager()->getPortID();
		CString name;
		
		for (int i=0;i<ports.GetSize();i++)
		{
			name = ports.GetAt(i)._name;
			if ((multiBellInterfaceManager()->isMBIAvailable())&&
				(ports.GetAt(i)._number == MBIPort))
				name += " (MBI)";

			if (serialControlManager()->isComPortActive(ports.GetAt(i)._number))
			{
				if (serialControlManager()->isComPortConnected(ports.GetAt(i)._number))
					name += " Connected";
				else
					name += " Error";
			}

			int index = _ports.AddString(name);
			_ports.SetItemData(index, ports.GetAt(i)._number);
			_ports.SetCheck(index, serialControlManager()->isComPortActive(ports.GetAt(i)._number));
		}	
	}
}

BOOL OptionsSerialControl::OnSetActive()
{
	update();

	return CBCGPPropertyPage::OnSetActive();
}

BOOL OptionsSerialControl::OnKillActive()
{
	serialControlManager()->isPortClash();

	return CBCGPPropertyPage::OnKillActive();
}

void OptionsSerialControl::OnClbnChkChangePorts()
{
	CWaitCursor wait;

	for (int i=0;i<_ports.GetCount();i++)
	{
		int portNo = _ports.GetItemData(i);
		BOOL check = _ports.GetCheck(i);
		
		serialControlManager()->setComPortActive(portNo, check);
	}

	update();
}

void OptionsSerialControl::serialControlManager_notifySerialControlStatusEvent() 
{
	update();
}


void OptionsSerialControl::OnBnClickedReconnect()
{
	serialControlManager()->reconnect();
}
