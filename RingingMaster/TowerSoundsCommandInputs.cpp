// TowerSoundsCommandInputs.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsCommandInputs.h"
#include "Tower.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsCommandInputs dialog


TowerSoundsCommandInputs::TowerSoundsCommandInputs(Tower& internalTower)
: TowerSoundsInputsBase(TowerSoundsCommandInputs::IDD, internalTower)
{

}
TowerSoundsCommandInputs::~TowerSoundsCommandInputs()
{

}


BEGIN_MESSAGE_MAP(TowerSoundsCommandInputs, TowerSoundsInputsBase)
	
END_MESSAGE_MAP()


SimulatorAction& TowerSoundsCommandInputs::getActionItem(int index) 
{
	return _internalTower.getAction((SimulatorActionType)(SIMULATOR_ACTION_FIRST_ACTION+ index));
}

int TowerSoundsCommandInputs::getActionCount() 
{
	return SIMULATOR_ACTION_LAST_ACTION - SIMULATOR_ACTION_FIRST_ACTION +1;
}

CString TowerSoundsCommandInputs::getActionName(int index) 
{
	ASSERT(index >= 0 && index < getActionCount());

	return getSimulatorActionTypeName((SimulatorActionType)(SIMULATOR_ACTION_FIRST_ACTION + index));
}

void TowerSoundsCommandInputs::addActionItem(int index)
{
	ASSERT(index >= 0 && index < getActionCount());

	_internalTower.addAction((SimulatorActionType)(SIMULATOR_ACTION_FIRST_ACTION + index));
}
