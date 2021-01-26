
#include "stdafx.h"
#include "DoubleSelectItem.h"



DoubleSelectItem::DoubleSelectItem() :
_type(dst_greater),
_double1(0),
_double2(0)
{
}

void DoubleSelectItem::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;

		ar << _type;
		ar << _double1;
		ar << _double2;
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
		_type = static_cast<DoubleSelectType>(type);

		ar >> _double1;
		ar >> _double2;
	}
}