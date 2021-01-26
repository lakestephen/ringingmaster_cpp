#include "StdAfx.h"
#include "OptionsManager.h"

//todo eventually route all registry info through here

OptionsManager::OptionsManager(void)
{
}

OptionsManager::~OptionsManager(void)
{
}

void OptionsManager::load()
{
	
}

void OptionsManager::save()
{
	
}

void OptionsManager::openUp()
{
}

void OptionsManager::closeDown()
{

}


void OptionsManager::addEventListener(OptionsManagerEventListener  *optionsManagerEventListener )
{
	ASSERT(optionsManagerEventListener != NULL);

	_listenerList.Add(optionsManagerEventListener);

}

void OptionsManager::removeEventListener(OptionsManagerEventListener *optionsManagerEventListener )
{
	ASSERT(optionsManagerEventListener!= NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == optionsManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

#ifdef _DEBUG
	if (hasRemoved != 1)
		TRACE("[OptionsManager::removeEventListener] window already removed.");
#endif
}

void OptionsManager::fireOptionsDlgClosed(  )
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->optionsManager_optionsDlgClosed(); 
	}  
}