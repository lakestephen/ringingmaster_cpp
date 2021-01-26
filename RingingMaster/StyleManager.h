// StyleManager.h: interface for the StyleManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "Style.h"
#include "ManagerBase.h"

class StyleManager:  public ManagerBase
{
public:
	void setAt(int index, Style style);
	void removeStyle(int index);
	void addStyle(Style style);
	StyleManager();
	virtual ~StyleManager();

	
	void setActiveStyleIndex(int index);
	int getActiveStyleIndex();

	void setupStyleMenu (CMenu* menu);
	Style *getStyle(int index = -1); //default is active
	BOOL isStyleUnique(CString strName);
	int getStyleIndexFromName(CString strName);
	int getStyleCount();
	void save();
	void load();

protected:
	Styles _styleArray;	
	int _activeStyle;
};


