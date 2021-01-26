// TowerSoundsBellInputs.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsBellInputs.h"
#include "PlayManager.h"
#include "Tower.h"
#include "KeyboardManager.h"
#include "KeyboardItem.h"
#include "KeyboardResponse.h"
#include "UserMessages.h"
#include "MultiBellInterfaceManager.h"
#include "SpeechManager.h"
#include "SerialControlManager.h"
#include "SerialControlItem.h"
#include "SpeechResponse.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsBellInputs dialog


TowerSoundsBellInputs::TowerSoundsBellInputs(Tower& internalTower)
: TowerSoundsInputsBase(TowerSoundsBellInputs::IDD, internalTower)
{

}
TowerSoundsBellInputs::~TowerSoundsBellInputs()
{
}


BEGIN_MESSAGE_MAP(TowerSoundsBellInputs, TowerSoundsInputsBase)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsBellInputs message handlers


SimulatorAction& TowerSoundsBellInputs::getActionItem(int index)
{
	return _internalTower.getAction((SimulatorActionType)index);
}					  

int TowerSoundsBellInputs::getActionCount() 
{
	return _internalTower.getNumber();
}

CString TowerSoundsBellInputs::getActionName(int index) 
{
	return getSimulatorActionTypeName((SimulatorActionType)index);
}

void TowerSoundsBellInputs::addActionItem(int index)
{
	ASSERT(index >= 0 && index < getActionCount());

	_internalTower.addAction((SimulatorActionType)index);

}
