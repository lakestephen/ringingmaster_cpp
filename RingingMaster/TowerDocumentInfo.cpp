#include "StdAfx.h"
#include ".\towerdocumentinfo.h"

TowerDocumentInfo::TowerDocumentInfo(void)
{
}

TowerDocumentInfo::~TowerDocumentInfo(void)
{
}

TowerDocumentInfo::TowerDocumentInfo(int selectedTowerIndex) :
_selectedTowerIndex(selectedTowerIndex)
{

}
IMPLEMENT_SERIAL( TowerDocumentInfo, CObject, 1)


void TowerDocumentInfo::Serialize(CArchive& ar)
{
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		ar << _selectedTowerIndex;
	}
	else
	{
		ar >> version;
		ar >> _selectedTowerIndex;
	}
}

int TowerDocumentInfo::getSelectedTowerIndex()
{
	return _selectedTowerIndex;
}