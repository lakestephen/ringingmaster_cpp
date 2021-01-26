// CribStyle.cpp: implementation of the CribStyle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "CribStyle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CribStyle::CribStyle()
{

}

CribStyle::~CribStyle()
{

}


void CribStyle::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;	
		_cribFont.Serialize(ar);
		_cribNotesFont.Serialize(ar);
	}
	else
	{
		ar >> version;
		_cribFont.Serialize(ar);
		_cribNotesFont.Serialize(ar);
	}
}

void CribStyle::setDefaults()
{
	_cribFont._color = RGB(102,102,102);
	_cribFont._size = 120;
	_cribNotesFont._color = RGB(0,0,0);
	_cribNotesFont._size = 120;
}
