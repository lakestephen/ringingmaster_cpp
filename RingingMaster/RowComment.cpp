// RowComment.cpp: implementation of the RowComment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "RowComment.h"
#include "Notation.h"

#include "Splice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RowComment::RowComment() :
_rowCommentType(rct_none),
_bumped(0)
{

}
RowComment::RowComment(const RowComment& rowComment):
RowPosition(rowComment),
_rowCommentType(rowComment._rowCommentType),
_str1(rowComment._str1),
_str2(rowComment._str2),
_str3(rowComment._str3),
_bumped(rowComment._bumped)
{};


RowComment::~RowComment()
{

}
  
RowCommentType RowComment::getRowCommentType()
{
	return _rowCommentType;
}

void RowComment::setCall(CString shorthandName, CString longhandName, CString callPosition)
{
	_rowCommentType = rct_call;
	_str1 = shorthandName;
	_str2 = longhandName;
	_str3 = callPosition;
}

void RowComment::setAborted()
{
	_rowCommentType = rct_abort;
} 

void RowComment::setSplice(Splice* splice)
{
	_rowCommentType = rct_splice;
	_str1 = splice->_spliceShorthand;
	_str2 = splice->_spliceLonghand;
}

CString RowComment::getDisplayString(bool shorthand)
{	  
	switch(_rowCommentType)
	{
	case rct_call:
		if (_str3.IsEmpty())
		{
			if (!shorthand && !_str2.IsEmpty())
				return " " + _str2;
			else 
				return " " + _str1;
		}
		else
		{
			if (!shorthand && !_str2.IsEmpty())
				return " " + _str2 + "," + _str3;
			else 
				return " " + _str1 + "," + _str3;
		}
		
	case rct_splice:
	case rct_part:
	case rct_comment:
	case rct_go:
		if (!shorthand && !_str2.IsEmpty())
			return " " + _str2;
		else 
			return " " + _str1;
	case rct_abort:
		return " Abort";
	case rct_none:
	default:
		ASSERT(FALSE);
		return "";
	}
}



void RowComment::setPart(CString part, CString partNumOnly)
{
	_rowCommentType = rct_part;
	_str1 = part;
	_str2 = partNumOnly;
}

void RowComment::setComment(CString comment)
{
	_rowCommentType = rct_comment;
	_str1 = comment;
}

void RowComment::setGo(Notation* notation)
{
	_rowCommentType = rct_go;
	_str1 = notation->getSpliceLetter();
	_str2 = notation->getName();
}

void RowComment::setThatsAll()
{
	_rowCommentType = rct_thatsAll;
}

CString RowComment::getStr1()
{
	return _str1;
}

CString RowComment::getStr2()
{
	return _str2;
}


//operator used to test the precidence
bool RowComment::operator > (RowComment & rowComment)
{
	//the most important one is the one with the lowest type number 
	int res = (int)_rowCommentType - (int)rowComment._rowCommentType;

	if (res > 0)
		return false;
	else if (res < 0)
		return true;
	else
	{
		//the most important one is the one with the highest number
		if ((_bumped - rowComment._bumped) > 0)
			return true;
		else 
			return false;
	}		 	
}

bool RowComment::incrementRow(Method *method)
{
	if (RowPosition::incrementRow(method))
	{
		_bumped++;
		return true;
	}
	return false;
}


bool RowComment::verify(Method* method)
{
	bool didRollOver = false;
	bool ret = RowPosition::verify(method, didRollOver);
	if (didRollOver)
		_bumped++;

	return ret;
}
