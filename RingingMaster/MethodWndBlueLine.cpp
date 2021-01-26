// MethodWndBlueLine.cpp: implementation of the MethodWndBlueLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodWndBlueLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(MethodWndBlueLine, MethodWnd)


MethodWndBlueLine::MethodWndBlueLine()
{

}

MethodWndBlueLine::~MethodWndBlueLine()
{

}		

BEGIN_MESSAGE_MAP(MethodWndBlueLine, MethodWnd)
	//{{AFX_MSG_MAP(MethodWndBlueLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


UINT MethodWndBlueLine::getContectMenuId()
{
	return IDR_METHOD_STYLE_BLUE_LINE;
}


