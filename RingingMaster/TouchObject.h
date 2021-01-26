// TouchObject.h: interface for the TouchObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOUCHOBJECT_H__B4D120E4_F581_11D7_B711_000255162CF0__INCLUDED_)
#define AFX_TOUCHOBJECT_H__B4D120E4_F581_11D7_B711_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Call.h"
#include "Splice.h"

class TouchObject  
{
public:
	void dump();
	void tidy();
	void createTouchObject(TouchDoc* pTouchDoc);
	TouchObject();
	virtual ~TouchObject();

	

	Calls _calls;
	Splices _splices;

};

#endif // !defined(AFX_TOUCHOBJECT_H__B4D120E4_F581_11D7_B711_000255162CF0__INCLUDED_)
