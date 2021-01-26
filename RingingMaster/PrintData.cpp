// PrintData.cpp: implementation of the PrintData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PrintData.h"
#include "MethodWndPrint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PrintData::PrintData():
_numPages(100000), 
_methodWndPrt(NULL) //this will be set the fiest time the pages are printed
{

}

PrintData::~PrintData()
{

	delete _methodWndPrt;
}
