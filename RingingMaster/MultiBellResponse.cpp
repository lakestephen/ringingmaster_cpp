#include "StdAfx.h"
#include ".\multibellresponse.h"

MultiBellResponse::MultiBellResponse(const MultiBellAction& action) :
MultiBellAction(action)
{
}

MultiBellResponse::MultiBellResponse(void)
{

}

MultiBellResponse::~MultiBellResponse(void)
{
}

CString MultiBellResponse::getDescription() const
{				  
	return MultiBellAction::getDescription() + (_valid?"":" x");
}

void MultiBellResponse::setValid(bool valid)
{
	_valid = valid;
}

bool MultiBellResponse::isValid()
{
	return _valid;
}