// Met.cpp: implementation of the Met class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Met.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Met::Met()
{

}

Met::~Met()
{

}

CString Met::getColumn(int index)
{
	if (index <_strs.GetSize())
		return _strs.GetAt(index);
	return "";
}
