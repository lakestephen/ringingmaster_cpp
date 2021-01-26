#pragma once

#include "Stroke.h"
#include "SimulatorStateType.h"



#define STROKES_TO_RESTART 4
#define HANDSTROKES_TO_AUTOSTART 2


class SimulatorState
{
public:
	SimulatorState(void);
	~SimulatorState(void);


	void setAllStroke(Stroke stroke = hand);
	void setAllPlayBell(bool state);

	void setStroke(int index, Stroke stroke);
	Stroke getStroke(int index);
	Stroke toggleStroke(int index);

	bool isPlayingBell(int bell);
	void setPlayingBell(int bell, bool state);
	void decrementPlayingBell(int bell)  ;

	void resetAutoStart();
	void pulseAutoStart(int bell, DWORD timestamp);

	SimulatorStateType getStateType();
	void setStateType(SimulatorStateType type);

private:

	CCriticalSection _stateCriticalSection;
	SimulatorStateType _state;

	CCriticalSection _strokeCriticalSection;
	Stroke _stroke[MAXBELLS];
		
	CCriticalSection _playBellCriticalSection;
	int _playBell[MAXBELLS];

	CCriticalSection _autoStartCriticalSection;
	int _autoStartBell;
	Stroke _autoStartStroke;
	int _autoStartCount;
	DWORD _autoStartTimestamp;

};
