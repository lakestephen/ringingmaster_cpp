#pragma once

#include "SerialControlAction.h"

class SerialControlResponse : public SerialControlAction
{
public:
	SerialControlResponse(const SerialControlAction& action, bool state);
	SerialControlResponse();
	~SerialControlResponse(void);

	CString getDescription() const;
	
	void setValid(bool valid);

	bool isValid();

protected:
	bool _state;
	bool _valid;
};
