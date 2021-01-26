#pragma once


class Row;

enum MusicRuleType
{
	MRT_USER,
	MRT_MUSICAL_CHANGES,
	MRT_CRU
};

const int DontCare = 99;

class MusicRule
{
public:
	void clearDown();
	int getNumber();
	void setUseStyleColor(BOOL useStyleColor);
	BOOL getUseStyleColor();

	void setColor(COLORREF color);
	COLORREF getColor();
	
	CString getName();
	void setName(CString name);

	void setHand(BOOL hand);
	BOOL getHand();

	void setBack(BOOL back);
	BOOL getBack();
	
	MusicRuleType getType();
	void setType(MusicRuleType type);

	CString getDisplayString(bool includeBothStrokes = true);

	void Serialize(CArchive &ar);
	MusicRule(int number);
	MusicRule(MusicRule& musicRule);

	virtual ~MusicRule();

	void deserialize_v1(CArchive& ar);
	void deserialize_v2(CArchive& ar);

	static CString checkRule(int number, CString ruleStr);

	CString getRule();
	bool setRule(CString str);

	bool operator ==(const Row & row);


protected:
	CString _name;

	BOOL _handStroke;
	BOOL _backStroke;
	BOOL _active;
	BOOL _useStyleColor;
	COLORREF _color;
	MusicRuleType _type;
	int _number;
	Intss _data;	

};

typedef CArray<MusicRule*, MusicRule*> MusicRules;
