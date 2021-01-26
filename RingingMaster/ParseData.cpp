// ParseData.cpp: implementation of the ParseData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "ParseData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString getParseText(ParseType parseType)
{
	switch (parseType)
	{
	case PT_CALL_POS:
		return "Calling position";
	case PT_CALL_POS_AGREGATE:
		return "Agregate calling position";
	case PT_CALL:
	case PT_CALL_NUM:
		return "Call";
	case PT_CALL_DEFAULT_NUM:
		return "Default Call";
	case PT_BLOCK:
	case PT_BLOCK_NUM:
		return "Block definition";
	case PT_DEFINITION:
	case PT_DEFINITION_NUM:
		return "Definition";
	case PT_SPLICE:
	case PT_SPLICE_NUM:
		return "Splice";
	case PT_PLAIN_LEAD:
	case PT_PLAIN_LEAD_NUM:
		return "Plain Lead Token";
	case PT_VARIANCE:
		return "Variance";
	case PT_GROUP:
	case PT_GROUP_NUM:
		return "Group";
	case PT_NONE:
	case PT_WHITESPACE:
	default:
		return "";
	}
};


ParseData::ParseData(CString name, ParseType type) :
_name(name), 
_type(type)
{

}

ParseData::~ParseData()
{

}


