#pragma once


//make sure #defines below are maintained

enum ToolbarTypes
{
	//tool bars 
	tt_toolBarStandard,
	tt_toolBarTouchMain,
	tt_toolBarTouchBlueLine,
	tt_toolBarSimulator,
	tt_toolBarSimulatorCalls,
	tt_toolBarRecord,
	tt_toolBarRecordLogic,
	tt_toolBarCrib,
	tt_toolBarStrike,
	tt_toolBarInternet,
	tt_toolBarImage,
	tt_toolBarMap,
	tt_toolBarPlay,
	tt_toolBarPlaySimulator,
	tt_toolBarTools,
	
	//conmtrol bars
	tt_controlBarProperty, 
	tt_controlBarPlayPosition, //roll up window
	tt_controlBarTouchAnalysisStatus,
	tt_controlBarTouchAnalysisMusic,
	tt_controlBarTouchAnalysisCourseHead,
	tt_controlBarTouchAnalysisLeadEnd,
	tt_controlBarTouchAnalysisAllTheWork,
	tt_controlBarTouchAnalysisMultiBlock,
	tt_controlBarTouchAnalysisCallsMade,
	tt_controlBarTouchAnalysisSplice,
	tt_controlBarTouchAnalysisSpliceTransition,
	tt_controlBarTouchAnalysisFalseness,
	tt_controlBarSimulatorMethod,
	tt_controlBarSimulatorStriking,
	tt_controlBarSimulatorSingleBell1,
	tt_controlBarSimulatorSingleBell2,
	tt_controlBarSimulatorInputSpy,
	tt_controlBarSimulatorInputsMap,
	tt_controlBarSimulatorStrikingControls,//roll up window
	tt_controlBarStrikeGraph,
	tt_controlBarStrikeInfo, 
	tt_controlBarRecordHomeTower,
	tt_controlBarRecordSelectedTower,

};

#define FIRST_TOOLBAR  tt_toolBarStandard
#define FIRST_CONTROLBAR tt_controlBarProperty
#define LAST_CONTROLBAR  tt_controlBarRecordSelectedTower

