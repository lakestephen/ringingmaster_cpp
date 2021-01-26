#if !defined(AFX_PROPITEM_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_)
#define AFX_PROPITEM_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropItem.h : header file
//

#include "UserMessages.h"



enum PROPTYPE
{
	PT_COMBO,  
	PT_EDIT,
	PT_BUTTON,
	PT_STATIC,
	PT_STATIC_GREY

};


/////////////////////////////////////////////////////////////////////////////
//PropList Items
class PropItem
{
// Attributes


public:
	CString _col[3];  //column array of 3
	PROPTYPE _type; //type
	CString _list;	 //list for drop down
	PropItem(PROPTYPE type, CString col0 = CString(""), CString col1 = CString(""), CString col2 = CString(""), CString list= CString(""));
	COLORREF _backgroundColor;
	COLORREF _textColor;
	long _data1;
	long _data2;
};

typedef CArray<PropItem*, PropItem*> PropArray;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPITEM_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_)
