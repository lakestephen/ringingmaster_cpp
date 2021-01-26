// MultiBellInterfaceChooseChannelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MultiBellInterfaceChooseChannelDlg.h"
#include "MultiBellInterfaceManager.h"
#include "MultiBellInterfaceCaptureDlg.h"


// MultiBellInterfaceChooseChannelDlg dialog

IMPLEMENT_DYNAMIC(MultiBellInterfaceChooseChannelDlg, CDialog)
MultiBellInterfaceChooseChannelDlg::MultiBellInterfaceChooseChannelDlg(CWnd* pParent, const MultiBellItem& item)
	: CDialog(MultiBellInterfaceChooseChannelDlg::IDD, pParent)
	, _multiBellItem(item), 
	_delay(0)
{
}

MultiBellInterfaceChooseChannelDlg::~MultiBellInterfaceChooseChannelDlg()
{
}

void MultiBellInterfaceChooseChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COUNT, _count);
	DDX_Control(pDX, IDC_NEXT_MBI, _nextMBI);
	DDX_Control(pDX, IDC_DELAY_SPIN, _delaySpin);
	DDX_Control(pDX, IDC_CHANNEL, _channel);
	DDX_Text(pDX, IDC_DELAY, _delay);
	DDV_MinMaxInt(pDX, _delay, 0, 2500);
	DDX_Control(pDX, IDOK, _ok);
}


BEGIN_MESSAGE_MAP(MultiBellInterfaceChooseChannelDlg, CDialog)
	ON_BN_CLICKED(IDC_NEXT_MBI, OnBnClickedNextMbi)
END_MESSAGE_MAP()


// MultiBellInterfaceChooseChannelDlg message handlers

BOOL MultiBellInterfaceChooseChannelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	update();

	 _delaySpin.SetRange(0, 2500);

	_nextMBI.m_bDrawFocus = FALSE;
	_nextMBI.SetTooltip("Press this button, and then ring the bell to trigger the sensor.");
	_nextMBI.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_nextMBI.SetImage(IDB_MBI);

	_ok.m_bDrawFocus = FALSE;
	_ok.SetTooltip("OK");
	_ok.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void MultiBellInterfaceChooseChannelDlg::OnOK()
{
	captureData();

	CDialog::OnOK();
}				

void MultiBellInterfaceChooseChannelDlg::captureData()
{
	UpdateData();
	
	int channel = _channel.GetItemData(_channel.GetCurSel());
	EventSequenceType eventSequenceType = (EventSequenceType)_count.GetItemData(_count.GetCurSel());

	_multiBellItem.setChannel(channel);
	_multiBellItem.getSimulatorEventData().setEventSequenceType(eventSequenceType);
	_multiBellItem.getSimulatorEventData().setDelay(_delay);
}

void MultiBellInterfaceChooseChannelDlg::update()
{
	CString str;
	int index;  
		 
	_channel.ResetContent();
	for (int i=0;i<MultiBellInterfaceManager::_MaxChannel;i++)
	{
		str.Format("%d", i+1);
		index = _channel.AddString(str);
		_channel.SetItemData(index, i);
		if (_multiBellItem.getChannel() == i)
			_channel.SetCurSel(index);
	}	   

	_count.ResetContent();
	for (int i=0;i<=est_4of4;i++)
	{
		index = _count.AddString(SimulatorEventData::getSequenceTypeDescription((EventSequenceType)i));
		_count.SetItemData(index, i);
		if (_multiBellItem.getSimulatorEventData().getEventSequenceType() == i)
			_count.SetCurSel(index);
	}

	_delay = _multiBellItem.getSimulatorEventData().getDelay();

	UpdateData(FALSE);
}

void MultiBellInterfaceChooseChannelDlg::OnBnClickedNextMbi()
{
	captureData();
	MultiBellInterfaceCaptureDlg dlg;
	if (dlg.DoModal() == IDOK )
	{
		_multiBellItem.setChannel(dlg._multiBellResponse.getChannel());

		update();
	}
}
