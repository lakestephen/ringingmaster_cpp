// Splice.cpp: implementation of the Splice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Splice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Splice::Splice(Variance variance, int compositionRow, CString spliceShorthand,CString spliceLonghand) :
_variance(variance),
_compositionRow(compositionRow), 
_spliceShorthand(spliceShorthand),
_spliceLonghand(spliceLonghand)
{
//	TRACE ("%s %s (%d) \n", _splice, _variance.getGridString(), _compositionRow);
}


Splice::Splice(const Splice& splice) 
{
	_variance = splice._variance;
	_compositionRow = splice._compositionRow;
	_spliceShorthand = splice._spliceShorthand;
	_spliceLonghand = splice._spliceLonghand;

//	TRACE ("%s %s (%d) \n", _spliceShorthand, _variance.getGridString(), _compositionRow);	
}


Splice::~Splice()
{

}
