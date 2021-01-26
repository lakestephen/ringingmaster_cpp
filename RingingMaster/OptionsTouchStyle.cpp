// OptionsTouchStyle.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsTouchStyle.h"
#include "GlobalDataManager.h"
#include ".\optionstouchstyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsTouchStyle dialog


OptionsTouchStyle::OptionsTouchStyle()
	: CBCGPPropertyPage(OptionsTouchStyle::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsTouchStyle)
	//}}AFX_DATA_INIT
}

void OptionsTouchStyle::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsTouchStyle)
	DDX_Control(pDX, IDC_NOTES_COLOR, _notesColor);
	DDX_Control(pDX, IDC_PLAIN_LEAD_TOKEN_COLOR, _plainLeadTokenColor);
	DDX_Control(pDX, IDC_GROUP_COLOR, _groupColor);
	DDX_Control(pDX, IDC_DEFINITION_COLOR, _definitionColor);
	DDX_Control(pDX, IDC_VAR_COLOR, _varColor);
	DDX_Control(pDX, IDC_BLOCK_COLOR, _blockDefColor);
	DDX_Control(pDX, IDC_UNCHANGED_CH_COLOR, _unchangedCHColor);
	DDX_Control(pDX, IDC_METHOD_SPLICE_COLOR, _methodSplicedColor);
	DDX_Control(pDX, IDC_CHANGED_CH_COLOR, _changedCHColor);
	DDX_Control(pDX, IDC_CALL_POS_COLOR, _callPosColor);
	DDX_Control(pDX, IDC_CALL_COLOR, _callColor);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TITLE_FONT, _titleFont);
	DDX_Control(pDX, IDC_AUTHOR_FONT, _authorFont);
	DDX_Control(pDX, IDC_MAIN_FONT, _mainFont);
	DDX_Control(pDX, IDC_NOTES_FONT, _notesFont);
}


BEGIN_MESSAGE_MAP(OptionsTouchStyle, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsTouchStyle)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TITLE_FONT, OnBnClickedTitleFont)
	ON_BN_CLICKED(IDC_AUTHOR_FONT, OnBnClickedAuthorFont)
	ON_BN_CLICKED(IDC_MAIN_FONT, OnBnClickedMainFont)
	ON_BN_CLICKED(IDC_NOTES_FONT, OnBnClickedNotesFont)
END_MESSAGE_MAP()
   

BOOL OptionsTouchStyle::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_unchangedCHColor.EnableOtherButton("More Colors");
	_unchangedCHColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_unchangedLHColor));
	_unchangedCHColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_methodSplicedColor.EnableOtherButton("More Colors");
	_methodSplicedColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_methodSplicedColor));
	_methodSplicedColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_changedCHColor.EnableOtherButton("More Colors");
	_changedCHColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_changedLHColor));
	_changedCHColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_callPosColor.EnableOtherButton("More Colors");
	_callPosColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_callPosColor));
	_callPosColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_callColor.EnableOtherButton("More Colors");
	_callColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_callColor));
	_callColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_blockDefColor.EnableOtherButton("More Colors");
	_blockDefColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_blockColor));
	_blockDefColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_definitionColor.EnableOtherButton("More Colors");
	_definitionColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_definitonColor));
	_definitionColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_plainLeadTokenColor.EnableOtherButton("More Colors");
	_plainLeadTokenColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_plainLeadTokenColor));
	_plainLeadTokenColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_varColor.EnableOtherButton("More Colors");
	_varColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_varianceColor));
	_varColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_groupColor.EnableOtherButton("More Colors");
	_groupColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_groupColor));
	_groupColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_notesColor.EnableOtherButton("More Colors");
	_notesColor.SetColor(globalDataManager()->_touchStyle.getColor(tsc_notesColor));
	_notesColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_titleFont.SetImage(IDB_FONT);
	_titleFont.m_bDrawFocus = FALSE;
	_titleFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_authorFont.SetImage(IDB_FONT);
	_authorFont.m_bDrawFocus = FALSE;
	_authorFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_mainFont.SetImage(IDB_FONT);
	_mainFont.m_bDrawFocus = FALSE;
	_mainFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_notesFont.SetImage(IDB_FONT);
	_notesFont.m_bDrawFocus = FALSE;
	_notesFont.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	updateTooltips();

	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsTouchStyle::updateTooltips()
{
	_authorFont.SetTooltip("Edit Author Font (" + 
		globalDataManager()->_touchStyle.getFontInfoAuthor().getDisplayName() + ")");
	_titleFont.SetTooltip("Edit Title Font (" + 
		globalDataManager()->_touchStyle.getFontInfoTitle().getDisplayName() + ")");
	_mainFont.SetTooltip("Edit Main Font (" + 
		globalDataManager()->_touchStyle.getFontInfoMain().getDisplayName() + ")");
	_notesFont.SetTooltip("Edit Notes Font (" + 
		globalDataManager()->_touchStyle.getFontInfoNotes().getDisplayName() + ")");
}

BOOL OptionsTouchStyle::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		globalDataManager()->_touchStyle.setColor(tsc_unchangedLHColor, _unchangedCHColor.GetColor());
		globalDataManager()->_touchStyle.setColor(tsc_methodSplicedColor, _methodSplicedColor.GetColor());
		globalDataManager()->_touchStyle.setColor(tsc_changedLHColor, _changedCHColor.GetColor());
		globalDataManager()->_touchStyle.setColor(tsc_callPosColor, _callPosColor.GetColor());
		globalDataManager()->_touchStyle.setColor(tsc_callColor, _callColor.GetColor());	
		globalDataManager()->_touchStyle.setColor(tsc_blockColor, _blockDefColor.GetColor());	
		globalDataManager()->_touchStyle.setColor(tsc_definitonColor, _definitionColor.GetColor());	
		globalDataManager()->_touchStyle.setColor(tsc_plainLeadTokenColor, _plainLeadTokenColor.GetColor());	
		globalDataManager()->_touchStyle.setColor(tsc_varianceColor, _varColor.GetColor());	
		globalDataManager()->_touchStyle.setColor(tsc_groupColor, _groupColor.GetColor());	
		globalDataManager()->_touchStyle.setColor(tsc_notesColor, _notesColor.GetColor());	

		return TRUE;
	}

	return FALSE;

}

BOOL OptionsTouchStyle::OnSetActive()
{
	updateTooltips();

	return CBCGPPropertyPage::OnSetActive();
}

void OptionsTouchStyle::OnBnClickedTitleFont()
{
	FontInfo info = globalDataManager()->_touchStyle.getFontInfoTitle();
	info.editFont(this, true);
	globalDataManager()->_touchStyle.setFontInfoTitle(info);
	updateTooltips();
}

void OptionsTouchStyle::OnBnClickedAuthorFont()
{
	FontInfo info = globalDataManager()->_touchStyle.getFontInfoAuthor();
	info.editFont(this, true);
	globalDataManager()->_touchStyle.setFontInfoAuthor(info);
	updateTooltips();
}

void OptionsTouchStyle::OnBnClickedMainFont()
{
	FontInfo info = globalDataManager()->_touchStyle.getFontInfoMain();
	info.editFont(this, false);
	globalDataManager()->_touchStyle.setFontInfoMain(info);
	updateTooltips();
}

void OptionsTouchStyle::OnBnClickedNotesFont()
{
	FontInfo info = globalDataManager()->_touchStyle.getFontInfoNotes();
	info.editFont(this, true);
	globalDataManager()->_touchStyle.setFontInfoNotes(info);
	updateTooltips();
}
