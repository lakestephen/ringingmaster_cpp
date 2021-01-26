#pragma once


enum DoubleSelectType
{
	dst_greater,
	dst_less,
	dst_between,
	dst_exact
};

class DoubleSelectItem
{
	public:
	DoubleSelectItem();

	void Serialize(CArchive& ar);

	DoubleSelectType _type;
	double _double1;
	double _double2;
};

