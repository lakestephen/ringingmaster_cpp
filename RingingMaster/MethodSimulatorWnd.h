// MethodSimulatorWnd.h: interface for the MethodSimulatorWnd class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MethodWnd.h"
#include "SimulatorManagerEventListener.h"

class MethodSimulatorWnd : public MethodWnd, SimulatorManagerEventListener
{
public:
	MethodSimulatorWnd();
	virtual ~MethodSimulatorWnd();

	virtual void simulatorManager_notifyNewMethod(Method* method );
	virtual void simulatorManager_notifyMethodStructureChangedFromSeperateThread();
	virtual void simulatorManager_notifyLatestPositionFromSeperateThread(int rowIndex, int place);



protected:
	void doToolsStyle();


	afx_msg LRESULT OnSimulatorMethodStructureChanged(WPARAM wParam, LPARAM lParam);
	ShouldDrawType shouldDrawRow(int rowIndex, int place);
	UINT getContectMenuId();

	virtual CSize getLeadOrigin(int leadNo);
	virtual const CRect& getLeadBox(int leadNo);
	
	afx_msg void OnSimulatorOptions();

	int _latestPlace;
	int _latestRowIndex;

	DECLARE_MESSAGE_MAP()

};


