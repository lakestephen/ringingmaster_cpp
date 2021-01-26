// allPosDefinition.cpp: implementation of the CallPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "CallPosition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Method.h"
#include "Lead.h"
#include "ProofManager.h"
#include "Notation.h"
#include "ProofInput.h"
#include "Row.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CallPosition::CallPosition(int place, int afterRow) :
_place(place),
_afterRow(afterRow)						   
{

}

CallPosition::~CallPosition()
{

}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CallPositionMap::CallPositionMap(Notation* notation, int tenor) 
{	
	
	//create a plain lead
	ProofInput proofInput(notation->getNumber());
	proofInput.addNotation(*notation);

	Method plainLead;
	proofManager()->requestProof(proofInput, plainLead);

	for (int i=0;i<notation->getCallPosMethodCount();i++)
	{
		NotationCallPosMethod * pNCPM = notation->getCallPosMethod(i);
		
		//find the 
		int place = plainLead.getLead(pNCPM->_lead).GetAt(pNCPM->_pos)->getPositionOfBell(tenor);
		CallPosition* CPD = new CallPosition(place, pNCPM->_pos);

		SetAt(pNCPM->_name, CPD);
	}
	
}

CallPositionMap::~CallPositionMap()
{
	//tidy up the call position map 
	CString position;
	CallPosition* callPosition;
	POSITION pos = GetStartPosition();
	while (pos)
	{
		GetNextAssoc(pos, position, callPosition);
		delete callPosition;
	}	
	RemoveAll();
}


