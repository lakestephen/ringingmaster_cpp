#pragma once

#include "Row.h"
#include "RowPosition.h"
#include "RowComment.h"

class Method;

class RowExtra : public Row, public RowPosition, public RowComments
{
public:
	RowExtra(const Row& row, int leadLength) :
		Row(row), 
		_leadLength(leadLength)
	{

	}

	virtual ~RowExtra()
	{
		for (int i=0;i<GetSize();i++)
			delete GetAt(i);
		
		RemoveAll();
	}

	int _leadLength;
	
};

typedef CArray<RowExtra*, RowExtra*> RowExtras;
typedef CList<RowExtra*, RowExtra*> RowExtraList;

class RowHolder :
	public RowExtras
{
public:
	RowHolder(const Method &method);
	RowHolder(int number);
	virtual ~RowHolder(void);

	int getNumber() const;
protected:
	int _number;
};
