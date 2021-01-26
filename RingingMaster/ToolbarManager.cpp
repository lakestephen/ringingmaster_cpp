// ToolbarManager.cpp: implementation of the ToolbarManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolbarManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


ToolbarManager::ToolbarManager()
{
	setStateAllowedDefaults();
	setActiveStateDefaults();
	setDefaultStateDefaults();
}		  

ToolbarManager::~ToolbarManager()
{

}


void ToolbarManager::save()
{
}

void ToolbarManager::load()
{
}

CString ToolbarManager::getConfigLocation(StateType stateType)
{
	return RingingMasterApp::_RegistryBase + "\\" + getStateName(stateType);	
}

CString ToolbarManager::getConfigLocation(ViewType viewType)
{
	return RingingMasterApp::_RegistryBase + "\\" + getViewName(viewType);	
}

const ToolbarFileState& ToolbarManager::getActivateState()
{
	return _activateState;	
}

const ToolbarFileState& ToolbarManager::getDefaultState(ViewType viewType)
{
	ASSERT(viewType != vt_null);

	return _defaultState[viewType];
}

const ToolbarFileState& ToolbarManager::getStateAllowed(ViewType viewType)
{
	ASSERT(viewType != vt_null);

	return _stateAllowed[viewType];
}

void ToolbarManager::setDefaultStateDefaults()
{
	// this provides the ability to have some of the allowable 
	// toolbars not on initially.


	//copy the stateAllowed
	for (int i=FIRST_VIEW_TYPE;i<=LAST_VIEW_TYPE;i++)
		_defaultState[i] = _stateAllowed[i];


	//NEVER TURN ANYTHING ON HERE ONLY OFF.

	//now set up the exceptions
	_defaultState[vt_touch_simulator].setState(tt_controlBarSimulatorInputSpy, tfs_HIDE);
	_defaultState[vt_touch_simulator].setState(tt_controlBarSimulatorInputsMap, tfs_HIDE);
	_defaultState[vt_touch_simulator].setState(tt_controlBarSimulatorSingleBell1, tfs_HIDE);
	_defaultState[vt_touch_simulator].setState(tt_controlBarSimulatorSingleBell2, tfs_HIDE);
	_defaultState[vt_touch_simulator].setState(tt_controlBarTouchAnalysisCallsMade, tfs_HIDE);

}

void ToolbarManager::setActiveStateDefaults()
{
	//active state
	//this is a list of what control bars are given focus when docking
	_activateState.setState(tt_controlBarTouchAnalysisStatus, tfs_SHOW); 
	_activateState.setState(tt_controlBarTouchAnalysisFalseness, tfs_SHOW); 
	_activateState.setState(tt_controlBarSimulatorMethod, tfs_SHOW); 
}

void ToolbarManager::setStateAllowedDefaults()
{

	//none
	_stateAllowed[vt_none].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_none].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_none].setState(tt_toolBarPlay, tfs_SHOW);

	//touch main
	_stateAllowed[vt_touch_main].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_toolBarTouchMain, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarProperty, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisStatus, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisMusic, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisCourseHead, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisLeadEnd, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisAllTheWork, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisMultiBlock, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisCallsMade, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisSplice, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisSpliceTransition, tfs_SHOW);
	_stateAllowed[vt_touch_main].setState(tt_controlBarTouchAnalysisFalseness, tfs_SHOW);

	//touch blue line
	_stateAllowed[vt_touch_blue_line].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_toolBarTouchBlueLine, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarProperty, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisStatus , tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisMusic, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisCourseHead, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisLeadEnd, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisAllTheWork, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisMultiBlock, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisCallsMade, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisSplice, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisSpliceTransition, tfs_SHOW);
	_stateAllowed[vt_touch_blue_line].setState(tt_controlBarTouchAnalysisFalseness, tfs_SHOW);

	//touch simulator
	_stateAllowed[vt_touch_simulator].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_toolBarPlaySimulator, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_toolBarSimulator, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_toolBarSimulatorCalls, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarProperty, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarSimulatorMethod, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarSimulatorStriking, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarSimulatorInputSpy, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarSimulatorInputsMap, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarSimulatorStrikingControls, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarSimulatorSingleBell1, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarSimulatorSingleBell2, tfs_SHOW);
	_stateAllowed[vt_touch_simulator].setState(tt_controlBarTouchAnalysisCallsMade, tfs_SHOW);

	//record towers
	_stateAllowed[vt_tower].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_tower].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_tower].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_tower].setState(tt_toolBarRecord, tfs_SHOW);
	_stateAllowed[vt_tower].setState(tt_toolBarRecordLogic, tfs_SHOW);
	_stateAllowed[vt_tower].setState(tt_controlBarProperty, tfs_SHOW);
	_stateAllowed[vt_tower].setState(tt_controlBarRecordHomeTower, tfs_SHOW);
	_stateAllowed[vt_tower].setState(tt_controlBarRecordSelectedTower, tfs_SHOW);

	//record towers
	_stateAllowed[vt_grab].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_grab].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_grab].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_grab].setState(tt_toolBarRecord, tfs_SHOW);
	_stateAllowed[vt_grab].setState(tt_toolBarRecordLogic, tfs_SHOW);
	_stateAllowed[vt_grab].setState(tt_controlBarProperty, tfs_SHOW);
	_stateAllowed[vt_grab].setState(tt_controlBarRecordHomeTower, tfs_SHOW);
	_stateAllowed[vt_grab].setState(tt_controlBarRecordSelectedTower, tfs_SHOW);

	//internet
	_stateAllowed[vt_internet].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_internet].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_internet].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_internet].setState(tt_toolBarInternet, tfs_SHOW);
	_stateAllowed[vt_internet].setState(tt_controlBarProperty, tfs_SHOW);

	//image 
	_stateAllowed[vt_image].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_image].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_image].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_image].setState(tt_toolBarImage, tfs_SHOW);

	//map	  
	_stateAllowed[vt_map].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_map].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_map].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_map].setState(tt_toolBarMap, tfs_SHOW);
	_stateAllowed[vt_map].setState(tt_controlBarProperty, tfs_SHOW);
	_stateAllowed[vt_map].setState(tt_controlBarRecordHomeTower, tfs_SHOW);
	_stateAllowed[vt_map].setState(tt_controlBarRecordSelectedTower, tfs_SHOW);

	//crib
	_stateAllowed[vt_crib].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_crib].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_crib].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_crib].setState(tt_toolBarCrib, tfs_SHOW);
	_stateAllowed[vt_crib].setState(tt_controlBarProperty, tfs_SHOW);

	//strike
	_stateAllowed[vt_strike].setState(tt_toolBarStandard, tfs_SHOW);
	_stateAllowed[vt_strike].setState(tt_toolBarTools, tfs_SHOW);
	_stateAllowed[vt_strike].setState(tt_toolBarPlay, tfs_SHOW);
	_stateAllowed[vt_strike].setState(tt_toolBarStrike, tfs_SHOW);
	_stateAllowed[vt_strike].setState(tt_controlBarStrikeGraph, tfs_SHOW);
	_stateAllowed[vt_strike].setState(tt_controlBarStrikeInfo, tfs_SHOW);

}

StateType ToolbarManager::getStateType(ViewType viewType)
{
	switch (viewType)
	{
	//use the same value for some of the values - 
	//prevents the layout change.
	case vt_touch_main: 
	case vt_touch_blue_line:	return st_touch_main;
	case vt_touch_simulator:	return st_touch_simulator;
	case vt_tower:				
	case vt_grab:				
	case vt_map:				return st_records;
	case vt_internet:			return st_internet;
	case vt_image:				return st_image;
	case vt_crib:				return st_crib;
	case vt_strike:				return st_strike;
	case vt_none:				return st_none;
	default:
		return st_null;
	}
}

CString ToolbarManager::getStateName(StateType stateType)
{
	switch (stateType)
	{
	//use the same value for some of the values - 
	//prevents the layout change.
	case st_touch_main:			return "st_touch_main";
	case st_touch_simulator:	return "st_touch_simulator";
	case st_records:			return "st_records";
	case st_internet:			return "st_internet";
	case st_image:				return "st_image";
	case st_crib:				return "st_crib";
	case st_strike:				return "st_strike";
	case st_none:				return "st_none";
	default:
		ASSERT(FALSE);
		return "Error";
	}	
}

CString ToolbarManager::getViewName(ViewType viewType)
{
	switch (viewType)
	{
	//use the same value for some of the values - 
	//prevents the layout change.

	case vt_touch_main:			return "vt_touch_main";
	case vt_touch_blue_line:	return "vt_touch_blue_line";
	case vt_touch_simulator:	return "vt_touch_simulator";
	case vt_tower:				return "vt_tower";
	case vt_grab:				return "vt_grab";
	case vt_map:				return "vt_map";
	case vt_internet:			return "vt_internet";
	case vt_image:				return "vt_image";
	case vt_crib:				return "vt_crib";
	case vt_strike:				return "vt_strike";
	case vt_none:				return "vt_none";
	default:
		ASSERT(FALSE);
		return "Error";
	}	
}

