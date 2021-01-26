// TowerSoundsMethodInputs.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsMethodInputs.h"
#include "Tower.h"
#include ".\towersoundsmethodinputs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsMethodInputs dialog


TowerSoundsMethodInputs::TowerSoundsMethodInputs(Tower& internalTower)
: TowerSoundsInputsBase(TowerSoundsMethodInputs::IDD, internalTower), 
_keyboardShortcuts(internalTower.getAddMethodShortcutsAsKeyboardInputs()), 
_microphoneNames(internalTower.getAddMethodNamesAsMicrophoneInputs())
{

}
TowerSoundsMethodInputs::~TowerSoundsMethodInputs()
{

}


BEGIN_MESSAGE_MAP(TowerSoundsMethodInputs, TowerSoundsInputsBase)
	
END_MESSAGE_MAP()

void TowerSoundsMethodInputs::DoDataExchange(CDataExchange* pDX)
{
	DDX_Check(pDX, IDC_KEYBOARD_SHORTCUTS, _keyboardShortcuts);
	DDX_Check(pDX, IDC_MICROPHONE_NAMES, _microphoneNames);
	
	TowerSoundsInputsBase::DoDataExchange(pDX);
}

SimulatorAction& TowerSoundsMethodInputs::getActionItem(int index) 
{
	return _internalTower.getAction((SimulatorActionType)(SIMULATOR_ACTION_FIRST_METHOD+ index));
}

int TowerSoundsMethodInputs::getActionCount() 
{
	return SIMULATOR_ACTION_LAST_METHOD - SIMULATOR_ACTION_FIRST_METHOD +1;
}

CString TowerSoundsMethodInputs::getActionName(int index) 
{
	ASSERT(index >= 0 && index < getActionCount());

	return getSimulatorActionTypeName((SimulatorActionType)(SIMULATOR_ACTION_FIRST_METHOD + index));
}

void TowerSoundsMethodInputs::addActionItem(int index)
{
	ASSERT(index >= 0 && index < getActionCount());

	_internalTower.addAction((SimulatorActionType)(SIMULATOR_ACTION_FIRST_METHOD + index));
}



void TowerSoundsMethodInputs::OnOK()
{
	UpdateData(FALSE);
	_internalTower.setAddMethodShortcutsAsKeyboardInputs(_keyboardShortcuts);
	_internalTower.setAddMethodNamesAsMicrophoneInputs(_microphoneNames);

	TowerSoundsInputsBase::OnOK();
}
