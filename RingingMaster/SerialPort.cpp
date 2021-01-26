/*
**	FILENAME			SerialPort.cpp
**
**	PURPOSE				This class can read, write and watch one serial port.
**						It sends messages to its owner when something happends on the port
**						The class creates a thread for reading and writing so the main
**						program is not blocked.
**
**	CREATION DATE		15-09-1997
**	LAST MODIFICATION	12-11-1997
**
**	AUTHOR				Remon Spekreijse
**
**
*/

#include "stdafx.h"
#include "SerialPort.h"
#include "SerialPortManagerEventListener.h"

SerialPort::SerialPort(SerialPortManagerEventListener* serialPortManagerEventListener) :
_serialPortManagerEventListener(serialPortManagerEventListener), 
_interval(INFINITE), 
_debounceCount(1)
{
	m_hComm = NULL;

	// initialize overlapped structure members to zero
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	// create events
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;
	m_bShutdownFlag = FALSE;

	m_szWriteBuffer = NULL;

	m_bThreadAlive = FALSE;

	m_Thread = NULL;
	m_nPortNr = -1;
}

SerialPort::~SerialPort()
{
	StopMonitoringThread();

	ClosePort();

	delete [] m_szWriteBuffer;
}

BOOL SerialPort::StartMonitoringThread(BOOL control)
{
	ASSERT(m_Thread == 0);

	if (control)
		m_Thread = AfxBeginThread(CommThreadControl, this);
	else
		m_Thread = AfxBeginThread(CommThread, this);

	if (!m_Thread)
		return FALSE;

	TRACE("[SerialPort::StartMonitoringThread] Thread started: %d\r\n", m_nPortNr);
	return TRUE;	
}

void SerialPort::StopMonitoringThread()
{
	do
	{
		m_bShutdownFlag = TRUE;
		SetEvent(m_hShutdownEvent);

	} while (m_bThreadAlive);

	TRACE("[SerialPort::StopMonitoringThread] Thread Stopped: %d\r\n", m_nPortNr);
}

void SerialPort::setInterval(DWORD interval, int debounceCount)
{
 	_interval = interval;
	_debounceCount = debounceCount;
}

BOOL SerialPort::OpenPort( UINT  portID,		// portnumber (1..4)
						   UINT  baud,			// baudrate
						   char  parity,		// parity 
						   UINT  databits,		// databits 
						   UINT  stopbits,		// stopbits 
						   DWORD dwCommEvents,	// EV_RXCHAR, EV_CTS, EV_RING (RI), EV_RLSD, EV_DSR, EV_RXFLAG, EV_BREAK, EV_ERR
						   UINT  writebuffersize)	// size to the writebuffer
{
	assert(portID > 0 && portID <= MAX_PORTS);

	// if the thread is alive: Kill
	if (m_bThreadAlive)
	{
		StopMonitoringThread();
	}

	// create events
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hWriteEvent != NULL)
		ResetEvent(m_hWriteEvent);
	m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_bShutdownFlag = FALSE;

	// initialize the event objects
	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	// initialize critical section
	InitializeCriticalSection(&m_csCommunicationSync);
	
	if (m_szWriteBuffer != NULL)
		delete [] m_szWriteBuffer;
	m_szWriteBuffer = new char[writebuffersize];

	m_nPortNr = portID;

	char *szPort = new char[50];
	char *szBaud = new char[50];

	// now it critical!
	EnterCriticalSection(&m_csCommunicationSync);

	// if the port is already opened: close it
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings
	sprintf(szPort, "COM%d", portID);
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);

	// get a handle to the port
	m_hComm = CreateFile(szPort,						// communication port string (COMX)
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0,								// comm devices must be opened with exclusive access
					     NULL,							// no security attributes
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// Async I/O
					     0);							// template must be 0 for comm devices

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;

		return FALSE;
	}

	// set the timeout values
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// configure
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{						   
		if (SetCommMask(m_hComm, dwCommEvents))
		{
			if (GetCommState(m_hComm, &m_dcb))
			{
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
				if (BuildCommDCB(szBaud, &m_dcb))
				{
					if (SetCommState(m_hComm, &m_dcb))
						; // normal operation... continue
					else
						ProcessErrorMessage("SetCommState()");
				}
				else
					ProcessErrorMessage("BuildCommDCB()");
			}
			else
				ProcessErrorMessage("GetCommState()");
		}
		else
			ProcessErrorMessage("SetCommMask()");
	}
	else
		ProcessErrorMessage("SetCommTimeouts()");

	delete [] szPort;
	delete [] szBaud;

	// flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// release critical section
	LeaveCriticalSection(&m_csCommunicationSync);

	TRACE("[SerialPort::OpenPort] Port Opened: %d\r\n", portID);

	return TRUE;
}

BOOL SerialPort::ClosePort()
{
	if (m_hComm != NULL && (unsigned int)m_hComm != -1)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
		TRACE("[SerialPort::ClosePort] Port Closed: %d\r\n", m_nPortNr);
		
		//give the port a chance to close.
		Sleep(200);
	}              

	return TRUE;
}

UINT SerialPort::CommThreadControl(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to
	// a pointer of SerialPort class
	SerialPort *port = (SerialPort*)pParam;
	
	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running.
	port->m_bThreadAlive = TRUE;

	bool bKnownState[4]; 
	int iStateCount[4];
	DWORD dwTimeStamp[4];

	for (int i=0;i<4;i++)
	{
		iStateCount[i] = 0;
		dwTimeStamp[i] = 0;
	}

	DWORD dwModemStatus;
	GetCommModemStatus(port->m_hComm, &dwModemStatus);

	bKnownState[pci_rlsd] = ((dwModemStatus & EV_RLSD) != 0);
	bKnownState[pci_ring] = ((dwModemStatus & EV_RING) != 0);
	bKnownState[pci_cts]  = ((dwModemStatus & EV_CTS)  != 0);
	bKnownState[pci_dsr]  = ((dwModemStatus & EV_DSR)  != 0);

	while(!port->m_bShutdownFlag)
	{
		Sleep(min(port->_interval, 1000));   //to2do replace with multi media timer   like keyboard - global simulator polling frequency / accuracy for lower processor speed?? 
		GetCommModemStatus(port->m_hComm, &dwModemStatus);
		
		CommThreadControlProcessState(port, bKnownState[pci_rlsd], iStateCount[pci_rlsd], 
			dwTimeStamp[pci_rlsd], ((dwModemStatus & MS_RLSD_ON) != 0), pci_rlsd); 
		CommThreadControlProcessState(port, bKnownState[pci_ring], iStateCount[pci_ring], 
			dwTimeStamp[pci_ring], ((dwModemStatus & MS_RING_ON) != 0), pci_ring); 
		CommThreadControlProcessState(port, bKnownState[pci_cts], iStateCount[pci_cts], 
			dwTimeStamp[pci_cts], ((dwModemStatus & MS_CTS_ON) != 0), pci_cts); 
		CommThreadControlProcessState(port, bKnownState[pci_dsr], iStateCount[pci_dsr], 
			dwTimeStamp[pci_dsr], ((dwModemStatus & MS_DSR_ON) != 0), pci_dsr); 
	}
					   
	port->m_bThreadAlive = FALSE;

	return 0;
}

inline 
void SerialPort::CommThreadControlProcessState(SerialPort* port, bool& bKnownState, 
													  int& iStateCount, DWORD& timestamp, 
													  bool bStateNow, PortControlItem pcEvent)
{
	if (bKnownState == bStateNow)
	{
	 	iStateCount = 0;
	}                    
	else
	{
		if (iStateCount == 0)
			timestamp = timeGetTime();   //NOTE: make sure that the timestamp is for the first trigger

		iStateCount++;
		if (iStateCount > port->_debounceCount)
		{
			bKnownState = bStateNow;
			iStateCount = 0;

			port->_serialPortManagerEventListener->serialPortManager_notifyPortDataEventFromSeperateThread(
				port->getPortID(), pcEvent,  bKnownState, timestamp);				
			
		}
	}
}

UINT SerialPort::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to
	// a pointer of SerialPort class
	SerialPort *port = (SerialPort*)pParam;
	
	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running.
	port->m_bThreadAlive = TRUE;	
		
	// Misc. variables
	DWORD dwCommEvent = 0;
	DWORD dwError = 0;
	DWORD dwWaitEvent = 0;
	BOOL bWaitingForCommEvent = FALSE;
	COMSTAT comstat;
	comstat.fCtsHold = true; //here just to satisfy the compiler at warning level 4
		
	// Clear comm buffers at startup
	if (port->m_hComm)		// check if the port is opened
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	//reset the status lines
	port->bRING = FALSE;
	port->bRLSD = FALSE;
	port->bCTS = FALSE;
	port->bDSR = FALSE;

	// begin forever loop. 
	while (true)
	{           
		if (!bWaitingForCommEvent)
		{
			if (!WaitCommEvent(port->m_hComm, &dwCommEvent, &port->m_ov))  
			{ 
				switch (dwError = GetLastError()) 
				{ 
				case ERROR_IO_PENDING: 	// This is a normal return value if there are no bytes
				case ERROR_INVALID_PARAMETER:// Under Windows NT, this value is returned for some reason.
					bWaitingForCommEvent = TRUE;
					break;
				default:// All other error codes indicate a serious error has occured.  
					ASSERT(FALSE);
					port->ProcessErrorMessage("WaitCommEvent()");
					port->m_bThreadAlive = FALSE;
					AfxEndThread(200);
					break;
				}
			}
			else
			{ 		
				//this processes events that are already waiting when we call WaitCommEvent 
				ClearCommError(port->m_hComm, &dwError, &comstat);
				CommThreadProcessMessage(port, comstat, dwCommEvent);
			}	
		}

		
		if (bWaitingForCommEvent)
		{
			// Main wait function.  This function will normally block the thread
			dwWaitEvent = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, port->_interval);  //timeout

			switch (dwWaitEvent)
			{
			case WAIT_OBJECT_0+0://kill event
				port->m_bThreadAlive = FALSE;
				AfxEndThread(0);
				break;
			case WAIT_OBJECT_0+1:	// read event
				CommThreadProcessMessage(port, comstat, dwCommEvent);
				bWaitingForCommEvent = FALSE;
				break;
			case WAIT_OBJECT_0+2: // write event
				WriteChar(port);
				break;
			case WAIT_TIMEOUT: //go round again, but first check that our state matches
				CommThreadProcessMessage(port, comstat, dwCommEvent, false);
				break;
			default: //error occured
				ASSERT(FALSE);
				port->ProcessErrorMessage("WaitForMultipleObjects()");
				port->m_bThreadAlive = FALSE;
				AfxEndThread(100);
			} 
		}           
	} 

	ASSERT(FALSE);
	return 0;
}

void SerialPort::CommThreadProcessMessage(SerialPort* port, COMSTAT& comstat, DWORD& dwCommEvent, bool triggered)
{
	//get the actual status
	DWORD dwModemStatus;
	if (!GetCommModemStatus(port->m_hComm, &dwModemStatus))
	{
		ASSERT(FALSE);	
		port->ProcessErrorMessage("GetCommModemStatus()");
	}                 

	//check the control states
	CommThreadProcessState(port, ((dwCommEvent & EV_RLSD) != 0), 
					((dwModemStatus & MS_RLSD_ON)!=0), pci_rlsd, port->getPortID(), port->bRLSD, triggered);
	CommThreadProcessState(port, ((dwCommEvent & EV_RING) != 0), 
					((dwModemStatus & MS_RING_ON)!=0), pci_ring, port->getPortID(), port->bRING, triggered);
	CommThreadProcessState(port, ((dwCommEvent & EV_CTS) != 0), 
					((dwModemStatus & MS_CTS_ON)!=0), pci_cts, port->getPortID(), port->bCTS, triggered);
	CommThreadProcessState(port, ((dwCommEvent & EV_DSR) != 0), 
					((dwModemStatus & MS_DSR_ON)!=0), pci_dsr, port->getPortID(), port->bDSR, triggered);	
	
	if (dwCommEvent & EV_RXCHAR)
		// Receive character event from port.
		ReceiveChar(port, comstat);	
}    

void SerialPort::CommThreadProcessState(SerialPort* port, bool bEvent, bool state, 
										PortControlItem pcEvent, int portID, bool& storedState, bool triggered)
{
	if (triggered && bEvent)
	{  
		ASSERT(FALSE);// we are not using this.	See ::CommThreadControl

		DWORD time = timeGetTime();

		if (state == storedState)
		{
			//if the actual status differs from the stored status, 
			// we force notify the opposite state first as we have missed one
			port->_serialPortManagerEventListener->serialPortManager_notifyPortDataEventFromSeperateThread(
				portID, pcEvent, !state, time);
		}
		
		//normal notify
		port->_serialPortManagerEventListener->serialPortManager_notifyPortDataEventFromSeperateThread(
			portID, pcEvent, state, time);

	}	
	else
	{
		//not triggered. check that it is the same.
		if (state != storedState)
		{
			port->_serialPortManagerEventListener->serialPortManager_notifyPortDataEventFromSeperateThread(
				portID, pcEvent, state, timeGetTime());
		}
	}    

	//set the port value to the 
	storedState = state;  			
}

//
// If there is a error, give the right message
//
void SerialPort::ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf(Temp, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr); 
	MessageBox(NULL, Temp, "Application Error", MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete[] Temp;
}

//
// Write a character.
//
void SerialPort::WriteChar(SerialPort* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;

	DWORD BytesSent = 0;

	ResetEvent(port->m_hWriteEvent);

	// Gain ownership of the critical section
	EnterCriticalSection(&port->m_csCommunicationSync);

	if (bWrite)
	{
		// Initailize variables
		port->m_ov.Offset = 0;
		port->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		bResult = WriteFile(port->m_hComm,							// Handle to COMM Port
							port->m_szWriteBuffer,					// Pointer to message buffer in calling finction
							strlen((char*)port->m_szWriteBuffer),	// Length of message to send
							&BytesSent,								// Where to store the number of bytes sent
							&port->m_ov);							// Overlapped structure

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						// continue to GetOverlappedResults()
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						// all other error codes
						port->ProcessErrorMessage("WriteFile()");
					}
			}
		} 
		else
		{
			LeaveCriticalSection(&port->m_csCommunicationSync);
		}
	} // end if(bWrite)

	if (!bWrite)
	{
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
									  &port->m_ov,		// Overlapped structure
									  &BytesSent,		// Stores number of bytes sent
									  TRUE); 			// Wait flag

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// deal with the error code 
		if (!bResult)  
		{
			port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	} // end if (!bWrite)

	// Verify that the data size send equals what we tried to send
	if (BytesSent != strlen((char*)port->m_szWriteBuffer))
	{
		TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
	}
}

//
// Character received. Inform the owner
//
void SerialPort::ReceiveChar(SerialPort* port, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff = '\0';

	while (true) 
	{ 
		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program 
		// is using the port object. 
		
		EnterCriticalSection(&port->m_csCommunicationSync);

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		
		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// start forever loop.  I use this type of loop because I
		// do not know at runtime how many loops this will have to
		// run. My solution is to start a forever loop and to
		// break out of it when I have processed all of the
		// data available.  Be careful with this approach and
		// be sure your loop will exit.
		// My reasons for this are not as clear in this sample 
		// as it is in my production code, but I have found this 
		// solutiion to be the most efficient way to do this.
		
		if (comstat.cbInQue == 0)
		{
			// break out when all bytes have been read
			break;
		}
						
		EnterCriticalSection(&port->m_csCommunicationSync);

		if (bRead)
		{
			bResult = ReadFile(port->m_hComm,		// Handle to COMM port 
							   &RXBuff,				// RX Buffer Pointer
							   1,					// Read one byte
							   &BytesRead,			// Stores number of bytes read
							   &port->m_ov);		// pointer to the m_ov structure
			// deal with the error code 
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							// asynchronous i/o is still in progress 
							// Proceed on to GetOverlappedResults();
							bRead = FALSE;
							break;
						}
					default:
						{
							// Another error has occured.  Process this error.
							port->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else
			{
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}  // close if (bRead)

		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
										  &port->m_ov,		// Overlapped structure
										  &BytesRead,		// Stores number of bytes read
										  TRUE); 			// Wait flag

			// deal with the error code 
			if (!bResult)  
			{
				port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}	
		}  // close if (!bRead)
				
		LeaveCriticalSection(&port->m_csCommunicationSync);

		// notify parent that a byte was received
		port->_serialPortManagerEventListener->serialPortManager_notifyPortDataEventFromSeperateThread(port->m_nPortNr, RXBuff);


	} // end forever loop

}

//
// Write a string to the port
//
void SerialPort::WriteToPort(char* string)
{		
	assert(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strcpy(m_szWriteBuffer, string);

	// set event for write
	SetEvent(m_hWriteEvent);
}
    
int SerialPort::getPortID()
{
	return m_nPortNr;
}

const SerialPortManagerEventListener* SerialPort::getSerialPortManagerEventListener()
{
	return _serialPortManagerEventListener;
}
