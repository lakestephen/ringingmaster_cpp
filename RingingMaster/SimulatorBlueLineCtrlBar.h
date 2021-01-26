#pragma once

#include "GenericCtrlBar.h"

class SimulatorBlueLineCtrlBar : public GenericCtrlBar
{
public:
	SimulatorBlueLineCtrlBar(void);
	~SimulatorBlueLineCtrlBar(void);

	afx_msg LRESULT OnSetMethodPos(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
