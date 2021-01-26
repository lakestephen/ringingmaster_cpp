#include "StdAfx.h"
#include ".\serialcontrolresponse.h"

SerialControlResponse::SerialControlResponse(const SerialControlAction& action, bool state) :
SerialControlAction(action), 
_state(state), 
_valid(false)
{
}

SerialControlResponse::SerialControlResponse() :
_state(true), 
_valid(true)
{

}

SerialControlResponse::~SerialControlResponse(void)
{
}

CString SerialControlResponse::getDescription() const
{				  
	return SerialControlAction::getDescription() + (_state?" on":" off")+ (_valid?"":" x");
}

void SerialControlResponse::setValid(bool valid)
{
	_valid = valid;
}

bool SerialControlResponse::isValid()
{
	return _valid;
}
