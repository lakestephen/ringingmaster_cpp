#pragma once


#include "SimulatorInput.h"



enum SimulatorActionType 
{
	sat_bell0,
	sat_bell1,
	sat_bell2,
	sat_bell3,
	sat_bell4,
	sat_bell5,
	sat_bell6,
	sat_bell7,
	sat_bell8,
	sat_bell9,
	sat_bell10,
	sat_bell11,
	sat_bell12,
	sat_bell13,
	sat_bell14,
	sat_bell15,
	sat_bell16,
	sat_bell17,
	sat_bell18,
	sat_bell19,
	sat_bell20,
	sat_bell21,
	sat_bell22,
	sat_bell23,
	sat_bell24,
	sat_bell25,
	sat_bell26,
	sat_bell27,
	sat_bell28,
	sat_bell29,

	//actions
	sat_startRingingRounds,
	sat_goNext,
	sat_startRingingRoundsAuto,
	sat_comeRoundNextTime,
	sat_stand, 
	sat_resetSimulator, 
	sat_playStop, 
	sat_playPause, 
	sat_speedUp, 
	sat_speedDown, 
	sat_pitchUp, 
	sat_pitchDown, 

	sat_extraAction0, //these are added in just to provide future growing room
	sat_extraAction1, 
	sat_extraAction2, 
	sat_extraAction3, 
	sat_extraAction4, 
	sat_extraAction5, 
	sat_extraAction6, 
	sat_extraAction7, 
	sat_extraAction8, 
	sat_extraAction9, 

	//calls
	sat_callPlain, 
	sat_callBob, 
	sat_callSingle, 
	sat_callX, 
	sat_callY, 
	sat_callZ, 
	
	sat_extraCall0, //these are added in just to provide future growing room
	sat_extraCall1, 
	sat_extraCall2, 
	sat_extraCall3, 
	sat_extraCall4, 
	sat_extraCall5, 
	sat_extraCall6, 
	sat_extraCall7, 
	sat_extraCall8, 
	sat_extraCall9, 

	//methods
	sat_method00,
	sat_method01,
	sat_method02,
	sat_method03,
	sat_method04,
	sat_method05,
	sat_method06,
	sat_method07,
	sat_method08,
	sat_method09,

	sat_method10,
	sat_method11,
	sat_method12,
	sat_method13,
	sat_method14,
	sat_method15,
	sat_method16,
	sat_method17,
	sat_method18,
	sat_method19,

	sat_method20,
	sat_method21,
	sat_method22,
	sat_method23,
	sat_method24,
	sat_method25,
	sat_method26,
	sat_method27,
	sat_method28,
	sat_method29,

	sat_method30,
	sat_method31,
	sat_method32,
	sat_method33,
	sat_method34,
	sat_method35,
	sat_method36,
	sat_method37,
	sat_method38,
	sat_method39,

	sat_method40,
	sat_method41,
	sat_method42,
	sat_method43,
	sat_method44,
	sat_method45,
	sat_method46,
	sat_method47,
	sat_method48,
	sat_method49,
};

#define SIMULATOR_ACTION_FIRST_BELL sat_bell0
#define SIMULATOR_ACTION_LAST_BELL sat_bell29

#define SIMULATOR_ACTION_FIRST_ACTION sat_startRingingRounds	 
#define SIMULATOR_ACTION_LAST_ACTION sat_pitchDown

#define SIMULATOR_ACTION_FIRST_CALL sat_callPlain
#define SIMULATOR_ACTION_LAST_CALL sat_callZ

#define SIMULATOR_ACTION_FIRST_METHOD sat_method00
#define SIMULATOR_ACTION_LAST_METHOD sat_method49

#define SIMULATOR_ACTION_COUNT (SIMULATOR_ACTION_LAST_METHOD +1)

typedef CArray<SimulatorActionType, SimulatorActionType> SimulatorActionTypes;


CString getSimulatorActionTypeName(SimulatorActionType type);



