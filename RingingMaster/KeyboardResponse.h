#pragma once

#include "KeyboardAction.h"

class KeyboardResponse : public KeyboardAction
{
public:
	KeyboardResponse(const KeyboardAction& action, bool state);
	KeyboardResponse(void);
	~KeyboardResponse(void);
	

	CString getDescription() const;

	void setValid(bool valid);
	bool isValid();
	
protected:
	bool _valid;
	bool _state;
};
