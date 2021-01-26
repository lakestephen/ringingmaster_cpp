#include "stdafx.h"
#include "TextSelectItem.h"


TextSelectItem::TextSelectItem() :
_type(sst_contains),
_caseSensitive(FALSE)
{
}

void TextSelectItem::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;

		ar << _type;
		ar << _string;
		ar << _caseSensitive;
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
		_type = static_cast<StringSearchType>(type);
		ar >> _string;
		ar >> _caseSensitive;  
	}
}

bool TextSelectItem::isValid(CString testString)
{
	CString string = _string;

	if (!_caseSensitive)
	{
		testString.MakeLower();
		string.MakeLower();
	}

	bool keep = false;

	switch (_type)
	{
		case sst_startsWith:
			keep = (testString.Find(string) == 0);
			break;
		case sst_endsWith:
			{
				int pos = testString.Find(string);
				keep = ((pos != -1)&&(testString.GetLength() == pos + string.GetLength()));
			}
			break;
		case sst_contains:
			keep = (testString.Find(string) != -1);
			break;
		case sst_empty:
			keep = testString.IsEmpty();
			break;    			
		case sst_not_empty:
			keep = !testString.IsEmpty();
			break;    			
		default:
		case sst_exact:
			keep = (testString == string);
			break;
	}

	return keep;
}