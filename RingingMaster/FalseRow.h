#pragma once

#include "StdAfx.h"
#include "Row.h"
#include "RowPosition.h"


class MultiBlockFalseRowPair : public RowPosition
{
public:
	int getSortedIndex();

	MultiBlockFalseRowPair(int sortedIndex);

protected:
	int _sortedIndex;
};

typedef CArray<MultiBlockFalseRowPair*,MultiBlockFalseRowPair*> MultiBlockFalseRowPairs;

class MultiBlockFalseRow : public MultiBlockFalseRowPairs, public Row
{
public:
	MultiBlockFalseRow(Row* row);
	virtual ~MultiBlockFalseRow();
	void setMultiBlockTrue();
	bool isMultiBlockTrue();

protected:
	bool _multiBlockTrue;	
};


class FalseRow : public RowPositions, public Row
{
public:
	bool isMultiBlockTrue();
	FalseRow(Row* row);
	FalseRow(MultiBlockFalseRow& multiBlockFalseRow);
	virtual ~FalseRow();
	CString getTipText(RowPosition& rowPos, bool common);
protected:
	bool _multiBlockTrue;	
};

