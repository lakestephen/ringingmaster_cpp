// TextSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ringingmaster.h"
#include "TextSelectDlg.h"
#include "WinPos.h"
#include ".\textselectdlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TextSelectDlg dialog


TextSelectDlg::TextSelectDlg(CWnd* pParent, TextSelectItem& textSelectItem, CString name)
	: CDialog(TextSelectDlg::IDD, pParent),
	_textSelectItem(textSelectItem),
	_name(name)
{
	_style = _textSelectItem._type;
	_text = _textSelectItem._string;
	_case = _textSelectItem._caseSensitive;
	//{{AFX_DATA_INIT(TextSelectDlg)
	//}}AFX_DATA_INIT
}			

void TextSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TextSelectDlg)
	DDX_Radio(pDX, IDC_STYLE, _style);
	DDX_Text(pDX, IDC_TEXT, _text);
	DDX_Control(pDX, IDC_TEXT, _textCtrl);
	DDX_Check(pDX, IDC_CASE, _case);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TextSelectDlg, CDialog)
	//{{AFX_MSG_MAP(TextSelectDlg)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TextSelectDlg message handlers

BOOL TextSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText( _name);

	WinPos::Load(this, "TextSelectDlg" + _name, false);	
	
	_textCtrl.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void TextSelectDlg::OnOK() 
{
	UpdateData();
	if (_text.IsEmpty() && ((StringSearchType)_style) != sst_empty )
	{
		AfxMessageBox("Please enter some text to search on");
		return;
	}

	_textSelectItem._type = (StringSearchType)_style;
	_textSelectItem._string = _text;
	_textSelectItem._caseSensitive = _case;
	  
	WinPos::Save(this, "TextSelectDlg" + _name) ;

	
	CDialog::OnOK();
}


