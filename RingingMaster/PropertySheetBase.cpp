// MyPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PropertySheetBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropertySheetBase

IMPLEMENT_DYNAMIC(PropertySheetBase, CBCGPPropertySheet)

PropertySheetBase::PropertySheetBase(CString caption, CWnd* parent, int width)
 : CBCGPPropertySheet(caption, parent), 
 _caption(caption)
{
	SetIconsList (IDB_TREE_ICONS, 16);
	SetLook (CBCGPPropertySheet::PropSheetLook_Tree, width);
}

PropertySheetBase::~PropertySheetBase()
{
}


BEGIN_MESSAGE_MAP(PropertySheetBase, CBCGPPropertySheet)
	//{{AFX_MSG_MAP(PropertySheetBase)
		ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PropertySheetBase message handlers


LRESULT PropertySheetBase::OnKickIdle(WPARAM, LPARAM)
{
	if (::IsWindow(m_hWnd))
		UpdateDialogControls(this, FALSE);

	for (int i=0;i<GetPageCount();i++)
	{
		CPropertyPage* page = GetPage(i);
		if (page && ::IsWindow(page->m_hWnd))
			page->UpdateDialogControls(page, FALSE);
	}

	return 0;
}     

BOOL PropertySheetBase::OnInitDialog()
{
	BOOL bResult = CBCGPPropertySheet::OnInitDialog();

	setupButton(IDCANCEL, _cancle, "Cancel");
	setupButton(IDOK, _ok, "Ok");
	setupButton(IDHELP, _help, "Help");
	setupButton(ID_APPLY_NOW, _apply, "Apply");

	return bResult;
}

BOOL PropertySheetBase::setupButton(UINT id, CBCGPButton& button, CString tooltip)
{
	CWnd* origBtn = GetDlgItem(id);
	if (origBtn && ::IsWindow(origBtn->m_hWnd))
	{
		//collect info about the original button
  		CRect origBtnRect;	
		origBtn->GetWindowRect (origBtnRect);
		ScreenToClient (origBtnRect);
		DWORD dwStyle = WS_CHILD|BS_CHECKBOX;
		if (origBtn->GetStyle() & WS_VISIBLE)
			dwStyle |= WS_VISIBLE;
		CString origBtnText;
		origBtn->GetWindowText(origBtnText);

		//destroy the original button
		origBtn->DestroyWindow();

		//make a new one.
		button.Create(origBtnText, dwStyle, origBtnRect, this, id);
//		button.SetImage(imageID);
		button.m_bDrawFocus = FALSE;
		button.SetTooltip(tooltip);
		button.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		return TRUE;
	}          	

	return FALSE;
}

void PropertySheetBase::setConstantText(CString name)
{
	ASSERT(::IsWindow(m_hWnd));
	//to2do at some point, add and update a gradient bar with this info.
	if (::IsWindow(m_hWnd))
		SetWindowText(_caption + " - " + name);
	
}