#include "StdAfx.h"
#include "SerialControlAction.h"


SerialControlAction::SerialControlAction(int port, PortControlItem pcEvent):
_port(port), 
_event(pcEvent)
{
}

SerialControlAction::SerialControlAction():
_port(1), 
_event(pci_dsr)
{
}

SerialControlAction::~SerialControlAction(void)
{
}

CString SerialControlAction::getPortControlItemDescription(PortControlItem pcEvent) 
{
	switch (pcEvent)
	{
	case pci_rlsd:
		return "RLSD";
	case pci_ring:
		return "RING";
	case pci_cts:
		return "CTS";
	case pci_dsr:
		return "DSR";
	default:
		ASSERT(FALSE);
		return "Unknown";
	}                    
}

CString SerialControlAction::getDescription() const
{
	CString str;
	str.Format("%d:", _port);

	return str + getPortControlItemDescription(_event);
}

bool SerialControlAction::operator ==(const SerialControlAction & serialControlAction) const
{
	return ((_port == serialControlAction._port) &&
			(_event == serialControlAction._event));
}

void SerialControlAction::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  

		ar << _port;
		ar << _event;
	}
	else
	{
		ar >> version;  

		ar >> _port;
		int temp;
		ar >> temp;
		_event = (PortControlItem)temp;
	}
}

int SerialControlAction::getPort() const
{
	return _port;
}

PortControlItem SerialControlAction::getEvent() const
{
	return _event;
}

void SerialControlAction::setPort(int port)
{
	_port = port;
}

void SerialControlAction::setEvent(PortControlItem pcEvent) 
{
	_event = pcEvent;
}


bool SerialControlAction::isValid() const
{
	return (_port > 0 && _port <= MAX_PORTS);
}

