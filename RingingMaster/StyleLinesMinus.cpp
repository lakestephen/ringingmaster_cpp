// StyleLinesMinus.cpp : implementation file
//
#include "stdafx.h"
#include "RingingMaster.h"
#include "StyleLinesMinus.h"
#include "StyleEditDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//IMPLEMENT_DYNCREATE(StyleLinesMinus, CBCGPPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// StyleLinesMinus property page

StyleLinesMinus::StyleLinesMinus(Style &style, int number, StyleLinesType type) : 
CBCGPPropertyPage(StyleLinesMinus::IDD),
_style(style),
_number(number),
_type(type)
{
	//{{AFX_DATA_INIT(StyleLinesMinus)
	//}}AFX_DATA_INIT
	_overrideMinus[0] = &_overrideMinus0;
	_overrideMinus[1] = &_overrideMinus1;
	_overrideMinus[2] = &_overrideMinus2;
	_overrideMinus[3] = &_overrideMinus3;
	
	_btnLineColor[0] = &_btnLineColor0;
	_btnLineColor[1] = &_btnLineColor1;
	_btnLineColor[2] = &_btnLineColor2;
	_btnLineColor[3] = &_btnLineColor3;
	
	_btnTextColor[0] = &_btnTextColor0;
	_btnTextColor[1] = &_btnTextColor1;
	_btnTextColor[2] = &_btnTextColor2;
	_btnTextColor[3] = &_btnTextColor3;
	
	_cbLineWidth[0] = &_cbLineWidth0;
	_cbLineWidth[1] = &_cbLineWidth1;
	_cbLineWidth[2] = &_cbLineWidth2;
	_cbLineWidth[3] = &_cbLineWidth3;

	for (int i=0;i<4;i++)
	{
		_btnLineColor[i]->EnableOtherButton("More Colors...");
		_btnLineColor[i]->EnableAutomaticButton("No Line", GetSysColor(COLOR_3DFACE));
		_btnTextColor[i]->EnableOtherButton("More Colors...");
		_btnTextColor[i]->EnableAutomaticButton("No Text", GetSysColor(COLOR_3DFACE));
	} 	
}

StyleLinesMinus::~StyleLinesMinus()
{
}

void StyleLinesMinus::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StyleLinesMinus)
	DDX_Control(pDX, IDC_TENOR_NOTE_MINUS, m_tenorNote);
	DDX_Control(pDX, IDC_OVERRIDE0, _overrideMinus0);
	DDX_Control(pDX, IDC_OVERRIDE1, _overrideMinus1);
	DDX_Control(pDX, IDC_OVERRIDE2, _overrideMinus2);
	DDX_Control(pDX, IDC_OVERRIDE3, _overrideMinus3);
	DDX_Control(pDX, IDC_TEXT_COLOR_0, _btnTextColor0);
	DDX_Control(pDX, IDC_TEXT_COLOR_1, _btnTextColor1);
	DDX_Control(pDX, IDC_TEXT_COLOR_2, _btnTextColor2);
	DDX_Control(pDX, IDC_TEXT_COLOR_3, _btnTextColor3);
	DDX_Control(pDX, IDC_LINE_WIDTH_0, _cbLineWidth0);
	DDX_Control(pDX, IDC_LINE_WIDTH_1, _cbLineWidth1);
	DDX_Control(pDX, IDC_LINE_WIDTH_2, _cbLineWidth2);
	DDX_Control(pDX, IDC_LINE_WIDTH_3, _cbLineWidth3);
	DDX_Control(pDX, IDC_LINES_COLOR_0, _btnLineColor0);
	DDX_Control(pDX, IDC_LINES_COLOR_1, _btnLineColor1);
	DDX_Control(pDX, IDC_LINES_COLOR_2, _btnLineColor2);
	DDX_Control(pDX, IDC_LINES_COLOR_3, _btnLineColor3);
	//}}AFX_DATA_MAP		  
}


BEGIN_MESSAGE_MAP(StyleLinesMinus, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(StyleLinesMinus)
	ON_WM_VSCROLL()	
	ON_BN_CLICKED(IDC_OVERRIDE0, modify)
	ON_BN_CLICKED(IDC_OVERRIDE1, modify)
	ON_BN_CLICKED(IDC_OVERRIDE2, modify)
	ON_BN_CLICKED(IDC_OVERRIDE3, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_0, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_1, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_2, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_3, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_0, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_1, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_2, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_3, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_0, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_1, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_2, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_3, modify)
	ON_WM_MOUSEWHEEL()

	ON_UPDATE_COMMAND_UI_RANGE(IDC_LINE_WIDTH_0,IDC_LINE_WIDTH_3, OnUpdateLineWidth)		   	
	ON_UPDATE_COMMAND_UI_RANGE(IDC_LINES_COLOR_0,IDC_LINES_COLOR_3, OnUpdateLineColor)		   	
	ON_UPDATE_COMMAND_UI_RANGE(IDC_TEXT_COLOR_0,IDC_TEXT_COLOR_3, OnUpdateTextColor)		   	
	

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void StyleLinesMinus::dataToCtrls()
{
	CString str;
	for (int i=0;i<4;i++)
	{	
		//line color
		_btnLineColor[i]->SetColor(_style.getMinusLine(i)->_lineColor);
		//line width
		_cbLineWidth[i]->SetCurSel(_style.getMinusLine(i)->_lineWidth - 1);
		//text color
		_btnTextColor[i]->SetColor(_style.getMinusLine(i)->_textColor);
		//override flag
		_overrideMinus[i]->SetCheck(_style._useMinus[i]);
	}
}

void StyleLinesMinus::ctrlsToData()
{
	for (int i=0;i<4;i++)
	{
		//line color
		_style.getMinusLine(i)->_lineColor = _btnLineColor[i]->GetColor();
		//line width
		_style.getMinusLine(i)->_lineWidth = _cbLineWidth[i]->GetCurSel() + 1;
		//text color
		_style.getMinusLine(i)->_textColor = _btnTextColor[i]->GetColor();
		//override flag
		_style._useMinus[i] = _overrideMinus[i]->GetCheck();
	}
}

void StyleLinesMinus::OnUpdateLineWidth(CCmdUI* pCmdUI) 
{
	int index = pCmdUI->m_nID - IDC_LINE_WIDTH_0;
	BOOL check = _overrideMinus[index]->GetCheck();
	_cbLineWidth[index]->EnableWindow(_btnLineColor[index]->GetColor() != -1);
	_cbLineWidth[index]->ShowWindow(check?SW_SHOW:SW_HIDE);
}

void StyleLinesMinus::OnUpdateLineColor(CCmdUI* pCmdUI) 
{
	int index = pCmdUI->m_nID - IDC_LINES_COLOR_0;
	BOOL check = _overrideMinus[index]->GetCheck();
	_btnLineColor[index]->ShowWindow((check)?SW_SHOW:SW_HIDE);
}

void StyleLinesMinus::OnUpdateTextColor(CCmdUI* pCmdUI) 
{
	int index = pCmdUI->m_nID - IDC_TEXT_COLOR_0;
	BOOL check = _overrideMinus[index]->GetCheck();
	_btnTextColor[index]->ShowWindow((check)?SW_SHOW:SW_HIDE);
}                    

BOOL StyleLinesMinus::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	//set the vert lines
	CRect rect;
	GetDlgItem(IDC_LINES_STATIC_LINE_TOP1)->GetClientRect(&rect);
	rect.bottom += (23 * ((_number>4)?4:_number)); 
	rect.right += 2;
	GetDlgItem(IDC_LINES_STATIC_LINE_TOP1)->SetWindowPos(NULL,0,0,
		rect.right, rect.bottom,SWP_NOMOVE|SWP_NOZORDER|SWP_NOREPOSITION);

	GetDlgItem(IDC_LINES_STATIC_LINE_TOP2)->GetClientRect(&rect);
	rect.bottom += (23 * ((_number>4)?4:_number)); 
	rect.right += 2;
	GetDlgItem(IDC_LINES_STATIC_LINE_TOP2)->SetWindowPos(NULL,0,0,
		rect.right, rect.bottom,SWP_NOMOVE|SWP_NOZORDER|SWP_NOREPOSITION);
	
	//set up the scroll bar
	if (_number <= 4)
	{
		for (int i=_number;i<4;i++)
		{
			hideLine(i);
		} 
		GetDlgItem(IDC_LINES_STATIC_LINE_LEFT)->ShowWindow(SW_HIDE);			
	}


	if (_type == slt_drawAMethod)
	{
		//hide text bits show line
		for (int i=0;i<4;i++)
		{
			_btnLineColor[i]->EnableAutomaticButton("",0,FALSE);
		}
	}

	if (_type == slt_edit) 
	{
		m_tenorNote.SetWindowText("NOTE: When editing a style from the style library, the tenor styles are ONLY editable from this page.");
	}


	//set up all other ctrls	
	dataToCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL StyleLinesMinus::OnKillActive() 
{
	ctrlsToData();
	
	return CBCGPPropertyPage::OnKillActive();
}

void StyleLinesMinus::hideLine(int line)
{
	_btnTextColor[line]->ShowWindow(SW_HIDE);
	_cbLineWidth[line]->ShowWindow(SW_HIDE);
	_btnLineColor[line]->ShowWindow(SW_HIDE);
}


BOOL StyleLinesMinus::OnApply() 
{
	((StyleEditDlg*)GetParent())->Apply() ;

	return CBCGPPropertyPage::OnApply();
}


void StyleLinesMinus::modify() 
{
	SetModified();	
}

BOOL StyleLinesMinus::OnSetActive() 
{
	dataToCtrls();
	
	return CBCGPPropertyPage::OnSetActive();
}



