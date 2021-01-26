#pragma once

#include "SpeechAction.h"

class SpeechResponse : public SpeechAction
{
public:
	SpeechResponse(const SpeechAction& action);
	SpeechResponse(void);
	~SpeechResponse(void);
	

	CString getDescription() const;

	void setValid(bool valid);
	bool isValid();
	
protected:
	bool _valid;
};
