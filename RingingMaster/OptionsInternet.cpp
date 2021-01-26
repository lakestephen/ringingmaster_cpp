// OptionsInternet.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsInternet.h"
#include "RecordTower.h"
#include "InternetManager.h"
#include "RecordManager.h"
#include ".\optionsinternet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsInternet dialog


OptionsInternet::OptionsInternet()
	: CBCGPPropertyPage(OptionsInternet::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsInternet)
	_connectionStyle = -1;
	_port = 0;
	_launchStyle = -1;
	//}}AFX_DATA_INIT
}


void OptionsInternet::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsInternet)
	DDX_Control(pDX, IDC_LAUNCH_STYLE, _internalBrowserCtrl);
	DDX_Control(pDX, IDC_TEST_CONNECTION, _testConection);
	DDX_Control(pDX, IDC_STATIC_PORT, _posrtStatic);
	DDX_Control(pDX, IDC_STATIC_IP, _ipStatic);
	DDX_Control(pDX, IDC_USE_PROXY, _useProxyCtrl);
	DDX_Control(pDX, IDC_PORT, _portCtrl);
	DDX_Control(pDX, IDC_CONNECTION_STYLE, _defaultConnectionCtrl);
	DDX_Control(pDX, IDC_IP_ADDRESS, _ipADdress);
	DDX_Radio(pDX, IDC_CONNECTION_STYLE, _connectionStyle);
	DDX_Text(pDX, IDC_PORT, _port);
	DDX_Radio(pDX, IDC_LAUNCH_STYLE, _launchStyle);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _port, 0, INT_MAX);
}


BEGIN_MESSAGE_MAP(OptionsInternet, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsInternet)
	ON_BN_CLICKED(IDC_TEST_CONNECTION, OnTestConnection)
	ON_UPDATE_COMMAND_UI(IDC_TEST_CONNECTION, OnUpdateTestConnection)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsInternet message handlers

BOOL OptionsInternet::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	_testConection.SetImage(IDB_LINK);
	_testConection.m_bDrawFocus = FALSE;
	_testConection.SetTooltip("Test Connection");
	_testConection.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_connectionStyle = app()->GetProfileInt("Internet", 
									"Connection Type", 0);
	_launchStyle = app()->GetProfileInt("Internet", 
									"Launch Type", 0);

	BYTE b1 = (BYTE)app()->GetProfileInt("Internet", "IP Address 1", 127);
	BYTE b2 = (BYTE)app()->GetProfileInt("Internet", "IP Address 2", 0);
	BYTE b3 = (BYTE)app()->GetProfileInt("Internet", "IP Address 3", 0);
	BYTE b4 = (BYTE)app()->GetProfileInt("Internet", "IP Address 4", 1);

	_ipADdress.SetAddress(b1,b2,b3,b4);	 

	_port = app()->GetProfileInt("Internet", "Port", 80);


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control

}



void OptionsInternet::OnUpdateTestConnection(CCmdUI* /*pCmdUI*/)  
{
	_ipADdress.EnableWindow((_useProxyCtrl.GetCheck() == 1));
	_portCtrl.EnableWindow((_useProxyCtrl.GetCheck() == 1));
	_ipStatic.EnableWindow((_useProxyCtrl.GetCheck() == 1));
	_posrtStatic.EnableWindow((_useProxyCtrl.GetCheck() == 1));
}



void OptionsInternet::OnTestConnection() 
{
	//update settings in the registry
	if (OnKillActive())
	{
		CWaitCursor wait;
		internetManager()->checkConnection(true);
	}
}

BOOL OptionsInternet::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		app()->WriteProfileInt("Internet", "Connection Type", _connectionStyle );
		app()->WriteProfileInt("Internet", "Launch Type", _launchStyle );
		BYTE b1,b2,b3,b4;
		_ipADdress.GetAddress(b1,b2,b3,b4);	 					

		app()->WriteProfileInt("Internet","IP Address 1", b1);
		app()->WriteProfileInt("Internet","IP Address 2", b2);
		app()->WriteProfileInt("Internet","IP Address 3", b3);
		app()->WriteProfileInt("Internet","IP Address 4", b4);
					  
		app()->WriteProfileInt("Internet", "Port", _port);	
		
		//make sure that the next request for a session gets one with the new details.
		internetManager()->deleteSession();
		return TRUE;
	}

	return FALSE;
}
