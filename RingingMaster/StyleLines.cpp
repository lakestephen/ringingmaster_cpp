// StyleLines.cpp : implementation file
//
#include "stdafx.h"
#include "RingingMaster.h"
#include "StyleLines.h"
#include "StyleEditDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//IMPLEMENT_DYNCREATE(StyleLines, CBCGPPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// StyleLines property page

StyleLines::StyleLines(Style &style, int number, StyleLinesType type) : 
CBCGPPropertyPage(StyleLines::IDD),
_style(style),
_number(number),
_type(type)
{
	//{{AFX_DATA_INIT(StyleLines)
	//}}AFX_DATA_INIT
	
	_btnLineColor[0] = &_btnLineColor0;
	_btnLineColor[1] = &_btnLineColor1;
	_btnLineColor[2] = &_btnLineColor2;
	_btnLineColor[3] = &_btnLineColor3;
	_btnLineColor[4] = &_btnLineColor4;
	_btnLineColor[5] = &_btnLineColor5;
	_btnLineColor[6] = &_btnLineColor6;
	_btnLineColor[7] = &_btnLineColor7;
	
	_btnTextColor[0] = &_btnTextColor0;
	_btnTextColor[1] = &_btnTextColor1;
	_btnTextColor[2] = &_btnTextColor2;
	_btnTextColor[3] = &_btnTextColor3;
	_btnTextColor[4] = &_btnTextColor4;
	_btnTextColor[5] = &_btnTextColor5;
	_btnTextColor[6] = &_btnTextColor6;
	_btnTextColor[7] = &_btnTextColor7;
	
	_cbLineWidth[0] = &_cbLineWidth0;
	_cbLineWidth[1] = &_cbLineWidth1;
	_cbLineWidth[2] = &_cbLineWidth2;
	_cbLineWidth[3] = &_cbLineWidth3;
	_cbLineWidth[4] = &_cbLineWidth4;
	_cbLineWidth[5] = &_cbLineWidth5;
	_cbLineWidth[6] = &_cbLineWidth6;
	_cbLineWidth[7] = &_cbLineWidth7;
	
	_staticLinesNum[0] = &_staticLinesNum0;
	_staticLinesNum[1] = &_staticLinesNum1;
	_staticLinesNum[2] = &_staticLinesNum2;
	_staticLinesNum[3] = &_staticLinesNum3;
	_staticLinesNum[4] = &_staticLinesNum4;
	_staticLinesNum[5] = &_staticLinesNum5;
	_staticLinesNum[6] = &_staticLinesNum6;
	_staticLinesNum[7] = &_staticLinesNum7;

	for (int i=0;i<8;i++)
	{
		_btnLineColor[i]->EnableOtherButton("More Colors...");
		_btnLineColor[i]->EnableAutomaticButton("No Line", GetSysColor(COLOR_3DFACE));
		_btnTextColor[i]->EnableOtherButton("More Colors...");
		_btnTextColor[i]->EnableAutomaticButton("No Text", GetSysColor(COLOR_3DFACE));
	} 	
}

StyleLines::~StyleLines()
{
}

void StyleLines::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StyleLines)
	DDX_Control(pDX, IDC_TENOR_NOTE, m_tenorNote);
	DDX_Control(pDX, IDC_LINES_NUM_0, _staticLinesNum0);
	DDX_Control(pDX, IDC_LINES_NUM_1, _staticLinesNum1);
	DDX_Control(pDX, IDC_LINES_NUM_2, _staticLinesNum2);
	DDX_Control(pDX, IDC_LINES_NUM_3, _staticLinesNum3);
	DDX_Control(pDX, IDC_LINES_NUM_4, _staticLinesNum4);
	DDX_Control(pDX, IDC_LINES_NUM_5, _staticLinesNum5);
	DDX_Control(pDX, IDC_LINES_NUM_6, _staticLinesNum6);
	DDX_Control(pDX, IDC_LINES_NUM_7, _staticLinesNum7);
	DDX_Control(pDX, IDC_LINES_SCROLL, _sbLines);
	DDX_Control(pDX, IDC_TEXT_COLOR_0, _btnTextColor0);
	DDX_Control(pDX, IDC_TEXT_COLOR_1, _btnTextColor1);
	DDX_Control(pDX, IDC_TEXT_COLOR_2, _btnTextColor2);
	DDX_Control(pDX, IDC_TEXT_COLOR_3, _btnTextColor3);
	DDX_Control(pDX, IDC_TEXT_COLOR_4, _btnTextColor4);
	DDX_Control(pDX, IDC_TEXT_COLOR_5, _btnTextColor5);
	DDX_Control(pDX, IDC_TEXT_COLOR_6, _btnTextColor6);
	DDX_Control(pDX, IDC_TEXT_COLOR_7, _btnTextColor7);
	DDX_Control(pDX, IDC_LINE_WIDTH_0, _cbLineWidth0);
	DDX_Control(pDX, IDC_LINE_WIDTH_1, _cbLineWidth1);
	DDX_Control(pDX, IDC_LINE_WIDTH_2, _cbLineWidth2);
	DDX_Control(pDX, IDC_LINE_WIDTH_3, _cbLineWidth3);
	DDX_Control(pDX, IDC_LINE_WIDTH_4, _cbLineWidth4);
	DDX_Control(pDX, IDC_LINE_WIDTH_5, _cbLineWidth5);
	DDX_Control(pDX, IDC_LINE_WIDTH_6, _cbLineWidth6);
	DDX_Control(pDX, IDC_LINE_WIDTH_7, _cbLineWidth7);
	DDX_Control(pDX, IDC_LINES_COLOR_0, _btnLineColor0);
	DDX_Control(pDX, IDC_LINES_COLOR_1, _btnLineColor1);
	DDX_Control(pDX, IDC_LINES_COLOR_2, _btnLineColor2);
	DDX_Control(pDX, IDC_LINES_COLOR_3, _btnLineColor3);
	DDX_Control(pDX, IDC_LINES_COLOR_4, _btnLineColor4);
	DDX_Control(pDX, IDC_LINES_COLOR_5, _btnLineColor5);
	DDX_Control(pDX, IDC_LINES_COLOR_6, _btnLineColor6);
	DDX_Control(pDX, IDC_LINES_COLOR_7, _btnLineColor7);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_LINE_FONT, _lineFontBtn);
}


BEGIN_MESSAGE_MAP(StyleLines, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(StyleLines)
	ON_WM_VSCROLL()	
	ON_BN_CLICKED(IDC_EDIT_LINE_FONT, OnEditLineFont)
	ON_BN_CLICKED(IDC_TEXT_COLOR_0, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_1, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_2, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_3, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_4, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_5, modify)
	ON_BN_CLICKED(IDC_TEXT_COLOR_6, modify) 
	ON_BN_CLICKED(IDC_TEXT_COLOR_7, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_0, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_1, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_2, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_3, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_4, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_5, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_6, modify)
	ON_CBN_SELCHANGE(IDC_LINE_WIDTH_7, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_0, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_1, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_2, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_3, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_4, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_5, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_6, modify)
	ON_BN_CLICKED(IDC_LINES_COLOR_7, modify)
	ON_WM_MOUSEWHEEL()
	ON_UPDATE_COMMAND_UI_RANGE(IDC_LINE_WIDTH_0,IDC_LINE_WIDTH_7, OnUpdateLineWidth)		   	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void StyleLines::dataToCtrls()
{
	CString str;
	for (int i=0, j=_sbLines.GetScrollPos();i<8;i++,j++)
	{	
		//if we are in edit mode, allow the user to edit the normal lines, else 
		// allow them to edit the line that is dependant on the tenot/number of bells

		Style::StyleLine* stLine = NULL;
		if (_type == slt_edit)
			stLine = _style.getNormalLine(j);
		else
			stLine = _style.getLine(j, _number);

		//bell number 
		int tenorNumber = _style.getTenorNumer(j, _number);

		CString label;
		if ((tenorNumber == -1)||(_type == slt_edit))
			label = GlobalFunctions::bellNumbersToChar(j+1);
		else
		{
			if (tenorNumber == 0) 
				label = "Tenor";
			else
				label.Format("Tenor-%d",tenorNumber);
		}
		_staticLinesNum[i]->SetWindowText(label);

		//line color
		_btnLineColor[i]->SetColor(stLine->_lineColor);
		//line width
		_cbLineWidth[i]->SetCurSel(stLine->_lineWidth - 1);
		_cbLineWidth[i]->EnableWindow(stLine->_lineColor != -1);

		//text color
		_btnTextColor[i]->SetColor(stLine->_textColor);
	}

}

void StyleLines::ctrlsToData()
{
	for (int i=0, j=_sbLines.GetScrollPos();i<8;i++,j++)
	{
		//if we are in edit mode, allow the user to edit the normal lines, else 
		// allow them to edit the line that is dependant on the tenot/number of bells

		Style::StyleLine* stLine = NULL;
		if (_type == slt_edit)
			stLine = _style.getNormalLine(j);
		else
			stLine = _style.getLine(j, _number);

		//line color
		stLine->_lineColor = _btnLineColor[i]->GetColor();
		//line width
		stLine->_lineWidth = _cbLineWidth[i]->GetCurSel() + 1;
		//text color
		stLine->_textColor = _btnTextColor[i]->GetColor();
	}
}

void StyleLines::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	ctrlsToData();

	SCROLLINFO ScrollInfo;
	_sbLines.GetScrollInfo(&ScrollInfo);  // get information about the scroll
	int pos = _sbLines.GetScrollPos() ;
	switch(nSBCode)  
	{ 
    case SB_LINEDOWN:       //Scrolls one line down. 
		pos++;  
		break;
    case SB_LINEUP:         //Scrolls one line up. 
		pos--;      
		break;
    case SB_PAGEDOWN:       //Scrolls one page down. 
		pos += ScrollInfo.nPage;      
		break;
    case SB_PAGEUP:         //Scrolls one page up. 
		pos -= ScrollInfo.nPage;      
		break;
    case SB_THUMBTRACK:     //The user is dragging the scroll box. This message is sent repeatedly until the user releases the mouse button. The nPos parameter indicates the position that the scroll box has been dragged to. 
		pos = nPos;      
		break;
	}
	_sbLines.SetScrollPos(pos);

	dataToCtrls();
	
	CBCGPPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void StyleLines::OnUpdateLineWidth(CCmdUI* pCmdUI) 
{
	int index = pCmdUI->m_nID - IDC_LINE_WIDTH_0;
	_cbLineWidth[index]->EnableWindow(_btnLineColor[index]->GetColor() != -1);
}

												  
BOOL StyleLines::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	
	_lineFontBtn.SetImage(IDB_FONT);
	_lineFontBtn.m_bDrawFocus = FALSE;
	_lineFontBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	//set the vert lines
	CRect rect;
	GetDlgItem(IDC_LINES_STATIC_LINE_TOP1)->GetClientRect(&rect);
	rect.bottom += (23 * ((_number>8)?8:_number)); 
	rect.right += 2;
	GetDlgItem(IDC_LINES_STATIC_LINE_TOP1)->SetWindowPos(NULL,0,0,
		rect.right, rect.bottom,SWP_NOMOVE|SWP_NOZORDER|SWP_NOREPOSITION);

	GetDlgItem(IDC_LINES_STATIC_LINE_TOP2)->GetClientRect(&rect);
	rect.bottom += (23 * ((_number>8)?8:_number)); 
	rect.right += 2;
	GetDlgItem(IDC_LINES_STATIC_LINE_TOP2)->SetWindowPos(NULL,0,0,
		rect.right, rect.bottom,SWP_NOMOVE|SWP_NOZORDER|SWP_NOREPOSITION);


	//set up the scroll bar
	if (_number <= 8)
	{
		_sbLines.ShowWindow(SW_HIDE);
		for (int i=_number;i<8;i++)
		{
			hideLine(i);
		} 
		GetDlgItem(IDC_LINES_STATIC_LINE_LEFT)->ShowWindow(SW_HIDE);			
	}
	else 
	{
		SCROLLINFO ScrollInfo;
		ScrollInfo.cbSize = sizeof(ScrollInfo);     // size of this structure
		ScrollInfo.fMask = SIF_ALL;                 // parameters to set
		ScrollInfo.nMin = 0;                        // minimum scrolling position
		ScrollInfo.nMax = _number - 7;			// maximum scrolling position
		ScrollInfo.nPage = 2;						// the page size of the scroll box
		ScrollInfo.nPos = 0;						// initial position of the scroll box
		ScrollInfo.nTrackPos = 0;                   // immediate position of a scroll box that the user is dragging
		_sbLines.SetScrollInfo(&ScrollInfo);
	}	


	if (_type == slt_edit) 
	{
		m_tenorNote.ShowWindow(SW_HIDE);
	}

	if (_type == slt_drawAMethod)
	{
		//hide text bits show line
		for (int i=0;i<8;i++)
		{
			_btnLineColor[i]->EnableAutomaticButton("",0,FALSE);
		}
	}

	//set up all other ctrls	
	dataToCtrls();

	updateTooltips();

	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL StyleLines::OnKillActive() 
{
	ctrlsToData();
	
	return CBCGPPropertyPage::OnKillActive();
}

void StyleLines::hideLine(int line)
{
	_btnTextColor[line]->ShowWindow(SW_HIDE);
	_cbLineWidth[line]->ShowWindow(SW_HIDE);
	_btnLineColor[line]->ShowWindow(SW_HIDE);
	_staticLinesNum[line]->ShowWindow(SW_HIDE);
}


BOOL StyleLines::OnApply() 
{
	((StyleEditDlg*)GetParent())->Apply() ;

	return CBCGPPropertyPage::OnApply();
}

void StyleLines::OnEditLineFont() 
{
	if (_style._numbersFont.editFont(this) == true)
	{
		modify();
		updateTooltips();
	}
}

void StyleLines::updateTooltips()
{
	_lineFontBtn.SetTooltip("Edit Lines Font (" + 
		_style._numbersFont.getDisplayName() + ")");
}
 
void StyleLines::modify() 
{
	SetModified();	
}


BOOL StyleLines::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	OnVScroll((zDelta > 0)?SB_LINEUP:SB_LINEDOWN,0,NULL);
	return CBCGPPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL StyleLines::OnSetActive() 
{
	dataToCtrls();
	
	return CBCGPPropertyPage::OnSetActive();
}
