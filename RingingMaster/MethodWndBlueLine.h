// MethodWndBlueLine.h: interface for the MethodWndBlueLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METHODWNDBLUELINE_H__B596CEFE_C982_11D6_B60F_000255162CF0__INCLUDED_)
#define AFX_METHODWNDBLUELINE_H__B596CEFE_C982_11D6_B60F_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MethodWnd.h"

class MethodWndBlueLine : public MethodWnd 
{
public:
	MethodWndBlueLine();
	virtual ~MethodWndBlueLine();

	DECLARE_DYNCREATE(MethodWndBlueLine)

protected:
	UINT getContectMenuId();

	//{{AFX_MSG(MethodWndBlueLine)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_METHODWNDBLUELINE_H__B596CEFE_C982_11D6_B60F_000255162CF0__INCLUDED_)
