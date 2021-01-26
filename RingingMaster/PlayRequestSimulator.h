#pragma once
#include "playrequest.h"

class MethodSimulator;

class PlayRequestSimulator : //to2do create one of these for the call changes
	public PlayRequest
{
public:
	PlayRequestSimulator(MethodSimulator &method);
	~PlayRequestSimulator(void);

	bool getAnotherRow(Stroke stroke);
	virtual int getStartChangeRepeat(Tower* tower) const;
	virtual int getEndChangeRepeat(Tower* tower) const;

protected:
	MethodSimulator& _originalMethod;
};
