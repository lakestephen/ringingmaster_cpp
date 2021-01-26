#pragma once

#include "KeyboardAction.h"
#include "SimulatorEventData.h"

class KeyboardItem : public KeyboardAction
{
public:
	KeyboardItem();
	KeyboardItem(const KeyboardAction& keyboardAction );
	~KeyboardItem(void);

	CString getDescription() const;

	virtual void Serialize(CArchive &ar);


	SimulatorEventData& getSimulatorEventData();
protected:
	SimulatorEventData _data;

};
