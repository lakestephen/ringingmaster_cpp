#pragma once

#include "MultiBellAction.h"

class MultiBellResponse : public MultiBellAction
{
public:
	MultiBellResponse(const MultiBellAction& action);
	MultiBellResponse(void);
	~MultiBellResponse(void);
	

	CString getDescription() const;

	void setValid(bool valid);
	bool isValid();
	
protected:
	bool _valid;
};
