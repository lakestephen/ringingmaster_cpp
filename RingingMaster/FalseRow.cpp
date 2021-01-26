// FalseRow.cpp: implementation of the FalseRow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "FalseRow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FalseRow::FalseRow(Row * row) :
Row(*row),
_multiBlockTrue(false)
{

}

FalseRow::FalseRow(MultiBlockFalseRow& multiBlockFalseRow) :   
Row(multiBlockFalseRow),
_multiBlockTrue(multiBlockFalseRow.isMultiBlockTrue())
{
	for (int i=0;i<multiBlockFalseRow.GetSize();i++)
	{
		MultiBlockFalseRowPair* mbFalseRow = multiBlockFalseRow.GetAt(i);
		RowPosition * rowPosition = new RowPosition(*mbFalseRow);
		Add(rowPosition);
	}
}
  
FalseRow::~FalseRow()
{
	for (int i=0;i<GetSize();i++)
		delete GetAt(i);
}

MultiBlockFalseRow::MultiBlockFalseRow(Row * row) :
Row(*row),
_multiBlockTrue(false)
{

}

MultiBlockFalseRowPair::MultiBlockFalseRowPair(int sortedIndex) :
_sortedIndex(sortedIndex)
{
}

MultiBlockFalseRow::~MultiBlockFalseRow() 
{
	for (int i=0;i<GetSize();i++)
		delete GetAt(i);
}

void MultiBlockFalseRow::setMultiBlockTrue()
{
	_multiBlockTrue = true;;
}

bool MultiBlockFalseRow::isMultiBlockTrue()
{
	return _multiBlockTrue;
}

bool FalseRow::isMultiBlockTrue()
{
	return _multiBlockTrue;
}



int MultiBlockFalseRowPair::getSortedIndex()
{
	return _sortedIndex;
}

CString FalseRow::getTipText(RowPosition& rowPos, bool common)
{
	if ((!rowPos.isValid()))
		return "";

	CString str;			 
	str.Format("%s is %s with ", rowPos.format(rpft_full), common?"common":"false");

	int writtenIndex = 0;
	for (int i=0;i<GetSize();i++)
	{
		if (*GetAt(i) != rowPos)
		{
			if (writtenIndex != 0)
				str += ", ";
			str += GetAt(i)->format(rpft_short);
			writtenIndex++;
		}
	}
	
	return str;
}	 

