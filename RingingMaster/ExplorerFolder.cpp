// ExplorerFolder.cpp: implementation of the ExplorerFolder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "ExplorerFolder.h"
#include "Notation.h"
#include "MethodManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const unsigned long ExplorerFolder::_pivotPoint = 100000;
unsigned long ExplorerFolder::_userUniqueId = ExplorerFolder::_pivotPoint + 1;
unsigned long ExplorerFolder::_CCUniqueId = ExplorerFolder::_pivotPoint - 1;


ExplorerFolder::ExplorerFolder() :
_id(0),
_level(0),
_editable(FALSE),
_canCreateChildren(FALSE)
{

}

ExplorerFolder::~ExplorerFolder()
{
}

void ExplorerFolder::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _id;
		ar << _level;
		ar << _name;
		ar << _editable;
		ar << _canCreateChildren;

	}
	else
	{
		ar >> version; //just get the version number
		ar >> _id;
		ar >> _level;
		ar >> _name;
		ar >> _editable;
		ar >> _canCreateChildren;
		
		//static stuff
		if (_userUniqueId < _id)
			_userUniqueId = _id+1;
		if (_CCUniqueId > _id)
			_CCUniqueId = _id-1;

	}
}

void ExplorerFolder::setName(const CString &name)
{
	methodManager()->resetFolderNameCache();
	_name = name;
}

CString ExplorerFolder::getName()
{
	return _name;
}

unsigned long ExplorerFolder::createUniqueUserID()
{
	methodManager()->resetFolderNameCache();	
	return ++_userUniqueId;	
}				 

unsigned long ExplorerFolder::createUniqueCCID()
{
	methodManager()->resetFolderNameCache();
	return --_CCUniqueId;	
}				 


void ExplorerFolder::setLevel(int level)
{	
	methodManager()->resetFolderNameCache();
	_level = level;
}

unsigned long ExplorerFolder::getID()
{
	return _id;
}

int ExplorerFolder::getLevel()
{
	return _level;
}

void ExplorerFolder::setID(unsigned long ID)
{
	methodManager()->resetFolderNameCache();
	_id = ID;
}








