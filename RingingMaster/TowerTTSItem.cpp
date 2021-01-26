// TowerTTSItem.cpp: implementation of the TowerTTSItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerTTSItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


TowerTTSItem::TowerTTSItem(TowerTTSTypes type) :
_type(type)
{
	switch (_type)
	{

	case tttst_start:
		_active = true;
		_string = "Look two, treble's going, she's gone";
		break;
	case tttst_go:
		_active = true;
		_string = "Go <>";
		break;
	case tttst_thatsAll:
		_active = true;
		_string = "Thats All";
		break;
	case tttst_rounds:
		_active = true;
		_string = "Rounds";
		break;
	case tttst_stand:
		_active = true;
		_string = "Stand";
		break;
	case tttst_call:
		_active = true;
		_string = "<>";
		break;
	case tttst_splice:
		_active = true;
		_string = "<>";
		break;
	case tttst_part:
		_active = false;
		_string = "Part <>";
		break;
	default:
		ASSERT(FALSE);
	}
}

TowerTTSItem::TowerTTSItem() :
_type(tttst_start),
_active(FALSE)
{
}

TowerTTSItem::TowerTTSItem(const TowerTTSItem& towerTTSItem) :
_type(towerTTSItem._type),
_active(towerTTSItem._active),
_string(towerTTSItem._string)
{

}


void TowerTTSItem::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _type;
		ar << _string;
		ar << _active;
	}
	else
	{
		ar >> version;
		int type;
		ar >> type;
		_type = static_cast<TowerTTSTypes>(type);		
		ar >> _string;
		ar >> _active;
	}
}

bool TowerTTSItem::operator !=(const TowerTTSItem& towerTTSItem)
{
	return !(operator ==(towerTTSItem));
}

bool TowerTTSItem::operator ==(const TowerTTSItem& towerTTSItem)
{
	if (_active != towerTTSItem._active) return false;
	if (_string != towerTTSItem._string) return false;
	if (_type   != towerTTSItem._type)   return false;

	return true;
}


CString TowerTTSItem::getItemTypeText(TowerTTSTypes type)
{
	switch (type)
	{
	case tttst_start: 
		return "Start"; 
	case tttst_go:
		return "Go"; 
	case tttst_thatsAll:
		return "End"; 
	case tttst_rounds:
		return "Rounds";
	case tttst_stand:
		return "Stand"; 
	case tttst_call:
		return "Calls";
	case tttst_splice:
		return "Splices";
	case tttst_part:
		return "Parts";	
	default:
		ASSERT(FALSE);
		return "";
	}
}
