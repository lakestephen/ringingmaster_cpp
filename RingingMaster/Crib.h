// Crib.h: interface for the Crib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRIB_H__F97C3976_A2C6_11D6_B5F1_000255162CF0__INCLUDED_)
#define AFX_CRIB_H__F97C3976_A2C6_11D6_B5F1_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Notation.h"
#include "StdAfx.h"	// Added by ClassView
#include "CribRule.h"



class CribEntry
{
public:	
	void dumpData();
	CString getPlural(int number);
	CString getDescription();
	CribEntry(CribTypes type, int start,int end, int tenor) :
	_type(type),
	_start(start),
	_end(end),
	_tenor(tenor)
	{
		//dumpData();
	}


	CribTypes _type;
	int _start;
	int _end;
	int _tenor;
};

class Crib  
{
public:
	CString getDescription();
	void dumpData();
	void getAsStrings(CStringArray& strings);
	Crib(Notation& notation, int number);
	virtual ~Crib();

protected:
	void runRules();
	void constructRules();
	void runRule(CribRule* rule);

	Notation& _notation;
	int _number;
	CArray <CribEntry*, CribEntry*> _data;

	CArray<CribRule*, CribRule*> _rules;

	void parseMethod();

};

#endif // !defined(AFX_CRIB_H__F97C3976_A2C6_11D6_B5F1_000255162CF0__INCLUDED_)
