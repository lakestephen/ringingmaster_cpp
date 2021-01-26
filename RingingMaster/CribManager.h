// ribManager.h: interface for the CribManager class.
//
//////////////////////////////////////////////////////////////////////


#include "Style.h"	// Added by ClassView
#include "CribStyle.h"	// Added by ClassView
#include "ManagerBase.h"

#pragma once

class CribManager : public ManagerBase
{
public:
	void load();
	void save();
	const CribStyle& getDefaultCribStyle();
	void setDefaultCribStyle(const CribStyle& cribStyle);

	const Style& getDefaultBlueLineStyle();
	void setDefaultBlueLineStyle(const Style& style);

	CribManager();
	virtual ~CribManager();

protected:
	Style _style;
	CribStyle _cribStyle;

};

