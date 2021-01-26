// ribManager.cpp: implementation of the CribManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "cribManager.h"
#include "RegArchive.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CribManager::CribManager()
{
	_style.setDefault(ds_crib);
	_cribStyle.setDefaults();
}

CribManager::~CribManager()
{

}

void CribManager::setDefaultBlueLineStyle(const Style& style)
{
	_style = style;
}

const Style& CribManager::getDefaultBlueLineStyle()
{
	return _style;
}

void CribManager::setDefaultCribStyle(const CribStyle& cribStyle)
{
	_cribStyle = cribStyle;
}

const CribStyle& CribManager::getDefaultCribStyle()
{
	return _cribStyle;
}

void CribManager::save()
{	 
	RegArchive ar (CArchive::store, "Crib", "Blue Line Style");

	if (ar.isValid())
	{
		_style.Serialize(ar);
		_cribStyle.Serialize(ar);
	}
}

void CribManager::load()
{
	
	RegArchive ar (CArchive::load, "Crib", "Blue Line Style");
	
	if (ar.isValid()) 
	{
		_style.Serialize(ar);
		_cribStyle.Serialize(ar);
	}	
}
