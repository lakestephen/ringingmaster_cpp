// StyleView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "StyleView.h"
#include "StyleEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StyleView property page


StyleView::StyleView(Style &style, StyleLinesType type) : 
CBCGPPropertyPage(StyleView::IDD),
_style(style),
_type(type)
{
	//{{AFX_DATA_INIT(StyleView)
	_showTitle = FALSE;
	_showStart = FALSE;
	_showNotation = FALSE;
	_showStartBlobs = FALSE;
	_showCalls = FALSE;
	_showBackstroke = FALSE;
	_showHandstroke = FALSE;
	_showInCourse = FALSE;
	//}}AFX_DATA_INIT

	_falseRowsColor.EnableOtherButton("More Colors...");
	_falseRowsColor.EnableAutomaticButton("No False Rows", GetSysColor(COLOR_3DFACE));

	_musicRowsColor.EnableOtherButton("More Colors...");
	_musicRowsColor.EnableAutomaticButton("No Music Rows", GetSysColor(COLOR_3DFACE));

}

StyleView::~StyleView()
{
}

void StyleView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StyleView)
	DDX_Control(pDX, IDC_IN_COURSE, _showInCourseCtrl);
	DDX_Control(pDX, IDC_MUSIC_ROW_COLOR, _musicRowsColor);
	DDX_Control(pDX, IDC_STROKE_FONT, _strokeFont);
	DDX_Control(pDX, IDC_SHOW_HANDSTROKE, _showHandstrokeCtrl);
	DDX_Control(pDX, IDC_SHOW_BACKSTROKE, _showBackstrokeCtrl);
	DDX_Control(pDX, IDC_SHOW_CALLS, _showCallsCtrl);
	DDX_Control(pDX, IDC_CALLS_FONT, _callsFont);
	DDX_Control(pDX, IDC_FALSE_ROW_COLOR, _falseRowsColor);
	DDX_Control(pDX, IDC_TITLE_FONT, _titleFont);
	DDX_Control(pDX, IDC_SHOW_TITLE, _showTitleCtrl);
	DDX_Control(pDX, IDC_SHOW_STARTS, _showStartCtrl);
	DDX_Control(pDX, IDC_START_FONT, _startFont);
	DDX_Control(pDX, IDC_SHOW_START_BLOBS, _startsBlobsCtrl);
	DDX_Control(pDX, IDC_SHOW_NOTATION, _showNotationCtrl);
	DDX_Control(pDX, IDC_NOTATION_FONT, _notationFont);
	DDX_Check(pDX, IDC_SHOW_TITLE, _showTitle);
	DDX_Check(pDX, IDC_SHOW_STARTS, _showStart);
	DDX_Check(pDX, IDC_SHOW_NOTATION, _showNotation);
	DDX_Check(pDX, IDC_SHOW_START_BLOBS, _showStartBlobs);
	DDX_Check(pDX, IDC_SHOW_CALLS, _showCalls);
	DDX_Check(pDX, IDC_SHOW_BACKSTROKE, _showBackstroke);
	DDX_Check(pDX, IDC_SHOW_HANDSTROKE, _showHandstroke);
	DDX_Check(pDX, IDC_IN_COURSE, _showInCourse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StyleView, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(StyleView)
	ON_BN_CLICKED(IDC_SHOW_START_BLOBS, modify)
	ON_BN_CLICKED(IDC_NOTATION_FONT, OnNotationFont)
	ON_BN_CLICKED(IDC_SHOW_TITLE, OnShowTitle)
	ON_BN_CLICKED(IDC_SHOW_NOTATION, OnShowNotation)
	ON_BN_CLICKED(IDC_SHOW_STARTS, OnShowStarts)
	ON_BN_CLICKED(IDC_START_FONT, OnStartFont)
	ON_BN_CLICKED(IDC_TITLE_FONT, OnTitleFont)
	ON_BN_CLICKED(IDC_SHOW_CALLS, OnShowCalls)
	ON_BN_CLICKED(IDC_CALLS_FONT, OnCallsFont)
	ON_BN_CLICKED(IDC_FALSE_ROW_COLOR, modify)
	ON_BN_CLICKED(IDC_STROKE_FONT, OnStrokeFont)
	ON_BN_CLICKED(IDC_SHOW_BACKSTROKE, OnShowStroke)
	ON_BN_CLICKED(IDC_SHOW_HANDSTROKE, OnShowStroke)
	ON_BN_CLICKED(IDC_IN_COURSE, OnShowStroke)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StyleView message handlers

void StyleView::modify() 
{
	SetModified();	
	updateTooltips();
}

BOOL StyleView::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	if (_type == slt_drawAMethod)
	{
		GetDlgItem(IDC_STATIC_LINE_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_START_FONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_FALSE_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FALSE_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FALSE_ROW_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_MUSIC_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MUSIC_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MUSIC_ROW_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TITLE_FONT)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_SHOW_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_START_BLOBS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALLS)->ShowWindow(SW_HIDE); 
		GetDlgItem(IDC_STATIC_LINE_CALLS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_CALLS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CALLS_FONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_STROKES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_STROKES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_HANDSTROKE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_BACKSTROKE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STROKE_FONT)->ShowWindow(SW_HIDE); 
		GetDlgItem(IDC_IN_COURSE)->ShowWindow(SW_HIDE); 
		
	}
	else if (_type == slt_callchange)
	{
		GetDlgItem(IDC_STATIC_LINE_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_START_FONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_FALSE_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FALSE_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FALSE_ROW_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_MUSIC_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MUSIC_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MUSIC_ROW_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TITLE_FONT)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_SHOW_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_START_BLOBS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_HANDSTROKE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_BACKSTROKE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NOT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_NOTATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_NOTATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NOTATION_FONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_CALLS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALLS)->SetWindowText("Comments Font");		
	}
	else if (_type == slt_simulator)
	{
		GetDlgItem(IDC_STATIC_LINE_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_STARTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_START_FONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_FALSE_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FALSE_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FALSE_ROW_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_MUSIC_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MUSIC_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MUSIC_ROW_COLOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TITLE_FONT)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_SHOW_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_START_BLOBS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_HANDSTROKE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_BACKSTROKE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NOT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_NOTATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_NOTATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NOTATION_FONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_CALLS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CALLS)->SetWindowText("Comments Font");		
	}
	else if (_type == slt_normal)
	{
		GetDlgItem(IDC_STATIC_NOT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_NOTATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_NOTATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NOTATION_FONT)->ShowWindow(SW_HIDE);
	}
	else if (_type == slt_crib)
	{
		GetDlgItem(IDC_STATIC_CALLS)->ShowWindow(SW_HIDE); 
		GetDlgItem(IDC_STATIC_LINE_CALLS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SHOW_CALLS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CALLS_FONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LINE_MUSIC_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MUSIC_ROWS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MUSIC_ROW_COLOR)->ShowWindow(SW_HIDE);
	}


	//set up the buttons. 
	_strokeFont.SetImage(IDB_FONT);
	_strokeFont.m_bDrawFocus = FALSE;
	_strokeFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_callsFont.SetImage(IDB_FONT);
	_callsFont.m_bDrawFocus = FALSE;
	_callsFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_titleFont.SetImage(IDB_FONT);
	_titleFont.m_bDrawFocus = FALSE;
	_titleFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_startFont.SetImage(IDB_FONT);
	_startFont.m_bDrawFocus = FALSE;
	_startFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_notationFont.SetImage(IDB_FONT);
	_notationFont.m_bDrawFocus = FALSE;
	_notationFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	updateTooltips();

	return TRUE;  // return TRUE unless you set the focus to a control

}

void StyleView::updateTooltips()
{
	_strokeFont.SetTooltip("Edit Stroke Font (" + 
		_style._strokeFont.getDisplayName() + ")");

	_callsFont.SetTooltip("Edit Calls Font (" + 
		_style._commentsFont.getDisplayName() + ")");

	_titleFont.SetTooltip("Edit Title Font (" + 
		_style._titleFont.getDisplayName() + ")");

	_startFont.SetTooltip("Edit Start Font (" + 
		_style._startsFont.getDisplayName() + ")");

	_notationFont.SetTooltip("Edit Notation Font (" + 
		_style._notationFont.getDisplayName() + ")");
}

BOOL StyleView::OnKillActive() 
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		_style._startsShow = (_showStart == TRUE);
		_style._startsBlobs= _showStartBlobs;

		_style._falseRowsColor = _falseRowsColor.GetColor();
		_style._musicRowsColor = _musicRowsColor.GetColor();

		_style._titleShow = (_showTitle == TRUE);

		_style._notationShow = (_showNotation == TRUE);

		_style._backStrokeShow = (_showBackstroke == TRUE);
		_style._handStrokeShow = (_showHandstroke == TRUE);
		_style._showInCourse = (_showInCourse== TRUE);

		_style._commentsShow = (_showCalls == TRUE);
		
		return TRUE;
	}	

	return FALSE;
}

BOOL StyleView::OnSetActive() 
{
	_showTitle  = _style._titleShow;

	_showNotation = _style._notationShow;

	_falseRowsColor.SetColor(_style._falseRowsColor);
	_musicRowsColor.SetColor(_style._musicRowsColor);

	_showStart = _style._startsShow;
	_showStartBlobs = _style._startsBlobs;
		
	_showBackstroke = _style._backStrokeShow;
	_showHandstroke = _style._handStrokeShow;
	_showInCourse = _style._showInCourse;

	_showCalls = _style._commentsShow;

	UpdateData(FALSE);	

	DoShowTitle();
	DoShowStarts();
	DoShowNotation();
	DoShowStroke();
	DoShowCalls();

	return CBCGPPropertyPage::OnSetActive();
}

BOOL StyleView::OnApply() 
{
	((StyleEditDlg*)GetParent())->Apply() ;
	
	return CBCGPPropertyPage::OnApply();
}

void StyleView::OnNotationFont() 
{
	if (_style._notationFont.editFont(this) == true)
	{
		modify();
	}	
}

void StyleView::OnShowTitle() 
{
	modify();
	DoShowTitle();	
}

void StyleView::OnShowNotation() 
{
	modify();
	DoShowNotation();
}

void StyleView::OnShowStarts() 
{
	modify();
	DoShowStarts();
}

void StyleView::OnStartFont() 
{
	if (_style._startsFont.editFont(this) == true)
	{
		modify();
	}	
} 

void StyleView::OnTitleFont() 
{
	if (_style._titleFont.editFont(this) == true)
	{
		modify();
	}
}

void StyleView::DoShowTitle() 
{
	_titleFont.EnableWindow(_showTitleCtrl.GetCheck());
}	  

void StyleView::DoShowStarts()
{
	_startFont.EnableWindow(_showStartCtrl.GetCheck());
	_startsBlobsCtrl.EnableWindow(_showStartCtrl.GetCheck());
}

void StyleView::DoShowNotation()
{
	_notationFont.EnableWindow(_showNotationCtrl.GetCheck());
}

void StyleView::OnShowCalls() 
{
	modify();
	DoShowCalls();
}

void StyleView::DoShowCalls()
{
	_callsFont.EnableWindow(_showCallsCtrl.GetCheck());
}

void StyleView::OnCallsFont() 
{
	if (_style._commentsFont.editFont(this) == true)
	{
		modify();
	}	
}

void StyleView::OnStrokeFont() 
{
	if (_style._strokeFont.editFont(this) == true)
	{
		modify();
	}	
}

void StyleView::OnShowStroke() 
{
	modify();
	DoShowStroke();
}

void StyleView::DoShowStroke()
{
	//show the font any time there is a check
	_strokeFont.EnableWindow((_showHandstrokeCtrl.GetCheck())||
							 (_showBackstrokeCtrl.GetCheck())||
							 (_showInCourseCtrl.GetCheck()));

	//show the in course if there is no check in handstroke
	_showInCourseCtrl.EnableWindow((!_showHandstrokeCtrl.GetCheck())&&
							       (!_showBackstrokeCtrl.GetCheck()));
							 
	//show the stroke if there is no check in course 
	_showBackstrokeCtrl.EnableWindow(!_showInCourseCtrl.GetCheck());
	_showHandstrokeCtrl.EnableWindow(!_showInCourseCtrl.GetCheck());
					

}
