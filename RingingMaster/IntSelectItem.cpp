#include "stdafx.h"
#include "IntSelectItem.h"



IntSelectItem::IntSelectItem() :
_type(ist_greater),
_int1(0),
_int2(0)
{ 
}

void IntSelectItem::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;

		ar << _type;
		ar << _int1;
		ar << _int2;
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		int type;
		ar >> type;
		_type = static_cast<IntSelectType>(type);

		ar >> _int1;
		ar >> _int2;
	}
}