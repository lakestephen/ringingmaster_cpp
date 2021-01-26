// SerialPortManager.cpp: implementation of the SerialPortManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SerialPortManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



SerialPortManager::SerialPortManager()
{

}

SerialPortManager::~SerialPortManager()
{
	POSITION pos = _portMap.GetStartPosition();
	SerialPort* port = NULL;
	int portNo = 0;

	while (pos)
	{
		_portMap.GetNextAssoc(pos, portNo, port);
		delete port;
	}
	_portMap.RemoveAll(); 

}

void SerialPortManager::save()
{

}

void SerialPortManager::load()
{

}

void SerialPortManager::enumPorts(PortItems& ports)
{
	for (int i=0;i<MAX_PORTS;i++)
	{
		PortItem item;
		item._number = i+1;
		item._name.Format("COM%d",i+1);
		ports.Add(item);
	}
}

bool SerialPortManager::destroyPort(SerialPortManagerEventListener* listener, int portID)
{
	ASSERT(portID > 0 && portID <= MAX_PORTS);

	SerialPort* port = getPort(portID);
	if (port)
	{          

		if (doIOwnPort(listener, portID))
		{
			TRACE("[SerialPortManager::destroyPort] destroying port: %d\r\n", portID);

			delete port;
			_portMap.RemoveKey(portID);

			TRACE("[SerialPortManager::destroyPort] port destroyed: %d\r\n", portID);

			return true;
		}
		else
		{
			ASSERT(FALSE);
			TRACE("[SerialPortManager::destroyPort] cant destroy port: %d no ownership\r\n", portID);
		}
	}

	TRACE("[SerialPortManager::destroyPort] cant destroy port: %d none existing.\r\n", portID);

	return false;
}


bool SerialPortManager::createPort(SerialPortManagerEventListener* listener, 
								   BOOL control,
								   UINT portID,
								   UINT baud,
								   char parity, 
								   UINT databits, 
								   UINT stopsbits, 
								   DWORD dwCommEvents, 
								   UINT nBufferSize)
{
	ASSERT(listener != NULL);

	//kill if we already have one
	if (doIOwnPort(listener, portID))
		destroyPort(listener, portID);

	//create a new port object
	SerialPort* port = new SerialPort(listener);


	//initiate the port
	if (port->OpenPort(portID, baud, parity, databits, stopsbits, dwCommEvents, nBufferSize))
	{
		port->StartMonitoringThread(control);		

		_portMap.SetAt(portID, port);

		TRACE("[SerialPortManager::createPort] created port: %d\r\n", portID);

		return true;
	}
	else
	{
		TRACE("[SerialPortManager::createPort] failed to create port: %d\r\n", portID);
		delete port;
		TRACE("[SerialPortManager::createPort] port destroyed: %d\r\n", portID);
		return false;
	}	
}

bool SerialPortManager::writeToPort(int portID, char* data)
{
	SerialPort* port = getPort(portID);

	if (port)
	{
		TRACE("[SerialPortManager::writeToPort] Data: %d Port: %d\r\n", data, port->getPortID());
		port->WriteToPort(data);
		return true;
	}

	return false;
}


SerialPort* SerialPortManager::getPort(int portId)
{
	SerialPort* port = NULL;
	if (_portMap.Lookup(portId, port))
		return port;

	return NULL;
}

bool SerialPortManager::isValidPort(int portID)
{
	return (getPort(portID) != NULL);
}

BOOL SerialPortManager::setPortInterval(SerialPortManagerEventListener* listener, int index, DWORD interval, int debounceCount)
{
	SerialPort* port = getPort(index);

	if (port && doIOwnPort(listener, index))
	{
		port->setInterval(interval, debounceCount);
		return TRUE;
	}

	return FALSE;
}


BOOL SerialPortManager::doIOwnPort(SerialPortManagerEventListener* listener, int index)
{
	SerialPort* port = getPort(index);

	if (port)
	{
		return (port->getSerialPortManagerEventListener() == listener);
	}

	return FALSE;
}