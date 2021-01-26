#pragma once


enum IntSelectType
{
	ist_greater,
	ist_less,
	ist_between,
	ist_exact, 
};

class IntSelectItem
{
	public:
	IntSelectItem();

	void Serialize(CArchive& ar);

	IntSelectType _type;
	int _int1;
	int _int2;
};

