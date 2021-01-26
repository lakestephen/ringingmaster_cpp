#pragma once

#include "SpeechAction.h"
#include "SimulatorEventData.h"

class SpeechItem : public SpeechAction
{
public:
	SpeechItem();
	SpeechItem(const SpeechAction& speechAction );
	~SpeechItem(void);

	CString getDescription() const;

	virtual void Serialize(CArchive &ar);


	SimulatorEventData& getSimulatorEventData();
protected:
	SimulatorEventData _data;

};
