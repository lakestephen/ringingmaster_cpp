// AllTheWorkBell.h: interface for the AllTheWorkBell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLTHEWORKBELL_H__9CB7C740_DB64_11D5_BB7F_EF854CD42E6E__INCLUDED_)
#define AFX_ALLTHEWORKBELL_H__9CB7C740_DB64_11D5_BB7F_EF854CD42E6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Row;

class AllTheWorkBell  
{
public:
	void addRow(Row* row, int splicePos = 0);
	int getPositionCount(int bell, int start, int splicePos);
	int getSpliceCount();
	int getNumber();
	void incrementPosition(int bell, int start, int splicePos);
	AllTheWorkBell(int number, int spliceCount);
	virtual ~AllTheWorkBell();

protected:
	int getIndex(int bell, int start, int splicePos);
	const int _number;
	const int _spliceCount;
	int* _data;

	int getDataSize();

};

#endif // !defined(AFX_ALLTHEWORKBELL_H__9CB7C740_DB64_11D5_BB7F_EF854CD42E6E__INCLUDED_)
