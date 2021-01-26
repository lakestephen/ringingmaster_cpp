#pragma once
#include "SerialPort.h"

class SerialControlAction
{
public:
	SerialControlAction(int port, PortControlItem pcEvent);
	SerialControlAction();
	~SerialControlAction(void);

	virtual CString getDescription()const;
	static CString getPortControlItemDescription(PortControlItem pcEvent) ;

	bool operator ==(const SerialControlAction & serialControlAction)	const;
	virtual void Serialize(CArchive &ar);

	int getPort() const;
	PortControlItem getEvent() const;
	void setPort(int port);
	void setEvent(PortControlItem pcEvent) ;


	bool isValid() const ;
protected:
	int _port;
	PortControlItem _event;
};  

template<>
	inline UINT AFXAPI HashKey(const SerialControlAction& key)
{
	//create a hash of the values
	return (key.getPort() * 10) + key.getEvent();
}