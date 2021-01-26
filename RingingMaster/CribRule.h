// CribRule.h: interface for the CribRule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRIBRULE_H__60648B24_A473_11D6_B5F3_000255162CF0__INCLUDED_)
#define AFX_CRIBRULE_H__60648B24_A473_11D6_B5F3_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum CribTypes 
{
	ct_out,
	ct_in,
	ct_lie,
	ct_dodge_up,
	ct_dodge_down,
	ct_cambridge_up,
	ct_cambridge_down,
	ct_yorkshire_up,
	ct_yorkshire_down,
	ct_point

};



enum CribAction 
{
	ca_start, 
	ca_end,
	ca_increment,
	ca_decrement,
	ca_equals,
	ca_none
};


class CribRule  
{
public:
	CribRule(CribTypes cribType, CString rules, CribAction startAction, int startIndex, CribAction endAction, int endIndex);
	virtual ~CribRule();

	void addRule(CString rule);
	void parseRules(CString rules);

	CArray <CribTypes, CribTypes> _rules;
	CribTypes _cribType;

	CribAction _startAction;
	int _startIndex;
	CribAction _endAction;
	int _endIndex;



};

#endif // !defined(AFX_CRIBRULE_H__60648B24_A473_11D6_B5F3_000255162CF0__INCLUDED_)
