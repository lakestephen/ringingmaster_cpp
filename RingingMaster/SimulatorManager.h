// SimulatorManager.h: interface for the SimulatorManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "PlayManagerEventListener.h"
#include "KeyboardManagerEventListener.h"
#include "MultiBellInterfaceManagerEventListener.h"
#include "MainFrameEventListener.h"
#include "SimulatorManagerEventListener.h"
#include "SpeechManagerEventListener.h"
#include "SerialControlManagerEventListener.h"
#include "TowerManagerEventListener.h"
#include "MapTS.h"
#include "SimulatorActionMap.h"
#include "SimulatorGUIBell.h"
#include "StrikingData.h"
#include "Style.h"
#include "SimulatorStrikingWnd.h"
#include "SimulatorState.h"
#include "TowerTTSItem.h"
#include "SimulatorCallType.h"
#include "ManagerBase.h"


class MethodSimulator;
class MethodPosition;
class SimulatorAction;
class SimulatorState;
class TouchDoc;

enum SimulatorCallingType
{
	sct_touchCalls,
	sct_plainCourse,
	sct_random,
};


struct AutoStartPackage
{
	int _bell;
	int _interval;
	DWORD _timestamp;
};


typedef CMapTS<SimulatorInput, const SimulatorInput&, SimulatorActionTypes*, SimulatorActionTypes*> SimulatorActionMap;

class SimulatorManager: public 	CCmdTarget, 
						public  ManagerBase, 
								MainFrameEventListener, 
								PlayManagerEventListener,
								KeyboardManagerEventListener, 
								MultiBellInterfaceManagerEventListener, 
								SpeechManagerEventListener, 
								SerialControlManagerEventListener, 
								TowerManagerEventListener
{
public:

	int getFirstSimulatedBell();
	
	int getCoverBellCount();
	void setCoverBellCount(int count);

	const SimulatorActionMap& getSimulatorActionMap();

	void addComment(TowerTTSTypes type, CString token = CString(""));
	void addComment(CString msg, bool mute = false, DWORD timestamp = ULONG_MAX );

	void autoStart(int interval, int bell, DWORD timestamp);

	void documentDeleted(TouchDoc* pTouchDoc);
	void setMethodPositionSimulatorStatusBar(MethodPosition *methodPositionStatusBar);
	
	SimulatorStateType getSimulatorStateType();
	void setSimulatorStateType(SimulatorStateType type);

	int getSimulatorStrikingSngleBell(SimulatorStrikingWndType type);
	bool setSimulatorStrikingSngleBell(SimulatorStrikingWndType type, int val);

	const StrikingData& getStrikingData() const;
	void editStrikingControls(int bell); // this is the towers striking data.

	const SimulatorGUIBell& getGUIBell(int index);
	bool calculateBellBoxes(int cx, int cy, TouchDoc* doc);
	int hitTestBell(const CPoint& point);
	void towerManager_notifyUpdateTowerList();

	void addEventListener(SimulatorManagerEventListener* simulatorManagerEventListener) ;
	void removeEventListener(SimulatorManagerEventListener* simulatorManagerEventListener);

	void fireNotifyRawInputFromSeperateThread(const SimulatorInput& simulatorInput, DWORD timestamp );
	void fireNotifyLatestPositionFromSeperateThread(int rowIndex, int place);
	void fireNotifyBellStateChangeFromSeperateThread(int bell);
	void fireNotifySimulatorConnected();
	void fireNotifyNumberChanged(int number);
	void fireNotifyDocumentChanged();
	void fireNotifyMethodStructureChangedFromSeperateThread();
	void fireNotifyProfileChange();
		
	BOOL isDLLAvailable();
	bool isSimulatorAvailable();
	BOOL getUsingSimulator();
	void setUsingSimulator(BOOL usingSimulator);
	
	//key press listeners
	void keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp);
	void keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) ;

	//multiBellInterface Listeners.
	void multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp);
	void multiBellInterfaceManager_notifyMBIStatusEvent(bool portOK, bool MBIOK);

	//speech listeners
	void speechManager_notifySpeechRecognisedFromSeperateThread(const SpeechResponse& response, DWORD timestamp);
	void speechManager_notifyGetSpeechList(SpeechActionEventDataMap& speechActionEventDataMap);

	//serial control listeners
	void serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp);
	void serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& /*serialControlResponseSimulatorEventDataMap*/);

	//play manager listener
	void playManager_notifyBellPlayedFromSeperateThread(int place, int bell, Stroke stroke, DWORD perfectTimestamp, DWORD actualTimestamp, int rowIndex);
	void playManager_notifyStartPlayingFromSeperateThread(int numberIncludingCover, Stroke stroke, CString startString, DWORD startTimestamp);
	void playManager_notifyStopPlaying();
	void playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage);

	
	void setFirstSimulatedBell(int bell);

	SimulatorManager();
	virtual ~SimulatorManager();


	void save();
	void load();
	void closeDown();
	void openUp();

	//main frame listener methos
	void mainFrame_notifyUpdate(ViewType viewType);
	int getNumber();
	bool isValid(CString& msg, TouchDoc* doc );

	void drawBell(int index, CDC* pDC, TouchDoc* doc );

	bool isPlayingBell(int bell);
	bool isConnected();

	const Style& getDefaultBlueLineStyle();
	void setDefaultBlueLineStyle(const Style& style);

	SimulatorCallingType getSimulatorCallingType();

	DECLARE_MESSAGE_MAP()

	


protected:

	AutoStartPackage* _autoStartPackage;
	SimulatorCallingType _simulatorCallingType;

	void resetState();
	void reset();
	void setNumber();

	MethodSimulator* _method;
	void multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap);

	bool connectSimulator();
	bool disconnectSimulator();
	
	bool setupActionMap();
	void addActionToMap(const SimulatorAction& action, SimulatorActionType actionType, bool onlyUnique = false);
	void clearActionMap();
	
	void processActionFromSeperateThread(const SimulatorInput& simulatorInput, DWORD timestamp);


	BOOL	_usingSimulator;
	SimulatorActionMap _simulatorActionMap;

	SimulatorManagerEventListeners _listenerList;
	SimulatorState* _simulatorState;
	TouchDoc* _activeSimulatorTouchDoc;

	SimulatorGUIBell _bellGUI[MAXBELLS];
	


	BOOL _alreadyDeclinedStupNewProfile;

	StrikingData _strikingData;

	int _simulatorStrikingWndTypeBell1;
	int _simulatorStrikingWndTypeBell2;

	Style _style;

	int _number;
	BOOL _comeRoundNextTime;

	bool _portClashWarningDone;

	CStringArray _methodSuffix;



public:
	BOOL getComeRoundNextTime();
	BOOL anyInputsAvailable(void);
	afx_msg void OnSimulatorReset();
	afx_msg void OnCreateStrikeReport();
	afx_msg void OnUpdateCreateStrikeReport(CCmdUI *pCmdUI);
	afx_msg void OnPlayMethod();
	afx_msg void OnUpdatePlayMethod(CCmdUI *pCmdUI);
	afx_msg void OnPlayRounds();
	afx_msg void OnUpdatePlayRounds(CCmdUI *pCmdUI);
	afx_msg void OnPlayRoundsAuto();
	afx_msg void OnUpdatePlayRoundsAuto(CCmdUI *pCmdUI);
	afx_msg void OnComeRoundNextTime();
	afx_msg void OnUpdateComeRoundNextTime(CCmdUI *pCmdUI);

	afx_msg void OnStandUp();
	afx_msg void OnUpdateStandUp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSimulatorReset(CCmdUI *pCmdUI);
	afx_msg void OnStrikingTest();
	afx_msg void OnStrikingTestResults();
	afx_msg void OnAutoStart();
	afx_msg void OnStriking();
	afx_msg void OnCallsPlain();
	afx_msg void OnUpdateCallsPlain(CCmdUI *pCmdUI);
	afx_msg void OnCallsUseTouch();
	afx_msg void OnUpdateCallsUseTouch(CCmdUI *pCmdUI);
	afx_msg void OnCallsRandom();
	afx_msg void OnUpdateCallsRandom(CCmdUI *pCmdUI);

	afx_msg void OnCallPlain();
	afx_msg void OnUpdateCallPlain(CCmdUI *pCmdUI);
	afx_msg void OnCallBob();
	afx_msg void OnUpdateCallBob(CCmdUI *pCmdUI);
	afx_msg void OnCallSingle();
	afx_msg void OnUpdateCallSingle(CCmdUI *pCmdUI);
	afx_msg void OnCallX();
	afx_msg void OnUpdateCallX(CCmdUI *pCmdUI);
	afx_msg void OnCallY();
	afx_msg void OnUpdateCallY(CCmdUI *pCmdUI);
	afx_msg void OnCallZ();
	afx_msg void OnUpdateCallZ(CCmdUI *pCmdUI);

	void doCallEnable(CCmdUI *pCmdUI, SimulatorCallType type);
};

