// ListBoxHS.cpp : implementation file
//

#include "stdafx.h"

#include "ListBoxHS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ListBoxHS

ListBoxHS::ListBoxHS()
{
}

ListBoxHS::~ListBoxHS()
{
}


BEGIN_MESSAGE_MAP(ListBoxHS, CListBox)
	//{{AFX_MSG_MAP(ListBoxHS)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ListBoxHS message handlers

int ListBoxHS::AddStringHS(CString string)
{
	int ret = AddString(string);
	
	CClientDC dc(this);

	CFont * oldFont = dc.SelectObject(GetFont());

	int entrySize = dc.GetOutputTextExtent(string).cx;

	if (GetHorizontalExtent() < entrySize +10)
		SetHorizontalExtent(entrySize + 10);

	dc.SelectObject(oldFont);

	return ret;
}

void ListBoxHS::ResetContentHS()
{
	ResetContent();
	SetHorizontalExtent(10);
}
