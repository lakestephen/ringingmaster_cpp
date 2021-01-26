#pragma once

#include "SerialControlAction.h"
#include "SimulatorEventData.h"

class SerialControlItem : public SerialControlAction
{
public:
	SerialControlItem();
	SerialControlItem(const SerialControlAction& serialControlAction );
	~SerialControlItem(void);

	CString getDescription() const;

	virtual void Serialize(CArchive &ar);


	SimulatorEventData& getSimulatorEventData();
protected:
	SimulatorEventData _data;

};
