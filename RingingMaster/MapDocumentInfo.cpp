#include "StdAfx.h"
#include ".\mapdocumentinfo.h"


MapDocumentInfo::MapDocumentInfo() :
_selectedTowerIndex(0)
{

}

MapDocumentInfo::MapDocumentInfo(	const CString& name,
									const MapGridRef& mapGridRef,
									int selectedTowerIndex) :
_name(name), 
_mapGridRef(mapGridRef), 
_selectedTowerIndex(selectedTowerIndex)
{

}

MapDocumentInfo::~MapDocumentInfo(void)
{
}

IMPLEMENT_SERIAL( MapDocumentInfo, CObject, 1)


void MapDocumentInfo::Serialize(CArchive& ar)
{
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		ar << _name;
		_mapGridRef.Serialize(ar);
		ar << _selectedTowerIndex;
	}
	else
	{
		ar >> version;
		ar >> _name;
		_mapGridRef.Serialize(ar);
		ar >> _selectedTowerIndex;
	}
}

const MapGridRef& MapDocumentInfo::getMapRef()
{
	return _mapGridRef;
}

const CString& MapDocumentInfo::getName()
{
	return _name;
}

int MapDocumentInfo::getSelectedTowerIndex()
{
	return _selectedTowerIndex;
}
