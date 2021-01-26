// PlayRequest.h: interface for the PlayRequest class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

class Method;
#include "StrikingData.h"

#include "RowHolder.h"

class PlayRequest : public RowHolder 
{
public:
	bool getLastRowIndefiniteRepeat();
	void setLastRowIndefiniteRepeat(bool lastRowIndefiniteRepeat);
	void setPullsPerChange(int pullsPerChange);
	int getPullsPerChange();
	PlayRequest(const Method& method);
	PlayRequest(const StrikingData& strikingData);
	virtual ~PlayRequest();

	int getUniqueId() const;
	virtual bool getAnotherRow(Stroke stroke);

	virtual int getStartChangeRepeat(Tower* tower) const;
	virtual int getEndChangeRepeat(Tower* tower) const;

	bool isStrikingData() const;
	const StrikingData& getStrikingData();

protected:

	int _pullsPerChange;
	bool _lastRowIndefiniteRepeat;
	int _methdodRequestId;
	StrikingData _strikingData;
	bool _isStrikingData;
};
