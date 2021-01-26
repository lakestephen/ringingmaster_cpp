// ToolbarManager.h: interface for the ToolbarManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "ToolbarFileState.h"
#include "RingingMaster.h"
#include "ManagerBase.h"


class ToolbarManager : public ManagerBase
{
public:
	void load();
	void save();
	ToolbarManager();
	virtual ~ToolbarManager();

	const ToolbarFileState& getDefaultState(ViewType viewType);
	const ToolbarFileState& getActivateState();
	const ToolbarFileState& getStateAllowed(ViewType viewType);

	StateType getStateType(ViewType viewType);
	CString getConfigLocation(ViewType viewType);
	CString getConfigLocation(StateType stateType);

	CString getStateName(StateType stateType);
	CString getViewName(ViewType viewType);

protected:
	void setStateAllowedDefaults();
	void setDefaultStateDefaults();
	void setActiveStateDefaults()
;
	ToolbarFileState _stateAllowed[LAST_VIEW_TYPE+1];
	ToolbarFileState _defaultState[LAST_VIEW_TYPE+1];
	ToolbarFileState _activateState;   

};

