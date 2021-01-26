#include "stdafx.h"
#include "SimulatorEventData.h"

SimulatorEventData::SimulatorEventData(EventSignalType eventSignalType, EventSequenceType eventSequenceType, int delay) :
_eventSequenceType(eventSequenceType),
_eventSignalType(eventSignalType), 
_delay(delay), 
_sequence(0)
{
}


bool SimulatorEventData::eventTrigger(bool state)
{
	//check for riseing or falling
	switch (_eventSignalType)
	{
		case esigt_both:
			break;
		case esigt_riseing:
			if (state)
				break;
			else 
				return false;
		case esigt_falling:	
			if (!state)
				break;
			else 
				return false;
		default:
			ASSERT(FALSE);
			return false;
	}
	
	//do the state check
	bool retSstate = false;
	switch (_eventSequenceType)
	{
	case est_1of1: 
	case est_1of2: 
	case est_1of3: 
	case est_1of4: 
		retSstate = (_sequence == 0) ;
		break;
	case est_2of2:
	case est_2of3:
	case est_2of4:
		retSstate = (_sequence == 1);
		break;
	case est_3of3:
	case est_3of4:
		retSstate = (_sequence == 2);
		break;
	case est_4of4:
		retSstate = (_sequence == 3);
		break;
	default:
		ASSERT(FALSE);
	}

	//do the increment math for next time round
	_sequence++;
	switch (_eventSequenceType)
	{
	case est_1of1: 
		if (_sequence >=1)
			_sequence = 0;
		break;
	case est_1of2: 
	case est_2of2:
		if (_sequence >=2)
			_sequence = 0;
		break;
	case est_1of3: 
	case est_2of3:
	case est_3of3:
		if (_sequence >=3)
			_sequence = 0;
		break;
	case est_1of4: 
	case est_2of4:
	case est_3of4:
	case est_4of4:
		if (_sequence >=4)
			_sequence = 0;
		break;
	default:
		ASSERT(FALSE);
	}

	return retSstate;
}

CString SimulatorEventData::getSequenceTypeDescription() const
{
	return getSequenceTypeDescription(_eventSequenceType);
}

CString SimulatorEventData::getSequenceTypeDescription(EventSequenceType eventSequenceType) 
{
	switch (eventSequenceType)
	{
	case est_1of1:
		return "Every";
		break;
	case est_1of2:
		return "1of2";
		break;
	case est_2of2:
		return "2of2";
		break;
	case est_1of3:
		return "1of3";
		break;
	case est_2of3:
		return "2of3";
		break;
	case est_3of3:
		return "3of3";
		break;
	case est_1of4:
		return "1of4";
		break;
	case est_2of4:
		return "2of4";
		break;
	case est_3of4:
		return "3of4";
		break;
	case est_4of4:
		return "4of4";
		break;
	default:
		ASSERT(FALSE);
		return "";
	}
}

CString SimulatorEventData::getSignalTypeDescription() const
{
	return getSignalTypeDescription(_eventSignalType);
}

CString SimulatorEventData::getSignalTypeDescription(EventSignalType eventSignalType)
{
	switch (eventSignalType)
	{
	case esigt_both:
		return "Rise/Fall";
		break;
	case esigt_riseing:
		return "Rise";
		break;
	case esigt_falling:
		return "Fall";
		break;
	default:
		ASSERT(FALSE);
		return "";
	}
}


EventSignalType SimulatorEventData::getEventSignalType() const 
{
	return _eventSignalType;
}

void SimulatorEventData::setEventSignalType(EventSignalType type)
{
	_eventSignalType = type;
}

EventSequenceType SimulatorEventData::getEventSequenceType() const 
{
	return _eventSequenceType;
}

void SimulatorEventData::setEventSequenceType(EventSequenceType type)
{
	_eventSequenceType = type;
}

int SimulatorEventData::getDelay() const 
{
	return _delay;
}

void SimulatorEventData::setDelay(int delay)
{
	_delay = delay;
}

void SimulatorEventData::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
		ar << _eventSequenceType;
		ar << _eventSignalType;
		ar << _delay;
	}
	else
	{
		ar >> version;
		int temp;
		
		ar >> temp;
		_eventSequenceType = (EventSequenceType)temp;
		
		ar >> temp;
		_eventSignalType = (EventSignalType)temp;

		ar >> _delay;
	}
}