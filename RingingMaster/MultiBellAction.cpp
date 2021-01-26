#include "StdAfx.h"
#include ".\multibellaction.h"
#include "MultiBellInterfaceManager.h"

MultiBellAction::MultiBellAction(int channel) :
_channel(channel)
{
}

MultiBellAction::MultiBellAction() :
_channel(MultiBellInterfaceManager::_NoChannel)
{
}

MultiBellAction::~MultiBellAction(void)
{
}

CString MultiBellAction::getDescription() const
{
	CString str;
	str.Format("%d", _channel + 1);

	return str;
}

int MultiBellAction::getChannel()   const
{
	return _channel;
}

void MultiBellAction::setChannel(int channel)
{
	_channel = channel;
}

bool MultiBellAction::operator ==(const MultiBellAction & multiBellAction) const
{
	return (_channel == multiBellAction._channel);
}

void MultiBellAction::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  

		ar << _channel;
	}
	else
	{
		ar >> version;  

		ar >> _channel;
	}
}

bool MultiBellAction::isValid() const
{
	return (_channel >= 0 && _channel < MultiBellInterfaceManager::_MaxChannel);
}