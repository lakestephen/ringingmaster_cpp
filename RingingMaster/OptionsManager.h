#pragma once
#include "managerbase.h"
#include "OptionsManagerEventListener.h"

class OptionsManager :
	public ManagerBase
{
public:
	OptionsManager(void);
public:
	~OptionsManager(void);

	void save();
	void load();
	void openUp();
	void closeDown();

	void removeEventListener(OptionsManagerEventListener *optionsManagerEventListener );
	void addEventListener(OptionsManagerEventListener  *optionsManagerEventListener );
	
	void fireOptionsDlgClosed();

protected:

	OptionsManagerEventListeners _listenerList;

};
