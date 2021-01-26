// NotationCallPosAgregate.cpp: implementation of the NotationCallPosAgregate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationCallPosAgregate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NotationCallPosAgregate::NotationCallPosAgregate() :
_name("")
{
	_callPosMethods.RemoveAll();
}

NotationCallPosAgregate::NotationCallPosAgregate(NotationCallPosAgregate & notationCallPosAgregate) :
_name(notationCallPosAgregate._name)
{
	for (int i=0;i<notationCallPosAgregate._callPosMethods.GetSize();i++)
	{
		_callPosMethods.Add(notationCallPosAgregate._callPosMethods.GetAt(i));
	}
}

NotationCallPosAgregate::~NotationCallPosAgregate()
{

}

NotationCallPosAgregate& NotationCallPosAgregate::operator=(NotationCallPosAgregate& NotationCallPosAgregate)
{
	_name = NotationCallPosAgregate._name;

	_callPosMethods.RemoveAll();
	for (int i=0;i<NotationCallPosAgregate._callPosMethods.GetSize();i++)
	{
		_callPosMethods.Add(NotationCallPosAgregate._callPosMethods.GetAt(i));
	}

	return *this;
}

void NotationCallPosAgregate::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _name;
		ar << _callPosMethods.GetSize();
		for (int i=0;i<_callPosMethods.GetSize();i++)
			ar << _callPosMethods.GetAt(i);
	}
	else
	{
		ar >> version;
		ar >> _name;
		int size = 0;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			CString str;
			ar >> str;
			_callPosMethods.Add(str);
		} 
	}
}

CString NotationCallPosAgregate::getDisplay() const
{
	CString str = "call in: ";
	for (int i=0;i<_callPosMethods.GetSize();i++)
	{
		str += _callPosMethods.GetAt(i);
		if (i != _callPosMethods.GetUpperBound())
			str += ", "; 
	}
	return str;
}
