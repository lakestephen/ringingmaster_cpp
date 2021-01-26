// KeyboardSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "KeyboardSelectDlg.h"
#include "SerialPort.h"			 
#include "SimulatorEventData.h"
#include "KeyboardItem.h"
#include "KeyboardCaptureDlg.h"
#include "KeyboardManager.h"


// KeyboardSelectDlg dialog

IMPLEMENT_DYNAMIC(KeyboardSelectDlg, CDialog)
KeyboardSelectDlg::KeyboardSelectDlg(CWnd* pParent , const KeyboardItem& item)
	: CDialog(KeyboardSelectDlg::IDD, pParent) ,
	_keyboardItem(item)
	, _delay(0)
{
}

KeyboardSelectDlg::~KeyboardSelectDlg()
{
}

void KeyboardSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TYPE, _type);
	DDX_Control(pDX, IDC_COUNT, _count);
	DDX_Control(pDX, IDC_NEXT_PORT_CONTROL, _nextPortControl);
	DDX_Control(pDX, IDC_DELAY_SPIN, _delaySpin);
	DDX_Text(pDX, IDC_DELAY, _delay);
	DDV_MinMaxInt(pDX, _delay, 0, 10000);
	DDX_Control(pDX, IDOK, _ok);
	DDX_Control(pDX, IDC_KEY_STATIC, _keyStatic);
}


BEGIN_MESSAGE_MAP(KeyboardSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_NEXT_PORT_CONTROL, OnBnClickedNextPortControl)
END_MESSAGE_MAP()


// KeyboardSelectDlg message handlers

BOOL KeyboardSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	 update();

	 _delaySpin.SetRange(0, 10000);

	_nextPortControl.m_bDrawFocus = FALSE;
	_nextPortControl.SetTooltip("Press this button, and then trigger the input.");
	_nextPortControl.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_nextPortControl.SetImage(IDB_KEYBOARD);

	_ok.m_bDrawFocus = FALSE;
	_ok.SetTooltip("OK");
	_ok.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void KeyboardSelectDlg::update()
{
	CString str;
	int index;  

	_type.ResetContent();
	for (int i=0;i<=esigt_falling;i++)
	{
		index = _type.AddString(SimulatorEventData::getSignalTypeDescription((EventSignalType)i));
		_type.SetItemData(index, i);
		if (_keyboardItem.getSimulatorEventData().getEventSignalType() == i)
			_type.SetCurSel(index);
	}

	_count.ResetContent();
	for (int i=0;i<=est_4of4;i++)
	{
		index = _count.AddString(SimulatorEventData::getSequenceTypeDescription((EventSequenceType)i));
		_count.SetItemData(index, i);
		if (_keyboardItem.getSimulatorEventData().getEventSequenceType() == i)
			_count.SetCurSel(index);
	}

	_keyStatic.SetWindowText(KeyboardManager::scan2String(_keyboardItem.getScancode()));

	_delay = _keyboardItem.getSimulatorEventData().getDelay();
	UpdateData(FALSE);
}


void KeyboardSelectDlg::OnOK()
{
	captureData();
	CDialog::OnOK();
}

void KeyboardSelectDlg::captureData()
{
	UpdateData();

	EventSignalType eventSignalType = (EventSignalType)_type.GetItemData(_type.GetCurSel());
	EventSequenceType eventSequenceType = (EventSequenceType)_count.GetItemData(_count.GetCurSel());

	_keyboardItem.getSimulatorEventData().setEventSignalType(eventSignalType);
	_keyboardItem.getSimulatorEventData().setEventSequenceType(eventSequenceType);
	_keyboardItem.getSimulatorEventData().setDelay(_delay);
}

void KeyboardSelectDlg::OnBnClickedNextPortControl()
{
	captureData();
	KeyboardCaptureDlg dlg;
	if (dlg.DoModal() == IDOK )
	{
		_keyboardItem.setScancode(dlg._response.getScancode());
		update();
	}     
}
