#pragma once

#include "MultiBellAction.h"
#include "SimulatorEventData.h"

class MultiBellItem : public MultiBellAction
{
public:
	MultiBellItem();
	MultiBellItem(const MultiBellAction& multiBellAction );
	~MultiBellItem(void);

	CString getDescription() const;

	virtual void Serialize(CArchive &ar);


	SimulatorEventData& getSimulatorEventData();
protected:
	SimulatorEventData _data;

};
