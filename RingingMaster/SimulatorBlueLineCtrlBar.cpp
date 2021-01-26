#include "StdAfx.h"
#include ".\simulatorbluelinectrlbar.h"
#include "UserMessages.h"
#include "MethodPosition.h"
#include "SimulatorManager.h"

SimulatorBlueLineCtrlBar::SimulatorBlueLineCtrlBar(void)
{
}

SimulatorBlueLineCtrlBar::~SimulatorBlueLineCtrlBar(void)
{
}

BEGIN_MESSAGE_MAP(SimulatorBlueLineCtrlBar, GenericCtrlBar)
	ON_MESSAGE(WM_SET_METHOD_POS,OnSetMethodPos)	
END_MESSAGE_MAP()


LRESULT SimulatorBlueLineCtrlBar::OnSetMethodPos(WPARAM wParam, LPARAM lParam)  
{
	MethodPosition * pos = (MethodPosition*)lParam;

	if (wParam == TRUE)
		simulatorManager()->setMethodPositionSimulatorStatusBar(pos);
	else
		delete pos;
	
	return 0;
}
