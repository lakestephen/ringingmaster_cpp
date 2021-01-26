// MusicRule.cpp: implementation of the MusicRule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MusicRule.h"
#include "rOW.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MusicRule::MusicRule(int number) :
_handStroke(TRUE),
_backStroke(TRUE),
_type(MRT_USER),
_active(TRUE),
_useStyleColor(TRUE),
_color(ULONG_MAX),
_number(number)
{
	for (int i=0;i<MAXBELLS;i++)
		_data.Add(new Ints); 
}

MusicRule::MusicRule(MusicRule& musicRule) :
_handStroke(musicRule._handStroke),
_backStroke(musicRule._backStroke),
_type(musicRule._type),
_active(musicRule._active),
_name(musicRule._name),
_useStyleColor(musicRule._useStyleColor),
_color(musicRule._color),
_number(musicRule._number)
{
	for (int i=0;i<MAXBELLS;i++)
		_data.Add(new Ints); 

	setRule(musicRule.getRule());
}

MusicRule::~MusicRule()
{
	for (int i=0;i<_data.GetSize();i++)
		delete _data.GetAt(i);
}




void MusicRule::Serialize(CArchive &ar)
{
	int version = 2;
	if(ar.IsStoring())
	{
		ar << version;
		CString rule = getRule();
		ar << rule;
		ar << _name;
		ar << _handStroke;
		ar << _backStroke;
		ar << _type;
		ar << _active;
		ar << _useStyleColor;
		ar << _color;
	}
	else
	{
		ar >> version;

		TRACE("Deserialising Music Check Version %d \r\n", version);

		if (version == 1)
			deserialize_v1(ar);
		else if (version == 2)
			deserialize_v2(ar);
		else
			AfxThrowArchiveException(CArchiveException::endOfFile);	   

	}
}

//version 2 is version 1.08 onwards
void MusicRule::deserialize_v2(CArchive& ar) 
{
	CString rule;
	ar >> rule;
	setRule(rule);
	ar >> _name;
	ar >> _handStroke;
	ar >> _backStroke;
	int temp;
	ar >> temp;
	_type = static_cast<MusicRuleType>(temp);
	ar >> _active;
	ar >> _useStyleColor;
	ar >> _color;
}


//version 1 is version 1.07 onwards
void MusicRule::deserialize_v1(CArchive& ar) 
{
	CString rule;
	ar >> rule;
	setRule(rule);

	ar >> _name;
	ar >> _handStroke;
	ar >> _backStroke;
	int temp;
	ar >> temp;
	_type = static_cast<MusicRuleType>(temp);
	ar >> _active;
}


CString MusicRule::getDisplayString(bool includeBothStrokes)
{
	CString str = getRule();
	if ((_backStroke)&&(_handStroke))
		str += "";
	else if ((_backStroke)&&(!_handStroke))
		str += " at backstroke";
	else if ((!_backStroke)&&(_handStroke))
		str += " at handstroke";
	else if ((_backStroke)&&(_handStroke)&&(includeBothStrokes))
		str += " at both strokes";

	return str;			
}


void MusicRule::setType(MusicRuleType type)
{
	_type = type;
}

MusicRuleType MusicRule::getType()
{
	return _type;
}

BOOL MusicRule::getBack()
{
	return _backStroke;

}

BOOL MusicRule::getHand()
{
	return _handStroke;
}

void MusicRule::setBack(BOOL back)
{
	_backStroke = back;
}

void MusicRule::setHand(BOOL hand)
{
	_handStroke = hand;
}

   
//will perform a check of the format 
CString MusicRule::checkRule(int number, CString ruleStr)
{
	MusicRule check(number);
	check.setRule(ruleStr);
	return check.getRule();
}	  	  

void MusicRule::setName(CString name)
{
	_name = name;
}

CString MusicRule::getName()
{
	return _name;
}

COLORREF MusicRule::getColor()
{
	return _color;

}

void MusicRule::setColor(COLORREF color)
{
	_color = color;
}

BOOL MusicRule::getUseStyleColor()
{
	return _useStyleColor;
}

void MusicRule::setUseStyleColor(BOOL useStyleColor)
{
	_useStyleColor = useStyleColor;
}



bool MusicRule::setRule(CString str)
{
	clearDown();
	
	while(str.Remove(' '));
	
	int place = 0;
	bool inBrace = false;

	for (int i=0;i<str.GetLength();i++)
	{
		if (place >= _number)
		{
			break;
		}
		else if (str.GetAt(i) == '?') 
		{
			_data.GetAt(place)->Add(DontCare);
			place++;
		}
		else if (str.GetAt(i) == '(') 
		{
			inBrace = true;
		}
		else if (str.GetAt(i) == ')') 
		{
			inBrace = false;
			place++;
		}
		else
		{
			int bellNum = GlobalFunctions::charToBellNumbers(str.GetAt(i));
			if ((bellNum != BADDATA)&&(bellNum <= _number))
				_data.GetAt(place)->Add(bellNum);
			else 
				_data.GetAt(place)->Add(DontCare);

			if (!inBrace)
				place++;
		}
	}
	//fill with dont knows 
	for (int j=place;j<_number;j++)
		_data.GetAt(j)->Add(DontCare);

	return true;
}


bool MusicRule::operator ==(const Row & row)
{
	if (_number != row._number)
		return false;

	Ints* ints;
	bool check;

	if ((row.getStroke() == hand)&&(!_handStroke))
		return false;

	if ((row.getStroke() == back)&&(!_backStroke))
		return false;

	for (int i=0;i<_number;i++)
	{
		ints = _data.GetAt(i);
		check = false;
		for (int j=0;j<ints->GetSize();j++)
		{
			if((ints->GetAt(j) == DontCare)||
			   (ints->GetAt(j) == row.getBellInPlace(i)))
			{
				check = true;
				break;
			}
		}
		if (check != true) 
			return false;

	}
	return true;		
}

CString MusicRule::getRule()
{
	CString str;
	Ints* ints;

	for (int i=0;i<_number;i++)
	{
		ints = _data.GetAt(i);
		if (ints->GetSize() > 1)
			str +="(";
		if (ints->GetSize() < 1)
			str +="?";
		for (int j=0;j<ints->GetSize();j++)
		{
			if (ints->GetAt(j) == DontCare)
				str+= "?";
			else
				str += GlobalFunctions::bellNumbersToChar(ints->GetAt(j));			 
		}
		if (ints->GetSize() > 1)
			str +=")";
	}
	return str;
}

int MusicRule::getNumber()
{
	return _number;
}

void MusicRule::clearDown()
{
	for (int i=0;i<MAXBELLS;i++)
	{
		_data.GetAt(i)->RemoveAll();		
	}
}
