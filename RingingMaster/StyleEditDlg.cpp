// StyleEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "RingingMaster.h"
#include "StyleManager.h"
#include "UserMessages.h"
#include "WinPos.h"
#include "StyleEditDlg.h"  

#include "StyleLines.h"  
#include "StyleLinesMinus.h"  
#include "StyleLayout.h"  
#include "StyleView.h"  
#include "StyleGeneral.h"  


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StyleEditDlg

IMPLEMENT_DYNAMIC(StyleEditDlg, PropertySheetBase)

StyleEditDlg::StyleEditDlg(Style &style, CWnd* pParent, int number, 
						   StyleLinesType type, CString title) :
PropertySheetBase("Style Edit", pParent, 120),
_style(style),		   //create a copy
_originalStyle(style), //keep ref to original
_title(title),
_parent(pParent),
_originalName(style._name),
_type(type)
{

	//set up the dialog stuff	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	_lines		= new StyleLines(_style, number, type);                             
	_linesMinus	= new StyleLinesMinus(_style, number, type);                             
	_layout		= new StyleLayout(_style, type);                             
	_view		= new StyleView(_style, type);                             
	_general	= new StyleGeneral(_style, type);                             
	
	AddPageToTree (NULL, _lines, -1, 2);
	AddPageToTree (NULL, _linesMinus, -1, 2);
	AddPageToTree (NULL, _layout, -1, 2);
	AddPageToTree (NULL, _view, -1, 2);
	AddPageToTree (NULL, _general, -1, 2);

	SetActivePage(_lines);					  

}

StyleEditDlg::~StyleEditDlg()
{
	delete _lines;
	delete _linesMinus;
	delete _layout;
	delete _view;
	delete _general;

}
	 
BEGIN_MESSAGE_MAP(StyleEditDlg, PropertySheetBase)
	//{{AFX_MSG_MAP(StyleEditDlg)
		ON_BN_CLICKED(ID_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// StyleEditDlg message handlers

BOOL StyleEditDlg::OnInitDialog()
{
	BOOL bResult = PropertySheetBase::OnInitDialog();
 
	setConstantText(_style._name);

	WinPos::Load(this, "StyleEditDlg", false);


	CWnd* okBtn = GetDlgItem(IDOK);
	if (okBtn && ::IsWindow(okBtn->m_hWnd))
	{
		//get the rect of the OK btn
  		CRect okBtnRect;	
		okBtn->GetWindowRect (okBtnRect);
		ScreenToClient (okBtnRect);

		_apply.ShowWindow(SW_SHOW);
		CRect applyBtnRect = (okBtnRect - CPoint(85,0));

		_applyBtn.Create("Apply", WS_CHILD|BS_CHECKBOX|WS_VISIBLE, applyBtnRect , this, ID_APPLY);
		_applyBtn.m_bDrawFocus = FALSE;
		_applyBtn.SetTooltip("Apply");
		_applyBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	}

	return bResult;
}			 

void StyleEditDlg::OnApply() 
{
	PressButton(PSBTN_APPLYNOW);
}

BOOL StyleEditDlg::DestroyWindow() 
{
	WinPos::Save(this, "StyleEditDlg");

	return PropertySheetBase::DestroyWindow();
}

void StyleEditDlg::Apply()
{

 	if ((styleManager()->isStyleUnique(_style._name))||
		((_type == slt_edit)&&(_style._name == _originalName))||
		(_type == slt_normal)||
		(_type == slt_preview)||
		(_type == slt_callchange)||
		(_type == slt_simulator)||
		(_type == slt_crib))

	{
		
	}
	else
	{
		AfxMessageBox("Cant change the name, as it is not unique.\nThe rest of the style changes have been applyed.");
		_style._name = _originalName;
	}

	_originalStyle = _style;
	_parent->PostMessage(WM_UPDATE_METHOD, NULL, NULL);
}
	 
