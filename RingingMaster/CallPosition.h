// allPosDefinition.h: interface for the CallPosDefinition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLPOSDEFINITION_H__9F8D60C8_A3D2_11D5_BB7F_FC6C81823874__INCLUDED_)
#define AFX_ALLPOSDEFINITION_H__9F8D60C8_A3D2_11D5_BB7F_FC6C81823874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Notation;

class CallPosition
{
public:
	CallPosition(int place, int afterRow);
	virtual ~CallPosition();

	int getPlace() {return _place;}
	int getAfterRow() {return _afterRow;}

protected:
	int _place;
	int _afterRow;
};

class CallPositionMap :public CMap<CString, LPCSTR, CallPosition*, CallPosition*>
{
public:
	CallPositionMap(Notation* notation, int tenor);
	virtual ~CallPositionMap();
};


#endif // !defined(AFX_ALLPOSDEFINITION_H__9F8D60C8_A3D2_11D5_BB7F_FC6C81823874__INCLUDED_)
