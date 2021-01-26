#pragma once

#include "SimulatorManagerEventListener.h"
#include "CJListCtrl.h"

class SimulatorInputsMapWnd : public CCJListCtrl  , SimulatorManagerEventListener
{
	DECLARE_DYNAMIC(SimulatorInputsMapWnd)

public:
	SimulatorInputsMapWnd();
	virtual ~SimulatorInputsMapWnd();

protected:
	DECLARE_MESSAGE_MAP()

	int _timestampOrigin;

public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID);

	void simulatorManager_notifySimulatorConnected(bool connected);
	void simulatorManager_notifyProfileChange();
	void update();
	CImageList _imageList;	 

	
};


