// TowerSoundsCallInputs.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsCallInputs.h"
#include "Tower.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsCallInputs dialog


TowerSoundsCallInputs::TowerSoundsCallInputs(Tower& internalTower)
: TowerSoundsInputsBase(TowerSoundsCallInputs::IDD, internalTower)
{

}
TowerSoundsCallInputs::~TowerSoundsCallInputs()
{

}


BEGIN_MESSAGE_MAP(TowerSoundsCallInputs, TowerSoundsInputsBase)
	
END_MESSAGE_MAP()


SimulatorAction& TowerSoundsCallInputs::getActionItem(int index) 
{
	return _internalTower.getAction((SimulatorActionType)(SIMULATOR_ACTION_FIRST_CALL + index));
}

int TowerSoundsCallInputs::getActionCount() 
{
	return SIMULATOR_ACTION_LAST_CALL - SIMULATOR_ACTION_FIRST_CALL +1;
}

CString TowerSoundsCallInputs::getActionName(int index) 
{
	ASSERT(index >= 0 && index < getActionCount());

	return getSimulatorActionTypeName((SimulatorActionType)(SIMULATOR_ACTION_FIRST_CALL + index));
}

void TowerSoundsCallInputs::addActionItem(int index)
{
	ASSERT(index >= 0 && index < getActionCount());

	_internalTower.addAction((SimulatorActionType)(SIMULATOR_ACTION_FIRST_CALL + index));
}
