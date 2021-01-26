// MethodCrib.cpp: implementation of the MethodCrib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodCrib.h"
#include "Lead.h"
#include "Crib.h"
#include "CribDoc.h"
#include "ProofInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


MethodCrib::MethodCrib() :
Method(),
_crib(NULL),
_cribDoc(NULL)
{
}


MethodCrib::~MethodCrib()
{
	delete _crib;
	_crib = NULL;
}

void MethodCrib::create(ProofInput* proofInput)
{
/*	todo Method::create(proofInput);

	//all we need to do here is to test that we have only one lead of correct size.
	ASSERT(GetSize() == 1);
	ASSERT(GetAt(0)->GetUpperBound() == getCurNot()->getChangesPerPlainLead());
	ASSERT(proofInput->getNotations().GetSize() == 1);	  

	Notation* notation = proofInput->getNotations().GetAt(0);

	if (notation)
	{
		delete _crib;
		_crib = new Crib(*notation, _cribDoc->getCribNumber());
	}  */
}

Crib* MethodCrib::getCrib()
{
	return _crib;
}

void MethodCrib::setDocument(CribDoc* cribDoc)
{
	_cribDoc = cribDoc;
}


CribDoc* MethodCrib::getDocument()
{
	return _cribDoc;
}
