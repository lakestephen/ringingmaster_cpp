#pragma once

#include "SimulatorManagerEventListener.h"
#include "CJListCtrl.h"

class SimulatorInputSpyWnd : public CCJListCtrl  , SimulatorManagerEventListener
{
	DECLARE_DYNAMIC(SimulatorInputSpyWnd)

public:
	SimulatorInputSpyWnd();
	virtual ~SimulatorInputSpyWnd();

protected:
	DECLARE_MESSAGE_MAP()

	int _timestampOrigin;

public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID);

	virtual void simulatorManager_notifyRawInputFromSeperateThread(const SimulatorInput& simulatorInput, DWORD timestamp );
	void simulatorManager_notifySimulatorConnected(bool connected);

	afx_msg LRESULT OnSimulatorSpyItem(WPARAM wParam, LPARAM lParam);
	CImageList _imageList;	 

	
};


