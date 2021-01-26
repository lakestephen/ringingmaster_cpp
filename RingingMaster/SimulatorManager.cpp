// SimulatorManager.cpp: implementation of the SimulatorManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorManager.h"
#include "KeyboardManager.h"
#include "MultiBellInterfaceManager.h"
#include "SerialControlManager.h"
#include "PlayManager.h"
#include "PlayRequestSimulator.h"
#include "SpeechManager.h"
#include "SerialControlResponse.h"
#include "SerialControlItem.h"
#include "KeyboardItem.h"
#include "KeyboardResponse.h"
#include "SpeechResponse.h"
#include "StrikingItem.h"
#include "TowerManager.h"
#include "Tower.h"
#include "TouchDoc.h"
#include "MethodSimulator.h"
#include "MethodPosition.h"
#include "GenericNumberSpinDlg.h"
#include "SimulatorState.h"
#include "StrikeDoc.h"
#include "StrikingTestDlg.h"
#include "GenericPropDlg.h"
#include "ProofManager.h"


#include "RegArchive.h"
#include ".\simulatormanager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SimulatorManager::SimulatorManager() : 
_usingSimulator(TRUE),
_activeSimulatorTouchDoc(NULL), 
_alreadyDeclinedStupNewProfile(false),
_number(-1), 
_method(0), 
_simulatorStrikingWndTypeBell1(0),
_simulatorStrikingWndTypeBell2(1), 
_comeRoundNextTime(TRUE), 
_simulatorState(NULL), 
_autoStartPackage(NULL), 
_simulatorCallingType(sct_touchCalls), 
_portClashWarningDone(false)
{
	_simulatorState = new SimulatorState;
	_style.setDefault(ds_simulator);	

	_methodSuffix.Add("Little Bob");
	_methodSuffix.Add("Little Place");
	_methodSuffix.Add("Treble Bob");
	_methodSuffix.Add("Treble Place");
	_methodSuffix.Add("Slow Course");

	_methodSuffix.Add("Bob");
	_methodSuffix.Add("Surprise");
	_methodSuffix.Add("Alliance");
	_methodSuffix.Add("Place");
	_methodSuffix.Add("Treble");
	_methodSuffix.Add("Delight");
}
						  
SimulatorManager::~SimulatorManager()
{
	reset();
	
	clearActionMap();


	delete _simulatorState;
	_simulatorState = NULL;
	
	ASSERT(_autoStartPackage == NULL);
	delete _autoStartPackage;
	_autoStartPackage = NULL;
}

void SimulatorManager::openUp()
{
	mainFrame()->addEventListener(this);
	towerManager()->addEventListener(this);	
}

void SimulatorManager::closeDown()
{
	mainFrame()->removeEventListener(this);
	towerManager()->removeEventListener(this);	

	VERIFY(disconnectSimulator());    
}

BEGIN_MESSAGE_MAP(SimulatorManager, CCmdTarget)
	ON_COMMAND(ID_SIMULATOR_RESET, OnSimulatorReset)
	ON_COMMAND(ID_CREATE_STRIKE_REPORT, OnCreateStrikeReport)
	ON_UPDATE_COMMAND_UI(ID_CREATE_STRIKE_REPORT, OnUpdateCreateStrikeReport)
	ON_COMMAND(ID_PLAY_METHOD, OnPlayMethod)
	ON_COMMAND(ID_PLAY_ROUNDS, OnPlayRounds)
	ON_UPDATE_COMMAND_UI(ID_PLAY_ROUNDS, OnUpdatePlayRounds)
	ON_COMMAND(ID_PLAY_ROUNDS_AUTO, OnPlayRoundsAuto)
	ON_UPDATE_COMMAND_UI(ID_PLAY_ROUNDS_AUTO, OnUpdatePlayRoundsAuto)       
	ON_COMMAND(ID_COME_ROUND_NEXT_TIME, OnComeRoundNextTime)
	ON_UPDATE_COMMAND_UI(ID_COME_ROUND_NEXT_TIME, OnUpdateComeRoundNextTime)
	ON_UPDATE_COMMAND_UI(ID_PLAY_METHOD, OnUpdatePlayMethod)
	ON_COMMAND(ID_STAND_UP, OnStandUp)
	ON_UPDATE_COMMAND_UI(ID_STAND_UP, OnUpdateStandUp)
	ON_UPDATE_COMMAND_UI(ID_SIMULATOR_RESET, OnUpdateSimulatorReset)
	ON_COMMAND(ID_STRIKING_TEST, OnStrikingTest)
	ON_COMMAND(ID_STRIKING_TEST_RESULTS, OnStrikingTestResults)
	ON_COMMAND(SIMULATOR_AUTOSTART, OnAutoStart)
	ON_COMMAND(ID_CALLS_PLAIN, OnCallsPlain)
	ON_UPDATE_COMMAND_UI(ID_CALLS_PLAIN, OnUpdateCallsPlain)
	ON_COMMAND(ID_CALLS_USE_TOUCH, OnCallsUseTouch)
	ON_UPDATE_COMMAND_UI(ID_CALLS_USE_TOUCH, OnUpdateCallsUseTouch)
	ON_COMMAND(ID_CALLS_RANDOM, OnCallsRandom)
	ON_UPDATE_COMMAND_UI(ID_CALLS_RANDOM, OnUpdateCallsRandom)
	
	ON_COMMAND(ID_CALLPLAIN, OnCallPlain)	
	ON_UPDATE_COMMAND_UI(ID_CALLPLAIN, OnUpdateCallPlain)
	ON_COMMAND(ID_CALLBOB, OnCallBob)
	ON_UPDATE_COMMAND_UI(ID_CALLBOB, OnUpdateCallBob)
	ON_COMMAND(ID_CALLSINGLE, OnCallSingle)
	ON_UPDATE_COMMAND_UI(ID_CALLSINGLE, OnUpdateCallSingle)
	ON_COMMAND(ID_CALLX, OnCallX)
	ON_UPDATE_COMMAND_UI(ID_CALLX, OnUpdateCallX)
	ON_COMMAND(ID_CALLY, OnCallY)
	ON_UPDATE_COMMAND_UI(ID_CALLY, OnUpdateCallY)
	ON_COMMAND(ID_CALLZ, OnCallZ)
	ON_UPDATE_COMMAND_UI(ID_CALLZ, OnUpdateCallZ)
END_MESSAGE_MAP()


void SimulatorManager::documentDeleted(TouchDoc* pTouchDoc)
{
	if (pTouchDoc == _activeSimulatorTouchDoc)
	{
		_activeSimulatorTouchDoc = NULL;
		fireNotifyDocumentChanged();
	}			
}

void SimulatorManager::mainFrame_notifyUpdate(ViewType viewType)
{
	if (viewType == vt_touch_simulator)
	{
		TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
		ASSERT(pTouchDoc);

		if (pTouchDoc == _activeSimulatorTouchDoc)
		{
			if (!isConnected())
				connectSimulator();
		}
		else
		{		
			_activeSimulatorTouchDoc = pTouchDoc;

			fireNotifyDocumentChanged();
		}
	}
	else
	{
		if (isConnected())
		{
			playManager()->OnPlayStop();
			disconnectSimulator();
		}
	}
}

void SimulatorManager::setNumber()
{
	if(_activeSimulatorTouchDoc)
	{
		int number = _activeSimulatorTouchDoc->getNumber();
		Tower* tower = towerManager()->getTower();
		if (tower)
			_number = tower->getNumberInclCover(number);
		else
			_number = number;

		fireNotifyNumberChanged(_number);
	}
}

void SimulatorManager::addEventListener(SimulatorManagerEventListener* simulatorManagerEventListener)
{
	ASSERT(simulatorManagerEventListener != NULL);

	for (int i=0;i<_listenerList.GetSize();i++)
		if (_listenerList.GetAt(i) == simulatorManagerEventListener)
			return;

	_listenerList.Add(simulatorManagerEventListener);

	TRACE("[SimulatorManager::addEventListener]Added \r\n" ); 

}

void SimulatorManager::removeEventListener(SimulatorManagerEventListener* simulatorManagerEventListener)
{
	ASSERT(simulatorManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == simulatorManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	ASSERT(hasRemoved == 1);
}

	   
void SimulatorManager::fireNotifyRawInputFromSeperateThread(const SimulatorInput& simulatorInput, DWORD timestamp )
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->simulatorManager_notifyRawInputFromSeperateThread(simulatorInput, timestamp); 
	}
}

void SimulatorManager::fireNotifyBellStateChangeFromSeperateThread(int bell)
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->simulatorManager_notifyBellStateChangeFromSeperateThread(bell); 
	}
}

void SimulatorManager::fireNotifyLatestPositionFromSeperateThread(int rowIndex, int place)
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->simulatorManager_notifyLatestPositionFromSeperateThread(rowIndex, place); 
	}
}

void SimulatorManager::towerManager_notifyUpdateTowerList()
{
	if (!setupActionMap())
		disconnectSimulator();

	setNumber();

	keyboardManager()->updateKeyMap();
	speechManager()->updateWordList();
	multiBellInterfaceManager()->updateMBIList();
	serialControlManager()->updateSerialControlList();

	fireNotifyProfileChange();
}

void SimulatorManager::fireNotifyProfileChange()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->simulatorManager_notifyProfileChange(); 
	}
}

void SimulatorManager::fireNotifyNumberChanged(int number)
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->simulatorManager_notifyNumberChanged(number); 
	}
}

void SimulatorManager::fireNotifySimulatorConnected()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->simulatorManager_notifySimulatorConnected(isConnected()); 
	}
}

void SimulatorManager::fireNotifyDocumentChanged()
{
	reset();

	setNumber();			

	if (_activeSimulatorTouchDoc)
		connectSimulator();
	else
		disconnectSimulator();

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->simulatorManager_notifyDocumentChanged(); 
	}
}

void SimulatorManager::fireNotifyMethodStructureChangedFromSeperateThread()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->simulatorManager_notifyMethodStructureChangedFromSeperateThread(); 
	}
}

void SimulatorManager::load()
{

	_usingSimulator = AfxGetApp()->GetProfileInt("Simulator", "Using Simulator", TRUE);	

	_simulatorStrikingWndTypeBell1 = AfxGetApp()->GetProfileInt("Simulator", "Single Bell 1", 0);	
	_simulatorStrikingWndTypeBell2 = AfxGetApp()->GetProfileInt("Simulator", "Single Bell 2", 1);	
	_comeRoundNextTime = AfxGetApp()->GetProfileInt("Simulator", "Come Round Next Time", TRUE);	
	_simulatorCallingType = (SimulatorCallingType)AfxGetApp()->GetProfileInt("Simulator", "Simulator Calling Type", sct_touchCalls);	

	RegArchive ar (CArchive::load, "Simulator", "Blue Line Style");
	
	if (ar.isValid()) 
	{
		_style.Serialize(ar);
	}	

}


void SimulatorManager::save()
{
	AfxGetApp()->WriteProfileInt("Simulator", "Using Simulator", _usingSimulator);	

	AfxGetApp()->WriteProfileInt("Simulator", "Single Bell 1", _simulatorStrikingWndTypeBell1);	
	AfxGetApp()->WriteProfileInt("Simulator", "Single Bell 2", _simulatorStrikingWndTypeBell2);	
	AfxGetApp()->WriteProfileInt("Simulator", "Come Round Next Time", _comeRoundNextTime);	
	AfxGetApp()->WriteProfileInt("Simulator", "Simulator Calling Type", _simulatorCallingType);	
	

	RegArchive ar (CArchive::store, "Simulator", "Blue Line Style");

	if (ar.isValid())
	{
		_style.Serialize(ar);
	}



}

void SimulatorManager::keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp)
{
	static SimulatorInput keyInput;
	keyInput.setKeyboard(new KeyboardResponse(response));
	processActionFromSeperateThread(keyInput, timestamp);
}

void SimulatorManager::keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) 
{
	SimulatorInput input;
	SimulatorActionTypes* types;

	CSingleLock lock(&_simulatorActionMap._criticalSection, TRUE);	

	POSITION pos = _simulatorActionMap.GetStartPosition();   
	while (pos)
	{
		_simulatorActionMap.GetNextAssoc(pos, input, types);
		if (input.isKeyboard())
		{
			KeyboardAction* action = input.getKeyboard();
			ASSERT(action);
			KeyboardItem* item = dynamic_cast<KeyboardItem*>(action);
			ASSERT(item);
			if (item)
			{
				keyboardActionSimulatorEventDataMap.SetAt(*action,new SimulatorEventData(item->getSimulatorEventData()));
			}
		}
	}
}

void SimulatorManager::multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp)
{
	static SimulatorInput mbiInput;
	mbiInput.setMultiBellInterface(new MultiBellResponse(multiBellResponse));
	processActionFromSeperateThread(mbiInput, timestamp);
}

void SimulatorManager::multiBellInterfaceManager_notifyMBIStatusEvent(bool /*portOK*/, bool /*MBIOK*/)
{
	//to2do
}

void SimulatorManager::multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap)
{
	SimulatorInput input;
	SimulatorActionTypes* types;
	
	CSingleLock lock(&_simulatorActionMap._criticalSection, TRUE);	

	POSITION pos = _simulatorActionMap.GetStartPosition();   
	while (pos)
	{
		_simulatorActionMap.GetNextAssoc(pos, input, types);
		if (input.isMultiBellInterface())
		{
			MultiBellAction* action = input.getMultiBellInterface();
			ASSERT(action);
			MultiBellItem* item = dynamic_cast<MultiBellItem*>(action);
			ASSERT(item);
			if (item)
			{
				multiBellActionSimulatorEventDataMap.SetAt(*action,new SimulatorEventData(item->getSimulatorEventData()));
			}
		}
	}
}

void SimulatorManager::speechManager_notifySpeechRecognisedFromSeperateThread(const SpeechResponse& response, DWORD timestamp)
{
	static SimulatorInput wordInput;
	wordInput.setSpeech(new SpeechResponse(response));
	processActionFromSeperateThread(wordInput,timestamp); 
}

void SimulatorManager::speechManager_notifyGetSpeechList(SpeechActionEventDataMap& speechActionEventDataMap)
{
	//iterate through the map and get all the speech words 
	SimulatorInput input;
	SimulatorActionTypes* types;
	
	CSingleLock lock(&_simulatorActionMap._criticalSection, TRUE);	

	POSITION pos = _simulatorActionMap.GetStartPosition();		  
	while (pos)
	{
		_simulatorActionMap.GetNextAssoc(pos, input, types);
		if (input.isSpeech())
		{
			SpeechAction* action = input.getSpeech();
			ASSERT(action);
			SpeechItem* item = dynamic_cast<SpeechItem*>(action);
			ASSERT(item);
			if (item)
			{
				speechActionEventDataMap.SetAt(*action,new SimulatorEventData(item->getSimulatorEventData()));
			}
		}
	}
}

void SimulatorManager::serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp)
{
	static SimulatorInput serialInput;
	serialInput.setSerialControl(new SerialControlResponse(response));
	processActionFromSeperateThread(serialInput, timestamp);
}

void SimulatorManager::serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& serialControlResponseSimulatorEventDataMap)
{
	SimulatorInput input;
	SimulatorActionTypes* types;
	
	CSingleLock lock(&_simulatorActionMap._criticalSection, TRUE);		

	POSITION pos = _simulatorActionMap.GetStartPosition();   
	while (pos)
	{
		_simulatorActionMap.GetNextAssoc(pos, input, types);
		if (input.isSerialControl())
		{
			SerialControlAction* action = input.getSerialControl();
			ASSERT(action);
			SerialControlItem* item = dynamic_cast<SerialControlItem*>(action);
			ASSERT(item);
			if (item)
			{
				serialControlResponseSimulatorEventDataMap.SetAt(*action,new SimulatorEventData(item->getSimulatorEventData()));
			}
		}
	}

}

void SimulatorManager::setUsingSimulator(BOOL usingSimulator)
{
	_usingSimulator = usingSimulator;
}

BOOL SimulatorManager::getUsingSimulator()
{	
	return _usingSimulator;
}

bool SimulatorManager::isSimulatorAvailable()
{
	return (isDLLAvailable() && getUsingSimulator());
}

BOOL SimulatorManager::isDLLAvailable()
{
	//in this case no DLL to be available
	return TRUE;
}



BOOL SimulatorManager::anyInputsAvailable(void)
{
	return	((keyboardManager()->isKeyboardAvailable())||
			 (multiBellInterfaceManager()->isMBIAvailable())||
			 (speechManager()->isSpeechRecognitionAvailable())||
			 (serialControlManager()->isSerialControlAvailable())); 
}

bool SimulatorManager::connectSimulator()
{
	CWaitCursor wait;

	if (isConnected())
		VERIFY(disconnectSimulator());

	//do we have a profile?
	Tower* tower = towerManager()->getTower();

	if (!tower)
	{
		towerManager()->editTower();
		tower = towerManager()->getTower();
	}
	
	if ((tower)&&(!isConnected()))
	{
		if (setupActionMap())
		{
			//connect to the managers
			setSimulatorStateType(sst_connectedSilent);
			playManager()->addEventListener(this);
			
			if (keyboardManager()->isKeyboardAvailable())
				keyboardManager()->addEventListener(this);

			if (speechManager()->isSpeechRecognitionAvailable())
				speechManager()->addEventListener(this);

			multiBellInterfaceManager()->addEventListener(this);//we have to add to get a connect attempt
			Sleep(50);	 //allow the second stage of the connection to take place
			if (!multiBellInterfaceManager()->isMBIConnected())
				multiBellInterfaceManager()->removeEventListener(this);				

			serialControlManager()->addEventListener(this);//we have to add to get a connect attempt
			if (!serialControlManager()->isComPortConnected())
				serialControlManager()->removeEventListener(this);

			//stop the play thread
			playManager()->OnPlayStop();
		}
	}

	//update the listeners
	fireNotifySimulatorConnected();

	return isConnected();
}

bool SimulatorManager::isConnected()
{
	return getSimulatorStateType() != sst_disconnected;
}

void SimulatorManager::clearActionMap()
{
	CSingleLock lock(&_simulatorActionMap._criticalSection, TRUE);
	
	POSITION pos = _simulatorActionMap.GetStartPosition();
	SimulatorInput key;
	SimulatorActionTypes* value = NULL;

	while (pos)
	{
		_simulatorActionMap.GetNextAssoc(pos, key, value);
		delete value;
	}

	_simulatorActionMap.RemoveAll();
}

bool SimulatorManager::setupActionMap()
{
/* todo	Tower* tower = towerManager()->getTower();
	if (tower)
	{      
		CSingleLock lock(&_simulatorActionMap._criticalSection, TRUE);
		//build action maps.
		clearActionMap();

		if (serialControlManager()->isPortClash(true))
		{
			if (!_portClashWarningDone)
			{
				AfxMessageBox("Serial Control and Multi Bell Interface are using the same port. Please change ports so that they do not clash. \r\n(NOTE: If you are not using either the Multi Bell Interface or the Serial Ports, you can disable them.)");
				mainFrame()->doSimulatorOptions(false, mainFrame());
				_portClashWarningDone = true;
			}
		}

		if (!serialControlManager()->isPortClash(true))
		{
										
			int number = getNumber();
			ASSERT(number == -1 ||(number >=0 && number < MAXBELLS));
			
			if (number >=0 && number < MAXBELLS)
			{
				//add the bell actions
				int firstSimulatedBell = tower->getFirstSimulatedBell();
				if (firstSimulatedBell <0 || firstSimulatedBell >= number)
				{
					AfxMessageBox("First simulated bell is out of range. \r\nSetting back to 1.");
					tower->setFirstSimulatedBell(0);
					firstSimulatedBell = tower->getFirstSimulatedBell();
				}

				ASSERT(firstSimulatedBell >=0 && firstSimulatedBell < number);
				firstSimulatedBell = max(0,min(number,firstSimulatedBell));

				//iterate through the actions
				for (int i=0;i<number;i++)
				{
					const SimulatorAction& action = tower->getAction((SimulatorActionType)i);
					
					//set the map
					int bell = i + firstSimulatedBell;
					if (bell > number)
						bell -= number;
					SimulatorActionType actionType = (SimulatorActionType)bell;

					addActionToMap(action, actionType);
				}   

				//add the other actions
				for (int i=SIMULATOR_ACTION_FIRST_ACTION;i<=SIMULATOR_ACTION_LAST_METHOD;i++)
				{
					SimulatorActionType actionType = (SimulatorActionType)i;
					const SimulatorAction& action = tower->getAction(actionType);

					addActionToMap(action, actionType);		
				}

				//now attempt to add in the keyboard shortcuts for the method.
				if (_method && _method->isSpliced())
				{
					const Notations& notations = _method->getNotations();
					for (int i=0;i<notations.GetSize();i++)
					{
						Notation* notation = notations.GetAt(i);

						if (tower->getAddMethodShortcutsAsKeyboardInputs())
						{
							CString spliceLetter = notation->getSpliceLetter();
							if (spliceLetter.GetLength() == 1)
							{
								SimulatorAction action;
								SimulatorInput* input = new SimulatorInput;
								input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan(spliceLetter.GetAt(0)))));
								action.addInput(input);
								addActionToMap(action, (SimulatorActionType)(i+SIMULATOR_ACTION_FIRST_METHOD), true);
							}
						}
						if (tower->getAddMethodNamesAsMicrophoneInputs())
						{
							CString methodName = notation->getName();
							if (!methodName.IsEmpty())
							{
								SimulatorAction action;
								SimulatorInput* input = new SimulatorInput;
								input->setSpeech(new SpeechItem(SpeechAction(methodName)));
								if (input->isValid())
								{
									action.addInput(input);
									addActionToMap(action, (SimulatorActionType)(i+SIMULATOR_ACTION_FIRST_METHOD), true);
								}
							}

							bool changed = false;
							for (int j=0;j<_methodSuffix.GetSize();j++)
							{
								if (methodName.Replace(_methodSuffix.GetAt(j), CString("")) > 0)
									changed = true;
							}
							
							if (changed && !methodName.IsEmpty())
							{
								SimulatorAction action;
								SimulatorInput* input = new SimulatorInput;
								input->setSpeech(new SpeechItem(SpeechAction(methodName)));
								if (input->isValid())
								{
									action.addInput(input);
									addActionToMap(action, (SimulatorActionType)(i+SIMULATOR_ACTION_FIRST_METHOD), true);
								}
    						}
						}
					}
				}

				fireNotifyProfileChange();

				return true;
			}
		}               		
	}
	  */
	return false;
}

void SimulatorManager::addActionToMap(const SimulatorAction& action, SimulatorActionType actionType, bool onlyUnique)
{
	for (int j=0;j<action.getInputCount();j++)
	{

		SimulatorInput* input = action.getInput(j);
		ASSERT(input);
		if (input)
		{
			//check to see if there is one there already.
			SimulatorActionTypes* simulatorActionTypes = NULL;
			if (!_simulatorActionMap.Lookup(*input, simulatorActionTypes))
			{
				//not one there - create one.
				simulatorActionTypes = new SimulatorActionTypes;
				_simulatorActionMap.SetAt(*input, simulatorActionTypes);
				simulatorActionTypes->Add(actionType);
			}
			else if (!onlyUnique)
			{
				simulatorActionTypes->Add(actionType);
			}
		}
	}					
}

bool SimulatorManager::disconnectSimulator()
{
	if (isConnected())
	{
		clearActionMap();

		//disconnect the managers
		setSimulatorStateType(sst_disconnected);
		playManager()->removeEventListener(this);
		keyboardManager()->removeEventListener(this);
		multiBellInterfaceManager()->removeEventListener(this);
		speechManager()->removeEventListener(this);
		serialControlManager()->removeEventListener(this);

		//stop the play thread
		playManager()->OnPlayStop();
	}                         

	fireNotifySimulatorConnected();

	return !isConnected();
}

void SimulatorManager::playManager_notifyStopPlaying()
{
	if (isConnected())
		resetState();
}            

void SimulatorManager::OnPlayRoundsAuto()
{
	switch (getSimulatorStateType())
	{
	case sst_connectedSilent:
		playManager()->doTowerChecksLite(getNumber());
		setSimulatorStateType( sst_connectedAutoStart);
		reset();
		_simulatorState->resetAutoStart();
		break;
	case sst_connectedAutoStart:
		setSimulatorStateType( sst_connectedSilent);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void SimulatorManager::OnUpdatePlayRoundsAuto(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(getSimulatorStateType() == sst_connectedSilent ||
					getSimulatorStateType() == sst_connectedAutoStart);
	pCmdUI->SetCheck(getSimulatorStateType() == sst_connectedAutoStart);
}


void SimulatorManager::OnComeRoundNextTime()
{
	_comeRoundNextTime = !_comeRoundNextTime;
}

void SimulatorManager::OnUpdateComeRoundNextTime(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_comeRoundNextTime);
	pCmdUI->Enable();
}

BOOL SimulatorManager::getComeRoundNextTime()
{
	return _comeRoundNextTime;
}

void SimulatorManager::autoStart(int interval, int bell, DWORD timestamp)
{
	if (_autoStartPackage == NULL)
	{
		_autoStartPackage = new AutoStartPackage;
		_autoStartPackage->_interval = interval; 
		_autoStartPackage->_bell = bell; 
		_autoStartPackage->_timestamp = timestamp; 
		//work out how long it is untill we need to play the first bell  
		::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, SIMULATOR_AUTOSTART,0);		
	}
	else
	{
		ASSERT(FALSE);
	}
}

void SimulatorManager::OnAutoStart()
{
	if (_autoStartPackage)
	{
		Tower* tower = towerManager()->getTower();
		if (tower)
		{
			tower->setSpeed(_autoStartPackage->_interval);
			towerManager()->fireUpdateTowerList();
			OnPlayRounds();
		}

		delete _autoStartPackage;
		_autoStartPackage = NULL;
	}
	else
	{
		ASSERT(FALSE);
	}
}

void SimulatorManager::OnPlayRounds()
{/* todo
	CWaitCursor (wait);
	ASSERT(isConnected());
	if (isConnected())
	{					  
		SimulatorStateType type = getSimulatorStateType();
		switch (type)
		{
		case sst_connectedRounds:
			OnStandUp();
			break;
		case sst_connectedMethod:
			setSimulatorStateType(sst_connectedRoundsPendingFromMethod);
			break;
		case sst_connectedSilent:
		case sst_connectedAutoStart:
			{
				//we are stopped and we need to start

				TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
				Tower* tower = towerManager()->getTower();
				if (pTouchDoc && tower)
				{
					if (type == sst_connectedSilent)
						reset();

					setSimulatorStateType(sst_connectedRoundsPendingFromSilent);

	#ifdef _DEBUG
					int oldNumber = _number; 
					setNumber();//should not be necessary
					ASSERT(_number == oldNumber);
	#endif              

					ProofInput* proofInput = pTouchDoc->setupProofInput();
					_method = new MethodSimulator(proofInput);
					
					//setup and prove a dummy method to get the calls and splices after the proof.
					ProofInput* dummyProofInput = pTouchDoc->setupProofInput();
					proofInput.setPerformAnalysis(true); //todo do we want to do analysis here?
					Method* dummyMethod = proofManager()->requestProof(dummyProofInput);

					//find all the possible calls  in the active notation
					CStringArray possibleCalls;
					if (!pTouchDoc->isSpliced())
					{
						Notation* notation = pTouchDoc->getActiveNotation();
						ASSERT(notation);
						if (notation)
						{
							for (int i=0;i<notation->getCallCount();i++)
							{
								NotationCall* notationCall = notation->getCall(i);
								possibleCalls.Add(notationCall->_nameShorthand);
							}
						}
					}
					else
					{
						//we only want calls defined in every notation.
						const Notations& notations = pTouchDoc->getNotations();

						if (notations.GetSize() > 0)
						{
							Notation* notation = notations.GetAt(0);
							ASSERT(notation);
							if (notation)
							{
								for (int i=0;i<notation->getCallCount();i++)
								{
									NotationCall* notationCall = notation->getCall(i);
									possibleCalls.Add(notationCall->_nameShorthand);
								}
							}
						}



						for (int i=0;i<notations.GetSize();i++)
						{
							Notation* notation = notations.GetAt(i);

							for (int j=0;j<possibleCalls.GetSize();j++)
							{
								const CString& call = possibleCalls.GetAt(j);
								bool foundCall = false;

								for (int k=0;k<notation->getCallCount();k++)
								{
									NotationCall* notationCall = notation->getCall(k);
									if (call == notationCall->_nameShorthand)
										foundCall = true;
								}

								if (!foundCall)
									possibleCalls.RemoveAt(j--);
							}
						}
					}
					_method->addPossibleCalls(possibleCalls, pTouchDoc->getPlainLeadToken());

					//find all the possible splices 
					if(_method->Method::isSpliced())
					{
						const Notations& notations = _method->getNotations();
						static Variance blankVariance;	

						for (int i=0;i<notations.GetSize();i++)
						{
							//add in notation index order
							Notation* notation = notations.GetAt(i);
							Splice* splice = new Splice(blankVariance, 0, notation->getSpliceLetter(), notation->getName());
							_method->_simulatorSpliceArray.Add(splice);
						}
					} 

					//make an array of the actual calls.
					for (int i=0;i<dummyMethod->getCallsMade().GetSize();i++)
					{                             
						CStringArray* callMade = dummyMethod->getCallsMade().GetAt(i);
						if (callMade != NULL)
						{
							//calls
    						for (int j=0;j<callMade->GetSize();j++) 
							{
								SimulatorCallType type = _method->getSimulatorCallTypeFromString(callMade->GetAt(j));
								_method->_simulatorCalls.Add(type);
							}
						}
					}

					//make an array of actual splices
					if (dummyMethod->isSpliced())
					{
						const Notations& notations = _method->getNotations();
						for (int i=0;i<dummyMethod->getSplicesMade().GetSize();i++)
						{                             
							CStringArray* splicesMade = dummyMethod->getSplicesMade().GetAt(i);
							if (splicesMade != NULL)
							{
								//splices
    							for (int j=0;j<splicesMade->GetSize();j++) 
								{
									for (int k=0;k<notations.GetSize();k++)
									{                                      
										if (notations.GetAt(k)->isSpliceLetter(splicesMade->GetAt(j)))
											_method->_simulatorSplices.Add(k);
									}
								}
							}
						}
					}

					//remove the calls just added  (has to be after all checks for is splices
					for (int i=0;i<_method->_proofInput->getCalls().GetSize();i++)
					{
						delete _method->_proofInput->getCalls().GetAt(i);
					}
					_method->_proofInput->getCalls().RemoveAll();

					//remove the splices just added
					for (int i=0;i<_method->_proofInput->getSplices().GetSize();i++)
					{
						delete _method->_proofInput->getSplices().GetAt(i);
					}
					_method->_proofInput->getSplices().RemoveAll();
		
					//get the stroke frm the toewr
					_method->_proofInput->setStartStroke(tower->getSimulatorRoundsStartStroke());

					//we only respect the end change. the user can terminate when he wants.
					_method->_proofInput->setRowLimit(INT_MAX);
					_method->_proofInput->setLeadLimit(-1);
					_method->_proofInput->setPartLimit(-1);
					_method->_proofInput->setCircularCourseLimit(-1);

					//kick off the thread
					_method->startCreateMethodThread();

					//allow the thread time to calculate the first row/
					while (_method->getRowCreated() == false && !_method->Method::hasRequestedTerminate())
						Sleep(5);                  			

					int delay = 0;
					if (type == sst_connectedAutoStart)
					{
						ASSERT(_autoStartPackage != NULL);
						if (_autoStartPackage)
						{
							int place = _method->_proofInput->getStartRow().getPositionOfBell(_autoStartPackage->_bell);
							delay = _autoStartPackage->_interval * ((getNumber()*2) - place - 2);
							TRACE("Interval %d \r\n", _autoStartPackage->_interval);
							TRACE("START DELAY %d \r\n", delay);
							delay -= (timeGetTime() - _autoStartPackage->_timestamp);
							TRACE("START DELAY %d \r\n", delay);
							delay = max(delay, 1);
						}
					}

					//have to ensure the splice items are in teh action map.
					setupActionMap();
					keyboardManager()->updateKeyMap();
					speechManager()->updateWordList();
					multiBellInterfaceManager()->updateMBIList();
					serialControlManager()->updateSerialControlList();

					playManager()->play(new PlayRequestSimulator(*_method), delay);

					for (int i=0;i<_listenerList.GetSize();i++)
						_listenerList.GetAt(i)->simulatorManager_notifyNewMethod(_method);

					delete dummyMethod;
				}  
			}
			break;
		default:
			//this should not be pressed
			ASSERT(FALSE);
			break;
		}
	}*/
}



void SimulatorManager::OnUpdatePlayRounds(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( getSimulatorStateType() == sst_connectedSilent || 
					getSimulatorStateType() == sst_connectedMethod ||
					getSimulatorStateType() == sst_connectedAutoStart ||
					getSimulatorStateType() == sst_connectedRounds);
	pCmdUI->SetCheck(getSimulatorStateType() == sst_connectedRounds || 
					getSimulatorStateType() == sst_connectedRoundsPendingFromSilent ||
					getSimulatorStateType() == sst_connectedSilentPendingFromRounds ||
					getSimulatorStateType() == sst_connectedMethodPendingHandFromRounds || 
					getSimulatorStateType() == sst_connectedMethodPendingBackFromRounds );

}

void SimulatorManager::OnPlayMethod()
{  
	Stroke stroke = hand;
	if (_activeSimulatorTouchDoc)
		stroke = _activeSimulatorTouchDoc->getStartStroke();
	
	setSimulatorStateType((stroke == hand)?sst_connectedMethodPendingHandFromRounds:sst_connectedMethodPendingBackFromRounds);
}

void SimulatorManager::OnUpdatePlayMethod(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(getSimulatorStateType() == sst_connectedRounds);
	pCmdUI->SetCheck(getSimulatorStateType() == sst_connectedMethod|| 
					getSimulatorStateType() == sst_connectedMethodPendingBackFromRounds|| 
					getSimulatorStateType() == sst_connectedMethodPendingHandFromRounds|| 
					getSimulatorStateType() == sst_connectedRoundsPendingFromMethod	);
}

void SimulatorManager::OnStandUp()
{
	switch(getSimulatorStateType())
	{
	case sst_connectedMethod:
		setSimulatorStateType( sst_connectedSilentPendingFromMethod);
		break;
	case sst_connectedRounds:
		setSimulatorStateType( sst_connectedSilentPendingFromRounds);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void SimulatorManager::OnUpdateStandUp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(getSimulatorStateType() == sst_connectedMethod || 
					getSimulatorStateType() == sst_connectedRounds);	
	pCmdUI->SetCheck(getSimulatorStateType() == sst_connectedSilent || 
					getSimulatorStateType() == sst_connectedSilentPendingFromMethod || 
					getSimulatorStateType() == sst_connectedSilentPendingFromRounds ||
					getSimulatorStateType() == sst_connectedRoundsPendingFromSilent ||
					getSimulatorStateType() == sst_connectedAutoStart);
}

void SimulatorManager::playManager_notifyStartPlayingFromSeperateThread(int /*numberIncludingCover*/, Stroke /*stroke*/, CString startString, DWORD startTimestamp)
{
	//ASSERT(_number == numberIncludingCover);

	setSimulatorStateType( sst_connectedRounds);

	addComment(startString, true, startTimestamp);
}

void SimulatorManager::processActionFromSeperateThread(const SimulatorInput& simulatorInput, DWORD timestamp)
{
	SimulatorActionTypes* simulatorActionTypes;

	if (!towerManager()->isEditingProfile() && simulatorInput.isValidResponse())
	{
		if (_simulatorActionMap.Lookup(simulatorInput, simulatorActionTypes))   
		{
			for (int i=0;i<simulatorActionTypes->GetSize();i++)
			{
				SimulatorActionType simulatorActionType = simulatorActionTypes->GetAt(i);

				//take the action			
				//NOTE: if the action is a bell, then it is the mapping for the actual bell number.
				if (simulatorActionType >= SIMULATOR_ACTION_FIRST_BELL && simulatorActionType <= SIMULATOR_ACTION_LAST_BELL)
				{
//					TRACE("Bell %d\r\n",simulatorActionType); 
					if (getSimulatorStateType() == sst_connectedAutoStart)
					{
						_simulatorState->pulseAutoStart((int)simulatorActionType, timestamp);	
					}

					//turn off playing on the play thread
					_simulatorState->setPlayingBell(simulatorActionType, false);

					//play the bell
					playManager()->playBell(simulatorActionType, _simulatorState->getStroke(simulatorActionType));		

					//record info
					_strikingData.addRealStrike(simulatorActionType, timestamp); 

					//swap the thread safe state
					_simulatorState->toggleStroke(simulatorActionType);

					//notify all the lsiteners
					fireNotifyBellStateChangeFromSeperateThread((int)simulatorActionType);
				}
				else if (simulatorActionType >= SIMULATOR_ACTION_FIRST_METHOD && simulatorActionType <= SIMULATOR_ACTION_LAST_METHOD)
				{
					if (_method && getSimulatorStateType() == sst_connectedMethod)
						_method->setSpliceType(simulatorActionType);
				}
				else
				{
					switch (simulatorActionType)
					{
					case sat_startRingingRounds:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_PLAY_ROUNDS,0);
						break;
					case sat_startRingingRoundsAuto:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_PLAY_ROUNDS_AUTO,0);
						break;
					case sat_comeRoundNextTime:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_COME_ROUND_NEXT_TIME,0);
						break;
					case sat_goNext:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_PLAY_METHOD,0);
						break;
					case sat_stand:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STAND_UP,0);
						break;
					case sat_resetSimulator:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_SIMULATOR_RESET,0);
						break;                                                                      					
					case sat_playStop:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_STOP,0);
						break;                                                                      					
					case sat_playPause:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_PAUSE,0);
						break;          
					case sat_speedUp:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_SPEED_UP,0);
						break;       
					case sat_speedDown:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_SLOW_DOWN,0);
						break;       
					case sat_pitchUp:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_OVERALL_FREQ_UP,0);
						break;       
					case sat_pitchDown:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_OVERALL_FREQ_DOWN,0);
						break;       

					case sat_callPlain:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_CALLPLAIN,0);
						break;                                                                      					
					case sat_callBob:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_CALLBOB,0);
						break;                                                                      					
					case sat_callSingle:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_CALLSINGLE,0);
						break;                                                                      					
					case sat_callX:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_CALLX,0);
						break;                                                                      					
					case sat_callY:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_CALLY,0);
						break;                                                                      					
					case sat_callZ:
						::PostMessage(mainFrame()->GetSafeHwnd(), WM_COMMAND, ID_CALLZ,0);
						break;                                                                      					
					default:
						ASSERT(FALSE);
						break;
					}
				}
		
			}    
		}
	}
	else
	{
		TRACE("[SimulatorManager::processActionFromSeperateThread] Not Actioned: " + simulatorInput.getDescription() + "\r\n");
	}

	fireNotifyRawInputFromSeperateThread(simulatorInput,timestamp); 
}

//NOTE: This only gets called by the play manager generated bells. 
//      It does NOT get called for manually called bells.
void SimulatorManager::playManager_notifyBellPlayedFromSeperateThread(int place, int bell, Stroke stroke, DWORD perfectTimestamp, DWORD actualTimestamp, int rowIndex)
{
	if (!isConnected())
	{
		TRACE("[SimulatorManager::playManager_notifyBellPlayedFromSeperateThread]Dumping \r\n");
		return;
	}

	//need to check that the bell should be played. 
	if (actualTimestamp != 0)
	{
		//as the row we just played is the oposite to the one we need to display for the next row
		// we invert the stroke for the state, but leave it as it is for recording the historical 
		// striking information. 
		// i.e. we just rang back stroke. Therefore record back, and set the bell at hand.
		Stroke oppositeStroke = (stroke==hand)?back:hand;
		
		//we setStroke rather than toggle here as this is computer generated rather than user input.
		_simulatorState->setStroke(bell, oppositeStroke);

		//record strike info
		_strikingData.addGeneratedStrike(place, bell, stroke, perfectTimestamp, actualTimestamp); 

		//notify all the lsiteners
		fireNotifyBellStateChangeFromSeperateThread(bell);
	}
	else
	{
		//just notfifcation of the bell not being played.

		_simulatorState->decrementPlayingBell(bell);
	}

	fireNotifyLatestPositionFromSeperateThread(rowIndex, place);
}


void SimulatorManager::playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage)
{
	if (rowPositionChangedPackage._timestamp)
	{

		_strikingData.startNewRow(	rowPositionChangedPackage._stroke, 
									rowPositionChangedPackage._timestamp, 
									rowPositionChangedPackage._speed, 
									*rowPositionChangedPackage._row, 
									_number);

	}
}

void SimulatorManager::OnSimulatorReset()
{
	//the connect process will update the simulator
	reset();
	connectSimulator();

	fireNotifyBellStateChangeFromSeperateThread(-1);
}   

void SimulatorManager::OnUpdateSimulatorReset(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(getSimulatorStateType() != sst_connectedRoundsPendingFromSilent);
}

void SimulatorManager::reset()
{
	playManager()->OnPlayStop();
		  
	resetState();
	_strikingData.reset(getNumber());
	
	for (int i=0;i<_listenerList.GetSize();i++)
		_listenerList.GetAt(i)->simulatorManager_notifyNewMethod(NULL);

	//terminate the method gracefully
	if (_method)
	{
		//this will wait untill we have finished the calculation. 
		while (_method->isInThread())
		{
			_method->requestEarlyTerminate();
		}
		delete _method;	
		_method = NULL;
	}

	setMethodPositionSimulatorStatusBar(NULL);

}

void SimulatorManager::resetState()
{
	simulatorManager()->_simulatorState->setAllPlayBell(true);
	Stroke stroke = hand;
	Tower* tower = towerManager()->getTower();
	if (tower)
		stroke = tower->getSimulatorRoundsStartStroke();
	
	_simulatorState->setAllStroke(stroke);
	fireNotifyBellStateChangeFromSeperateThread(-1);

	if (_method)
	{
		//this will wait untill we have finished the calculation. 
		_method->requestEarlyTerminate();
	}
}





bool SimulatorManager::calculateBellBoxes(int cx, int cy, TouchDoc* doc)
{
	Tower* tower = towerManager()->getTower();

	if ((doc) && (tower))
	{
		if (doc != _activeSimulatorTouchDoc)
			return false;

		tower->rebuildSimulatorGUIComponents();

		int startBell = tower->getFirstSimulatedBell();

		bool clockwise = (tower->getClockwise() == TRUE);

		ASSERT((startBell  >= 0)&&(startBell < getNumber()));

		if ((startBell  >= 0)&&(startBell < getNumber()))
		{    
			double X, Y;
			
			//calculate the number of rad between bells
			double segment = PI2/getNumber();
			
			//calculate the raduis, minus a gap round the edge
			double radius = (min((double)cx, (double)cy)/2.0) - 10; 
			
			//calculate the box width as a ratio of the segment size at a raduis
			GlobalFunctions::convertAngleToXY(segment/2, radius, X, Y);	
			int boxWidth = (int)max(X*0.6,20.0);//scale

			//remove the box width from teh radius
			radius = max(radius-boxWidth, 20);
			
			//calculate the start position, then the offset for teh start bell
			double theta = 0.0;

			if (clockwise)
				theta = (segment * ((double)startBell + 0.5));
			else
				theta = (-segment * ((double)startBell + 0.5));
			
			//calculate the box positions
			for (int i=0;i<getNumber();i++)
			{
				GlobalFunctions::convertAngleToXY(theta, radius, X, Y);

				CRect bellRect((int)X - boxWidth,(int)Y-boxWidth,(int)X+ boxWidth, (int)Y+boxWidth);
				bellRect.OffsetRect((cx/2), (cy/2));
				_bellGUI[i].setBellRect(bellRect);
				CString strNum;
				strNum.Format("%d",i+1);
				_bellGUI[i].setText(strNum, X <0);

				if (clockwise)
					theta -= segment;
				else
					theta += segment; 
			} 
			return true;
		}
	}

	//the defaut - something has gone wrong
	for (int i=0;i<MAXBELLS;i++)
	{
		_bellGUI[i].setBellRect(CRect(0,0,0,0));
		_bellGUI[i].setText("", false);
	}

	return false;

}

int SimulatorManager::hitTestBell(const CPoint& point)
{
	//go through all the gui bells, and check for contains
	for (int i=0;i<getNumber();i++)
	{
		if (_bellGUI[i].getBellRect().PtInRect(point))
			return i;
	}                

	return -1;
} 


const SimulatorGUIBell& SimulatorManager::getGUIBell(int index) 
{
	ASSERT(index >= 0 && index < MAXBELLS);
	
	if (index >= 0 && index < MAXBELLS)
		return _bellGUI[index];

	return _bellGUI[0];
}

bool SimulatorManager::isValid(CString& msg, TouchDoc* doc )
{
	if ((doc == NULL)||(doc != _activeSimulatorTouchDoc))
	{
		msg += "Inactive";
		return false;   		
	}

	Tower* tower = towerManager()->getTower();

	if (!tower)
	{
		msg += "Please create a profile";
		return false;
	}

	return true;
}

int SimulatorManager::getNumber()
{
	return _number;
}

void SimulatorManager::drawBell(int index, CDC* pDC, TouchDoc* doc)
{
	Tower* tower = towerManager()->getTower();

	ASSERT(doc && _activeSimulatorTouchDoc == doc && tower);
	ASSERT(index >=0 && index < MAXBELLS);
	
	if ((doc && _activeSimulatorTouchDoc == doc && tower) &&
		(index >=0 && index < MAXBELLS))
	{

		CFont* oldFont = pDC->SelectObject(&tower->getBellFont());
		COLORREF oldColor = pDC->SetTextColor(tower->getBellFontInfo()._color);

		Stroke stroke = _simulatorState->getStroke(index);
		SimulatorPictureType imageType = (stroke == hand)?
			(_bellGUI[index].isLeftAlign()?spt_hand_left:spt_hand_right):
			(_bellGUI[index].isLeftAlign()?spt_back_left:spt_back_right);
		tower->getBellImage(imageType).Draw(pDC->m_hDC, _bellGUI[index].getBellRect());

		//keep for debug 
		//memdc->Draw3dRect(imageRect, RGB(0,0,0), RGB(0,0,0));

		UINT format = DT_WORDBREAK| DT_NOPREFIX;
		if (!_bellGUI[index].isLeftAlign()) 
			format |= DT_RIGHT;

		pDC->DrawText(_bellGUI[index].getText(), CRect(_bellGUI[index].getBellRect()), format);

		pDC->SelectObject(oldFont);
		pDC->SetTextColor(oldColor);
	}
}

void SimulatorManager::setFirstSimulatedBell(int bell)
{
	TRACE("[SimulatorManager::setFirstSimulatedBell] %d\r\n", bell);

	ASSERT(bell >=0 && bell < getNumber());
	if (bell >=0 && bell < getNumber())
	{
		Tower* tower = towerManager()->getTower();

		if (tower)
		{
			tower->setFirstSimulatedBell(bell);
			
			towerManager()->fireUpdateTowerList();

		}
	}
}
int SimulatorManager::getFirstSimulatedBell()
{
	Tower* tower = towerManager()->getTower();
	if (tower)
	{
		return tower->getFirstSimulatedBell();
	}

	return 0;
}

void SimulatorManager::setCoverBellCount(int count)
{
	TRACE("[SimulatorManager::setCoverBellCount] %d\r\n", count);

	ASSERT(count >=0 && count < MAXBELLS);
	if (count >=0 && count < MAXBELLS)
	{
		Tower* tower = towerManager()->getTower();

		if (tower)
		{
			tower->setCoverCount(count);

			setNumber();
			
			//change all the input mappings   
			towerManager()->fireUpdateTowerList();
		}
	}
}

int SimulatorManager::getCoverBellCount()
{
	Tower* tower = towerManager()->getTower();
	if (tower)
	{
		return tower->getCoverCount();
	}

	return 0;
}

const StrikingData& SimulatorManager::getStrikingData() const
{
	return _strikingData;
}

void SimulatorManager::editStrikingControls(int bell) 
{
	Tower* tower = towerManager()->getTower();

	ASSERT(tower);

	if (!tower)
		return;

	mainFrame()->showStrikingControls(tower, bell)	;
}


bool SimulatorManager::isPlayingBell(int bell)
{
	return _simulatorState->isPlayingBell(bell);
}



void SimulatorManager::OnCreateStrikeReport()
{
	CDocument* pDoc = app()->openDocument(vt_strike);

	StrikeDoc* strikeDoc = dynamic_cast<StrikeDoc*>(pDoc);

	ASSERT(strikeDoc);

	if (strikeDoc)
	{
		if (_activeSimulatorTouchDoc)
		{
			CString title = _activeSimulatorTouchDoc->GetTitle();
			title.Replace(".tch", "");
			CString strDateTime = COleDateTime::GetCurrentTime().Format(" %x %H_%M");
			strDateTime.Replace("\\", "_");
			strDateTime.Replace("/", "_");
			title += strDateTime;

			strikeDoc->SetTitle(title);
		}
		strikeDoc->setInitialData(_strikingData);
	} 
}

void SimulatorManager::OnUpdateCreateStrikeReport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(_strikingData.canCreateReport() && playManager()->isStopped());
}

void SimulatorManager::setDefaultBlueLineStyle(const Style& style)
{
	_style = style;
	save();
}

const Style& SimulatorManager::getDefaultBlueLineStyle()
{
	return _style;
}

	
int SimulatorManager::getSimulatorStrikingSngleBell(SimulatorStrikingWndType type)
{
	int val = 0;

	switch (type)
	{
	case sswt_single_1:
		val = _simulatorStrikingWndTypeBell1;
		break;
	case sswt_single_2:
		val = _simulatorStrikingWndTypeBell2;
		break;
	default:
		ASSERT(FALSE);
	}

	return min(val, getNumber()-1);
}

bool SimulatorManager::setSimulatorStrikingSngleBell(SimulatorStrikingWndType type, int val)
{
	bool retVal = false;

	switch (type)
	{
	case sswt_single_1:
		if (_simulatorStrikingWndTypeBell1 != val)
		{
			_simulatorStrikingWndTypeBell1 = val;
			retVal = true;
		}
		break;
	case sswt_single_2:
		if (_simulatorStrikingWndTypeBell2 != val)
		{
			_simulatorStrikingWndTypeBell2 = val;
			retVal =true;
		}
		break;
	default:
		ASSERT(FALSE);
	}
	

	fireNotifyBellStateChangeFromSeperateThread(-1);
	return retVal;
}	  



void SimulatorManager::setMethodPositionSimulatorStatusBar(MethodPosition *methodPositionStatusBar)
{
/*	todo if (_activeSimulatorTouchDoc && isConnected())
	{
		CString methodName;
		if (_method && _method->getCurNot())
		{
			methodName = _method->getCurNot()->getName(false, true);
		}
		_activeSimulatorTouchDoc->setMethodPositionSimulatorStatusBar(methodPositionStatusBar, methodName);  
	}
	else
		delete methodPositionStatusBar; */
}

void SimulatorManager::setSimulatorStateType(SimulatorStateType type) 
{ 
	_simulatorState->setStateType(type);
}         

SimulatorStateType SimulatorManager::getSimulatorStateType() 
{ 
	return _simulatorState->getStateType();
}

void SimulatorManager::OnStrikingTest()
{
	StrikingTestDlg dlg(mainFrame(), getNumber());
	if (dlg.DoModal() == IDOK)
	{
		Tower* tower = towerManager()->getTower();
		
		if (tower)
		{
			if (dlg._hideVisualFeedback)
			{
				mainFrame()->hideVisualFeedbackForSimulator();
			}

			tower->setUpListeningTest(dlg._bellsAffected, 
								(dlg._allowFllowOn == TRUE), 
								(dlg._lockHandBack == TRUE), 
								getNumber(), 
								dlg._maxErrorValue);
		}
	}
}

void SimulatorManager::OnStrikingTestResults()
{

	Tower* tower = towerManager()->getTower();

	if (tower)
	{
		GenericPropDlg dlg(mainFrame(), "Striking Errors", "StrikingTestResults");

		for (int i=0;i<getNumber();i++)
		{
			StrikingErrors& error = tower->getStrikingError(i);	

			if (error.getLockHandAndBack())
			{
				if (error.getHandstrokeLateness() != 0.0)
				{
					CString item;
					item.Format("Bell %d", i+1);
					CString value;
					double lateness = error.getHandstrokeLateness();
					value.Format("%.3f%% %s", abs(lateness), ((lateness == 0)?"":((lateness> 0)?"Late":"Early")));
					dlg.addProp(item, value);
				}
				if (error.getHandstrokeQuality() != 0.0)
				{
					CString item;
					item.Format("Bell %d", i+1);
					CString value;
					double quality = error.getHandstrokeQuality();
					value.Format("%.3f%%", abs(quality));
					dlg.addProp(item, value);
				}
			}
			else
			{
				if (error.getHandstrokeLateness() != 0.0)
				{
					CString item;
					item.Format("Bell %d Handstroke", i+1);
					CString value;
					double lateness = error.getHandstrokeLateness();
					value.Format("%.3f%% %s", abs(lateness), ((lateness == 0)?"":((lateness> 0)?"Late":"Early")));
					dlg.addProp(item, value);
				}
				if (error.getHandstrokeQuality() != 0.0)
				{
					CString item;
					item.Format("Bell %d Handstroke", i+1);
					CString value;
					double quality = error.getHandstrokeQuality();
					value.Format("%.3f%% %s", abs(quality), "Erratic");
					dlg.addProp(item, value);
				}

				if (error.getBackstrokeLateness() != 0.0)
				{
					CString item;
					item.Format("Bell %d Backstroke", i+1);
					CString value;
					double lateness = error.getBackstrokeLateness();
					value.Format("%.3f%% %s", abs(lateness), ((lateness == 0)?"":((lateness> 0)?"Late":"Early")));
					dlg.addProp(item, value);
				}
				if (error.getBackstrokeQuality() != 0.0)
				{
					CString item;
					item.Format("Bell %d Backstroke", i+1);
					CString value;
					double quality = error.getBackstrokeQuality();
					value.Format("%.3f%% %s", abs(quality), "Erratic");
					dlg.addProp(item, value);
				}    
			}

			if (error.getFollowPreviousBellError() )
			{
				CString item;
				item.Format("Bell %d Following Previous Error", i+1);
				CString value;
			
				dlg.addProp(item, "True");
			}

		}



		dlg.DoModal();
	}	
}

void SimulatorManager::addComment(TowerTTSTypes type, CString token )
{
	Tower* tower = towerManager()->getTower();
	if (tower)
	{
		addComment(tower->getTTSString(type, token));
	}
}

void SimulatorManager::addComment(CString msg, bool mute, DWORD timestamp )
{
	if (timestamp == ULONG_MAX)
		timestamp = timeGetTime();

	//speak
	if (!playManager()->isMute() && !mute)
		speechManager()->speak(msg);

	//add to striking
	_strikingData.addSpeaking(msg, timestamp);

	//add to method
	if (_method)
		_method->addSimulatorComment(msg);
}

void SimulatorManager::OnCallsUseTouch()
{
	_simulatorCallingType = sct_touchCalls;
	if (_method)
	{
		_method->forceRecalculationOfCurrentCallAndSplice();
	}
}

void SimulatorManager::OnUpdateCallsUseTouch(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_simulatorCallingType == sct_touchCalls);
}


void SimulatorManager::OnCallsPlain()
{
	_simulatorCallingType = sct_plainCourse;
	if (_method)
	{
		_method->forceRecalculationOfCurrentCallAndSplice();
	}
}

void SimulatorManager::OnUpdateCallsPlain(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_simulatorCallingType == sct_plainCourse);
}

void SimulatorManager::OnCallsRandom()
{
	_simulatorCallingType = sct_random;
	if (_method)
	{
		_method->forceRecalculationOfCurrentCallAndSplice();
	}
}

void SimulatorManager::OnUpdateCallsRandom(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_simulatorCallingType == sct_random);
}

SimulatorCallingType SimulatorManager::getSimulatorCallingType()
{
	return _simulatorCallingType;
}

void SimulatorManager::doCallEnable(CCmdUI *pCmdUI, SimulatorCallType type)
{
	if (_method)
	{
		pCmdUI->Enable(getSimulatorStateType() == sst_connectedMethod &&
						_method->isPossibleCall(type));

		pCmdUI->SetCheck(_method->getSimulatorCurrentCallType() == type);
	}
	else
		pCmdUI->Enable(FALSE); 
}


void SimulatorManager::OnCallPlain()
{
	if (_method)
		_method->setCallType(sct_plain);
}

void SimulatorManager::OnUpdateCallPlain(CCmdUI *pCmdUI)
{
	doCallEnable(pCmdUI, sct_plain);
}


void SimulatorManager::OnCallBob()
{
	if (_method)
		_method->setCallType(sct_bob);
}

void SimulatorManager::OnUpdateCallBob(CCmdUI *pCmdUI)
{
	doCallEnable(pCmdUI, sct_bob);
}

void SimulatorManager::OnCallSingle()
{
	if (_method)
		_method->setCallType(sct_single);
}

void SimulatorManager::OnUpdateCallSingle(CCmdUI *pCmdUI)
{
	doCallEnable(pCmdUI, sct_single);
}

void SimulatorManager::OnCallX()
{
	if (_method)
		_method->setCallType(sct_callX);
}

void SimulatorManager::OnUpdateCallX(CCmdUI *pCmdUI)
{
	doCallEnable(pCmdUI, sct_callX);
}

void SimulatorManager::OnCallY()
{
	if (_method)
		_method->setCallType(sct_callY);
}

void SimulatorManager::OnUpdateCallY(CCmdUI *pCmdUI)
{
	doCallEnable(pCmdUI, sct_callY);
}

void SimulatorManager::OnCallZ()
{
	if (_method)
		_method->setCallType(sct_callZ);
}

void SimulatorManager::OnUpdateCallZ(CCmdUI *pCmdUI)
{
	doCallEnable(pCmdUI, sct_callZ);
}

const SimulatorActionMap& SimulatorManager::getSimulatorActionMap()
{
	return _simulatorActionMap;
}




