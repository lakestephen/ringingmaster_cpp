// ParseData.h: interface for the ParseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSEDATA_H__C4FE1B64_437D_11D5_B5CC_009027BB3286__INCLUDED_)
#define AFX_PARSEDATA_H__C4FE1B64_437D_11D5_B5CC_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ParseType
{							
	PT_NONE,				//null - set before other types set
	PT_WHITESPACE,			//space		
	PT_CALL_POS,			// a normal calling position
	PT_CALL_POS_AGREGATE,	//agregate calling position
	PT_CALL,				//an actual call
	PT_BLOCK,				//a block include from right curly brace
	PT_SPLICE,				//a normal splice letter
	PT_VARIANCE,			//a variance bracket - open or close '[' ']'
	PT_GROUP,				//a normal bracket for grouping '(' ')' - needs a multiplyer
	PT_DEFINITION,			//macro to replace on piece of text with another
	PT_PLAIN_LEAD,			//plain lead in lead based
	PT_CALL_NUM,			// an ordinary call multiplyer
	PT_CALL_DEFAULT_NUM,	//a number on its own acting as a default call
	PT_SPLICE_NUM,			//splice multiplyer
	PT_BLOCK_NUM,			//a block include  multiplyer from right curly brace
	PT_DEFINITION_NUM,		//multiplyer for macro to replace on piece of text with another
	PT_GROUP_NUM,			//multiplyer for a normal bracket for grouping '(' ')'
	PT_PLAIN_LEAD_NUM,		//multiplyer for a plain lead in lead based
	
};

CString getParseText(ParseType parseType);

class ParseData  
{
public:
	ParseData(CString name, ParseType type);
	virtual ~ParseData();	 

	CString getName() { return _name; }
	ParseType getType() { return _type; }


protected:

	CString _name;
	ParseType _type;

};

typedef CArray <ParseData*, ParseData*> ParseDataArray;

#endif // !defined(AFX_PARSEDATA_H__C4FE1B64_437D_11D5_B5CC_009027BB3286__INCLUDED_)
