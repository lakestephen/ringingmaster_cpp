// MusicResults.cpp: implementation of the MusicResults class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MusicResults.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
						 

MusicResultRowPosition::MusicResultRowPosition(CString change) :
_change(change)
{

}

CString MusicResultRowPosition::getChange()
{
	return _change;
}


MusicResult::MusicResult(MusicRule& musicRule, CString name) :
MusicRule(musicRule)
{
	//provide an override name
	if (!name.IsEmpty())
		setName(name);

}

MusicResult::~MusicResult()
{
	for (int i=0;i<GetSize();i++)
		delete GetAt(i);
}







