// TouchBlock.h: interface for the TouchBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOUCHBLOCK_H__8D47C2F8_6946_11D5_B5F7_009027BB3286__INCLUDED_)
#define AFX_TOUCHBLOCK_H__8D47C2F8_6946_11D5_B5F7_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TouchBlock
{
public:
	void Serialize(CArchive& ar);
	TouchBlock (int start, int end, CString name) :
	_start(start),
	_end(end),
	_name(name)
	{ }

	TouchBlock () :
	_start(-1),
	_end(-1),
	_name("")
	{ }


	int _start;
	int _end;
	CString _name;
};

typedef CArray<TouchBlock*, TouchBlock*> TouchBlockArray;

#endif // !defined(AFX_TOUCHBLOCK_H__8D47C2F8_6946_11D5_B5F7_009027BB3286__INCLUDED_)
