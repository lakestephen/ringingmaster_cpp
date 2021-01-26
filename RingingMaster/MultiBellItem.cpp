#include "StdAfx.h"
#include "MultiBellItem.h"

MultiBellItem::MultiBellItem(const MultiBellAction& multiBellAction ) :
MultiBellAction(multiBellAction),
_data(esigt_both)
{
}

MultiBellItem::MultiBellItem() :
_data(esigt_both)
{
}

MultiBellItem::~MultiBellItem(void)
{
}

CString MultiBellItem::getDescription() const
{
	CString delay;
	if (_data.getDelay() > 0)
		delay.Format(", %dms", _data.getDelay());

	return MultiBellAction::getDescription() + 
		((_data.getEventSequenceType() == est_1of1)?"":(", " + _data.getSequenceTypeDescription())) + 
		delay;
}

SimulatorEventData& MultiBellItem::getSimulatorEventData()
{
	return _data;
}

void MultiBellItem::Serialize(CArchive &ar)
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

	MultiBellAction::Serialize(ar);
	_data.Serialize(ar);
}