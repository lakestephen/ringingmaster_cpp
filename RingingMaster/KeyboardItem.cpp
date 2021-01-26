#include "StdAfx.h"
#include "KeyboardItem.h"

KeyboardItem::KeyboardItem(const KeyboardAction& keyboardAction ) :
KeyboardAction(keyboardAction),
_data(esigt_riseing)
{
}

KeyboardItem::KeyboardItem() :
_data(esigt_riseing)
{
}

KeyboardItem::~KeyboardItem(void)
{
}

CString KeyboardItem::getDescription() const
{
	CString delay;
	if (_data.getDelay() > 0)
		delay.Format(", %dms", _data.getDelay());

	return KeyboardAction::getDescription() +
		((_data.getEventSequenceType() == est_1of1)?", ":(", " + _data.getSequenceTypeDescription() + " ")) + 
		_data.getSignalTypeDescription() +  
		delay;
}

SimulatorEventData& KeyboardItem::getSimulatorEventData()
{
	return _data;
}

void KeyboardItem::Serialize(CArchive &ar)
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

	KeyboardAction::Serialize(ar);
	_data.Serialize(ar);
}