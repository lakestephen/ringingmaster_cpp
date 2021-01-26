#include "StdAfx.h"
#include "KeyboardResponse.h"

KeyboardResponse::KeyboardResponse(const KeyboardAction& action, bool state) :
KeyboardAction(action),
_state(state), 
_valid(false)
{
}

KeyboardResponse::KeyboardResponse(void) :
_state(true), 
_valid(true)
{

}

KeyboardResponse::~KeyboardResponse(void)
{
}

CString KeyboardResponse::getDescription() const
{				  
	return KeyboardAction::getDescription() + (_state?" on":" off")+ (_valid?"":" x");
}

void KeyboardResponse::setValid(bool valid)
{
	_valid = valid;
}

bool KeyboardResponse::isValid()
{
	return _valid;
}