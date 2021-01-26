// NotationPersist.cpp: implementation of the NotationPersist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "RingingMaster.h"

#include "NotationPersist.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction


NotationPersist::NotationPersist() :
_folder(0), 
_number(8), 
_name(""),
_symmetric(TRUE),
_notation(""),
_leadEnd(""),	
_leadHead(""),
_spliceLetter(""),
_notes(""),
_standardCall("-"),
_defaultCalls(TRUE),
_defaultCallPosLead(TRUE),
_defaultCallPosMethod(TRUE),
_defaultLeadLines(TRUE),
_defaultSplice(TRUE),
_defaultCourseHeadPos(TRUE),
_call(FALSE)
{
}



NotationPersist::NotationPersist(const NotationPersist & notationPersist) :
_folder(notationPersist._folder), 
_call(notationPersist._call), //this was TRUE - causing problems with the creationof the methods from the wizard
_number(notationPersist._number), //just a valid number
_name(notationPersist._name),
_symmetric(notationPersist._symmetric),
_notation(notationPersist._notation),
_leadEnd(notationPersist._leadEnd),	
_leadHead(notationPersist._leadHead),
_spliceLetter(notationPersist._spliceLetter),
_notes(notationPersist._notes),
_defaultCalls(notationPersist._defaultCalls),
_defaultCallPosLead(notationPersist._defaultCallPosLead),
_defaultCallPosMethod(notationPersist._defaultCallPosMethod),
_defaultSplice(notationPersist._defaultSplice),
_defaultCourseHeadPos(notationPersist._defaultCourseHeadPos),
_defaultLeadLines(notationPersist._defaultLeadLines),
_standardCall(notationPersist._standardCall),
_courseHeadPos(notationPersist._courseHeadPos)
{
	int i;
	for (i=0;i<notationPersist._calls.GetSize();i++)
	{
		_calls.Add(new NotationCall(*notationPersist._calls[i]));
	}

	for (i=0;i<notationPersist._callPosLead.GetSize();i++)
	{
		_callPosLead.Add(notationPersist._callPosLead[i]);
	}

	for (i=0;i<notationPersist._callPosMethod.GetSize();i++)
	{
		_callPosMethod.Add(new NotationCallPosMethod(*notationPersist._callPosMethod[i]));
	}

	for (i=0;i<notationPersist._callPosAgregate.GetSize();i++)
	{
		_callPosAgregate.Add(new NotationCallPosAgregate(*notationPersist._callPosAgregate[i]));
	}					 

	for (i=0;i<notationPersist._splice.GetSize();i++)
	{
		_splice.Add(notationPersist._splice[i]);
	}

	for (i=0;i<notationPersist._leadLines.GetSize();i++)
	{
		_leadLines.Add(notationPersist._leadLines[i]);
	}

	for (i=0;i<notationPersist._info.GetSize();i++)
	{
		_info.Add(new NotationCCInfo(*notationPersist._info[i]));	
	}
}

NotationPersist::~NotationPersist()
{
	//remove all dynamic alloc data
	int i;
	
	for (i=0;i<_calls.GetSize();i++)		
		delete _calls.GetAt(i);
	_calls.RemoveAll();
	
	for (i=0;i<_callPosMethod.GetSize();i++)	
		delete _callPosMethod.GetAt(i);
	_callPosMethod.RemoveAll();
	
	for (i=0;i<_callPosAgregate.GetSize();i++)	
		delete _callPosAgregate.GetAt(i);
	_callPosAgregate.RemoveAll();

	for (i=0;i<_info.GetSize();i++)			
		delete _info.GetAt(i);
	_info.RemoveAll();

	_callPosLead.RemoveAll();
	_splice.RemoveAll();
	_leadLines.RemoveAll();
}

//set back to the default state
void NotationPersist::init()
{
	_folder = 0;
	_number = 3;
	if(!_name.IsEmpty())
		_name.Empty();
	_symmetric = TRUE;
	if(!_notation.IsEmpty())
		_notation.Empty();
	if(!_leadEnd.IsEmpty())
		_leadEnd.Empty();
	_leadHead.Empty();
	_spliceLetter.Empty();
	_notes.Empty();
	_defaultCalls = TRUE;
	_defaultCallPosLead = TRUE;
	_defaultCallPosMethod = TRUE;
	_defaultSplice = TRUE;
	_defaultLeadLines = TRUE;
	_defaultCourseHeadPos = TRUE;
	_call = TRUE;
	_standardCall = "-";

	int i;
	for (i=0;i<_calls.GetSize();i++) 
		delete _calls.GetAt(i);
	_calls.RemoveAll();
	
	_callPosLead.RemoveAll();


	for (i=0;i<_callPosMethod.GetSize();i++) 
		delete _callPosMethod.GetAt(i);
	_callPosMethod.RemoveAll();

	for (i=0;i<_callPosAgregate.GetSize();i++)	
		delete _callPosAgregate.GetAt(i);
	_callPosAgregate.RemoveAll();

	_splice.RemoveAll();
	_leadLines.RemoveAll();

	for (i=0;i<_info.GetSize();i++) 
		delete _info.GetAt(i);
	_info.RemoveAll();
}

void NotationPersist::operator =(const NotationPersist &notationPersist)
{
	init();
	
	_folder = notationPersist._folder;
	_number = notationPersist._number;
	_name = notationPersist._name;
	_symmetric = notationPersist._symmetric;
	_notation = notationPersist._notation;
	_leadEnd = notationPersist._leadEnd;
	_leadHead = notationPersist._leadHead;
	_spliceLetter = notationPersist._spliceLetter;
	_notes = notationPersist._notes;
	_defaultCalls = notationPersist._defaultCalls;
	_defaultCourseHeadPos = notationPersist._defaultCourseHeadPos;
	_defaultCallPosLead = notationPersist._defaultCallPosLead;
	_defaultCallPosMethod = notationPersist._defaultCallPosMethod;
	_defaultSplice = notationPersist._defaultSplice;
	_defaultLeadLines = notationPersist._defaultLeadLines;
	_call = notationPersist._call;
	_standardCall = notationPersist._standardCall;
	_courseHeadPos = notationPersist._courseHeadPos;

	int i;
	
	for (i=0;i<notationPersist._calls.GetSize();i++)
	{
		_calls.Add(new NotationCall(*notationPersist._calls[i]));
	}

	for (i=0;i<notationPersist._callPosLead.GetSize();i++)
	{
		_callPosLead.Add(notationPersist._callPosLead[i]);
	}

	for (i=0;i<notationPersist._callPosMethod.GetSize();i++)
	{
		_callPosMethod.Add(new NotationCallPosMethod(*notationPersist._callPosMethod[i]));
	}

	for (i=0;i<notationPersist._callPosAgregate.GetSize();i++)
	{
		_callPosAgregate.Add(new NotationCallPosAgregate(*notationPersist._callPosAgregate[i]));
	}					 

	for (i=0;i<notationPersist._splice.GetSize();i++)
	{
		_splice.Add(notationPersist._splice[i]);
	}

	for (i=0;i<notationPersist._leadLines.GetSize();i++)
	{
		_leadLines.Add(notationPersist._leadLines[i]);
	}

	for (i=0;i<notationPersist._info.GetSize();i++)
	{
		_info.Add(new NotationCCInfo(*notationPersist._info[i]));
	}
}

void NotationPersist::Serialize(CArchive &ar)
{
	int i;
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _folder;
		ar << _number;
		ar << _name;
		ar << _symmetric;  
		ar << _notation;
		ar << _leadEnd;	
		ar << _leadHead; 
		ar << _spliceLetter;
		ar << _notes;
		ar << _defaultCalls;
		ar << _defaultCallPosLead;
		ar << _defaultCallPosMethod;
		ar << _defaultSplice;
		ar << _defaultLeadLines;
		ar << _defaultCourseHeadPos;
		ar << _call;
		ar << _standardCall;
		
		if (!_defaultCalls)
		{
			ar << _calls.GetSize();
			for (i=0;i<_calls.GetSize();i++)	 _calls[i]->Serialize(ar); 
		}

		if (!_defaultCallPosLead)
		{
			ar << _callPosLead.GetSize();
			for (i=0;i<_callPosLead.GetSize();i++)	 ar << _callPosLead[i];
		}

		if (!_defaultCallPosMethod)
		{
			ar << _callPosMethod.GetSize();
			for (i=0;i<_callPosMethod.GetSize();i++) _callPosMethod[i]->Serialize(ar);
		}

		ar << _callPosAgregate.GetSize();
		for (i=0;i<_callPosAgregate.GetSize();i++) _callPosAgregate[i]->Serialize(ar);
		
		if (!_defaultSplice)
		{
			ar << _splice.GetSize();
			for (i=0;i<_splice.GetSize();i++)	 ar << _splice[i];
		}

		if (!_defaultLeadLines)
		{
			ar << _leadLines.GetSize();
			for (i=0;i<_leadLines.GetSize();i++) ar << _leadLines[i];
		}
		
		if (!_defaultCourseHeadPos)
		{
			ar << _courseHeadPos;
		}

		ar << _info.GetSize();
		for (i=0;i<_info.GetSize();i++) _info[i]->Serialize(ar);
	}
	else
	{
		init();//clear out the dynamic data, just in case

		int count;
		int value;
		NotationCallPosMethod * callPosMethod;
		NotationCallPosAgregate * callPosAgregate;
		NotationCall * call;
		NotationCCInfo * info;

		ar >> version; //just get the version number
		ar >> _folder;
		ar >> _number;
		ar >> _name;
		ar >> _symmetric;  
		ar >> _notation;
		ar >> _leadEnd;	
		ar >> _leadHead; 
		ar >> _spliceLetter;
		ar >> _notes;
		ar >> _defaultCalls;
		ar >> _defaultCallPosLead;
		ar >> _defaultCallPosMethod;
		ar >> _defaultSplice;
		ar >> _defaultLeadLines;
		ar >> _defaultCourseHeadPos;
		ar >> _call;
		ar >> _standardCall;
		
		if (!_defaultCalls)
		{
			ar >> count;
			for (i=0;i<count;i++) 
			{
				call = new NotationCall;
				call->Serialize(ar);
				_calls.Add(call);
			}
		}
		
		if (!_defaultCallPosLead)
		{
			ar >> count;
			for (i=0;i<count;i++) 
			{
				ar >> value;
				_callPosLead.Add(value);
			}
		}

		if (!_defaultCallPosMethod)
		{
			ar >> count;
			for (i=0;i<count;i++) 
			{
				callPosMethod = new NotationCallPosMethod;
				callPosMethod->Serialize(ar);
				_callPosMethod.Add(callPosMethod);
			}
		}

		ar >> count;
		for (i=0;i<count;i++) 
		{
			callPosAgregate = new NotationCallPosAgregate;
			callPosAgregate->Serialize(ar);
			_callPosAgregate.Add(callPosAgregate);
		}

		if (!_defaultSplice)
		{
			ar >> count;
			for (i=0;i<count;i++) 
			{
				ar >> value;
				_splice.Add(value);
			}
		}

		if (!_defaultLeadLines)
		{
			ar >> count;
			for (i=0;i<count;i++) 
			{
				ar >> value;
				_leadLines.Add(value);
			}
		}

		if (!_defaultCourseHeadPos)
		{
			ar >> _courseHeadPos;
		}

		ar >> count;
		for (i=0;i<count;i++) 
		{
			info = new NotationCCInfo;
			info->Serialize(ar);
			_info.Add(info);
		}

	}
}



/*************************************************************************/

NotationCall::NotationCall() :
_nameShorthand(""),
_nameLonghand(""),
_notation("")
{
}

NotationCall::NotationCall(NotationCall & notationCall) :
_nameShorthand(notationCall._nameShorthand),
_nameLonghand(notationCall._nameLonghand),
_notation(notationCall._notation)
{
}						

void NotationCall::Serialize(CArchive &ar)
{

	//versions 
	//upto 2.01 = version 1
	//version 3.01 onwards version 2

	int version = 2;

	if (ar.IsStoring())
	{
		ar << version;
		ar << _nameShorthand;
		ar << _notation;

		/***************new in v 3.01 / v2 ***************************/
		ar << _nameLonghand;
	}
	else
	{
		//init
		int thisVersion;
		ar >> thisVersion;

		if (thisVersion == 1) //upto and including version 1.07
			deserialize_v1(ar);
		else if (thisVersion == 2)
			deserialize_v2(ar);
		else
			AfxThrowArchiveException(CArchiveException::endOfFile);	   

	}
}

void NotationCall::deserialize_v2(CArchive& ar)
{
	TRACE("Loading a version 2 NotationCall \r\n");

	ar >> _nameShorthand;
	ar >> _notation;

	/***************new in v 3.01 / v2 ***************************/
	ar >> _nameLonghand;

	if (_nameLonghand.IsEmpty())
	{
		if (_nameShorthand == "-")
			_nameLonghand = "Bob";
		else if (_nameShorthand == "s")
			_nameLonghand = "Single";
		else
			_nameLonghand = _nameShorthand;
	}

}

void NotationCall::deserialize_v1(CArchive& ar)
{
	TRACE("Loading a version 1 NotationCall \r\n");

	ar >> _nameShorthand;
	ar >> _notation;

	/***************new in v 3.01 / v2 ***************************/
	//need to create call longhand

	if (_nameShorthand == "-")
		_nameLonghand = "Bob";
	else if (_nameShorthand == "s")
		_nameLonghand = "Single";
	else
		_nameLonghand = _nameShorthand;
}

/*************************************************************************/

NotationCallPosMethod::NotationCallPosMethod() :
_lead(0),
_pos(0),
_name("")
{
}

NotationCallPosMethod::NotationCallPosMethod(NotationCallPosMethod & notationCallPosMethod) :
_lead(notationCallPosMethod._lead),
_pos(notationCallPosMethod._pos),
_name(notationCallPosMethod._name)
{
}



void NotationCallPosMethod::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _lead;
		ar << _pos;
		ar << _name;
	}
	else
	{
		ar >> version;
		ar >> _lead;
		ar >> _pos;
		ar >> _name;
	} 
}

/*************************************************************************/

NotationCCInfo::NotationCCInfo() :
_type(cc_none),
_text("")
{
}

NotationCCInfo::NotationCCInfo(NotationCCInfo & notationCCInfo) :
_type(notationCCInfo._type),
_text(notationCCInfo._text)
{
}

void NotationCCInfo::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << (int)_type;
		ar << _text;
	}
	else
	{
		ar >> version;
		int type;
		ar >> type;
		_type = (NotationCCInfoType)type ;
		ar >> _text;
	} 
}
	


CString NotationCall::getDisplayName()
{
   CString name;
   name.Format("%s (%s)",_nameShorthand, _nameLonghand);
   return name;
}
