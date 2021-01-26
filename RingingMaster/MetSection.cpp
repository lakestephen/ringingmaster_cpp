// MetSection.cpp: implementation of the MetSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Extract methods.h"
#include "MetSection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MetSection::MetSection() :
_number(0)
{

}

MetSection::~MetSection()
{

	for(int i=0;i<_mets.GetSize();i++)
		delete _mets.GetAt(i);
}

CString MetSection::getStr(int index, CString token)
{
	//find the column
	if (index >= _mets.GetSize())
	{
		ASSERT(false);
		return "";
	}

	for (int i=0;i<_columnNames.GetSize();i++)
	{
		if (_columnNames.GetAt(i) == token)
			return _mets.GetAt(index)->_strs.GetAt(i); 
	}
	return "";
}

int MetSection::getNumber()
{
	return _number;
}

bool MetSection::init()
{	
	//remove method
	_type.Replace("methods", "");
	_type.Replace("Unnamed", "");
	
	//deal with everything in lower case. Conversion will happen when we add directorys
	_type.MakeLower();
	
	//remove spare spaces
	while(_type.Replace("  ", " "));

	//identify type
	CStringArray types;
	types.Add("alliance");
	types.Add("delight");
	types.Add("differentials");
	types.Add("hybrid");
	types.Add("surprise");
	types.Add("principles");
	types.Add("treble bob");
	types.Add("treble place");
	types.Add("single-hunt");
	types.Add("twin-hunt");
	types.Add("triple-hunt");

	for (int j=0;j<types.GetSize();j++)
	{
		if (_type.Find(types.GetAt(j)) != -1)
		{
			_strSectionType = types.GetAt(j);
			_type.Replace(types.GetAt(j), "");
			break;
		}
	}

	CStringArray suffexTypes;
	suffexTypes.Add("alliance");
	suffexTypes.Add("delight");
	suffexTypes.Add("surprise");
	suffexTypes.Add("treble bob");
	suffexTypes.Add("treble place");

	for (int k=0;k<suffexTypes.GetSize();k++)
	{
		if (_strSectionType == suffexTypes.GetAt(k))
		{
			_strSuffex = _strSectionType;
		}
	}

	//remove spare spaces
	while(_type.Replace("  ", " "));

	//get the number
	for (int i=MAXBELLS;i>=3;i--)
	{
		CString numberName = GlobalFunctions::numberName(i);
		numberName.MakeLower();
		if (_type.Find(numberName) != -1)
		{
			_number = i;
			_type.Replace(numberName, "");
			while(_type.Replace("  ", " "));
			return true;
		}
	}	

	//should not get here
	ASSERT(FALSE);
	return false;
}

CString MetSection::getSectionType()
{
	return _strSectionType;
}

void MetSection::setStr(int index, CString token, CString value)
{
	//find the column
	if (index >= _mets.GetSize())
	{
		ASSERT(false);
		return ;
	}

	for (int i=0;i<_columnNames.GetSize();i++)
	{
		if (_columnNames.GetAt(i) == token)
		{
			_mets.GetAt(index)->_strs.SetAt(i, value); 
			return;
		}

	}
	ASSERT(false);
}
