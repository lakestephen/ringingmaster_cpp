// SimulatorManagerEventListener.h: interface for the SimulatorManagerEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class Method;
class SimulatorInput;
enum SimulatorActionType;

class SimulatorManagerEventListener  
{
public:
	SimulatorManagerEventListener() {};
	virtual ~SimulatorManagerEventListener(){};

	virtual void simulatorManager_notifyRawInputFromSeperateThread(const SimulatorInput& /*simulatorInput*/, DWORD /*timestamp*/ ) {};
	virtual void simulatorManager_notifyBellStateChangeFromSeperateThread(int /*bell*/){};
	virtual void simulatorManager_notifyLatestPositionFromSeperateThread(int /*rowIndex*/, int /*place*/){};
	virtual void simulatorManager_notifySimulatorConnected(bool /*connected*/){};
	virtual void simulatorManager_notifyProfileChange(){};
	virtual void simulatorManager_notifyDocumentChanged(){};
	virtual void simulatorManager_notifyNewMethod(Method* /*method*/ ){};
	virtual void simulatorManager_notifyMethodStructureChangedFromSeperateThread(){};
	virtual void simulatorManager_notifyNumberChanged(int /*number*/){};

};

typedef CArray<SimulatorManagerEventListener*, SimulatorManagerEventListener*> SimulatorManagerEventListeners;


