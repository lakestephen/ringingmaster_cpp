// PropItem.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "PropItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropList
PropItem::PropItem(PROPTYPE type, CString col0, CString col1, CString col2, CString list) :
_type(type),
_list(list),
_backgroundColor(::GetSysColor(COLOR_WINDOW)),
_textColor(::GetSysColor(COLOR_WINDOWTEXT)),
_data1(0),
_data2(0)
{
	_col[0] = col0;
	_col[1] = col1;
	_col[2] = col2;
}



