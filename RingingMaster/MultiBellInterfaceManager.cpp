// MultiBellInterfaceManager.cpp: implementation of the MultiBellInterfaceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MultiBellInterfaceManager.h"
#include "SerialPortManager.h"
#include "SerialControlManager.h"
#include "MultiBellInterfaceTestDlg.h"
#include "MultiBellInterfaceChooseChannelDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int MultiBellInterfaceManager::_MaxChannel = 12;
const int MultiBellInterfaceManager::_NoChannel = -1;  


MultiBellInterfaceManager::MultiBellInterfaceManager() :
_portID(1),
_MBIConnected(false),
_mode(mbim_recieve_bell),
_statusCheckTimer(0)
{
	for (int i=0;i<_MaxChannel;i++)
	{
		_delay[i] = 1;
	}	
}

MultiBellInterfaceManager::~MultiBellInterfaceManager()
{
	closePort();
}


void MultiBellInterfaceManager::addEventListener(MultiBellInterfaceManagerEventListener* multiBellInterfaceManagerEventListener)
{
	
	ASSERT(multiBellInterfaceManagerEventListener != NULL);

	_listenerList.Add(multiBellInterfaceManagerEventListener);

	//if we are the first to be added, then open the port
	if (_listenerList.GetSize() == 1)					 
		openPort();

	updateMBIList();
}

void MultiBellInterfaceManager::removeEventListener(MultiBellInterfaceManagerEventListener* multiBellInterfaceManagerEventListener)
{
	ASSERT(multiBellInterfaceManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == multiBellInterfaceManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 


	//if all are removed, then close the port
	if (_listenerList.GetSize() == 0)
		closePort();

	updateMBIList();
}

void MultiBellInterfaceManager::load()
{
	_usingMBI = AfxGetApp()->GetProfileInt("Multi Bell Interface", "Using MBI", FALSE);	
	_portID = AfxGetApp()->GetProfileInt("Multi Bell Interface", "Com Port", 1);	

	CString key;

	for (int i=0;i<_MaxChannel;i++)
	{
		key.Format("Delay %d", i);
		_delay[i] = AfxGetApp()->GetProfileInt("Multi Bell Interface", key, 20);	
	}

}

void MultiBellInterfaceManager::save()
{
	AfxGetApp()->WriteProfileInt("Multi Bell Interface", "Using MBI", _usingMBI );	
	AfxGetApp()->WriteProfileInt("Multi Bell Interface", "Com Port", _portID);	

	CString key;

	for (int i=0;i<_MaxChannel;i++)
	{
		key.Format("Delay %d", i);
		AfxGetApp()->WriteProfileInt("Multi Bell Interface", key, _delay[i]);	
	}
}

BOOL MultiBellInterfaceManager::isMBIDLLAvailable()
{
	return TRUE; //just for consistancy
}

BOOL MultiBellInterfaceManager::isMBIAvailable()
{
	return (isMBIDLLAvailable() && getUsingMBI());
}

BOOL MultiBellInterfaceManager::isMBIConnected()
{
	return (isMBIAvailable() && _MBIConnected);
}

BOOL MultiBellInterfaceManager::getUsingMBI()
{
	return _usingMBI;
}

void MultiBellInterfaceManager::setUsingMBI(BOOL usingMBI)
{						 
	_usingMBI = usingMBI;

	openPort();

	void updateMBIList();
}

int MultiBellInterfaceManager::getPortID()
{  
	return _portID;
}

void MultiBellInterfaceManager::setPortID(int port)
{
	closePort();

	_portID = port;

	openPort();

	updateMBIList();
}

BOOL MultiBellInterfaceManager::isPortClash()
{
	//all ok if we are not using Serial Control	 or MBI
	if (!serialControlManager()->isSerialControlAvailable() ||
		!isMBIAvailable())
		return FALSE;

	int MBIPort = getPortID();
	ASSERT ((MBIPort > 0)&&(MBIPort <= MAX_PORTS));

	if ((MBIPort > 0)&&(MBIPort <= MAX_PORTS) && serialControlManager()->isComPortActive(MBIPort))
	{
		CString str;
		str.Format("Port %d is also being used by and the Serial Control. Please change ports so that they do not clash. \r\n(NOTE: If you are not using either the Multi Bell Interface or the Serial Ports, you can disable them.)", MBIPort);

		AfxMessageBox(str);
		return TRUE;
	}		

	return FALSE;
}

void MultiBellInterfaceManager::sendDelaysToMBI()
{
	if (_MBIConnected && serialPortManager()->isValidPort(_portID))
	{
		//255 is a MBI terminator char. 0 is the string terminator
		BYTE data[_MaxChannel+2] = {1,1,1,1,1,1,1,1,1,1,1,1,255,0}; 

		//this is commented out as we are using software timers.
		// for the delays
	/*	for (int i=0;i<_MaxChannel;i++)
		{
			data[i] = (unsigned char)_delay[i];
		}
	*/
		TRACE("Send delays to the MBI device\r\n");
		
		serialPortManager()->writeToPort(_portID, (char*)data);
	}
}

void MultiBellInterfaceManager::setDelay(int index, int delay)
{
	ASSERT(index >=0 && index <_MaxChannel);

	if (_delay[index] != delay)
	{
		_delay[index] = delay;
		sendDelaysToMBI();
	}

//	TRACE("Index: %d, Delay: %d\r\n", index, delay);
}

int MultiBellInterfaceManager::getDelay(int index)
{
	ASSERT(index >=0 && index <_MaxChannel);

	return _delay[index];
}

void MultiBellInterfaceManager::serialPortManager_notifyPortDataEventFromSeperateThread(int /*port*/, BYTE data)
{
	TRACE("MBI: Port Data: %d\r\n", data);

	if (_mode == mbim_recieve_presence)
	{
		if (data == 0xFD)
			_MBIConnected = true;

		gotStatusResponce();
	}
	else if (_mode == mbim_recieve_bell)
	{
		//do the translation
		switch (data)
		{
		case 49: //char'1'
			fireMBIInterfaceEventFromSeperateThread(0); break;
		case 50: //char'2'
			fireMBIInterfaceEventFromSeperateThread(1); break;
		case 51: //char'3'
			fireMBIInterfaceEventFromSeperateThread(2); break;
		case 52: //char'4'
			fireMBIInterfaceEventFromSeperateThread(3); break;
		case 53: //char'5'
			fireMBIInterfaceEventFromSeperateThread(4); break;
		case 54: //char'6'
			fireMBIInterfaceEventFromSeperateThread(5); break;
		case 55: //char'7'
			fireMBIInterfaceEventFromSeperateThread(6); break;
		case 56: //char'8'
			fireMBIInterfaceEventFromSeperateThread(7); break;
		case 57: //char'9'
			fireMBIInterfaceEventFromSeperateThread(8); break;
		case 48: //char'0'
			fireMBIInterfaceEventFromSeperateThread(9); break;
		case 69: //char'E'
			fireMBIInterfaceEventFromSeperateThread(10); break;
		case 84: //char'T'
			fireMBIInterfaceEventFromSeperateThread(11); break;
		}
	}
}

void MultiBellInterfaceManager::gotStatusResponce()
{
	KillTimer(NULL, _statusCheckTimer);

	_mode = mbim_recieve_bell;
	
	fireMBIStatusEvent();

	if (_MBIConnected)
	{
		sendDelaysToMBI();
	}
	else
	{
		closePort();
	}
}


bool MultiBellInterfaceManager::reconnect()
{
	CWaitCursor wait;
	closePort();

	if (_listenerList.GetSize() >= 1)					 
	{
		if (openPort())
		{
		 	updateMBIList();
			return true;
		}
	}

	return false;
}

bool MultiBellInterfaceManager::openPort()
{
	//make sure that it is already closed
	closePort();

  	if (!_usingMBI)
		return false;

	ASSERT(_listenerList.GetSize() > 0);

	//create the port
	bool ret = serialPortManager()->createPort(this, FALSE, _portID, 2400);

	//request that the status is checked
	requestTestMBIConnection();

	return ret;
}

bool MultiBellInterfaceManager::closePort()
{
	//signal that the MBI is not available
	_MBIConnected = false;
	fireMBIStatusEvent();

	if (serialPortManager()->doIOwnPort(this, _portID))
		return serialPortManager()->destroyPort(this, _portID);

	return false;
}

//the result will be bradcast using the fireMBIStatusEvent() function
void MultiBellInterfaceManager::requestTestMBIConnection()
{
	if (_usingMBI && serialPortManager()->isValidPort(_portID))
	{
		//send the presence check 
		_mode = mbim_recieve_presence;

		BYTE data[2] = {0xFD,0};
		serialPortManager()->writeToPort(_portID, (char*)data);
	}

	::KillTimer(NULL, _statusCheckTimer);
	_statusCheckTimer = ::SetTimer(NULL, 
						  NULL, 
						  (UINT)2000, // mS 
						  MultiBellInterfaceManager::TimerCallback);
}

void MultiBellInterfaceManager::fireMBIInterfaceEventFromSeperateThread(int inputID)
{
	SimulatorEventData* data;
	MultiBellAction action(inputID);
	MultiBellResponse response(action);

	DWORD time = 0;

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		MultiBellInterfaceManagerEventListener* listener = _listenerList.GetAt(i);
		
		if (listener->_mbiEvents.Lookup(action, data))
		{
			//calculate if this is valid
			response.setValid(data->eventTrigger());
			
			if (data->getDelay() > 0)
			{
				//package up all the information that it needed by the timer proc
				MultiBellInterfaceTimerPackage* multiBellInterfaceTimerPackage = 
					new MultiBellInterfaceTimerPackage(listener, response);
				//start the timer
				timeSetEvent(data->getDelay(), 
							0, 
							MultiBellInterfaceTimerProc, 
							(unsigned long)multiBellInterfaceTimerPackage,  
							TIME_ONESHOT);	 
			}
			else
			{
				if (time == 0)
					time = timeGetTime();
				listener->multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(response, time); 
			}
		}
	}
}

VOID CALLBACK MultiBellInterfaceManager::MultiBellInterfaceTimerProc(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{					
//we only want the catch block in release

#ifndef _DEBUG
	try 
	{
#endif
		MultiBellInterfaceTimerPackage* multiBellInterfaceTimerPackage = (MultiBellInterfaceTimerPackage*)dwUser;
		
		multiBellInterfaceTimerPackage->_listener->multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(
			multiBellInterfaceTimerPackage->_response, timeGetTime()); 

		delete multiBellInterfaceTimerPackage;

#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in MultiBellInterfaceManager::MultiBellInterfaceTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}

void MultiBellInterfaceManager::fireMBIStatusEvent()
{
	bool portGood = serialPortManager()->isValidPort(_portID);

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->multiBellInterfaceManager_notifyMBIStatusEvent(portGood, _MBIConnected); 
	}
}


VOID CALLBACK MultiBellInterfaceManager::TimerCallback(
					HWND /*hWnd*/,      // handle of CWnd that called SetTimer
					UINT /*nMsg*/,      // WM_TIMER   
					UINT nIDEvent,   // timer identification
					DWORD /*dwTime*/)   // system time
{
	multiBellInterfaceManager()->OnTimer(nIDEvent);
}	

void MultiBellInterfaceManager::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == _statusCheckTimer)
	{
		gotStatusResponce();
	}
}

MultiBellItem MultiBellInterfaceManager::captureMultiBellInterface(CWnd* pWnd, const MultiBellItem& item)
{
	MultiBellInterfaceChooseChannelDlg dlg(pWnd, item);

	if (dlg.DoModal() == IDOK)
	{
		return dlg._multiBellItem;
	}

	return item;
}

void MultiBellInterfaceManager::testMBI(CWnd* wnd)
{   
	if (isMBIConnected())
	{
		MultiBellInterfaceTestDlg dlg(wnd);

		dlg.DoModal();
	}
}

void MultiBellInterfaceManager::updateMBIList()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		MultiBellInterfaceManagerEventListener* listener = _listenerList.GetAt(i);
		listener->clearEventMap();
		listener->multiBellInterfaceManager_notifyGetMultiBellInterfaceList(listener->_mbiEvents); 
	} 
}  
