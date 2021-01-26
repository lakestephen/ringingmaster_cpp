// Definitions.cpp: implementation of the Definitions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Definition.h"
#include "TouchDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Definition::Definition(TouchDoc* pTouchDoc) :
_touchDoc(pTouchDoc),
_type(dt_call),
_cellData(pTouchDoc)
{

}

Definition::~Definition()
{

}


void Definition::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _name;
		ar << _definition;
		ar << _type;
	}
	else
	{
		ar >> version;
		ar >> _name;
		ar >> _definition;
		int type;
		ar >> type;
		_type = static_cast<DefinitionType>(type);
		create();
	}
}

		
CString Definition::getName(bool includeType)
{
	if (includeType) 
	{
		return _name + 
			   " (" + 
			   ((_type == dt_call)? "Call":"Splice") +
				")";
	}
	else
		return _name ;
}


CString Definition::getDefinition()
{
	return _definition;
}

void Definition::setName(const CString &name)
{
	_name = name;
}

void Definition::setDefinition(const CString definition)
{
	_definition = definition;
	create();
}

void Definition::setType(DefinitionType type)
{
	_type = type;
	create();
}

DefinitionType Definition::getType()
{
	return _type;
}	 

void Definition::create()
{
	_cellData.destroyElements();
	for (int i=0;i<_definition.GetLength();i++)
		_cellData.onAddElement(_definition.GetAt(i), _cellData.getLength()); 
}

CellData* Definition::getCellData()
{
	return &_cellData;
}
