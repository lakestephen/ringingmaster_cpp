// MultiBellInterfaceManager.h: interface for the MultiBellInterfaceManager class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "SerialPortManagerEventListener.h"
#include "MultiBellInterfaceManagerEventListener.h"
#include "MultiBellItem.h"
#include "MultiBellResponse.h"
#include "ManagerBase.h"

enum MBIMode 
{
	mbim_recieve_bell,
	mbim_recieve_presence
};

class MultiBellInterfaceTimerPackage
{
public:

	MultiBellInterfaceTimerPackage(MultiBellInterfaceManagerEventListener* listener, 
							const MultiBellResponse& response) :
	_listener(listener), 
	_response(response)
	{

	}

	MultiBellInterfaceManagerEventListener* _listener;
	MultiBellResponse _response;
};



class MultiBellInterfaceManager : public ManagerBase, SerialPortManagerEventListener
{
public:
	void removeEventListener(MultiBellInterfaceManagerEventListener* multiBellInterfaceManagerEventListener);
	void addEventListener(MultiBellInterfaceManagerEventListener* multiBellInterfaceManagerEventListener);

	int getDelay(int index);
	void setDelay(int index, int delay);

	void setPortID(int port);
	int getPortID();
	BOOL isPortClash();

	void setUsingMBI(BOOL usingMBI);
	BOOL getUsingMBI();

	BOOL isMBIDLLAvailable();
	BOOL isMBIAvailable();
	BOOL isMBIConnected();

	void testMBI(CWnd* wnd);

	void save();
	void load();

	MultiBellInterfaceManager();
	virtual ~MultiBellInterfaceManager();

	bool reconnect();
	void updateMBIList();

	MultiBellItem captureMultiBellInterface(CWnd* pWnd, const MultiBellItem& item);

	static const int _MaxChannel;
	static const int _NoChannel;

protected:

	static VOID CALLBACK TimerCallback(HWND hWnd, UINT nMsg,
											UINT nIDEvent, DWORD dwTime) ;   
	
	static VOID CALLBACK MultiBellInterfaceTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

	bool openPort();	
	bool closePort();

	void requestTestMBIConnection();

	void gotStatusResponce();
	void serialPortManager_notifyPortDataEventFromSeperateThread(int port,BYTE data);
	
	void fireMBIInterfaceEventFromSeperateThread(int inputID);
	void fireMBIStatusEvent();

	void sendDelaysToMBI();
	
	void OnTimer(UINT nIDEvent);


	BOOL _usingMBI;
	int _portID;
	int _delay[12]; // [_MaxChannel]
	bool _MBIConnected;
	MBIMode _mode;

	UINT _statusCheckTimer;

	MultiBellInterfaceManagerEventListeners _listenerList;

};

