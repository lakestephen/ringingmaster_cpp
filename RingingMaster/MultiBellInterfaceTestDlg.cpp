// MultiBellInterfaceTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MultiBellInterfaceTestDlg.h"
#include "MultiBellInterfaceManager.h"
#include "PlayManager.h"
#include "UserMessages.h"
#include "WinPOs.h"


// MultiBellInterfaceTestDlg dialog

IMPLEMENT_DYNAMIC(MultiBellInterfaceTestDlg, ResizeDialog)
MultiBellInterfaceTestDlg::MultiBellInterfaceTestDlg(CWnd* pParent /*=NULL*/)
	: ResizeDialog(MultiBellInterfaceTestDlg::IDD, pParent)	,
_timestampOrigin(0)
{
}

MultiBellInterfaceTestDlg::~MultiBellInterfaceTestDlg()
{
}

void MultiBellInterfaceTestDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_LIST, _list);
}


BEGIN_MESSAGE_MAP(MultiBellInterfaceTestDlg, ResizeDialog)
	ON_MESSAGE(MULTI_BELL_INTERFACE_EVENT, OnMultiBellInterfaceEvent)	
END_MESSAGE_MAP()						 


// MultiBellInterfaceTestDlg message handlers

BOOL MultiBellInterfaceTestDlg::OnInitDialog()
{
	__super::OnInitDialog();

	AddControl(IDC_LIST, resize, resize);

	WinPos::Load(this, "MultiBellInterfaceTestDlg", true);

	multiBellInterfaceManager()->addEventListener(this);

	_list.SetTabStops(50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



BOOL MultiBellInterfaceTestDlg::DestroyWindow()
{
	multiBellInterfaceManager()->removeEventListener(this);

	WinPos::Save(this, "MultiBellInterfaceTestDlg");

	return __super::DestroyWindow();
}


void MultiBellInterfaceTestDlg::multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, MULTI_BELL_INTERFACE_EVENT,(WPARAM)(&multiBellResponse), timestamp); 
}

LRESULT MultiBellInterfaceTestDlg::OnMultiBellInterfaceEvent(WPARAM wParam, LPARAM lParam)	
{
	MultiBellResponse* multiBellResponse = (MultiBellResponse*)wParam;

	int time = lParam - _timestampOrigin;

	time = min(time, 99999);
	
	CString str;
	str.Format("%05d ms\t%s",time ,multiBellResponse->getDescription() );

	int nCount = _list.AddString(str);

	if (nCount >= 0)
		_list.SetCurSel(nCount);
	
	playManager()->playDefaultBell();

	_timestampOrigin = lParam;

	return 0;

}

void MultiBellInterfaceTestDlg::multiBellInterfaceManager_notifyMBIStatusEvent(bool /*portOK*/, bool /*MBIOK*/)
{

}


void MultiBellInterfaceTestDlg::multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap)
{
	for (int i=0;i<MultiBellInterfaceManager::_MaxChannel;i++)
		multiBellActionSimulatorEventDataMap.SetAt(MultiBellAction(i),new SimulatorEventData(esigt_both));
}