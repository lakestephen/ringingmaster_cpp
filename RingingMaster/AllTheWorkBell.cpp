// AllTheWorkBell.cpp: implementation of the AllTheWorkBell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "AllTheWorkBell.h"
#include "Row.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AllTheWorkBell::AllTheWorkBell(int number, int spliceCount) :
_number(number),
_spliceCount(spliceCount)
{
	ASSERT(number > 0);
	ASSERT(spliceCount > 0);
	//create the data structure to hold the information
	_data = new int[getDataSize()];

	for (int i=0;i<getDataSize();i++)
		_data[i] = 0;
}

AllTheWorkBell::~AllTheWorkBell()
{
	delete [] _data;
}

void AllTheWorkBell::incrementPosition(int bell, int start, int splicePos)
{
	_data[getIndex(bell, start, splicePos)]++;
}

int AllTheWorkBell::getDataSize()
{
	return _number*_number*_spliceCount;
}

//bell is 0 based
//start is 0 based
//splice is 0 based

int AllTheWorkBell::getIndex(int bell, int start, int splicePos)
{
	
	int bellOffset = (_number * _spliceCount) * (bell);
	int startOffset = (_spliceCount) * (start);
	int index = bellOffset + startOffset + splicePos;
	ASSERT(index < getDataSize());
	return index;
}

int AllTheWorkBell::getNumber()
{
	return _number;
}

int AllTheWorkBell::getSpliceCount()
{
	return _spliceCount;
}

int AllTheWorkBell::getPositionCount(int bell, int start, int splicePos)
{
	return _data[getIndex(bell, start, splicePos)];
}

void AllTheWorkBell::addRow(Row *row, int splicePos)
{
	ASSERT(row->_number == _number);
	for (int i=0;i<_number;i++)
	{
		incrementPosition(row->getBellInPlace(i)-1, i, splicePos);
	}															
}
