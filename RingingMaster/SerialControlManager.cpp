// SerialControlManager.cpp: implementation of the SerialControlManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MultiBellInterfaceManager.h"
#include "SerialControlManager.h"
#include "SerialControlTestDlg.h"
#include "SerialControlResponse.h"
#include "SerialControlItem.h"
#include "SerialControlSelectDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SerialControlManager::SerialControlManager() : 
_debounce(2), 
_interval(5)
{
}

SerialControlManager::~SerialControlManager()
{
	closePorts();
}


void SerialControlManager::addEventListener(SerialControlManagerEventListener* serialControlManagerEventListener)
{
	ASSERT(serialControlManagerEventListener != NULL);

	_listenerList.Add(serialControlManagerEventListener);

	//if we are the first to be added, then open the port
	if (_listenerList.GetSize() == 1)						 
		reconnect();

	updateSerialControlList();
}

void SerialControlManager::removeEventListener(SerialControlManagerEventListener* serialControlManagerEventListener)
{
	ASSERT(serialControlManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == serialControlManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	//if all are removed, then close the port
	if (_listenerList.GetSize() == 0)
		closePorts();

	updateSerialControlList();
}

void SerialControlManager::load()
{
	CString key;	
	for (int i=0;i<MAX_PORTS;i++)
	{
		key.Format("Port %d", i);
		_activePorts[i] = AfxGetApp()->GetProfileInt("Serial Control", key, FALSE);	
	}

	_usingSerialControl = AfxGetApp()->GetProfileInt("Serial Control", "Using Serial Control", FALSE);	

	_debounce = AfxGetApp()->GetProfileInt("Serial Control", "Debounce", 2);	
	_interval = AfxGetApp()->GetProfileInt("Serial Control", "Poll Interval", 5);	
}

void SerialControlManager::save()
{
	CString key;	
	for (int i=0;i<MAX_PORTS;i++)
	{
		key.Format("Port %d", i);
		AfxGetApp()->WriteProfileInt("Serial Control", key, _activePorts[i]);	
	}

	AfxGetApp()->WriteProfileInt("Serial Control", "Using Serial Control", _usingSerialControl);	

	AfxGetApp()->WriteProfileInt("Serial Control", "Debounce", _debounce);	
	AfxGetApp()->WriteProfileInt("Serial Control", "Poll Interval", _interval);	

}

BOOL SerialControlManager::isSerialControlDLLAvailable()
{
	return TRUE; //just for consistancy
}

BOOL SerialControlManager::isSerialControlAvailable()
{
	return (isSerialControlDLLAvailable() && getUsingSerialControl());
}


BOOL SerialControlManager::getUsingSerialControl()
{
	return _usingSerialControl ;
}

void SerialControlManager::setUsingSerialControl(BOOL usingSerialControl)
{						 
	_usingSerialControl = usingSerialControl;

	reconnect();
}

void SerialControlManager::serialPortManager_notifyPortDataEventFromSeperateThread(int port, PortControlItem pcEvent, bool state, DWORD timestamp)
{

	ASSERT(port > 0 && port <=MAX_PORTS);
	if (port <=0 || port >MAX_PORTS)
		return;

	ASSERT(pcEvent >= 0 && pcEvent <=pci_dsr);
	if(pcEvent < 0 || pcEvent >pci_dsr)
		return;

	SimulatorEventData* simulatorEventData;
	SerialControlAction action(port, pcEvent);
	SerialControlResponse response(action, state);

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		SerialControlManagerEventListener* listener = _listenerList.GetAt(i);

		//see if this listener wants this message
		if (listener->_serialControlEvents.Lookup(action, simulatorEventData))
		{
			//calculate if this is valid
			response.setValid(simulatorEventData->eventTrigger(state));
			
			if (simulatorEventData->getDelay() > 0)
			{
				//package up all the information that it needed by the timer proc
				//NOTE: we are using the timestamp, as it will be the first trigger 
				// rather than the end of the debounce trigger time
				SerialControlTimerPackage* serialControlTimerPackage = 
					new SerialControlTimerPackage(listener, response, timestamp + simulatorEventData->getDelay());
				//start the timer
				timeSetEvent(simulatorEventData->getDelay(), 
							0, 
							SerialControlTimerProc, 
							(unsigned long)serialControlTimerPackage,  
							TIME_ONESHOT);
			}
			else
			{
				//NOTE: we are using the timestamp, as it will be the first trigger 
				// rather than the end of the debounce trigger time
	 			listener->serialControlManager_notifySerialControlEventFromSeperateThread(response, timestamp); 
			}
		}
	}

	TRACE("[SerialControlManager::serialPortManager_notifyPortDataEventFromSeperateThread] PortControlItem = %d\r\n", pcEvent);
}

VOID CALLBACK SerialControlManager::SerialControlTimerProc(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{					
//we only want the catch block in release

#ifndef _DEBUG
	try 
	{
#endif
		SerialControlTimerPackage* serialControlTimerPackage = (SerialControlTimerPackage*)dwUser;
		
		serialControlTimerPackage->_listener->serialControlManager_notifySerialControlEventFromSeperateThread(
			serialControlTimerPackage->_response, serialControlTimerPackage->_timestamp); 

		delete serialControlTimerPackage;

#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in  SerialControlManager::SerialControlTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}

bool SerialControlManager::reconnect()
{
	CWaitCursor wait;

	closePorts();
	
	if (_listenerList.GetSize() >= 1)						 
	{
		if (openPorts())
		{
			updateSerialControlList();
			return true;
		}
	}


	return false;
}

bool SerialControlManager::openPorts()
{
	closePorts();

	//signal that the MBI is not available
	fireSerialControlStatusEvent();

	if (!_usingSerialControl)
		return false;

	ASSERT(_listenerList.GetSize() > 0);

	//create the port
	for (int i=0;i<MAX_PORTS;i++)
	{
		if (_activePorts[i])
			// as we aew not using TX/RX, port settings do not matter
			serialPortManager()->createPort(this, TRUE,  i+1, 19200, 78, 8, 1,
									EV_CTS|EV_DSR|EV_RLSD|EV_RING);
			serialPortManager()->setPortInterval(this, i+1, _interval, _debounce);
	}                


	//update
	fireSerialControlStatusEvent();

	return true;
}

bool SerialControlManager::closePorts()
{
	for (int i=0;i<MAX_PORTS;i++)
	{
		if (_activePorts[i] && serialPortManager()->doIOwnPort(this, i+1))
			serialPortManager()->destroyPort(this, i+1);
	}

	fireSerialControlStatusEvent();

	return true;
}

void SerialControlManager::testSerialControl(CWnd* parent)
{
	SerialControlTestDlg dlg(parent);
	dlg.DoModal();
}

BOOL SerialControlManager::isPortClash(bool silent)
{
	//all ok if we are not using MBI or Serial Control
	if (!multiBellInterfaceManager()->isMBIAvailable() ||
		!isSerialControlAvailable())
		return FALSE;

	int MBIPort = multiBellInterfaceManager()->getPortID();
	ASSERT ((MBIPort > 0)&&(MBIPort <= MAX_PORTS));

	if ((MBIPort > 0)&&(MBIPort <= MAX_PORTS) && isComPortActive(MBIPort))
	{
		if (!silent)
		{
			CString str;
			str.Format("Port %d is also being used the Multi Bell Interface. Please change ports so that they do not clash. \r\n(NOTE: If you are not using either the Multi Bell Interface or the Serial Port Control, you can disable them.)", MBIPort);
			AfxMessageBox(str);
		}

		return TRUE;
	}		

	return FALSE;
}


//NOTE 1 Based.
void SerialControlManager::setComPortActive(int index, BOOL active)
{
	ASSERT(index >0 && index <= MAX_PORTS);
	
	if(index >0 && index <= MAX_PORTS)
	{
		if (_activePorts[index-1] != active)
		{
			closePorts();
			_activePorts[index-1] = active;
			reconnect();
		}                          
	}	
}
//NOTE 1 Based.
BOOL SerialControlManager::isComPortActive(int index)
{
	ASSERT(index >0 && index <= MAX_PORTS);
	
	if(index >0 && index <= MAX_PORTS)
	{
		return _activePorts[index-1];
	}

	return FALSE;
}

BOOL SerialControlManager::isComPortConnected(int index)
{
	if (index <0)
	{
		for (int i=1;i<MAX_PORTS;i++)
		{
			if ((isComPortActive(i)) && (serialPortManager()->doIOwnPort(this, i)))
				return TRUE;
		}
	}

	else if (isComPortActive(index))
	{
		return serialPortManager()->doIOwnPort(this, index);
	}

	return FALSE;
}

int SerialControlManager::getDebounce()
{
	return _debounce;
}

void SerialControlManager::setDebounce(int debounce)
{
	_debounce = debounce;
	
	reconnect();
}

int SerialControlManager::getPollInterval()
{
	return _interval;
}

void SerialControlManager::setPollInterval(int pollInterval)
{
	_interval = pollInterval;

	reconnect();
}

void SerialControlManager::fireSerialControlStatusEvent()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->serialControlManager_notifySerialControlStatusEvent();
	}
}

SerialControlItem SerialControlManager::captureSerialControl(CWnd* wnd, const SerialControlItem& item)
{
	SerialControlSelectDlg dlg(wnd, item);
	if (dlg.DoModal() == IDOK)
	{
		return dlg._serialControlItem;
	}
	
	return item;
	
}

void SerialControlManager::updateSerialControlList()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		SerialControlManagerEventListener* listener = _listenerList.GetAt(i);
		listener->clearEventMap();
		listener->serialControlManager_notifyGetSerialControlList(listener->_serialControlEvents); 
	} 
} 
