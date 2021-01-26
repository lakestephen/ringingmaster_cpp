#include "stdafx.h"
#include "SimulatorActionMap.h"

CString getSimulatorActionTypeName(SimulatorActionType type)
{
	CString str; 
	if (type >=	sat_bell0 && type <= sat_bell29)
	{
		str.Format("Bell %d", (int)type + 1);
	}
	else if (type >=	sat_method00&& type <= sat_method49)
	{
		str.Format("Method %d", (int)type - sat_method00 + 1);
	}
	else
	{
		switch (type)
		{
		case sat_startRingingRounds:
			str = "Start (Rounds)";
			break;
		case sat_goNext:
			str = "Go Next";
			break;
		case sat_startRingingRoundsAuto:
			str = "Auto Start";
			break;
		case sat_comeRoundNextTime:
			str = "Stop at Rounds";
			break;
		case sat_stand:
			str = "Stand";
			break;
		case sat_resetSimulator:
			str = "Reset";
			break;
		case sat_playStop:
			str = "Stop";
			break;
		case sat_playPause:
			str = "Pause";
			break;
		case sat_callPlain:
			str = "Plain";
			break;
		case sat_callBob:
			str = "Bob";
			break;
		case sat_callSingle:
			str = "Single";
			break;
		case sat_callX:
			str = "Call X";
			break;
		case sat_callY:
			str = "Call Y";
			break;
		case sat_callZ:
			str = "Call Z";
			break;
		case sat_speedUp:
			str = "Speed Up";
			break;
		case sat_speedDown:
			str = "Slow Down";
			break;
		case sat_pitchUp:
			str = "Pitch Up";
			break;
		case sat_pitchDown:
			str = "Pitch Down";
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}
	return str;
}