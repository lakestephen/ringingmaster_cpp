/*
**	FILENAME			SerialPort.h
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

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#include "UserMessages.h"


#define MAX_PORTS 4

enum PortControlItem
{
	pci_rlsd,
	pci_ring,
	pci_cts,
	pci_dsr, //leave at the end, as used for array size definitions
};

class SerialPortManagerEventListener;


class SerialPort
{														 
public:
	int getPortID();
	// contruction and destruction
	SerialPort(SerialPortManagerEventListener* serialPortManagerEventListener);
	virtual		~SerialPort();

	// port initialisation											
	BOOL		OpenPort(UINT portID, UINT baud = 19200, char parity = 'N', UINT databits = 8, UINT stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR, UINT nBufferSize = 512);
	void		setInterval(DWORD interval, int debounceCount);

	// start/stop comm watching
	BOOL		StartMonitoringThread(BOOL control);

	void		WriteToPort(char* string);
	
	const SerialPortManagerEventListener* getSerialPortManagerEventListener();

protected:
	// protected memberfunctions
	BOOL ClosePort();
	void		ProcessErrorMessage(char* ErrorText);
	
	static UINT	CommThreadControl(LPVOID pParam);
	static void CommThreadControlProcessState(SerialPort* port, bool& bKnownState, int& iStateCount, DWORD& timestamp, bool bStateNow, PortControlItem pcEvent);

	static UINT	CommThread(LPVOID pParam);
	static void CommThreadProcessMessage(SerialPort* port, COMSTAT& comstat, DWORD& dwCommEvent, bool triggered = TRUE);
	static void CommThreadProcessState(SerialPort* port, bool bEvent, bool state, PortControlItem pcEvent, int portID, bool& storedState, bool triggered);
	static void	ReceiveChar(SerialPort* port, COMSTAT comstat);
	static void	WriteChar(SerialPort* port);

	void		StopMonitoringThread();

	// thread
	CWinThread*			m_Thread;
	SerialPortManagerEventListener* _serialPortManagerEventListener;

	// synchronisation objects
	CRITICAL_SECTION	m_csCommunicationSync;
	BOOL				m_bThreadAlive;

	// handles
	HANDLE				m_hShutdownEvent;
	BOOL				m_bShutdownFlag;
	HANDLE				m_hComm;
	HANDLE				m_hWriteEvent;
	HANDLE				m_hEventArray[3];

	// structures
	OVERLAPPED			m_ov;
	COMMTIMEOUTS		m_CommTimeouts;
	DCB					m_dcb;

	// misc
	int				m_nPortNr;
	char*				m_szWriteBuffer;

	//flags for lines
	bool bRING;
	bool bRLSD;
	bool bCTS;
	bool bDSR;

	DWORD _interval;
	int _debounceCount;
};

#endif __SERIALPORT_H__


