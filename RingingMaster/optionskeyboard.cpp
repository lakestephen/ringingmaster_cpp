// OptionsKeyboard.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsKeyboard.h"
#include "KeyboardManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsKeyboard dialog


OptionsKeyboard::OptionsKeyboard()
: CBCGPPropertyPage(OptionsKeyboard::IDD, NULL),
_inited(false)

{
	//{{AFX_DATA_INIT(OptionsKeyboard)
	//}}AFX_DATA_INIT
	_freq = keyboardManager()->getInterval();
}


void OptionsKeyboard::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsKeyboard)
	DDX_Control(pDX, IDC_TEST, _testBtn);
	DDX_Control(pDX, IDC_FREQ_SPIN, _freqSpin);
	DDX_Text(pDX, IDC_FREQ, _freq);
	DDV_MinMaxInt(pDX, _freq, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsKeyboard, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsKeyboard)
	ON_BN_CLICKED(IDC_TEST, OnBnClickedTest)
	ON_EN_CHANGE(IDC_FREQ, OnChangeFreq)
	ON_UPDATE_COMMAND_UI(IDC_TEST, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_FREQ, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_FREQ_SPIN, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_NOTE1, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_NOTE2, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_POLL_INTERVAL, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_STATIC_MS, OnUpdateGeneral)		   	
	ON_UPDATE_COMMAND_UI(IDC_ENABLE, OnUpdateEnable)		   	
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsKeyboard message handlers

BOOL OptionsKeyboard::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_freqSpin.SetRange32(1,100);

	_testBtn.SetImage(IDB_KEYBOARD);
	_testBtn.m_bDrawFocus = FALSE;
	_testBtn.SetTooltip("Launch the keyboard test dialog.");
	_testBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	
	_inited = true;

	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsKeyboard::OnBnClickedTest() 
{
	keyboardManager()->test(this);
}

void OptionsKeyboard::OnChangeFreq() 
{
	if (_inited)
	{
		UpdateData();
		keyboardManager()->setInterval(_freq);
	}
}

void OptionsKeyboard::OnUpdateGeneral(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(keyboardManager()->isKeyboardAvailable());
}	

void OptionsKeyboard::OnUpdateEnable(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(keyboardManager()->isDLLAvailable());
	pCmdUI->SetCheck(keyboardManager()->getUsingKeyboard()?1:0);
}		  
	  

void OptionsKeyboard::OnEnable() 
{
	keyboardManager()->setUsingKeyboard(!keyboardManager()->getUsingKeyboard());		
}
