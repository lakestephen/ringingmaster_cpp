
#pragma once

#include "SerialPortManagerEventListener.h"
#include "SerialControlManagerEventListener.h"
#include "SerialPortManager.h"
#include "SerialPort.h"	 
#include "SerialControlResponse.h"
#include "ManagerBase.h"


class SerialControlTimerPackage
{
public:

	SerialControlTimerPackage(SerialControlManagerEventListener* listener, 
							const SerialControlResponse& response, 
							DWORD timestamp) :
	_listener(listener), 
	_response(response), 
	_timestamp(timestamp)
	{

	}

	SerialControlManagerEventListener* _listener;
	SerialControlResponse _response;
	DWORD _timestamp;
};


class SerialControlItem;


class SerialControlManager : public ManagerBase, SerialPortManagerEventListener
{
public:
	void removeEventListener(SerialControlManagerEventListener* serialControlManagerEventListener);
	void addEventListener(SerialControlManagerEventListener* serialControlManagerEventListener);

	void setUsingSerialControl(BOOL usingSerialControl);
	BOOL getUsingSerialControl();

	BOOL isSerialControlDLLAvailable();
	BOOL isSerialControlAvailable();

	void testSerialControl(CWnd* wnd);
	BOOL isPortClash(bool silent = false);

	BOOL isComPortActive(int index);
	void setComPortActive(int index, BOOL active);
	
	BOOL isComPortConnected(int index = -1);
	
	bool reconnect();

	int getDebounce();
	void setDebounce(int debounceDelay);

	int getPollInterval();
	void setPollInterval(int pollInterval);

	SerialControlItem captureSerialControl(CWnd* wnd, const SerialControlItem& item);

	void save();
	void load();

	SerialControlManager();
	virtual ~SerialControlManager();

	void updateSerialControlList();

protected:
	
	static VOID CALLBACK SerialControlTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

	bool openPorts();	
	bool closePorts();

	void serialPortManager_notifyPortDataEventFromSeperateThread(int port, PortControlItem pcEvent, bool state, DWORD timestamp);
	
	void fireSerialControlStatusEvent();
	
	BOOL _usingSerialControl;
	
	BOOL _activePorts[MAX_PORTS];

	int _debounce;
	int _interval;

	SerialControlManagerEventListeners _listenerList;


};
