// Row.cpp: implementation of the Row class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Row.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//default constructor
Row::Row(int number, Stroke stroke, CString rowText) :
_number(number),
_false(false),
_common(false),
_musicResult(NULL),
_stroke(stroke),
_rowIndex(0),
_inCourse(true), 
_leadLine(false)
{
	if (_number == -1)
		_number = 0;

	_data = new short[_number];
	if (rowText.GetLength() == _number)
	{
		CString msg;
		setAsText(rowText, msg);
	}
	else
		setRounds();
} 

//copy constructor
Row::Row(const Row & row) :
_data(NULL),
_common(false),
_false(false),
_musicResult(NULL),
_inCourse(true), 
_leadLine(false)
{
	operator = (row);
}

//swap constructor
Row::Row(Row & row, Ints * intArr) :
_false(false),
_musicResult(NULL),
_common(false), 
_leadLine(false)
{
	_number = row._number;
	_rowIndex = row._rowIndex + 1;

	_data = new short[_number];
	
	if (row._stroke == hand) 
		_stroke = back;
	else
		_stroke = hand;

	for (int i=0;i<_number;i++)
	{
		if (contains(intArr, i+1))
		{
			getBellInPlace(i) = row.getBellInPlace(i);
		}
		else
		{
			getBellInPlace(i)   = row.getBellInPlace(i+1);
			getBellInPlace(i+1) = row.getBellInPlace(i);
			i++;
		}
	}
}

bool Row::contains(Ints *intArr, int place)
{	
	for (int i=0;i<intArr->GetSize();i++)
	{
		if (intArr->GetAt(i) == place)
			return true;
	}
	return false; 
}

Row::~Row()
{
	delete [] _data;
	_data = NULL;
}

Row& Row::operator =(const Row& row)
{
	//assign
	_number = row._number;
	//tidy
	delete _data;
	_data = NULL;
	//create
	_data = new short[_number];
	for (int i=0;i<_number;i++)
		_data[i] = row._data[i];

	_stroke = row._stroke;

	_rowIndex = row._rowIndex;

	_inCourse = row._inCourse;

	return *this;
}

short & Row::getBellInPlace(int place) const
{
	ASSERT(place < _number); 
	return _data[place];
}

int Row::getPositionOfBell(int bell) const 
{
	ASSERT(bell <= _number); 
	for (int i=0;i<_number;i++)
		if (_data[i] == bell)
			return i;
	return -1;
}

//_stroke or _rowIndex should not take part in the equality test 
bool Row::operator ==(const Row & row)
{
	if (_number != row._number)
		return false;
	for (int i=0;i<_number;i++)
	{
		if (row.getBellInPlace(i) != getBellInPlace(i))
			return false;
	}

	//dont need to check in course, as it is derived from the row
	return true;		
}

bool Row::operator !=(const Row & row)
{
	return !(operator == (row));
}

void Row::setRounds()
{
	for (int i=0;i<_number;i++)
	{
		getBellInPlace(i) = static_cast<short>(i+1);
	}	   
}

CString Row::getAsText() const
{
	CString str;
	for (int i=0;i<_number;i++)
		str += GlobalFunctions::bellNumbersToChar(getBellInPlace(i));

	return str;
}

CString Row::getAsTextDiff(Row* prevRow)
{
	CString thisStr = getAsText();

	if (prevRow == 0)
		return thisStr;

	CString prevStr = prevRow->getAsText();
	
	if (prevStr.GetLength() != thisStr.GetLength())
		return thisStr;

		for (int i=0;i<thisStr.GetLength();i++)
			if (prevStr.GetAt(i) == thisStr.GetAt(i))
				thisStr.SetAt(i,' ');

	return thisStr;
}

bool Row::setFalse()
{
	bool retVal = _false;
	_false = true;
	return retVal;
}

bool Row::setLeadLine()
{
	bool retVal = _leadLine;
	_leadLine = true;
	return retVal;
}

bool Row::setMusic(MusicResult* musicResult)
{
	bool retVal = (_musicResult != NULL);
	_musicResult = musicResult;
	return retVal;
}

bool Row::setCommon()
{
	bool retVal = _common;
	_common = true;
	return retVal;
}

bool Row::setAsText(CString str, CString& msg, bool forceTenor)
{
	while(str.Remove(' '));
	
	msg.Empty();

	if (str.GetLength() != _number)
	{
		msg.Format("Change is the wrong length.");
		return false;
	}

	//check all are valid
	int i;
	for (i=0;i<_number;i++)
	{
		int bellNo = GlobalFunctions::charToBellNumbers(str.GetAt(i));
		if ((bellNo < 0)||(bellNo > _number))
		{
			msg.Format("%c is not a valid bell number.", str.GetAt(i));
			return false;
		}

	}

	//check every number is used once and only once
	//create an array with all numbers required
	Ints allNums;
	for(i=1;i<=_number;i++)
		allNums.Add(i);
	//now remove them again
	for(i=0;i<_number;i++)
	{
		int bellNo = GlobalFunctions::charToBellNumbers(str.GetAt(i));
		//try and find in the array.
		for (int j=0;j<allNums.GetSize();j++)
		{
			if (allNums.GetAt(j) == bellNo)
			{
				allNums.RemoveAt(j);
				break;
			}
		}
	}

	//some numbers have not been used
	if (allNums.GetSize() != 0)
	{
		for (int j=0;j<allNums.GetSize();j++)
		{
			CString tmp;
			tmp.Format("%c ", GlobalFunctions::bellNumbersToChar(allNums.GetAt(j)));
			msg += tmp;
		}
		msg += " missing.";
		return false;
	}

	if (forceTenor)
	{
		if (str.GetLength() == _number)
		{
			CString char1(str.GetAt(_number-1));
			char1.MakeUpper();
			if(char1 != GlobalFunctions::bellNumbersToChar(_number))
			{
				msg = "Tenor is out of position.";
				return false;
			}
		}
	}

	//we should be ok here
	for (i=0;i<_number;i++)
	{
		int bellNo = GlobalFunctions::charToBellNumbers(str.GetAt(i));
		_data[i] = static_cast<short>(bellNo);		
	}
	
	return true;
}

MusicResult* Row::getMusicResult()
{
	return _musicResult;
}

void Row::calculateInCourse()
{
	static short copy[MAXBELLS] ;
	static short temp;

//	TRACE(getAsText() + "\r\n");
	
	//copy to static
	for (int i=0;i<_number;i++)
		copy[i] = _data[i];

	_inCourse = true;

	for (int i=0;i<_number;i++)
	{
		if (copy[i] != i+1)
		{
			//swap
			_inCourse = !_inCourse;
			for (int j=i;j<_number;j++)
			{
				if (copy[j] == i+1)
				{
					temp = copy[i];
					copy[i] = copy[j];
					copy[j] = temp;
					break;
				}
			}
		}
	}

//	TRACE((_inCourse?"+":"-") + getAsText() + "\r\n");

}

bool Row::getIsInCourse()
{
	return _inCourse;
}


//used for swapping rows in called changes
void Row::swap(int firstIndex)
{
	ASSERT(firstIndex < _number-1); 
	ASSERT(firstIndex >= 0); 
	
	short temp = _data[firstIndex];
	_data[firstIndex] = _data[firstIndex + 1];
	_data[firstIndex + 1] = temp;
}

bool Row::isRounds()
{
	for (int i=0;i<_number;i++)
	{
		if (getBellInPlace(i) != static_cast<short>(i+1))
		   return false;
	}	
	return true;
}
