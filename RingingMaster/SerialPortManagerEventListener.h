// SerialPortManagerEventListener.h: interface for the SerialPortManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum PortControlItem;

class SerialPortManagerEventListener  
{
public:
	SerialPortManagerEventListener() {};
	virtual ~SerialPortManagerEventListener() {};

	virtual void serialPortManager_notifyPortDataEventFromSeperateThread(int /*port*/, BYTE /*data*/) {};
	virtual void serialPortManager_notifyPortDataEventFromSeperateThread(int /*port*/, PortControlItem /*pcEvent*/, bool /*state*/, DWORD /*timestamp*/) {};
};


typedef CArray<SerialPortManagerEventListener*, SerialPortManagerEventListener*> SerialPortManagerEventListeners;


