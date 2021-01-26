#include "StdAfx.h"
#include "SpeechResponse.h"

SpeechResponse::SpeechResponse(const SpeechAction& action) :
SpeechAction(action)
{
}

SpeechResponse::SpeechResponse(void)
{

}

SpeechResponse::~SpeechResponse(void)
{
}

CString SpeechResponse::getDescription() const
{				  
	return SpeechAction::getDescription() + (_valid?"":" x");
}

void SpeechResponse::setValid(bool valid)
{
	_valid = valid;
}

bool SpeechResponse::isValid()
{
	return _valid;
}