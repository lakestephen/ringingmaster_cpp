#include "StdAfx.h"
#include "DateTimeSelectItem.h"


DateTimeSelectItem::DateTimeSelectItem() :
_type(dtst_greater),
_format(dtsft_date_time)
{
}


void DateTimeSelectItem::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;

		ar << _type;
		ar << _format;
		ar << _dateTime1;
		ar << _dateTime2;
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
		_type = static_cast<DateTimeSelectType>(type);
		int format;
		ar >> format;
		_format = static_cast<DateTimeSelectFormatType>(format);

		ar >> _dateTime1;
		ar >> _dateTime2;
	}
}

