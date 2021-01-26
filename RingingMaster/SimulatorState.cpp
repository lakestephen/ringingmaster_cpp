#include "StdAfx.h"
#include ".\simulatorstate.h"
#include "TowerManager.h"
#include "SimulatorManager.h"
#include "RingingMaster.h"
#include "Tower.h"


SimulatorState::SimulatorState(void) 
{
	setAllStroke(hand);
	setAllPlayBell(true);
	resetAutoStart();
	setStateType(sst_disconnected);
}

SimulatorState::~SimulatorState(void)
{
}


void SimulatorState::setAllStroke(Stroke stroke)
{
	CSingleLock lock(&_strokeCriticalSection, TRUE);

	for (int i=0;i<MAXBELLS;i++)
		_stroke[i] = stroke; 
}

void SimulatorState::setStroke(int index, Stroke stroke)
{
	CSingleLock lock(&_strokeCriticalSection, TRUE);

	ASSERT(index >=0 && index < MAXBELLS);

	if (index >=0 && index < MAXBELLS)
		_stroke[index] = stroke; 
}



Stroke SimulatorState::getStroke(int index)
{
	CSingleLock lock(&_strokeCriticalSection, TRUE);

	ASSERT(index >=0 && index < MAXBELLS);

	if (index >=0 && index < MAXBELLS)
		return _stroke[index];

	return hand;
}

Stroke SimulatorState::toggleStroke(int index)
{
	CSingleLock lock(&_strokeCriticalSection, TRUE);

	ASSERT(index >=0 && index < MAXBELLS);

	if (index >=0 && index < MAXBELLS)
	{
		if (_stroke[index] == hand)
			_stroke[index] = back;
		else
			_stroke[index] = hand;
	
		return _stroke[index];
	}

	return hand;
}


void SimulatorState::decrementPlayingBell(int bell)   
{
	CSingleLock lock(&_playBellCriticalSection, TRUE);
	ASSERT(bell >= 0 && bell < MAXBELLS);

	if (_playBell[bell] > 0)
		_playBell[bell]--;
}

bool SimulatorState::isPlayingBell(int bell)   
{
	CSingleLock lock(&_playBellCriticalSection, TRUE);

	ASSERT(bell >= 0 && bell < MAXBELLS);
	return (_playBell[bell] == 0);
}

void SimulatorState::setPlayingBell(int bell, bool state)
{
	CSingleLock lock(&_playBellCriticalSection, TRUE);
	ASSERT(bell >= 0 && bell < MAXBELLS);
	if (state)
		_playBell[bell] = 0;
	else
		_playBell[bell] = STROKES_TO_RESTART;
}

void SimulatorState::setAllPlayBell(bool state)
{
	CSingleLock lock(&_playBellCriticalSection, TRUE);

	for (int i=0;i<MAXBELLS;i++)
		setPlayingBell(i, state);
}

void SimulatorState::resetAutoStart()
{
	CSingleLock lock(&_autoStartCriticalSection, TRUE);
	
	_autoStartBell = -1;
	_autoStartStroke = hand;
	_autoStartCount = 0;	
	_autoStartTimestamp = 0;
}

void SimulatorState::pulseAutoStart(int bell, DWORD timestamp)
{
	CSingleLock lock(&_autoStartCriticalSection, TRUE);

	TRACE("TIMESTAMP %d\r\n", timestamp);

	if (_autoStartBell == -1)
	{
		//we are starting a fresh
		_autoStartBell = bell;
		_autoStartCount = 1;
		_autoStartTimestamp = timestamp;
		_autoStartStroke = getStroke(bell);
	}
	else if (_autoStartBell == bell)
	{
		_autoStartCount++;
		//need to calculate the interval between bells.
		Tower* tower = towerManager()->getTower();
		Stroke stroke = tower->getSimulatorRoundsStartStroke();
		if (tower)
		{     
			if (_autoStartCount >= (HANDSTROKES_TO_AUTOSTART*2) &&
				stroke == getStroke(bell))
			{
				double number = simulatorManager()->getNumber();
				double elapsedTime = timestamp - _autoStartTimestamp;
				double bellsPerHandstrokeLead = (double)(number*2) + tower->getOpenHandstrokeGap(); 
				int speed = (int)(elapsedTime/(bellsPerHandstrokeLead * (double)HANDSTROKES_TO_AUTOSTART));

				if (speed >= Tower::_minSpeed && speed <= Tower::_maxSpeed)
				{
					//calculate the time to play the treble and start the method.
					simulatorManager()->autoStart(speed, bell, timestamp);
					return;
				}				
				//we have errored
				setStateType(sst_connectedSilent);
			}
		}
	}
}

SimulatorStateType SimulatorState::getStateType() 
{ 
	return _state;
}

void SimulatorState::setStateType(SimulatorStateType type) 
{ 
	CSingleLock lock(&_stateCriticalSection, TRUE);

	TRACE("State = %d\r\n", type); 
	_state = type;
}   