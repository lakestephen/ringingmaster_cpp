// ListSelectItem.cpp: implementation of the ListSelectItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListSelectItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ListSelectItem::ListSelectItem(long ref, CString text) :
_ref(ref),
_text(text)
{

}

ListSelectItem::ListSelectItem() :
_ref(0),
_text("")
{

}

ListSelectItem::~ListSelectItem()
{

}

void ListSelectItem::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _ref;
		ar << _text;
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		ar >> _ref;
		ar >> _text;
	}
}