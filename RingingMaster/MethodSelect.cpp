// MethodSelect.cpp: implementation of the MethodSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodSelect.h"
#include "Lead.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MethodSelect::MethodSelect() : 
Method()
{
}

MethodSelect::~MethodSelect()
{

}

void MethodSelect::create(ProofInput* proofInput)
{
/* todo
	Method::create(proofInput);

	//all we need to do here is to test that we have only one lead of correct size.
	ASSERT(GetSize() >= 1);
	ASSERT(GetAt(0)->GetUpperBound() == getCurNot()->getChangesPerPlainLead());

		//add calling positions to the main method 
	NotationCallPosMethod * cpm; 
	for (int i=0;i<getCurNot()->getCallPosMethodCount();i++)
	{
		cpm = getCurNot()->getCallPosMethod(i);
		if (cpm->_lead < GetSize())
		{
			RowComment * rowComment = new RowComment;
			rowComment->setRow(cpm->_lead, (cpm->_pos > 0)?(cpm->_pos - 1):(cpm->_pos));
			rowComment->setComment("-"+cpm->_name);
			addComment(rowComment);
		}
	}

	distributePendingComments();
	*/
}
