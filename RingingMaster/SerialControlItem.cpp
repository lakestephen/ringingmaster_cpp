#include "StdAfx.h"
#include ".\serialcontrolitem.h"

SerialControlItem::SerialControlItem(const SerialControlAction& serialControlAction ) :
SerialControlAction(serialControlAction),
_data(esigt_riseing)
{

}

SerialControlItem::SerialControlItem() :
_data(esigt_riseing) 
{

}

SerialControlItem::~SerialControlItem(void)
{
}

CString SerialControlItem::getDescription() const
{
	CString delay;
	if (_data.getDelay() > 0)
		delay.Format(", %dms", _data.getDelay());

	return SerialControlAction::getDescription() +
		((_data.getEventSequenceType() == est_1of1)?", ":(", " + _data.getSequenceTypeDescription() + " ")) + 
		_data.getSignalTypeDescription() +  
		delay;
}

SimulatorEventData& SerialControlItem::getSimulatorEventData()
{
	return _data;
}

void SerialControlItem::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
	}
	else
	{
		ar >> version;  
	}

	SerialControlAction::Serialize(ar);
	_data.Serialize(ar);
}