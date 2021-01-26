// CribStyle.h: interface for the CribStyle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRIBSTYLE_H__3E2D68C9_ADF3_11D7_B6E5_000255162CF0__INCLUDED_)
#define AFX_CRIBSTYLE_H__3E2D68C9_ADF3_11D7_B6E5_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
				  
#include "FontInfo.h"

class CribStyle  
{
public:
	void setDefaults();
	CribStyle();
	virtual ~CribStyle();


	void Serialize(CArchive& ar);


	FontInfo _cribFont;
	FontInfo _cribNotesFont;


};

#endif // !defined(AFX_CRIBSTYLE_H__3E2D68C9_ADF3_11D7_B6E5_000255162CF0__INCLUDED_)
