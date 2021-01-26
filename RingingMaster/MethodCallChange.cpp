// MethodCallChange.cpp: implementation of the MethodCallChange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodCallChange.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MethodCallChange::MethodCallChange(int /*number*/) :
Method(/* todo number*/)
{
	// _proofInput->setName("Called Changes"); //todo this should be set on the method
}

MethodCallChange::~MethodCallChange()
{

}
			  /* todo
void MethodCallChange::addComment(RowComment* rowComment)
{
	Method::addComment(rowComment);
	distributePendingComments();
}
	 */