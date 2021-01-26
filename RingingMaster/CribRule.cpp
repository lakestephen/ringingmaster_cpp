// CribRule.cpp: implementation of the CribRule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "CribRule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CribRule::CribRule(CribTypes cribType, CString rules, 
				   CribAction startAction, int startIndex, 
				   CribAction endAction, int endIndex) :
_cribType(cribType),
_startAction(startAction),
_startIndex(startIndex),
_endAction(endAction),
_endIndex(endIndex)
{
	parseRules(rules);
}

CribRule::~CribRule()
{

}



void CribRule::parseRules(CString rules)
{
	int j=0;
	int i=0;

	for (i=0; i<rules.GetLength(); i++)
	{
		if (rules.GetAt(i)=='|')
		{
			addRule(rules.Mid(j,i-j));
			j=i+1;
		}
		
	}
	addRule(rules.Mid(j,i-j)); 
}

void CribRule::addRule(CString rule)
{
	//TRACE(rule + "\r\n");

	if (rule == "ct_out") _rules.Add(ct_out);
	else if (rule == "ct_in") _rules.Add(ct_in);
	else if (rule == "ct_lie") _rules.Add(ct_lie);
	else if (rule == "ct_dodge_up") _rules.Add(ct_dodge_up);
	else if (rule == "ct_dodge_down") _rules.Add(ct_dodge_down);
	else ASSERT(FALSE);
}
