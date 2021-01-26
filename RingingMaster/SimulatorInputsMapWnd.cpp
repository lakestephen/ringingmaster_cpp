// SimulatorInputsMapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorInputsMapWnd.h" 
#include "SimulatorManager.h"
#include "UserMessages.h"
#include ".\simulatorinputspywnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(SimulatorInputsMapWnd, CCJListCtrl  )
SimulatorInputsMapWnd::SimulatorInputsMapWnd() : 
_timestampOrigin(0)
{
	simulatorManager()->addEventListener(this);
	_imageList.Create(IDB_SIMULATOR_ICONS, 16, 1, RGB(255,0,255));
}

SimulatorInputsMapWnd::~SimulatorInputsMapWnd()
{
	simulatorManager()->removeEventListener(this);
}


BEGIN_MESSAGE_MAP(SimulatorInputsMapWnd, CCJListCtrl  )
END_MESSAGE_MAP()


BOOL SimulatorInputsMapWnd::Create( CWnd* pParentWnd, UINT nID)
{
	BOOL ret = CCJListCtrl::Create(LVS_REPORT | LVS_SORTASCENDING | LVS_SHOWSELALWAYS | WS_VISIBLE | WS_TABSTOP, 
					CRect(0,0,220,200), pParentWnd, nID);

	SetImageList(&_imageList, LVSIL_SMALL);
	SetMinimumColSize(30);

	AddColumn("Action");
	AddColumn("Input");

	AutoSaveColumns(NULL, "SimulatorInputsMapWnd", "60,75");

	return ret;
}

void SimulatorInputsMapWnd::update()	
{
	
	const SimulatorActionMap& map = simulatorManager()->getSimulatorActionMap();

	CSingleLock lock(&map._criticalSection, TRUE);	

	POSITION pos = map.GetStartPosition();
	SimulatorInput key;
	SimulatorActionTypes* value = NULL;

	DeleteAllItems();

	while (pos)
	{
		map.GetNextAssoc(pos, key, value);
		for (int i=0;i<value->GetSize();i++)
		{
			SimulatorActionType type = value->GetAt(i);

			int count = GetItemCount();
			count = InsertItem(count,getSimulatorActionTypeName(type),-1);
			SetItemText(count, 1, key.getDescription(sidt_long_base));

		}
	}
}


void SimulatorInputsMapWnd::simulatorManager_notifySimulatorConnected(bool /*connected*/)
{
	update();
}

void SimulatorInputsMapWnd::simulatorManager_notifyProfileChange()
{
	update();

}
