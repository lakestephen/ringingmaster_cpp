#pragma once

enum EventSequenceType
{
	est_1of1,
	est_1of2,
	est_2of2, 
	est_1of3, 
	est_2of3, 
	est_3of3, 
	est_1of4, 
	est_2of4, 
	est_3of4, 
	est_4of4, 
	//used at end for looping
};

enum EventSignalType
{
	esigt_both, 
	esigt_riseing,
	esigt_falling,
};


class SimulatorEventData  
{
public:
	SimulatorEventData(EventSignalType eventSignalType,EventSequenceType eventSequenceType = est_1of1, int delay = 0);

	bool eventTrigger(bool state = true);

	CString getSequenceTypeDescription() const;
	static CString getSequenceTypeDescription(EventSequenceType eventSequenceType) ;

	CString getSignalTypeDescription() const;
	static CString getSignalTypeDescription(EventSignalType eventSignalType);
	

	EventSignalType getEventSignalType() const ;
	void setEventSignalType(EventSignalType type);

	EventSequenceType getEventSequenceType() const ;
	void setEventSequenceType(EventSequenceType type);

	int getDelay() const ;
	void setDelay(int delay);

	void Serialize(CArchive &ar);

protected:
	EventSequenceType _eventSequenceType;
	EventSignalType _eventSignalType;
	int _delay;
	int _sequence;
};


