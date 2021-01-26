// MusicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MusicDlg.h"
#include "WinPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MusicDlg dialog


MusicDlg::MusicDlg(MusicRule* musicRule) 
	: CDialog(MusicDlg::IDD, NULL),
	_musicRule(musicRule)
{
	//{{AFX_DATA_INIT(MusicDlg)
	_back = musicRule->getBack();
	_hand = musicRule->getHand();
	_bells = musicRule->getRule();
	_bellsStatic = _T("");
	_name = musicRule->getName();
	_useTouchStyle = FALSE;
	//}}AFX_DATA_INIT

	
	_color.EnableOtherButton("More Colors...");
	_color.EnableAutomaticButton("No Color", GetSysColor(COLOR_3DFACE));
}


void MusicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MusicDlg)
	DDX_Control(pDX, IDC_MUSIC_ROW_COLOR, _color);
	DDX_Check(pDX, IDC_BACK, _back);
	DDX_Check(pDX, IDC_HANDSTROKE, _hand);
	DDX_Text(pDX, IDC_BELLS, _bells);
	DDX_Text(pDX, IDC_BELL_STATIC, _bellsStatic);
	DDX_Text(pDX, IDC_NAME, _name);
	DDX_Check(pDX, IDC_TOUCH_STYLE_COLOR, _useTouchStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MusicDlg, CDialog)
	//{{AFX_MSG_MAP(MusicDlg)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_BELLS, OnChangeBells)
	ON_BN_CLICKED(IDC_TOUCH_STYLE_COLOR, OnTouchStyleColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MusicDlg message handlers

BOOL MusicDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WinPos::Load(this, "MusicDlg", true); 

	_color.SetColor(_musicRule->getColor());

	_useTouchStyle = _musicRule->getUseStyleColor();
	UpdateData(FALSE);

	OnChangeBells();
	OnTouchStyleColor();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void MusicDlg::OnDestroy() 
{
	WinPos::Save(this, "MusicDlg");

	_musicRule->setColor(_color.GetColor());

	UpdateData();
	_musicRule->setUseStyleColor(_useTouchStyle);

	CDialog::OnDestroy();	
}

void MusicDlg::OnOK() 
{
	UpdateData();
	OnChangeBells();

	if ((!_back)&&(!_hand))
	{
		AfxMessageBox("Please choose handstroke, backstroke or both");
		return;
	}

	CString testString = _bellsStatic;
	while(testString.Replace("?", ""));
	
	if (testString.GetLength() <=0)
	{
		AfxMessageBox("Please choose at least one bell");
		return;
	}
	
	_musicRule->setBack(_back);
	_musicRule->setHand(_hand);
	_musicRule->setRule(_bellsStatic);
	_musicRule->setName(_name);
	
	CDialog::OnOK();
}

void MusicDlg::OnChangeBells() 
{
	UpdateData();

	_bellsStatic = MusicRule::checkRule(_musicRule->getNumber(), _bells);

	UpdateData(FALSE);	
}

void MusicDlg::OnTouchStyleColor() 
{
	UpdateData();
	_color.EnableWindow(!_useTouchStyle);
}
