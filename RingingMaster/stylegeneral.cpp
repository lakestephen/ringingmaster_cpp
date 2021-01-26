// StyleGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "StyleGeneral.h"
#include "StyleEditDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StyleGeneral property page


StyleGeneral::StyleGeneral(Style &style, StyleLinesType type) : 
CBCGPPropertyPage(StyleGeneral::IDD),
_style(style),
_type(type)
{
	//{{AFX_DATA_INIT(StyleGeneral)
	_bellEdit = 1;
	_leadEdit = 1;
	_styleName = _T("");
	//}}AFX_DATA_INIT

	_bellColor.EnableOtherButton("More Colors...");
	_bellColor.EnableAutomaticButton("No Bell Lines", GetSysColor(COLOR_3DFACE));
	_leadColor.EnableOtherButton("More Colors...");
	_leadColor.EnableAutomaticButton("No Lead Lines", GetSysColor(COLOR_3DFACE));
}

StyleGeneral::~StyleGeneral()
{
}

void StyleGeneral::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StyleGeneral)
	DDX_Control(pDX, IDC_STYLE_NAME, _styleNameEdit);
	DDX_Control(pDX, IDC_BELL_EDIT, _bellCtrl);
	DDX_Control(pDX, IDC_LEAD_EDIT, _leadsCtrl);
	DDX_Control(pDX, IDC_LEAD_SPIN, _scLeadEdit);
	DDX_Control(pDX, IDC_LEAD_COLOR, _leadColor);
	DDX_Control(pDX, IDC_BELL_SPIN, _scBellEdit);
	DDX_Control(pDX, IDC_BELL_COLOR, _bellColor);
	DDX_Text(pDX, IDC_BELL_EDIT, _bellEdit);
	DDV_MinMaxInt(pDX, _bellEdit, 1, 3);
	DDX_Text(pDX, IDC_LEAD_EDIT, _leadEdit);
	DDV_MinMaxInt(pDX, _leadEdit, 1, 3);
	DDX_Text(pDX, IDC_STYLE_NAME, _styleName);
	DDV_MaxChars(pDX, _styleName, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StyleGeneral, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(StyleGeneral)
	ON_BN_CLICKED(IDC_BELL_COLOR, OnShowBell)
	ON_BN_CLICKED(IDC_LEAD_COLOR, OnShowLead)
	ON_EN_CHANGE(IDC_BELL_EDIT, modify)
	ON_EN_CHANGE(IDC_LEAD_EDIT, modify)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StyleGeneral message handlers

BOOL StyleGeneral::OnApply() 
{
	((StyleEditDlg*)GetParent())->Apply() ;
	
	return CBCGPPropertyPage::OnApply();
}


BOOL StyleGeneral::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	if (_type != slt_edit)
	{
		GetDlgItem(IDC_STATIC_STYLE_NAME)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_STATIC_LINE_STYLE_NAME)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_STYLE_NAME)->ShowWindow(SW_HIDE);		
	
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void StyleGeneral::OnShowBell() 
{
	modify();
	DoShowBell();
}

void StyleGeneral::DoShowBell()
{
	_bellCtrl.EnableWindow(_bellColor.GetColor() != -1);
	_scBellEdit.EnableWindow(_bellColor.GetColor() != -1);
}


void StyleGeneral::OnShowLead() 
{
	modify();
	DoShowLead();
}

void StyleGeneral::DoShowLead()
{
	_leadsCtrl.EnableWindow(_leadColor.GetColor() != -1);
	_scLeadEdit.EnableWindow(_leadColor.GetColor() != -1);
}
 
BOOL StyleGeneral::OnKillActive() 
{
	if (CBCGPPropertyPage::OnKillActive())
	{	
		_style._gridWidthV = _bellEdit;
		_style._gridColorV = _bellColor.GetColor();

		_style._gridWidthH = _leadEdit;
		_style._gridColorH = _leadColor.GetColor();

		_style._name = _styleName;
		return TRUE;
	}

	return FALSE;
}

void StyleGeneral::modify() 
{
	SetModified();	
}



BOOL StyleGeneral::OnSetActive() 
{
	_bellEdit = _style._gridWidthV;
	_bellColor.SetColor(_style._gridColorV);
	_scBellEdit.SetRange32(1,3);
	
	_leadEdit = _style._gridWidthH;
	_leadColor.SetColor(_style._gridColorH);
	_scLeadEdit.SetRange32(1,3);

	_styleName = _style._name;

	UpdateData(FALSE);	

	DoShowBell();
	DoShowLead();

	return CBCGPPropertyPage::OnSetActive();
}
