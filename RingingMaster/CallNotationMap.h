
#pragma once


class Notation;


class CallNotationMap : public CMap<CString, LPCSTR, Notation*, Notation*>   
{
public:
	CallNotationMap(Notation* notation, bool onlyLonghand = false);
	virtual ~CallNotationMap();

};

