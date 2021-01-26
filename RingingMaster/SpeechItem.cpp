#include "StdAfx.h"
#include "SpeechItem.h"

SpeechItem::SpeechItem(const SpeechAction& speechAction ) :
SpeechAction(speechAction),
_data(esigt_both)
{
}

SpeechItem::SpeechItem() :
_data(esigt_both)
{
}

SpeechItem::~SpeechItem(void)
{
}

CString SpeechItem::getDescription() const
{
	CString delay;
	if (_data.getDelay() > 0)
		delay.Format(", %dms", _data.getDelay());

	return SpeechAction::getDescription() + 
		((_data.getEventSequenceType() == est_1of1)?"":(", " + _data.getSequenceTypeDescription())) + 
		delay;
}

SimulatorEventData& SpeechItem::getSimulatorEventData()
{
	return _data;
}

void SpeechItem::Serialize(CArchive &ar)
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

	SpeechAction::Serialize(ar);
	_data.Serialize(ar);
}