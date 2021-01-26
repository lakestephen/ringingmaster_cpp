// MethodSelectRow.h: interface for the MethodSelectRow class.
//
#pragma once

#include "MethodSelect.h"

class MethodSelectRow : public MethodSelect  
{
public:
	MethodSelectRow();
	virtual ~MethodSelectRow();

	Ints* getCallPositions();
	void setCallPositions(Ints* callPositions);

protected:
	Ints * _callPositions;		 

};
