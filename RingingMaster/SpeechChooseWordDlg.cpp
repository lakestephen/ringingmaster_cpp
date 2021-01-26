// SpeechChooseWordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SpeechChooseWordDlg.h"
#include "SpeechManager.h"



// SpeechChooseWordDlg dialog

IMPLEMENT_DYNAMIC(SpeechChooseWordDlg, CDialog)
SpeechChooseWordDlg::SpeechChooseWordDlg(CWnd* pParent, const SpeechItem& item)
	: CDialog(SpeechChooseWordDlg::IDD, pParent)
	, _speechItem(item), 
	_delay(0)
	, _word(_T(""))
{
}

SpeechChooseWordDlg::~SpeechChooseWordDlg()
{
}

void SpeechChooseWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COUNT, _count);
	DDX_Control(pDX, IDC_DELAY_SPIN, _delaySpin);
	DDX_Text(pDX, IDC_DELAY, _delay);
	DDV_MinMaxInt(pDX, _delay, 0, 2500);
	DDX_Control(pDX, IDOK, _ok);
	DDX_Text(pDX, IDC_WORD, _word);
}


BEGIN_MESSAGE_MAP(SpeechChooseWordDlg, CDialog)
	
END_MESSAGE_MAP()


// SpeechChooseWordDlg message handlers

BOOL SpeechChooseWordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	update();

	 _delaySpin.SetRange(0, 2500);

	_ok.m_bDrawFocus = FALSE;
	_ok.SetTooltip("OK");
	_ok.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void SpeechChooseWordDlg::OnOK()
{
	captureData();

	CDialog::OnOK();
}				

void SpeechChooseWordDlg::captureData()
{
	UpdateData();
	
	EventSequenceType eventSequenceType = (EventSequenceType)_count.GetItemData(_count.GetCurSel());

	_speechItem.setWord(_word);
	_speechItem.getSimulatorEventData().setEventSequenceType(eventSequenceType);
	_speechItem.getSimulatorEventData().setDelay(_delay);
}

void SpeechChooseWordDlg::update()
{
	CString str;
	int index;  
		 
	_word = _speechItem.getWord();

	_count.ResetContent();
	for (int i=0;i<=est_4of4;i++)
	{
		index = _count.AddString(SimulatorEventData::getSequenceTypeDescription((EventSequenceType)i));
		_count.SetItemData(index, i);
		if (_speechItem.getSimulatorEventData().getEventSequenceType() == i)
			_count.SetCurSel(index);
	}

	_delay = _speechItem.getSimulatorEventData().getDelay();

	UpdateData(FALSE);
}

