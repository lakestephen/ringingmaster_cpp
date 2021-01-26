// SimulatorInputSpyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorInputSpyWnd.h" 
#include "SimulatorManager.h"
#include "UserMessages.h"
#include ".\simulatorinputspywnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(SimulatorInputSpyWnd, CCJListCtrl  )
SimulatorInputSpyWnd::SimulatorInputSpyWnd() : 
_timestampOrigin(0)
{
	simulatorManager()->addEventListener(this);
	_imageList.Create(IDB_SIMULATOR_ICONS, 16, 1, RGB(255,0,255));
}

SimulatorInputSpyWnd::~SimulatorInputSpyWnd()
{
	simulatorManager()->removeEventListener(this);
}


BEGIN_MESSAGE_MAP(SimulatorInputSpyWnd, CCJListCtrl  )
	ON_MESSAGE(SIMULATOR_SPY_ITEM, OnSimulatorSpyItem)
END_MESSAGE_MAP()


BOOL SimulatorInputSpyWnd::Create( CWnd* pParentWnd, UINT nID)
{
	BOOL ret = CCJListCtrl::Create(LVS_REPORT | LVS_SHOWSELALWAYS | WS_VISIBLE | WS_TABSTOP, 
					CRect(0,0,220,200), pParentWnd, nID);

	SetImageList(&_imageList, LVSIL_SMALL);
	SetMinimumColSize(30);

	AddColumn("Time");
	AddColumn("Type");
	AddColumn("Value");

	AutoSaveColumns(NULL, "SimulatorInputSpyWnd", "75,35,40");

	return ret;
}


void SimulatorInputSpyWnd::simulatorManager_notifyRawInputFromSeperateThread(const SimulatorInput& simulatorInput, DWORD timestamp )
{
	if (::IsWindow(m_hWnd))
	{
		//take a copy of teh simulator input item.
		SimulatorInput* input = new SimulatorInput(simulatorInput);
		::PostMessage(m_hWnd ,SIMULATOR_SPY_ITEM, (WPARAM)input, timestamp );
	}		
}

LRESULT SimulatorInputSpyWnd::OnSimulatorSpyItem(WPARAM wParam, LPARAM lParam)	
{
	SimulatorInput* input = (SimulatorInput*)wParam;

	if (((CBCGPDockingControlBar*) GetParent())->IsVisible())
	{
		int time = lParam - _timestampOrigin;
		time = min(time, 99999);
		CString strTime;
		strTime.Format(" %05d ms",time );

		int count = GetItemCount();
		InsertItem(count,strTime , input->getSimulatorInputType());

		SetItemText(count, 1, input->getTypeText());
		SetItemText(count, 2, input->getDescription(sidt_short));

		EnsureVisible(count, FALSE);

		Invalidate();

		while (GetItemCount() > 100)
			DeleteItem(0);

		_timestampOrigin = lParam; 
	}
	else
	{
		if (GetItemCount())
			DeleteAllItems();
	}

	delete input;

	return 0;
}


void SimulatorInputSpyWnd::simulatorManager_notifySimulatorConnected(bool /*connected*/)
{
	DeleteAllItems();
}

