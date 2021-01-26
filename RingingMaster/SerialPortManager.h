// SerialPortManager.h: interface for the SerialPortManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORTMANAGER_H__6261CC57_E627_4CF0_A24F_411FA9C2F942__INCLUDED_)
#define AFX_SERIALPORTMANAGER_H__6261CC57_E627_4CF0_A24F_411FA9C2F942__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SerialPort.h"
#include "ManagerBase.h"

class SerialPortManagerEventListener;

struct PortItem
{
	CString _name;
	int _number;
};

typedef CArray<PortItem, PortItem> PortItems;

class SerialPortManager :  public ManagerBase
{
public:
	bool writeToPort(int portID, char* data);
	bool isValidPort(int portID);
	BOOL doIOwnPort(SerialPortManagerEventListener* listener, int index);
	bool createPort(SerialPortManagerEventListener* listener, BOOL control, UINT portID, UINT baud = 19200, char parity = 'N', UINT databits = 8, UINT stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR, UINT nBufferSize = 512);
	bool destroyPort(SerialPortManagerEventListener* listener, int portId);

	BOOL setPortInterval(SerialPortManagerEventListener* listener, int index, DWORD interval, int debounceCount);
	void enumPorts(PortItems& ports);
	void load();
	void save();
	SerialPortManager();
	virtual ~SerialPortManager();

protected:
	CMap<int, int, SerialPort*, SerialPort*> _portMap;
	SerialPort* getPort(int portId);


};

#endif // !defined(AFX_SERIALPORTMANAGER_H__6261CC57_E627_4CF0_A24F_411FA9C2F942__INCLUDED_)
