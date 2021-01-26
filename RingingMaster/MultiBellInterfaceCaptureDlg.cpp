// MultiBellInterfaceCaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MultiBellInterfaceCaptureDlg.h"
#include "MultiBellInterfaceManager.h"
#include "UserMessages.h"
#include "WinPos.h"
#include "PlayManager.h"


// MultiBellInterfaceCaptureDlg dialog
				   
IMPLEMENT_DYNAMIC(MultiBellInterfaceCaptureDlg, CDialog)
MultiBellInterfaceCaptureDlg::MultiBellInterfaceCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MultiBellInterfaceCaptureDlg::IDD, pParent)
{
}

MultiBellInterfaceCaptureDlg::~MultiBellInterfaceCaptureDlg()
{
}

void MultiBellInterfaceCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MultiBellInterfaceCaptureDlg, CDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(MULTI_BELL_INTERFACE_EVENT, OnMultiBellInterfaceEvent)
END_MESSAGE_MAP()


// MultiBellInterfaceCaptureDlg message handlers

BOOL MultiBellInterfaceCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WinPos::Load(this, "MultiBellInterfaceCaptureDlg", false);

	multiBellInterfaceManager()->addEventListener(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void MultiBellInterfaceCaptureDlg::OnDestroy()
{
	CDialog::OnDestroy();

	multiBellInterfaceManager()->removeEventListener(this);

	WinPos::Save(this, "MultiBellInterfaceCaptureDlg");
}

void MultiBellInterfaceCaptureDlg::multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp) 
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, MULTI_BELL_INTERFACE_EVENT,(WPARAM)(&multiBellResponse), timestamp); 
}  

LRESULT MultiBellInterfaceCaptureDlg::OnMultiBellInterfaceEvent(WPARAM wParam, LPARAM /*lParam*/)	
{
	MultiBellResponse* item = (MultiBellResponse*)wParam;

	_multiBellResponse = *item;

	playManager()->playDefaultBell();

	EndDialog(IDOK);

	return 0;
}

void MultiBellInterfaceCaptureDlg::multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap)
{
	for (int i=0;i<MultiBellInterfaceManager::_MaxChannel;i++)
		multiBellActionSimulatorEventDataMap.SetAt(MultiBellAction(i),new SimulatorEventData(esigt_both));
}

void MultiBellInterfaceCaptureDlg::multiBellInterfaceManager_notifyMBIStatusEvent(bool /*portOK*/, bool /*MBIOK*/) 
{
	ASSERT(FALSE);
}

