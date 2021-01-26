// Crib.cpp: implementation of the Crib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Crib.h"
#include "Method.h"
#include "Lead.h"
#include "Row.h"
#include "GlobalFunctions.h"
#include "ProofManager.h"
#include "ProofInput.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Crib::Crib(Notation& notation, int number):
_notation(notation), 
_number(number)
{		
	
	constructRules();

	parseMethod();   

	runRules();

}

Crib::~Crib()
{
	for (int i=0;i< _data.GetSize();i++)
		delete _data.GetAt(i);
	for (int i=0;i< _rules.GetSize();i++)
		delete _rules.GetAt(i);

}

void Crib::constructRules()
{
	//dodges
	CString up = "ct_out";
	CString down = "ct_in";

	//30 is an arbatiary value
	for (int i=0;i<30;i++)
	{
		up += "|ct_in|ct_out";
		_rules.InsertAt(0,new CribRule(ct_dodge_up, up, ca_start, 0, ca_equals, i+1));
		down += "|ct_out|ct_in";
		_rules.InsertAt(0,new CribRule(ct_dodge_down, down, ca_start, 0, ca_equals, i+1));
	}
	
	//treble bob


	//cambridge places 
	_rules.Add(new CribRule(ct_cambridge_up, "ct_dodge_up|ct_lie|ct_in|ct_lie|ct_dodge_up|ct_lie|ct_in|ct_lie|ct_dodge_up", ca_start, 0, ca_equals, 1));
	_rules.Add(new CribRule(ct_cambridge_down, "ct_dodge_down|ct_lie|ct_out|ct_lie|ct_dodge_down|ct_lie|ct_out|ct_lie|ct_dodge_down", ca_start, 0, ca_equals, 1));

	//yorkshire places 
	_rules.Add(new CribRule(ct_yorkshire_up, "ct_dodge_up|ct_lie|ct_in|ct_lie|ct_dodge_up", ca_start, 0, ca_equals, 1));
	_rules.Add(new CribRule(ct_yorkshire_down, "ct_dodge_down|ct_lie|ct_out|ct_lie|ct_dodge_down", ca_start, 0, ca_equals, 1));

	//point
	_rules.Add(new CribRule(ct_point, "ct_out|ct_out|ct_in|ct_in", ca_end, 1, ca_equals, 2));
	_rules.Add(new CribRule(ct_point, "ct_in|ct_in|ct_out|ct_out", ca_end, 1, ca_equals, 2));	 
	_rules.Add(new CribRule(ct_point, "ct_out|ct_in", ca_end, 0, ca_equals, 1));
	_rules.Add(new CribRule(ct_point, "ct_in|ct_out", ca_end, 0, ca_equals, 1));	 

	//normal processing of hunting
	_rules.Add(new CribRule(ct_out, "ct_out|ct_out", ca_start, 0, ca_end, 1));
	_rules.Add(new CribRule(ct_in, "ct_in|ct_in", ca_start, 0, ca_end, 1));
	_rules.Add(new CribRule(ct_lie, "ct_lie|ct_lie", ca_none, 0, ca_increment, 0));
  
  
}

void Crib::parseMethod()
{
	//create the method
	ProofInput proofInput(_notation.getNumber());	
	proofInput.addNotation(_notation);
	proofInput.setPerformAnalysis(); //todo do we want to do analysis here?
	Method met;
	proofManager()->requestProof(proofInput, met);

	   /*  todo can we loop through rows instead?
	//loop through every line, and extract the directions
	if ((met.GetSize() ==0)||
		(met.GetAt(0)->GetSize() == 0))
		return;

	int oldBellPos = met.GetAt(0)->GetAt(0)->getPositionOfBell(_number); //assume starting in rounds
	for (int i=0;i<met.GetSize();i++)
	{
		Lead* lead = met.GetAt(i);

		for (int j=1;j<lead->GetSize();j++)
		{
			Row* row = lead->GetAt(j);

			int bellPos = row->getPositionOfBell(_number);

			if (bellPos > oldBellPos) 
				_data.Add(new CribEntry(ct_out, oldBellPos,bellPos,_notation.getNumber()));
			else if (bellPos < oldBellPos) 
				_data.Add(new CribEntry(ct_in, oldBellPos,bellPos,_notation.getNumber()));
			else if (bellPos == oldBellPos) 
				_data.Add(new CribEntry(ct_lie, bellPos,1,_notation.getNumber()));

			oldBellPos = bellPos;
		}	
	}	  */
}

CString Crib::getDescription()
{
	CString description;
	
	for (int i=0;i< _data.GetSize();i++)
	{
		CString desc = _data.GetAt(i)->getDescription();
		if (desc != "")
		{
			if (i < _data.GetUpperBound())
				desc += ", ";
			if (i == _data.GetUpperBound())
				desc += ".";

			description += desc;
		}
	}		 

	return description;

}

void Crib::getAsStrings(CStringArray& strings)
{
	for (int i=0;i< _data.GetSize();i++)
	{
		CString desc = _data.GetAt(i)->getDescription();
		if (desc != "")
			strings.Add(desc);   
	}
}

CString CribEntry::getDescription()
{
	if ((_type == ct_out)||(_type == ct_in))
		return "";

	if (_type == ct_lie)
	{
		if (_start == 0)
			return "lead" +  CString((_end == 1)?"":(" for " + GlobalFunctions::getNumberName(_end) + " blow" + getPlural(_end)));
		if (_start+1 == _tenor)
			return "lie behind"+ CString((_end == 1)?"":(" for " + GlobalFunctions::getNumberName(_end) + " blow" + getPlural(_end)));
		else
		    return "make "+ GlobalFunctions::getPlaceName(_start,_tenor) + ((_end == 1)?"":(" for " + GlobalFunctions::getNumberName(_end) + " blow" + getPlural(_end)));
	}
	if (_type == ct_dodge_up)
		return GlobalFunctions::getDodgePullNumber(_end) + "dodge " + GlobalFunctions::getNumberName(_start+1) + "-" + GlobalFunctions::getNumberName(_start+2) + " up";

	if (_type == ct_dodge_down)
		return GlobalFunctions::getDodgePullNumber(_end) + "dodge " +  GlobalFunctions::getNumberName(_start) + "-" + GlobalFunctions::getNumberName(_start+1) + " down";
	if (_type == ct_cambridge_up)
		return "make cambridge places in " + GlobalFunctions::getNumberName(_start+1) + "-" + GlobalFunctions::getNumberName(_start+2) + " up";
	if (_type == ct_cambridge_down)
		return "make cambridge places in " + GlobalFunctions::getNumberName(_start) + "-" + GlobalFunctions::getNumberName(_start+1) + " down";
	if (_type == ct_yorkshire_up)
		return "make yorkshire places in " + GlobalFunctions::getNumberName(_start+1) + "-" + GlobalFunctions::getNumberName(_start+2) + " up";
	if (_type == ct_yorkshire_down)
		return "make yorkshire places in " + GlobalFunctions::getNumberName(_start) + "-" + GlobalFunctions::getNumberName(_start+1) + " down";
	if (_type == ct_point)
	{
		if (_start == 0)
			return "point lead"; 
		else if (_start+1 == _tenor)
			return "point behind";
		else			
			return "point " + GlobalFunctions::getPlaceName(_start, _tenor);
	}
	ASSERT(FALSE);


	
	return "";

}

CString CribEntry::getPlural(int number)
{
	return (number == 1)?"":"s";
}


void Crib::runRules()
{
	for (int i=0;i<_rules.GetSize();i++)
	{
//		dumpData();
//		TRACE("********************************\r\n");
		runRule(_rules.GetAt(i));
	}	
//	dumpData();

}

void Crib::runRule(CribRule *rule)
{
	//try and find a correct sequence
	for (int i=0;i<_data.GetSize()-rule->_rules.GetUpperBound();i++)
	{
		bool match = true;
		for(int j=0;j<rule->_rules.GetSize();j++)
		{
			if (_data.GetAt(i+j)->_type != rule->_rules.GetAt(j))
			{
				match = false;
				break;
			}
		}
		if (match)
		{
			CribEntry* entry = _data.GetAt(i);
			//set type
			entry->_type = rule->_cribType;
			//set start
			switch (rule->_startAction)
			{
			case ca_start:
				entry->_start = _data.GetAt(i+rule->_startIndex)->_start;
				break;
			case ca_end:
				entry->_start = _data.GetAt(i+rule->_startIndex)->_end;
				break;
			case ca_increment:
				entry->_start++;
				break;
			case ca_decrement:
				entry->_start--;
				break;
			case ca_equals:
				entry->_start = rule->_startIndex;
				break;
			}

			//set end
			switch (rule->_endAction)
			{
			case ca_start:
				entry->_end = _data.GetAt(i+rule->_endIndex)->_start;
				break;
			case ca_end:
				entry->_end = _data.GetAt(i+rule->_endIndex)->_end;
				break;
			case ca_increment:
				entry->_end++;
				break;
			case ca_decrement:
				entry->_end--;
				break;
			case ca_equals:
				entry->_end = rule->_endIndex;
				break;
			}

			//remove non wanted ones
			for (int k=i+1;k<i+rule->_rules.GetSize();k++)
			{
				delete _data.GetAt(i+1);
				_data.RemoveAt(i+1);
			}
			i--;
		}
	}						  	
}

void Crib::dumpData()
{
	for (int i=0;i<_data.GetSize();i++)
	{
		_data.GetAt(i)->dumpData();
	}	
}

void CribEntry::dumpData()
{
	TRACE("type = %d, start = %d, end = %d, tenor = %d\r\n", _type, _start, _end, _tenor);
}

