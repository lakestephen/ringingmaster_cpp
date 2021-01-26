// OptionsCrib.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsCrib.h"
#include "StyleEditDlg.h"
#include "CribManager.h"
#include ".\optionscrib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsCrib dialog


OptionsCrib::OptionsCrib( )
	: CBCGPPropertyPage(OptionsCrib::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsCrib)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void OptionsCrib::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsCrib)
	DDX_Control(pDX, IDC_NOTES_FONT_COLOR, _notesFontColor);
	DDX_Control(pDX, IDC_CRIB_FONT_COLOR, _cribFontColor);
	DDX_Control(pDX, IDC_NOTES_FONT_SIZE, _notesFontSize);
	DDX_Control(pDX, IDC_CRIB_FONT_SIZE, _cribFontSize);
	DDX_Control(pDX, IDC_NOTES_FONT_NAME, _notesFont);
	DDX_Control(pDX, IDC_CRIB_FONT_NAME, _cribFont);
	DDX_Control(pDX, IDC_OPTIONS_LINES_EDIT, _editBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsCrib, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsCrib)
	ON_BN_CLICKED(IDC_OPTIONS_LINES_EDIT, OnOptionsLinesEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsCrib message handlers

BOOL OptionsCrib::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_editBtn.SetImage(IDB_STYLE);
	_editBtn.m_bDrawFocus = FALSE;
	_editBtn.SetTooltip("Edit Style");
	_editBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_cribFont.SelectFont(cribManager()->getDefaultCribStyle()._cribFont._name);

	CString sizeStr;
	sizeStr.Format("%d", cribManager()->getDefaultCribStyle()._cribFont._size/10);
	_cribFontSize.SelectString(-1, sizeStr);

	_cribFontColor.EnableOtherButton("More Colors");
	_cribFontColor.SetColor(cribManager()->getDefaultCribStyle()._cribFont._color);
	_cribFontColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;



	_notesFont.SelectFont( cribManager()->getDefaultCribStyle()._cribNotesFont._name);

	sizeStr.Format("%d", cribManager()->getDefaultCribStyle()._cribNotesFont._size/10);
	_notesFontSize.SelectString(-1, sizeStr);

	_notesFontColor.EnableOtherButton("More Colors");
	_notesFontColor.SetColor(cribManager()->getDefaultCribStyle()._cribNotesFont._color);
	_notesFontColor.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	
	return TRUE;  // return TRUE unless you set the focus to a control

}


void OptionsCrib::OnOptionsLinesEdit() 
{
	Style style(cribManager()->getDefaultBlueLineStyle());

	StyleEditDlg styleEditDlg(style, this, MAXBELLS, slt_edit, "Crib Style");	

	styleEditDlg.DoModal();

	cribManager()->setDefaultBlueLineStyle(style);
	
}


BOOL OptionsCrib::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		CString sizeString;
		CBCGPFontDesc* fontDesc = NULL;

		CribStyle style = cribManager()->getDefaultCribStyle();

		fontDesc = _cribFont.GetSelFont();
		_cribFontSize.GetLBText(_cribFontSize.GetCurSel(),sizeString); 
		
		style._cribFont._name = (fontDesc)?fontDesc->m_strName:"";
		style._cribFont._size = (atoi(sizeString) * 10);
		style._cribFont._color= _cribFontColor.GetColor();

		fontDesc = _notesFont.GetSelFont();
		_notesFontSize.GetLBText(_notesFontSize.GetCurSel(),sizeString); 
		
		style._cribNotesFont._name = (fontDesc)?fontDesc->m_strName:"";;
		style._cribNotesFont._size = (atoi(sizeString) * 10);
		style._cribNotesFont._color= _notesFontColor.GetColor();

		cribManager()->setDefaultCribStyle(style);

		return TRUE;
	}

	return FALSE;
}
