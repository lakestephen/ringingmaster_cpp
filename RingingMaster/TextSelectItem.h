 
#pragma once

enum StringSearchType 
{
	sst_contains,
	sst_startsWith,
	sst_endsWith,
	sst_exact,
	sst_empty,
	sst_not_empty,
};

class TextSelectItem
{
public:
	TextSelectItem();
	
	bool isValid(CString testString);

	virtual void Serialize(CArchive& ar);

	StringSearchType _type;
	CString _string;
	BOOL _caseSensitive;
};	