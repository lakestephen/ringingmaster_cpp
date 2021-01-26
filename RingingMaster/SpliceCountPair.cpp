// SpliceCountPair.cpp: implementation of the SpliceCountPair class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SpliceCountPair.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SpliceCountPair::SpliceCountPair(CString name, int number) :
_name(name), 
_number(number)
{

}

SpliceCountPair::~SpliceCountPair()
{

}
