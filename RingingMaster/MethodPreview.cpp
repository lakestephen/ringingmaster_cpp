// MethodPreview.cpp: implementation of the MethodPreview class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodPreview.h"
#include "Lead.h"
#include "CallNotationMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//just copy the input parameters
MethodPreview::MethodPreview():
Method()
{
}

void MethodPreview::create(ProofInput* proofInput)
{
/* todo
	Method::create(proofInput);
	int i,j,k,l,index;	 
	int changesPerPlainLead = getCurNot()->getChangesPerPlainLead();

	//setup the call notations; 
	CallNotationMap callNotationMap(getCurNot(), true);

	CString call;
	Notation* not;
	POSITION pos = callNotationMap.GetStartPosition();

	while (pos)
	{
		//get the call notation
		callNotationMap.GetNextAssoc(pos, call, not);

		for (j=0;j<getCurNot()->getCallPosLeadCount();j++)
		{			
			//create our own notation.
			CString strNot;
			int callPosLead = getCurNot()->getCallPosLead(j) - 3;
			callPosLead = getNextPositionIndex(callPosLead, changesPerPlainLead);
			index = callPosLead;

			if ((GetSize() <2)||(GetAt(0)->GetUpperBound() < callPosLead))break;

			//initial notation;
			for (k=0;k<2;k++)
			{
				strNot += getCurNot()->getRowStr(index) + ".";
				index = getNextPositionIndex(index, changesPerPlainLead);
			}
			//call notation
			for (k=0;k<not->getChangesPerPlainLead();k++)
			{
				strNot += not->getRowStr(k) + ".";
				index = getNextPositionIndex(index, changesPerPlainLead);
			}
			//end of notation;
			for (k=0;k<2;k++)
			{
				strNot += getCurNot()->getRowStr(index) + ".";
				index = getNextPositionIndex(index, changesPerPlainLead);
			}

			//create the new notation
			Notation newNot;
			newNot.create(true, "", getCurNot()->getNumber(), false, strNot);
			newNot.prepare();				

			//create the data struct to show this call
			CallPreview * callPrev = new CallPreview();
			_callPreviews.Add(callPrev);
			callPrev->_name.Format("%s at pos %d", call,  + getCurNot()->getCallPosLead(j)+1) ;
			callPrev->_notation = newNot.getNotation();
			for (l=0;l<newNot.getChangesPerPlainLead();l++)
			{
				callPrev->_notationStrings.Add(newNot.getRowStr(l));
			}

			//create the Lead
			Notations notations;
			notations.Add(&newNot);
			Method tempMet(notations);
			tempMet._proofInput->setLeadLimit(1);
			tempMet.requestTerminate(TR_NOT_REQUESTED);
			tempMet.setDynamicStartRow(*GetAt(0)->GetAt(callPosLead));
			Lead * lead = new Lead(&tempMet);
			lead->create();

			ASSERT(lead->GetSize() > 0);

			Add(lead);		 

			//fill in the lead lines array
			lead->removeAllLeadLine();
			for (l=0;l<getCurNot()->getLeadLineCount();l++)
			{
				lead->addLeadLine(getNextPositionIndex(getCurNot()->getLeadLine(l) - callPosLead - 1, 
						         changesPerPlainLead));
			}

			//fill in the call calling positions
			RowComment* rowComment = new RowComment;
			rowComment->setRow(GetUpperBound(),1);
			rowComment->setComment(call);
			addComment(rowComment);
		}
	}
 	  
	//add calling positions to the main method 
	NotationCallPosMethod * cpm; 
	for (i=0;i<getCurNot()->getCallPosMethodCount();i++)
	{
		cpm = getCurNot()->getCallPosMethod(i);
		RowComment* rowComment = new RowComment;
		rowComment->setRow(cpm->_lead, (cpm->_pos > 0)?(cpm->_pos - 1):(cpm->_pos));
		rowComment->setComment("-"+cpm->_name);
		addComment(rowComment);
	}
	
	distributePendingComments();*/
}

MethodPreview::~MethodPreview()
{
	for (int i=0;i<_callPreviews.GetSize();i++)
	{
		delete _callPreviews.GetAt(i);
	} 
	_callPreviews.RemoveAll();
}

int MethodPreview::getNextPositionIndex(int index, int changesPerPlainLead)
{
	index++;
	while (index >= changesPerPlainLead)
		index -= changesPerPlainLead;
	while (index < 0)
		index += changesPerPlainLead;
	return index;
}
