#pragma once

class ListSelectItem  
{
public:
	ListSelectItem();
	ListSelectItem(long ref, CString text);
	virtual ~ListSelectItem();

	void Serialize(CArchive& ar);

	CString _text;
	long _ref;
};

typedef CArray <ListSelectItem*, ListSelectItem*> ListSelectItems;

