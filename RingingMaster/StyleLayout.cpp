// StyleLayout.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "StyleLayout.h"
#include "StyleEditDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// StyleLayout property page

StyleLayout::StyleLayout(Style &style, StyleLinesType type) : 
CBCGPPropertyPage(StyleLayout::IDD),
_style(style),
_type(type)
{
	//{{AFX_DATA_INIT(StyleLayout)
	_height = 1;
	_leads = 2;
	_left = 1;
	_top = 1;
	_width = 1;
	//}}AFX_DATA_INIT
}

StyleLayout::~StyleLayout()
{
}

void StyleLayout::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StyleLayout)
	DDX_Control(pDX, IDC_STATIC_LEADS, _staticLeads);
	DDX_Control(pDX, IDC_LEADS_CHECK, _bLeads);
	DDX_Control(pDX, IDC_LEADS, _editLeads);
	DDX_Control(pDX, IDC_WIDTH_SPIN, _spWidth);
	DDX_Control(pDX, IDC_TOP_SPIN, _spTop);
	DDX_Control(pDX, IDC_LEFT_SPIN, _spLeft);
	DDX_Control(pDX, IDC_LEADS_SPIN, _spLeads);
	DDX_Control(pDX, IDC_HEIGHT_SPIN, _spHeight);
	DDX_Text(pDX, IDC_HEIGHT, _height);
	DDV_MinMaxInt(pDX, _height, 1, 500);
	DDX_Text(pDX, IDC_LEADS, _leads);
	DDV_MinMaxInt(pDX, _leads, 1, 10000);
	DDX_Text(pDX, IDC_LEFT, _left);
	DDV_MinMaxInt(pDX, _left, 1, 500);
	DDX_Text(pDX, IDC_TOP, _top);
	DDV_MinMaxInt(pDX, _top, 1, 500);
	DDX_Text(pDX, IDC_WIDTH, _width);
	DDV_MinMaxInt(pDX, _width, 1, 500);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StyleLayout, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(StyleLayout)
	ON_EN_CHANGE(IDC_TOP, modify)
	ON_BN_CLICKED(IDC_LEADS_CHECK, OnLeadsCheck)
	ON_EN_CHANGE(IDC_LEFT, modify)
	ON_EN_CHANGE(IDC_WIDTH, modify)
	ON_EN_CHANGE(IDC_HEIGHT, modify)
	ON_EN_CHANGE(IDC_LEADS, modify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()	  


 BOOL StyleLayout::OnApply() 
{
	((StyleEditDlg*)GetParent())->Apply();
		
	return CBCGPPropertyPage::OnApply();
}

 BOOL StyleLayout::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_spLeft.SetRange32(1,500);
	_spTop.SetRange32(1,500);
	_spWidth.SetRange32(1,500);
	_spHeight.SetRange32(1,500);
	_spLeads.SetRange32(1,100);

	_left = _style._left;
	_top = _style._top;
	_width = _style._width;
	_height = _style._height;
	if (_style._leadsPerColumn == SHRT_MAX)
	{
		_bLeads.SetCheck(TRUE);
		_leads = 2 ;
	}
	else 
	{
		_bLeads.SetCheck(FALSE);   
		_leads = _style._leadsPerColumn;
	}

	DoLeadsCheck();

	if (_type == slt_drawAMethod)
	{
		GetDlgItem(IDC_STATIC_LINE_LAYOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LAYOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_METHOD_TEXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEADS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS_SPIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_HEIGHT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WIDTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEFT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TOP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEFT_SPIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TOP_SPIN)->ShowWindow(SW_HIDE);		
	}
	else if (_type == slt_callchange)
	{
		GetDlgItem(IDC_STATIC_LINE_LAYOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LAYOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_METHOD_TEXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEADS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS_SPIN)->ShowWindow(SW_HIDE);
	}
	else if (_type == slt_simulator)
	{
	}
	else if (_type == slt_crib)
	{
		GetDlgItem(IDC_STATIC_LINE_LAYOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_METHOD_TEXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LAYOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEADS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LEADS_SPIN)->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control

}

 void StyleLayout::OnLeadsCheck() 
{
	modify();
	DoLeadsCheck();
}

 void StyleLayout::DoLeadsCheck() 
{
	_editLeads.EnableWindow(!_bLeads.GetCheck());
	_spLeads.EnableWindow(!_bLeads.GetCheck());
	_staticLeads.EnableWindow(!_bLeads.GetCheck());
}

 BOOL StyleLayout::OnKillActive() 
{
	if (CBCGPPropertyPage::OnKillActive())
	{

		_style._left = _left;
		_style._top = _top;
		_style._width = _width;
		_style._height = _height;

		if (_bLeads.GetCheck())
		{
			_style._leadsPerColumn = SHRT_MAX;
		}
		else
		{
			_style._leadsPerColumn = _leads;
			
		}

		return TRUE;
	}

	return FALSE;
}


void StyleLayout::modify() 
{
	SetModified();	
}






