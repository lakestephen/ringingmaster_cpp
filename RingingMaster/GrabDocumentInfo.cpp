#include "StdAfx.h"
#include ".\grabdocumentinfo.h"

GrabDocumentInfo::GrabDocumentInfo(void)
{
}

GrabDocumentInfo::~GrabDocumentInfo(void)
{
}

GrabDocumentInfo::GrabDocumentInfo(int selectedGrabIndex) :
_selectedGrabIndex(selectedGrabIndex)
{

}
IMPLEMENT_SERIAL( GrabDocumentInfo, CObject, 1)


void GrabDocumentInfo::Serialize(CArchive& ar)
{
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		ar << _selectedGrabIndex;
	}
	else
	{
		ar >> version;
		ar >> _selectedGrabIndex;
	}
}

int GrabDocumentInfo::getSelectedGrabIndex()
{
	return _selectedGrabIndex;
}