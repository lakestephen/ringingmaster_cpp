#pragma once


class OptionsManagerEventListener  
{
public:
	OptionsManagerEventListener() {};
	~OptionsManagerEventListener() {};
	virtual void optionsManager_optionsDlgClosed() {};
};

typedef CArray<OptionsManagerEventListener*, OptionsManagerEventListener*> OptionsManagerEventListeners;

