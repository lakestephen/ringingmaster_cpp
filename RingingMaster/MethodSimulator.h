#pragma once
#include "method.h"
#include "SimulatorCallType.h"
#include "Stroke.h"
#include "SimulatorActionMap.h"

class ProofInput;
class RowComment;
class Call;
class Splice;



enum MethodSimulatorCallTransitionState
{
	mscts_system, 
	mscts_user, 
};

enum MethodSimulatorTransitionState
{
	msts_none, 
	msts_madeCall, 
};


enum WaitCallbackAction
{
	wca_continue,   //0
	wca_startNewLead, //1
	wca_addStartRow, //2
};

class MethodSimulator :
	public Method
{
public:
	MethodSimulator();
	~MethodSimulator(void);

	bool isInThread();
	void addSimulatorComment(CString msg);
	WaitCallbackAction waitCallback();
	Row* getAnotherRow(Stroke stroke);      
	void requestEarlyTerminate();
	void requestTerminate(TerminateReason terminateReason);
	bool getRowCreated() { return _simulatorRowCreated;}
	void waitNextRow();
	void addComment(RowComment* rowComment);


	void startCreateMethodThread();
	void createMethodFremSeperateThread();
	virtual void create(ProofInput* proofInput);

	CArray<SimulatorCallType> _simulatorCalls;
	CArray<int> _simulatorSplices;//this is a list of the notation index numbers
	bool addPossibleCalls(CStringArray& callShorthands, const CString plainLead);
	SimulatorCallType getSimulatorCallTypeFromString(const CString& string);
	SimulatorCallType getSimulatorCurrentCallType();
	bool isPossibleCall(SimulatorCallType type);
	Call* getNextCall();
	Splice* getNextSplice();
	bool setCallType(SimulatorCallType type);
	bool setSpliceType(SimulatorActionType type);


	bool shouldForceSetupNotationsCallsAndPositions();
	
	int getCallsSize();
	int getSplicesSize();

	bool hasDonePrepWork();

	void forceRecalculationOfCurrentCallAndSplice();

	CArray<Splice*, Splice*> _simulatorSpliceArray;
	bool isSpliced();

protected:

	Splice* doGetNextSplice();

	CCriticalSection _simulatorCallsCriticalSection;

	bool extractAndAddCall();
	bool extractAndAddSplice(bool forceSpeak = false);

	void notifySplicePosPending();
	void notifyCallPosPending();



	Call* getCall(SimulatorCallType type);
	bool addCall(SimulatorCallType type, const CString shorthand);
	bool _simulatorDonePrepWork;
	bool _simulatorRowCreated;

	SimulatorCallType _simulatorCurrentCallType;
	int _simulatorCurrentSpliceType;
	MethodSimulatorCallTransitionState _simulatorCallTransitionState;
	MethodSimulatorCallTransitionState _simulatorSpliceTransitionState;

	WaitCallbackAction _simulatorWaitCallbackAction;
	MethodSimulatorTransitionState _simulatorTransitionState;

	CMap <SimulatorCallType, SimulatorCallType, Call*, Call*> _simulatorCallMap;

	static UINT createMethodThread( LPVOID pParam );

	bool _simulatorInThread;

	int _simulatorCurrentCallIndex;
	int _simulatorCurrentSpliceIndex;
	CString _simulatorLastSpliceShorthand;


};
