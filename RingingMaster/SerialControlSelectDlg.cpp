// SerialControlSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SerialControlSelectDlg.h"
#include "SerialPort.h"			 
#include "SimulatorEventData.h"
#include "SerialControlItem.h"
#include "SerialControlCaptureDlg.h"

#include ".\serialcontrolselectdlg.h"


// SerialControlSelectDlg dialog

IMPLEMENT_DYNAMIC(SerialControlSelectDlg, CDialog)
SerialControlSelectDlg::SerialControlSelectDlg(CWnd* pParent , const SerialControlItem& item)
	: CDialog(SerialControlSelectDlg::IDD, pParent) ,
	_serialControlItem(item)
	, _delay(0)
{
}

SerialControlSelectDlg::~SerialControlSelectDlg()
{
}

void SerialControlSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORT, _port);
	DDX_Control(pDX, IDC_SIGNAL, _signal);
	DDX_Control(pDX, IDC_TYPE, _type);
	DDX_Control(pDX, IDC_COUNT, _count);
	DDX_Control(pDX, IDC_NEXT_PORT_CONTROL, _nextPortControl);
	DDX_Control(pDX, IDC_DELAY_SPIN, _delaySpin);
	DDX_Text(pDX, IDC_DELAY, _delay);
	DDV_MinMaxInt(pDX, _delay, 0, 10000);
	DDX_Control(pDX, IDOK, _ok);
}


BEGIN_MESSAGE_MAP(SerialControlSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_NEXT_PORT_CONTROL, OnBnClickedNextPortControl)
END_MESSAGE_MAP()


// SerialControlSelectDlg message handlers

BOOL SerialControlSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	 update();

	 _delaySpin.SetRange(0, 10000);

	_nextPortControl.m_bDrawFocus = FALSE;
	_nextPortControl.SetTooltip("Press this button, and then trigger the input.");
	_nextPortControl.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_nextPortControl.SetImage(IDB_PORT);

	_ok.m_bDrawFocus = FALSE;
	_ok.SetTooltip("OK");
	_ok.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SerialControlSelectDlg::update()
{
	CString str;
	int index;  

	_port.ResetContent();
	for (int i=1;i<=MAX_PORTS;i++)
	{
		str.Format("%d", i);
		index = _port.AddString(str);
		_port.SetItemData(index, i);
		if (_serialControlItem.getPort() == i)
			_port.SetCurSel(index);
	}

	_signal.ResetContent();
	for (int i=0;i<=pci_dsr;i++)
	{
		index = _signal.AddString(SerialControlAction::getPortControlItemDescription((PortControlItem)i));
		_signal.SetItemData(index, i);
		if (_serialControlItem.getEvent() == i)
			_signal.SetCurSel(index);
	}

	_type.ResetContent();
	for (int i=0;i<=esigt_falling;i++)
	{
		index = _type.AddString(SimulatorEventData::getSignalTypeDescription((EventSignalType)i));
		_type.SetItemData(index, i);
		if (_serialControlItem.getSimulatorEventData().getEventSignalType() == i)
			_type.SetCurSel(index);
	}

	_count.ResetContent();
	for (int i=0;i<=est_4of4;i++)
	{
		index = _count.AddString(SimulatorEventData::getSequenceTypeDescription((EventSequenceType)i));
		_count.SetItemData(index, i);
		if (_serialControlItem.getSimulatorEventData().getEventSequenceType() == i)
			_count.SetCurSel(index);
	}

	_delay = _serialControlItem.getSimulatorEventData().getDelay();
	UpdateData(FALSE);
}


void SerialControlSelectDlg::OnOK()
{
	captureData();
	CDialog::OnOK();
}

void SerialControlSelectDlg::captureData()
{
	UpdateData();

	int port = _port.GetItemData(_port.GetCurSel());
	PortControlItem portControlItem = (PortControlItem)(max(0, _signal.GetItemData(_signal.GetCurSel())));
	EventSignalType eventSignalType = (EventSignalType)_type.GetItemData(_type.GetCurSel());
	EventSequenceType eventSequenceType = (EventSequenceType)_count.GetItemData(_count.GetCurSel());

	_serialControlItem.setPort(port);
	_serialControlItem.setEvent(portControlItem);
	_serialControlItem.getSimulatorEventData().setEventSignalType(eventSignalType);
	_serialControlItem.getSimulatorEventData().setEventSequenceType(eventSequenceType);
	_serialControlItem.getSimulatorEventData().setDelay(_delay);
}

void SerialControlSelectDlg::OnBnClickedNextPortControl()
{
	captureData();
	SerialControlCaptureDlg dlg;
	if (dlg.DoModal() == IDOK )
	{
		_serialControlItem.setPort(dlg._serialControlResponse.getPort());
		_serialControlItem.setEvent(dlg._serialControlResponse.getEvent());

		update();
	}

}
