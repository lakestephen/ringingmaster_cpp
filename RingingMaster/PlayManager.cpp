// PlayManager.cpp: implementation of the PlayManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlayManager.h"
#include "PlayManager.h"
#include "TowerManager.h"
#include "UserMessages.h"
#include "Method.h"
#include "Lead.h"
#include "Tower.h"
#include "PlayPosition.h"
#include "RowPosition.h"
#include "SpeechManager.h"
#include "MethodCallChange.h"
#include "Lead.h"
#include "SimulatorManager.h"
#include "TouchDoc.h"
#include "CribDoc.h"
#include ".\playmanager.h"
#include "StrikeDoc.h"
#include "StrikingRow.h"
#include "ProofManager.h"
#include "MethodCrib.h"
#include "ProofInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PlayManager::PlayManager() :
_playRequest(NULL),
_tracking(TRUE),
_state(ps_stopped),
_numberInclCover(0),
_tower(NULL),
_waitingTowerReload(NULL),
_pullsRemainingOnChange(0),
_bellExactEvent(0),
_everyHandstrokeEvent(0),
_initialEvent(0),
_restartEveryHandstrokeEvent(0),
_pDS(NULL),
_offset(0),
_usingSound(TRUE),
_mute(FALSE)
{

}



PlayManager::~PlayManager()
{

	unLoadTower();

	release();

	CSingleLock lock(&_waitingForRestartCriticalSection, TRUE);

	ASSERT(_waitingTowerReload == 0);
	if (_waitingTowerReload)
	{
		delete _waitingTowerReload;
		_waitingTowerReload = NULL;
	}

	ASSERT(_waitingForLeadIncDec.GetSize() == 0);
	if (_waitingForLeadIncDec.GetSize() > 0)
	{
		_waitingForLeadIncDec.RemoveAll();
	}

	ASSERT(_playRequest == 0);
}

BEGIN_MESSAGE_MAP(PlayManager, CCmdTarget)
	//{{AFX_MSG_MAP(PlayManager)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, OnUpdatePauseMethod)
	ON_UPDATE_COMMAND_UI(ID_STOP, OnUpdateStopMethod)
	ON_UPDATE_COMMAND_UI(ID_PLAY, OnUpdatePlayPlay)
	ON_UPDATE_COMMAND_UI(ID_DEC_LEAD, OnUpdateDecLead)
	ON_UPDATE_COMMAND_UI(ID_INC_LEAD, OnUpdateIncLead)
	ON_UPDATE_COMMAND_UI(ID_SPEED_UP, OnUpdateSpeedUp)
	ON_UPDATE_COMMAND_UI(ID_SLOW_DOWN, OnUpdateSlowDown)
	ON_UPDATE_COMMAND_UI(ID_OVERALL_FREQ_UP, OnUpdateOverallFrequencyUp)
	ON_UPDATE_COMMAND_UI(ID_OVERALL_FREQ_DOWN, OnUpdateOverallFrequencyDown)
	ON_COMMAND(ID_STOP, OnPlayStop)
	ON_COMMAND(ID_SLOW_DOWN, OnSlowDown)
	ON_COMMAND(ID_SPEED_UP, OnSpeedUp)
	ON_COMMAND(ID_OVERALL_FREQ_DOWN, OnOverallFrequencyDown)
	ON_COMMAND(ID_OVERALL_FREQ_UP, OnOverallFrequencyUp)
	ON_COMMAND(ID_PAUSE, OnPlayPause)
	ON_COMMAND(ID_INC_LEAD, OnIncLead)
	ON_COMMAND(ID_DEC_LEAD, OnDecLead)
	ON_COMMAND(ID_PLAY, OnPlayPlay)	
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PLAY_MUTE, OnPlayMute)
	ON_UPDATE_COMMAND_UI(ID_PLAY_MUTE, OnUpdatePlayMute)
END_MESSAGE_MAP()


void PlayManager::addEventListener(PlayManagerEventListener* playManagerEventListener)
{
	//if we are the first to be added, then open the port
	ASSERT(playManagerEventListener != NULL);

	_listenerList.Add(playManagerEventListener);

}

void PlayManager::removeEventListener(PlayManagerEventListener* playManagerEventListener)
{
	ASSERT(playManagerEventListener != NULL);

	int hasRemoved = 0;

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == playManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	}

	ASSERT(hasRemoved == 1);
}


void PlayManager::load()
{
	_tracking = AfxGetApp()->GetProfileInt("Play", "Tracking", TRUE);
	_usingSound = AfxGetApp()->GetProfileInt("Play", "Using Sound", TRUE);
	_mute = AfxGetApp()->GetProfileInt("Play", "Mute", FALSE);

}

void PlayManager::openUp()
{
	//create the audio device
	initSound();

	towerManager()->addEventListener(this);
}

void PlayManager::closeDown()
{
	towerManager()->removeEventListener(this);
}

void PlayManager::initSound()
{
    HRESULT hr;

	// Create DirectSound Object
	if (FAILED(hr = DirectSoundCreate(NULL, &_pDS, NULL)))
	{
		TRACE("[PlayManager] Create\nAudio cannot be used\n");
		return;
	}

	// Set Cooperative Level
	if (FAILED(hr = _pDS->SetCooperativeLevel(*AfxGetMainWnd(), DSSCL_PRIORITY)))
	{
		TRACE("[PlayManager] SetCooperativeLevel\n");
		return;
	}
}


void PlayManager::save()
{
	doStop(1500);

	AfxGetApp()->WriteProfileInt("Play", "Tracking", _tracking);
	AfxGetApp()->WriteProfileInt("Play", "Using Sound", _usingSound);
	AfxGetApp()->WriteProfileInt("Play", "Mute", _mute);
}


Method* PlayManager::getActiveMethod()
{
	Method* method  = NULL;

	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
	if (pTouchDoc)
		method = pTouchDoc->getMethod();
	else
	{
		CribDoc* pCribDoc = mainFrame()->getActiveCribDoc();
		if (pCribDoc)
			method = dynamic_cast<Method*>(pCribDoc->getMethod());
	}  

	return method;
}

void PlayManager::OnUpdatePlayPlay(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((isPlayAvailable())&&
					 (isPlaying()));

	Method* method =  getActiveMethod();
	StrikeDoc* strikeDoc =  mainFrame()->getActiveStrikeDoc();

	pCmdUI->Enable(
					(isPlayAvailable())&&
					(_tower)&&
					(!simulatorManager()->isConnected())&&
					(_initialEvent == 0)&&
					(
						(
							(isStopped())&&
							((strikeDoc &&  towerManager()->getTower()->getNumber() >=strikeDoc->getStrikingData().getNumber()) ||
							(method && towerManager()->getTower()->getNumber() >= towerManager()->getTower()->getNumberInclCover(method->getNumber())))
						)||
						(isPlaying())||
						(isPaused ())
					)
				   );
}

void PlayManager::OnPlayPlay() 
{
	Method* method = getActiveMethod();
	if (method)
	{
		CribDoc* pCribDoc = mainFrame()->getActiveCribDoc();
		if (pCribDoc)
		{
				 
			//create a complete method rather than just a lead.
			Notation notation = pCribDoc->getNotation();

			ProofInput proofInput(notation.getNumber());
			proofInput.addNotation(notation);
			proofInput.setPerformAnalysis(); //todo do we want to do analysis here?
			Method met;
			proofManager()->requestProof(proofInput, met);
			met.setUniqueId(method->getUniqueId());

			play(new PlayRequest(met));			  
		}
		else
		{
			play(new PlayRequest(*method));	
		}
	}
	else
	{
		StrikeDoc* strikeDoc =  mainFrame()->getActiveStrikeDoc();

		if (strikeDoc)
		{
			play(new PlayRequest(strikeDoc->getStrikingData()));
		}		
	}
}

void PlayManager::OnUpdateCcPlayLive(CCmdUI* pCmdUI, int number, BOOL  enoughRows)
{
	//NOTE set check does not work in dialogs

	pCmdUI->Enable(isPlayAvailable()&&
				   _tower&&
				   isStopped()&&
				   enoughRows&&
				   towerManager()->getTower()->getNumber() >= number &&
				   (_initialEvent == 0));
}

void PlayManager::OnUpdatePauseMethod(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((isPlayAvailable())&&
					 (isPaused()));
	pCmdUI->Enable((isPlayAvailable())&&
				   ((isPlaying())||(isPaused ()))&&
				   (_initialEvent == 0));
}

void PlayManager::OnUpdateStopMethod(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(isPlayAvailable()&&
				   ( isPlaying()||isPaused ())&&
				   (_initialEvent == 0));
}

void PlayManager::OnUpdateDecLead(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(isPlayAvailable()&&
				   ( isPlaying()||isPaused ())&&
				   (_playRequest && !_playRequest->isStrikingData()) &&
				   (_initialEvent == 0) &&
				   !simulatorManager()->isConnected());
}

void PlayManager::OnUpdateIncLead(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(isPlayAvailable()&&
				   ( isPlaying()||isPaused ())&&
				   (_playRequest && !_playRequest->isStrikingData()) &&
				   (_initialEvent == 0) &&
				   !simulatorManager()->isConnected());
}


void PlayManager::OnUpdateSpeedUp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
	Tower* activeTower = towerManager()->getTower();
	pCmdUI->Enable(isPlayAvailable()&&
				   ( isPlaying()||isPaused ())&&
				   (_playRequest && !_playRequest->isStrikingData())&&
					 (activeTower)&&
					 (activeTower->canIncrementSpeed())&&
					 (_initialEvent == 0));
}

void PlayManager::OnUpdateSlowDown(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
	Tower* activeTower = towerManager()->getTower();
	pCmdUI->Enable(isPlayAvailable()&&
				   ( isPlaying()||isPaused ())&&
				   (_playRequest && !_playRequest->isStrikingData()) &&
					 (activeTower)&&
					 (activeTower->canDecrementSpeed())&&
					 (_initialEvent == 0));
}

void PlayManager::OnUpdateOverallFrequencyUp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
	Tower* activeTower = towerManager()->getTower();
	pCmdUI->Enable(isPlayAvailable()&&
				   ( isPlaying()||isPaused ())&&
					 (activeTower)&&
					 (activeTower->canIncrementOverallFrequency())&&
					 (_initialEvent == 0));
}

void PlayManager::OnUpdateOverallFrequencyDown(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
	Tower* activeTower = towerManager()->getTower();
	pCmdUI->Enable(isPlayAvailable()&&
				   ( isPlaying()||isPaused ())&&
					 (activeTower)&&
					 (activeTower->canDecrementOverallFrequency())&&
					 (_initialEvent == 0));
}

void PlayManager::play(PlayRequest* request, int delay)
{
	switch (_state)
	{
	case ps_playing:
		doStop();
		delete request;
		break;
	case ps_paused:
		doRestart();
		delete request;
		break;
	case ps_stopped:
		ASSERT(request);
		doPlay(request, delay);
		break;
	}
}

void PlayManager::OnPlayStop()
{
	doStop();
}

void PlayManager::OnPlayPause()
{
	switch (_state)
	{
	case ps_paused:
		doRestart();
		break;
	case ps_playing:
		doPause();
		break;
	}
}

void PlayManager::OnPlayMute()
{
	_mute = !_mute;
}

void PlayManager::OnUpdatePlayMute(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_mute == TRUE);

	pCmdUI->Enable(isPlayAvailable()&&
				   _tower);
}

void PlayManager::OnIncLead()
{
 	//we need to restart the timers.
	CSingleLock lock(&_waitingForRestartCriticalSection, TRUE);

	_waitingForLeadIncDec.Add(wflid_inc);
}

void PlayManager::OnDecLead()
{
 	//we need to restart the timers.
	CSingleLock lock(&_waitingForRestartCriticalSection, TRUE);

	_waitingForLeadIncDec.Add(wflid_dec);	
}

void PlayManager::OnSlowDown()
{
	towerManager()->decrementSpeed();
}

void PlayManager::OnSpeedUp()
{
	towerManager()->incrementSpeed();
}

void PlayManager::OnOverallFrequencyDown()
{
	towerManager()->decrementOverallFrequency();
}

void PlayManager::OnOverallFrequencyUp()
{
	towerManager()->incrementOverallFrequency();
}

bool PlayManager::shouldShowPlayInfo()
{
	return (isPlaying() || isPaused());	  //should we show when using the simulator
}

void PlayManager::removeRow(int methodRequestID)
{
	//add NULL to indicate that a row needs to be removed.
	// This allows the sequence of added and removed rows to be maintaied.
	if ((isPlaying())&&(_playRequest)&&(_playRequest->getUniqueId() == methodRequestID))
		_pendingRows.AddHead((RowExtra*)NULL);
}

void PlayManager::addRow(RowExtra* rowExtra, int methodId)
{
	if ((isPlaying())&&(_playRequest)&&(rowExtra)&&
		(_playRequest->getUniqueId() == methodId))
	{
		_pendingRows.AddHead(rowExtra);
	}
	else
	{
		delete rowExtra;
	}
}

void PlayManager::toggleTracking()
{
	_tracking = !_tracking;
	fireNotifyRowPositionChanged();
}

void PlayManager::fireNotifyRowPositionChanged(DWORD timestamp)
{
	//get the row and lead, fire an update
	static RowPositionChangedPackage rowPositionChangedPackage;


	rowPositionChangedPackage._timestamp = timestamp;
	rowPositionChangedPackage._tracking = _tracking;
	rowPositionChangedPackage._playRequestId = _playRequest?_playRequest->getUniqueId():-1;
	rowPositionChangedPackage._stroke = _stroke;
	
	rowPositionChangedPackage._row = 0;
	if (_playRequest && !_playRequest->isStrikingData())
		rowPositionChangedPackage._row = _playRequest->GetAt(_row);

	if (_tower)
		rowPositionChangedPackage._speed = _tower->getSpeed();
	else
		rowPositionChangedPackage._speed = 0;
		  
	if ((_state == ps_stopped)||(!_playRequest) )
		rowPositionChangedPackage._rowPosition.setInvalid();
	else
	{
		if (_playRequest->isStrikingData())
			rowPositionChangedPackage._rowPosition.setRow(0,_row);
		else
			rowPositionChangedPackage._rowPosition = *_playRequest->GetAt(_row);
	}

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->playManager_notifyRowPositionChangedFromSeperateThread(rowPositionChangedPackage);
	}  
}

BOOL PlayManager::isTracking()
{
	return _tracking;
}

void PlayManager::towerManager_notifyUpdateTowerList()
{
	Tower* activeTower = towerManager()->getTower();

	if ((!_usingSound)||(!activeTower))
	{
		doStop();
		unLoadTower();
	}
	else
	{
		bool needToRestart = true;

		if ((!_tower)||(_tower->hasChangedForPlayManager(*activeTower,needToRestart)))
		{
			//we must have a change to the tower
			switch (_state)
			{
			case ps_playing:
				{
					if (!needToRestart)
					{
						//do a simple copy.
						_tower->copyNonRestartChangesForPlayManager(*activeTower);
					}
					else
					{
						//we need to restart the timers.
						//take a copy of the tower, and let the play thread reload.
						CSingleLock lock(&_waitingForRestartCriticalSection, TRUE);

						if (_waitingTowerReload)
						{
							delete _waitingTowerReload;
							_waitingTowerReload = NULL;
						}

						_waitingTowerReload = new Tower(*activeTower);
					}
				}
				break;
			case ps_paused:
			case ps_stopped:
				loadTower(*activeTower);
				break;
			}
		}
	}
}

void PlayManager::removeAllPendingRows()
{
	while(!_pendingRows.IsEmpty())
	{
		delete _pendingRows.RemoveHead();
	}
}

//this makes sure that the offset is set correctly, so as to play 
// the correct bell when starting the simulator auto start
bool PlayManager::doTowerChecksLite(int number)
{
	if (_tower)
	{
		_numberInclCover = _tower->getNumberInclCover(number);
		_coverCount = (_numberInclCover - number);
		_offset = _tower->getOffset(_numberInclCover);
		return true;
	}

	return false;
}

bool PlayManager::doTowerChecks()
{
	//do we have a request?
	if (!_playRequest)
	{
		AfxMessageBox("No More Method - Stopping");
		return false;
	}

	//do we have a tower
	if (!_tower)
	{
		AfxMessageBox("No Tower Loaded - Stopping");
		return false;
	}

	_numberInclCover = _tower->getNumberInclCover(_playRequest->getNumber());
	_coverCount = (_numberInclCover - _playRequest->getNumber());

	if (_numberInclCover > _tower->getNumber())
	{
		CString str;
		str.Format("The tower (%s) must have at least as many bells\n as the method (including cover) requires (%d). ",
			_tower->getDisplayName(), _numberInclCover);
		AfxMessageBox(str);
		return false;
	}

	_offset = _tower->getOffset(_numberInclCover);

	return true;
}

void PlayManager::doRestart()
{
	if (!doTowerChecks())
	{
		//tower checks have failed. Make sure we are paused.
		doStop();
		return;
	}

	//restart
	_state = ps_playing;

	if (_playRequest && !_playRequest->isStrikingData())
		//start a timer to restart the evry handstroke timer
		startRestartEveryHandstrokeTimer();
}

bool PlayManager::doPlay(PlayRequest* request, int delay)
{
	ASSERT(_playRequest == NULL);
	ASSERT(request != NULL);

	delete _playRequest;
	_playRequest = request;

	if (!doTowerChecks())
	{
		delete _playRequest;
		_playRequest = NULL;
		doStop();
		return false;
	}

	//get going
	_state = ps_playing;

	_doneLastRow = false;
	_pullsRemainingOnChange = _playRequest->getStartChangeRepeat(_tower);

	if (_playRequest->isStrikingData())
	{
		//check that the timers are not running.
		stopTimers();
		AfxBeginThread(StrikingDataThread, this);
	}
	else
	{
		_place = 0;
		_row = 0;

		//ensure that we start onthe requested stroke
		_stroke = _playRequest->GetAt(0)->getStroke();

		if (!GlobalFunctions::isEven(_pullsRemainingOnChange))
		{
			if (_stroke == hand)
				_stroke = back;
			else
				_stroke = hand;
		}
		fireNotifyRowPositionChanged();

		//start the timer
		startInitialTimer(delay);
	}

	return true;
}

PlayRequest* PlayManager::getPlayRequest()
{
	return _playRequest;
}

void PlayManager::doPause()
{
	stopTimers();
	_state = ps_paused;
}


void PlayManager::doStop(int delay)
{
	stopTimers();

	Sleep(delay);//give the timer thread a chance to finish

	_state = ps_stopped;

	//use this to synchronise the striking data thread
	if (_playRequest && _playRequest->isStrikingData())
	{
		TRACE("[PlayManager::doStop] waiting for lock\r\n");
		CSingleLock lock(&_playRequest->getStrikingData().getCriticalSection(), TRUE);			
		TRACE("[PlayManager::doStop] got lock\r\n");
	}

	PlayRequest* playRequest = _playRequest;
	_playRequest = NULL;
	delete playRequest;

	_place = 0;
	_row = 0;


	for (int i=0;i<_listenerList.GetSize();i++)
		_listenerList.GetAt(i)->playManager_notifyStopPlaying();

	fireNotifyRowPositionChanged();

	ASSERT(_waitingTowerReload == 0);
	if (_waitingTowerReload)
	{
		CSingleLock lock(&_waitingForRestartCriticalSection, TRUE);

		if (_waitingTowerReload)//check again it has changes while getting lock
		{
			VERIFY(loadTower(_waitingTowerReload));
			_waitingTowerReload = NULL;
		}
	}	 

	ASSERT(_waitingForLeadIncDec.GetSize() <= 0);
	if (_waitingForLeadIncDec.GetSize() > 0)
	{
		CSingleLock lock(&_waitingForRestartCriticalSection, TRUE);
		_waitingForLeadIncDec.RemoveAll();
	}
}



void PlayManager::stopTimers()
{
	stopRestartEveryHandstrokeTimer();
	stopEveryHandstrokeTimer();
	stopBellExactTimer();
}

UINT PlayManager::StrikingDataThread(LPVOID pParam)
{
	//we only want the catch block in release

#ifndef _DEBUG
	try
	{
#endif
		//passed in pointer to teh play thread
		PlayManager* playManager = (PlayManager*)pParam;

		return playManager->strikingDataThread();                                                	

#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in PlayManager::StrikingDataThread()") ;
		ASSERT(FALSE);
		return 1;
	}
#endif
}

UINT PlayManager::strikingDataThread()
{
	{//provide scope for the critical section lock as doStop will delete the strikingData

		const StrikingData& strikingData = _playRequest->getStrikingData();
		CSingleLock lock(&strikingData.getCriticalSection(), TRUE);

		
		DWORD timeNow = timeGetTime();
		int wholeOffset = (int)timeNow - (int)strikingData.getBaseTimestamp();
		int delay = 0;
		_row = 0;

		fireNotifyRowPositionChanged();


		for ( POSITION pos = strikingData.GetHeadPosition (); pos != NULL; )
		{
			const StrikingRow* strikingRow = strikingData.GetNext (pos);

			POSITION rowPos = strikingRow->GetHeadPosition();
			StrikingItem* realStrikeItem = NULL;

			DWORD rowBaseTimestampWithOffset = strikingRow->getBaseTimestamp() + wholeOffset;
			int rowBaseDelay =	 rowBaseTimestampWithOffset - timeNow;


			while(rowPos) 
			{
				realStrikeItem  = (StrikingItem *) strikingRow->GetNext(rowPos);

				delay = realStrikeItem->getTimestamp() + rowBaseDelay;

				ASSERT(delay > 0);	    

				delay = max(1,delay);

				PlayBellPackage* playBellPackage = NULL;

				if (realStrikeItem->isComment())
				{
					playBellPackage = 
						new PlayBellPackage(this, realStrikeItem->getComment());
				}
				else
				{
					playBellPackage = 
						new PlayBellPackage(this, 0,realStrikeItem->getBell(), strikingRow->getStroke(), 
							0,0,_tower->isMute(realStrikeItem->getBell()),0);
				}
				if(playBellPackage)
					startBellUnevenTimer(playBellPackage, delay);
			} 

			const int thinkingdelay = 100;
			int sleepDelay = delay - thinkingdelay;
			Sleep(min(max(sleepDelay,1),10000));	 //give ourselves half a second to set up the next row.

			if (isPaused() || _waitingTowerReload)
			{
				//need to know how long we are paused for.
				DWORD startPauseTime = timeGetTime();

				while (isPaused())
				{
					Sleep(10);
				}

				if (_waitingTowerReload)
				{
					//make sure we are finished
					Sleep( thinkingdelay * 3);
					CSingleLock lock(&_waitingForRestartCriticalSection, TRUE);
					if (_waitingTowerReload) //check again in case it has changes while getting lock
					{
			 			bool ok = true;

						//take the action
						if (_waitingTowerReload)
							if (! loadTower(_waitingTowerReload))
								ok = false;

						_waitingTowerReload = NULL;

						if (ok)
							ok = doTowerChecks();

						if (!ok)
						{
							break;
						}

					}

				}


				wholeOffset +=	(timeGetTime() - startPauseTime);
			}

			_row++;
			fireNotifyRowPositionChanged();


			if (isStopped())
			{
				//make sure we are finished
				Sleep(thinkingdelay);
				break;	   
			}
		
			timeNow = timeGetTime();
		}            
	}

	//stop the playing
	mainFrame()->PostMessage(WM_COMMAND,ID_STOP, 0); 
	return 0;    
}




void PlayManager::startInitialTimer(int delay)
{
	//check that the timers are not running.
	ASSERT(_bellExactEvent == 0);
	stopBellExactTimer();

	ASSERT(_everyHandstrokeEvent == 0);
	stopEveryHandstrokeTimer();

	ASSERT(_restartEveryHandstrokeEvent == 0);
	stopRestartEveryHandstrokeTimer();

	//start the timer
	if (delay == 0)
	{
		TRACE("\r\nStartInitialTimerProc\r\n");
		_initialEvent =  timeSetEvent(1,
					0,
					InitialTimerProc,
					(unsigned long)this,
					TIME_ONESHOT/*|TIME_KILL_SYNCHRONOUS Do Not Use - Causes timeSetEvent not ot work under Windows 2000*/);
	}
	else
	{
		TRACE("\r\nStartInitialTimerProcDelay\r\n");
		_initialEvent =  timeSetEvent(delay,
					0,
					InitialTimerProcDelay,
					(unsigned long)this,
					TIME_ONESHOT/*|TIME_KILL_SYNCHRONOUSDo Not Use - Causes timeSetEvent not ot work under Windows 2000*/);
	}
}

VOID CALLBACK PlayManager::InitialTimerProc(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
	//we only want the catch block in release

#ifndef _DEBUG
	try
	{
#endif
		//passed in pointer to teh play thread
		PlayManager* playManager = (PlayManager*)dwUser;

		playManager->initialTimerProc();


#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in PlayManager::InitialTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}

VOID CALLBACK PlayManager::InitialTimerProcDelay(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
	//we only want the catch block in release

#ifndef _DEBUG
	try
	{
#endif
		//passed in pointer to teh play thread
		PlayManager* playManager = (PlayManager*)dwUser;

		playManager->initialTimerProcDelay();


#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in PlayManager::InitialTimerProcDelay()") ;
		ASSERT(FALSE);
	}
#endif
}

void PlayManager::speak(CString str, bool async)
{
	if (!_mute)
		speechManager()->speak(str, async);
}
                    
void PlayManager::initialTimerProc()
{
	//speak the start string synchronously
	CString start = _tower->getTTSString(tttst_start);
	DWORD timestamp = timeGetTime();

	speak(start, false);

	for (int i=0;i<_listenerList.GetSize();i++)
		_listenerList.GetAt(i)->playManager_notifyStartPlayingFromSeperateThread(_numberInclCover, _stroke, start, timestamp);

	//start the handstroke heartbeat timer
	startRestartEveryHandstrokeTimer();

	_initialEvent = 0;
}

void PlayManager::initialTimerProcDelay()
{

	//we do not speak here as this is for the auto start

	for (int i=0;i<_listenerList.GetSize();i++)
		_listenerList.GetAt(i)->playManager_notifyStartPlayingFromSeperateThread(_numberInclCover, _stroke, "", 0);


	//start the handstroke heartbeat timer
	startRestartEveryHandstrokeTimer();

	_initialEvent = 0;
}

void PlayManager::startEveryHandstrokeTimer()
{
	//check that the timers are not running.
	ASSERT(_bellExactEvent == 0);
	stopBellExactTimer();

	ASSERT(_everyHandstrokeEvent == 0);
	stopEveryHandstrokeTimer();

	ASSERT(_restartEveryHandstrokeEvent == 0);
	stopRestartEveryHandstrokeTimer();


	//calculate teh delay
	UINT delay = (_numberInclCover * 2 * _tower->getSpeed());
	delay += (int)(_tower->getOpenHandstrokeGap() * (double)_tower->getSpeed());

	_everyHandstrokeEvent = timeSetEvent(delay,
				 0,
				 EveryHandstrokeTimerProc,
				 (unsigned long)this,
				 TIME_PERIODIC/*|TIME_KILL_SYNCHRONOUSDo Not Use - Causes timeSetEvent not ot work under Windows 2000*/);

	//simulate a first shot at the timer
	everyHandstrokeTimerProc();
}

void PlayManager::stopEveryHandstrokeTimer()
{
	if (_everyHandstrokeEvent != 0)
		timeKillEvent(_everyHandstrokeEvent);
	_everyHandstrokeEvent = 0;
}

VOID CALLBACK PlayManager::EveryHandstrokeTimerProc(UINT uID, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
//we only want the catch block in release

#ifndef _DEBUG
	try
	{
#endif
		//passed in pointer to teh play thread
		PlayManager* playManager = (PlayManager*)dwUser;
		ASSERT(uID == playManager->_everyHandstrokeEvent);
		if (uID == playManager->_everyHandstrokeEvent)
			playManager->everyHandstrokeTimerProc();

#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in PlayManager::EveryHandstrokeTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}

void PlayManager::everyHandstrokeTimerProc()
{
	ASSERT(_playRequest);

	if (_playRequest == NULL)
	{
		doStop(0);
		return;
	}

	startBellExactTimer(_tower->getSpeed());
}



void PlayManager::startRestartEveryHandstrokeTimer()
{
	//check that the timers are not running.
	ASSERT(_bellExactEvent == 0);
	stopBellExactTimer();

	ASSERT(_everyHandstrokeEvent == 0);
	stopEveryHandstrokeTimer();

	ASSERT(_restartEveryHandstrokeEvent == 0);
	stopRestartEveryHandstrokeTimer();


	//calculate the delay to complete the current pait of rows
	UINT delay = (_numberInclCover - _place) * _tower->getSpeed();

	delay += (int)(_tower->getOpenHandstrokeGap() * (double)_tower->getSpeed());

	if (_stroke == hand)
		delay += _numberInclCover * _tower->getSpeed();

	//start the timer
	_restartEveryHandstrokeEvent = timeSetEvent(delay,
				 0,
				 RestartEveryHandstrokeTimerProc,
				 (unsigned long)this,
				 TIME_ONESHOT/*|TIME_KILL_SYNCHRONOUSDo Not Use - Causes timeSetEvent not ot work under Windows 2000*/);

	//simulate a first shot at the timer
	everyHandstrokeTimerProc();
}

void PlayManager::stopRestartEveryHandstrokeTimer()
{
	if (_restartEveryHandstrokeEvent != 0)
		timeKillEvent(_restartEveryHandstrokeEvent);
	_restartEveryHandstrokeEvent = 0;
}

VOID CALLBACK PlayManager::RestartEveryHandstrokeTimerProc(UINT uID, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
//we only want the catch block in release

#ifndef _DEBUG
	try
	{
#endif
		//passed in pointer to teh play thread
		PlayManager* playManager = (PlayManager*)dwUser;
		ASSERT(uID == playManager->_restartEveryHandstrokeEvent);
		if (uID == playManager->_restartEveryHandstrokeEvent)
			playManager->restartEveryHandstrokeTimerProc();

#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in PlayManager::RestartEveryHandstrokeTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}

void PlayManager::restartEveryHandstrokeTimerProc()
{
	ASSERT(_playRequest);

	if (_playRequest == NULL)
	{
		doStop(0);
		return;
	}

	_restartEveryHandstrokeEvent = 0;
	startEveryHandstrokeTimer();
}


void PlayManager::startBellExactTimer(UINT delay)
{
	//now start the exact bell timer.
//	ASSERT(_bellExactEvent == 0);
	if (_bellExactEvent != 0)
		TRACE("[PlayManager::startBellExactTimer] _bellExactEvent still running\r\n");

	stopBellExactTimer();

	//start the timer
//	TRACE("\r\nStartBellExact\r\n");
	_bellExactEvent = timeSetEvent(delay,
				 0,
				 BellExactTimerProc,
				 (unsigned long)this,
				 TIME_PERIODIC/*|TIME_KILL_SYNCHRONOUSDo Not Use - Causes timeSetEvent not ot work under Windows 2000*/);

	//simulate the first shot at the timer
	bellExactTimerProc();
}

void PlayManager::stopBellExactTimer()
{
	if (_bellExactEvent != 0)
	{
		timeKillEvent(_bellExactEvent);
//		TRACE("\r\nStopBellExact\r\n");
	}
	_bellExactEvent = 0;
}

VOID CALLBACK PlayManager::BellExactTimerProc(UINT uID, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
//we only want the catch block in release

#ifndef _DEBUG
	try
	{
#endif
		//passed in pointer to teh play thread
		PlayManager* playManager = (PlayManager*)dwUser;

		ASSERT(uID == playManager->_bellExactEvent);
		if (uID == playManager->_bellExactEvent)
			playManager->bellExactTimerProc();


#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in PlayManager::EveryHandstrokeTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}


void PlayManager::bellExactTimerProc()
{
	ASSERT(_playRequest);

	if (_playRequest == NULL)
	{
		doStop(0);
		return;
	}

	if ((_waitingTowerReload) || (_waitingForLeadIncDec.GetSize() > 0))
	{
		CSingleLock lock(&_waitingForRestartCriticalSection, TRUE);
		if ((_waitingTowerReload) || (_waitingForLeadIncDec.GetSize() >0))//check again in case it has changes while getting lock
		{
			doPause();

			bool ok = true;

			//take the action
			if (_waitingTowerReload)
				if (! loadTower(_waitingTowerReload))
					ok = false;

			TRACE("Before Row %d, Place %d, Stroke %d\r\n", _row, _place, _stroke);

			for (int i=0;i<_waitingForLeadIncDec.GetSize();i++)
			{
				WaitingForLeadIncDec type = _waitingForLeadIncDec.GetAt(i);

				if (type == wflid_inc)
				{
					int size = (_playRequest->GetAt(_row)->_leadLength) * _numberInclCover;
					TRACE("Inc Size %d\r\n", size);
					for (int i=0;i<size;i++)
					{
						if (!incrementPlace()) 
							break;
					}
				}
				else if (type == wflid_dec)
				{
					int size = (_playRequest->GetAt(_row)->_leadLength) * _numberInclCover;
					TRACE("Dec Size %d\r\n", size);
					for (int i=0;i<size;i++)
					{
						if (!decrementPlace()) 
							break;
					}
				}
			}

			TRACE("After  Row %d, Place %d, Stroke %d\r\n", _row, _place, _stroke);

			//reset the variables
			_waitingTowerReload = NULL;
			_waitingForLeadIncDec.RemoveAll();

			if (ok)
				doRestart();
			else
				doStop();

			return;// the restart imediatly calls the method, as well as startin the timer. Therefore do not play this bell.
		}
	}

	DWORD time = timeGetTime();
	if (_place == 0)
		fireNotifyRowPositionChanged(time);


	//find the bell that needs playing
	int bellToPlay = 0;
	int rowIndex = _playRequest->GetAt(_row)->getRowIndex();

	if ((_place >= _numberInclCover - _coverCount))
		bellToPlay = _place;
	else
	{
		bellToPlay =  _playRequest->GetAt(_row)->getBellInPlace(_place) - 1;
#ifdef _DEBUG
//		TRACE("%d ", _place);
//		TRACE("%c", GlobalFunctions::bellNumbersToChar(bellToPlay+1));
#endif
	}


	//package up and play
	bool mute = false;
	int delay = _tower->getDelay(bellToPlay, _stroke, mute);
	PlayBellPackage* package =
		new PlayBellPackage(this, _place, bellToPlay,_stroke, time + _tower->getSpeed(), time + delay, mute, rowIndex);
	startBellUnevenTimer(package, delay);


	//do we have a comment?
	CString comment;
	getComment(comment);
	if (!comment.IsEmpty())
		speak(comment);


	//prep the next bell
	incrementPlace();

	//call an end to this pati of rows. New timer will kick in.
	if (_place == 0 && _stroke == hand)
	{
//		TRACE("\r\nCalling stopBellExactTimer\r\n");
		stopBellExactTimer();
	}

}

void PlayManager::startBellUnevenTimer(PlayBellPackage* package, UINT delay)
{
	ASSERT(delay > 0);
	ASSERT(package != 0);
	//now start the other timers.
	timeSetEvent(delay,
				 0,
				 BellUnevenTimerProc,
				 (unsigned long)package,
				 TIME_ONESHOT/*|TIME_KILL_SYNCHRONOUSDo Not Use - Causes timeSetEvent not ot work under Windows 2000*/);
//	TRACE("\r\n%d UnevenTimer %d\r\n", timeGetTime(), delay);
}

VOID CALLBACK PlayManager::BellUnevenTimerProc(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
//we only want the catch block in release

#ifndef _DEBUG
	try
	{
#endif
		//passed in pointer to teh play thread
		PlayBellPackage* playBellPackage = (PlayBellPackage*)dwUser;
		playBellPackage->_playManager->bellUnevenTimerProc(playBellPackage);

#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in PlayManager::EveryHandstrokeTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}

void PlayManager::bellUnevenTimerProc(PlayBellPackage* playBellPackage)
{
//		TRACE("%d %d BellUnevenTimerProc %d %d\r\n ", timeGetTime(), uID, playBellPackage->_bell, playBellPackage->_stroke);


	if (playBellPackage->_bell == -1)
	{
		//must be a comment
		speak(playBellPackage->_comment);
	}
	else
	{
		//NOTE: The isPlaying bell is an indication that the user is playing a bell, and we notify differently
		// The mute however is should look and feel like a play, just without theplay.
		bool playingBell = simulatorManager()->isPlayingBell(playBellPackage->_bell); 

		if (!playingBell ||
			playBellPackage->_mute ||
			_mute ||
			playPlayBuffer(( playBellPackage->_stroke == hand)?smt_tower_hand:smt_tower_back, playBellPackage->_bell + _offset))
		{
			
			for (int i=0;i<_listenerList.GetSize();i++)
				_listenerList.GetAt(i)->playManager_notifyBellPlayedFromSeperateThread(
					playBellPackage->_place,
					playBellPackage->_bell, playBellPackage->_stroke,
					playBellPackage->_perfectTimestamp, playingBell? playBellPackage->_actualTimestamp:0, 
					playBellPackage->_rowIndex);
		}
	}

	delete playBellPackage;

}

bool PlayManager::isPlaying() const
{
	return (_state == ps_playing);

}
bool PlayManager::isStopped() const
{
	return (_state == ps_stopped);
}

bool PlayManager::isPaused() const
{
	return (_state == ps_paused);
}

bool PlayManager::isPlayAvailable()
{
	return (isDLLAvailable() && getUsingSound());
}

BOOL PlayManager::isDLLAvailable()
{
	return (_pDS != NULL);
}

BOOL PlayManager::getUsingSound()
{
	return _usingSound;
}

void PlayManager::setUsingSound(BOOL usingSound)
{
	_usingSound = usingSound;
	towerManager_notifyUpdateTowerList();
}


bool PlayManager::loadTower(const Tower& originalTower)
{
	Tower* tower = new Tower(originalTower);
	return loadTower(tower);
}

//we take ownership of this object
bool PlayManager::loadTower(Tower* tower)
{

	CString str;
	delete _tower;
	_tower = NULL;

	if ((!tower)||(!isPlayAvailable()))
	{
		TRACE("[PlayManager] Tower Null or Direct Sound Null");
		ASSERT(FALSE);
		delete tower;
		return false;
	}

	for (int i=0;i<tower->getNumber();i++)
	{
		if (!updatePlayBuffer(tower->getTowerBell(i, hand), smt_tower_hand, i))
		{
			str.Format("There is a problem with loading your tower (%s)\n Please check Tools > Options > Simulation > Tower. \n This is usually caused by the sound files being moved. \n The problem is with '%s'", tower->getName(), tower->getTowerBell(i, hand));
			AfxMessageBox(str);
			unLoadTower();
			delete tower;
			return false;
		}
		if (!updatePlayBuffer(tower->getTowerBell(i, back), smt_tower_back, i))
		{
			str.Format("There is a problem with loading your tower (%s)\n Please check Tools > Options > Simulation > Tower. \n This is usually caused by the sound files being moved. \n The problem is with '%s'", tower->getName(), tower->getTowerBell(i, back));
			AfxMessageBox(str);
			unLoadTower();
			delete tower;
			return false;
		}
	}

	_tower = tower;
	return true;
}

void PlayManager::unLoadTower()
{
	ASSERT(_state != ps_playing);

	removeAllPendingRows();

	for (int i=0;i<MAXBELLS;i++)
	{
		removePlayBuffer(smt_tower_hand, i);
		removePlayBuffer(smt_tower_back, i);
	}

	delete _tower;
	_tower = NULL;
}


bool PlayManager::incrementPlace()
{
	//increment to the next bell
	_place++;
	if (_place >= _numberInclCover)
	{
		//add and remove the pending rows.
		reconsilePendingRows();

		// move to the next row
		if (_stroke == back)
			_stroke = hand;	  
		else
			_stroke = back;	  

		//reset the place
		_place = 0;

		//do we repeat the row, or go onto the next one.
		if (_pullsRemainingOnChange > 0)
			_pullsRemainingOnChange--;
		else
		{
			_row++;

			bool onDynamicLastRow = false;

			if (_row > _playRequest->GetUpperBound())
			{
				if (_playRequest->getAnotherRow(_stroke))
				{
					onDynamicLastRow = true;
				}
				else
				{
					doStop(0);
					return false;//must return here, else we will access null data;
				}
			}

			//set up the next pulls per change
			//is the next row the last?
			if ((!_doneLastRow)&&
				(_row >= _playRequest->GetUpperBound())&&
				(!onDynamicLastRow))
			{
				_doneLastRow = true;
				TRACE("Setting Last Row repeat");
				_pullsRemainingOnChange = getLastRowRepeat();
			}
			else
				_pullsRemainingOnChange = _playRequest->getPullsPerChange();
		}

		if ((_pullsRemainingOnChange < 1)&&(_doneLastRow))
			speak(_tower->getTTSString(tttst_stand));
	}		

	return true;
}

//only used for call changes
void PlayManager::reconsilePendingRows()
{
	//do the pending rows.
	if (_playRequest && !_pendingRows.IsEmpty())
	{
		while (!_pendingRows.IsEmpty())
		{
			RowExtra* rowExtra = _pendingRows.RemoveTail();

			if (rowExtra == NULL)
			{
				//remove row
				if (_playRequest->GetSize() > 0)
				{
					TRACE("(Removed Row%s) ", _playRequest->GetAt(_playRequest->GetUpperBound())->getAsText());
					delete _playRequest->GetAt(_playRequest->GetUpperBound());
					_playRequest->RemoveAt(_playRequest->GetUpperBound());
				}
			}
			else
			{
				//add a row to the last lead
				RowExtra* lastRow = _playRequest->GetAt(_playRequest->GetUpperBound());
				if (lastRow)
				{
					//transfer the comments to the last but one row.
					for (int i=0;i<rowExtra->GetSize();i++)
						lastRow->Add(rowExtra->GetAt(i));
					rowExtra->RemoveAll();
				}
				_playRequest->Add(rowExtra);

				TRACE("(Added Row %s) ", rowExtra->getAsText());

				//if we were on the infinite repeat on what was the last row, remove it.
				if (_pullsRemainingOnChange > 1000) //arbatory high number
				{
					_pullsRemainingOnChange = _playRequest->getPullsPerChange();
					_doneLastRow = false;
				}  
			}
		}

		// cope with removed rows
		while (_row > _playRequest->GetUpperBound() )
		{
			_row--;
			TRACE("(Decrement row)\r\n");
		}

		//if we are on the last row, then set up the last row again.
		if ((!_doneLastRow)&&
			(_row >= _playRequest->GetUpperBound()))
		{
			//as we are doing a decrement, we must be on the last row,
			_pullsRemainingOnChange = getLastRowRepeat();
			_doneLastRow = true;
		}
	}	  
}

int PlayManager::getLastRowRepeat()
{
	if (_playRequest)
	{
		if (_playRequest->getLastRowIndefiniteRepeat())
			return INT_MAX;

		if (_tower)
			return _playRequest->getEndChangeRepeat(_tower);
	}

	return 2;//default in Tower
}

bool PlayManager::decrementPlace()
{
	//increment to the next bell
	_place--;
	if (_place < 0)
	{
		if (_stroke == hand)
			_stroke = back;
		else
			_stroke = hand;

		_place = _numberInclCover - 1;

		TRACE("\n");

		_row--;

		if (_row < 0)
		{
			_place = 0;
			_row = 0;
		}
	}
			 
	return true;
}



void PlayManager::setPullsPerChange(int pullsPerChange)
{
	if(_playRequest)
		_playRequest->setPullsPerChange(pullsPerChange);
}

const CString silence = "<SILENCE MSEC = \"400\"/>";

void PlayManager::getComment(CString& comment)
{
	if (_playRequest)
	{
		if ((_place == 0)&&(_pullsRemainingOnChange == 0))
		{
			RowExtra* row = _playRequest->GetAt(_row);

			for (int i=0;i<row->GetSize();i++)
			{
				if (!comment.IsEmpty())
					comment += silence;

			 	RowComment* rowComment = row->GetAt(i);

				extractCommentForSpeech(rowComment, comment);
			}
		}
	} 

	comment.Replace("\t", silence);
}


void PlayManager::extractCommentForSpeech(RowComment* rowComment, CString& comment)
{
	if (!extractCommentForSpeech(rowComment->getStr2(), 
							rowComment->getRowCommentType(), 
							comment))
	{
		//use the standard display strings
		comment +=  rowComment->getDisplayString();
	}
}

bool  PlayManager::extractCommentForSpeech(CString str2, RowCommentType rowCommentType, CString& comment)
{
	
	switch (rowCommentType)
	{
	case rct_call:
		{
			if (_tower)
				comment+= _tower->getTTSString(tttst_call, str2);
			else
				comment+= str2;
		}
		return true;
	case rct_splice:
		{
			if (_tower)
				comment+=  _tower->getTTSString(tttst_splice, str2);
			else
				comment+= str2;
		}
		return true;
	case rct_part:
		{
			if (_tower)
				comment+= _tower->getTTSString(tttst_part, str2);
			else
				comment+= str2;
		}
		return true;
	case rct_go:
		{
			if (_tower)
				comment+= _tower->getTTSString(tttst_go, str2);
			else
				comment+= str2;
		}
		return true;
	case rct_thatsAll:
		{
			if (_tower)
				comment+= _tower->getTTSString(tttst_thatsAll, "");
			else
				comment+= str2;
		}
		return true;
	case rct_none:
		//do nothing
		return true;
	default:
		return false;
		break;
	}
}

BOOL PlayManager::playTowerBell(TowerBell towerBell)
{
	if (updatePlayBuffer(towerBell, smt_temp))
		return playPlayBuffer(smt_temp);

	return FALSE;
}

void PlayManager::playDefaultBell()
{
	if (!(playPlayBuffer(smt_tower_hand,  _offset)))//play bell 0
		Beep(100,100);
}

BOOL PlayManager::playBell(int bellNumber, Stroke stroke)   
{
	TRACE("[PlayManager::playBell] %d\t\n", bellNumber);
	if (!_tower->isMute(bellNumber) && !_mute)
		return (playPlayBuffer((stroke == hand)?smt_tower_hand:smt_tower_back, bellNumber + _offset));
	return FALSE;
}


BOOL PlayManager::playPlayBuffer(SoundMapType type, int index)
{
	PlayBuffer* playBuffer = getPlayBuffer(type, index);
	if (playBuffer)
		return playBuffer->playSound(0);

	return FALSE;
}

BOOL PlayManager::updatePlayBuffer(TowerBell towerBell, SoundMapType type, int index)
{
	if (!_pDS)
	{
		ASSERT(FALSE);
		TRACE("[PlayManager] Audio not initialized\n");
		return FALSE;
	}

	if (towerBell.getFile().IsEmpty())
	{
		ASSERT(FALSE);
		TRACE("[PlayManager] File not Valid\n");
		return FALSE;
	}

	//attempt to get the existing play buffer.
	PlayBuffer* playBuffer = getPlayBuffer(type, index);

	//attempt to update
	if ((playBuffer)&&(playBuffer->update(towerBell)))
			return TRUE;

	//*** complete reload ***

	TRACE("PlayManager reloading" + towerBell.getFile() + "\r\n");

	//remove the old one if it exists
	removePlayBuffer(type, index);

	// Try creating the new buffer
	PlayBuffer* newBuffer = new PlayBuffer(towerBell, _pDS);

 	ASSERT(getPlayBuffer(type, index) == NULL);

	// If succesfull add to playlist
	if ((newBuffer)&&(newBuffer->isValid()))
	{
		int key = getSoundMapIndex(type, index);
		_playBufferMap.SetAt(key, newBuffer);
		return TRUE;
	}
	else
	{
		delete(newBuffer);
		return FALSE;
	}
}


BOOL PlayManager::removePlayBuffer(SoundMapType type, int index)
{
	int key = getSoundMapIndex(type, index); //tdo use compound key
	PlayBuffer* playBuffer = NULL;

	if (_playBufferMap.Lookup(key, playBuffer))
	{
		VERIFY(playBuffer->stopSound());
		delete playBuffer;
		return _playBufferMap.RemoveKey(key);
	}
	return FALSE;
}

PlayBuffer* PlayManager::getPlayBuffer(SoundMapType type, int index)
{
	int key = getSoundMapIndex(type, index);
	PlayBuffer* playBuffer = NULL;

	if (_playBufferMap.Lookup(key, playBuffer))
		return playBuffer;

	return NULL;
}

int PlayManager::getSoundMapIndex(SoundMapType type, int index)
{
	switch (type)
	{
	case smt_tower_hand:
		return 1000 + index;
	case smt_tower_back:
		return 2000 + index;
	case smt_temp:
		return 3000 + index;
	default:
		ASSERT(FALSE);
		return -1;
	}
}

void PlayManager::release()
{
	// Stop all ps_playing buffers
	POSITION pos = _playBufferMap.GetStartPosition();
	int key = 0;
	PlayBuffer* playBuffer = NULL;

	while (pos)
	{
		_playBufferMap.GetNextAssoc(pos, key, playBuffer);
		if (playBuffer)
		{
			VERIFY(playBuffer->stopSound());
			delete playBuffer;
			VERIFY(_playBufferMap.RemoveKey(key));
		}
	}

	if(_pDS)
		_pDS->Release();
	_pDS = NULL;
}

void PlayManager::test(bool allowStopping)
{
	Tower* tower = towerManager()->getTower();

	if (tower)
	{

		if ((allowStopping)&&
			(isPlaying())&&
			(AfxMessageBox("Do you want to stop the existing method ps_playing?", MB_YESNO) == IDNO))
			return;

		if (allowStopping)
			doStop();

		if (isStopped())
		{

			MethodCallChange methodCallChange(tower->getNumber());

/*			//add rounds
			Lead* lead = methodCallChange.GetAt(0);
			if (lead)
			{
				for (int i=0;i<4;i++)
				{
					Row* newRow = new Row(tower->getNumber(), GlobalFunctions::isEven(i)?back:hand);
					newRow->setRounds();
					newRow->setRowIndex(i);
					lead->Add(newRow);
				}
			}

			play(new PlayRequest(methodCallChange));		*/
		}
	}
}

bool PlayManager::isMute()
{
	return (_mute == TRUE);
}


int PlayManager::getNumberInclCover()
{
	return _numberInclCover;
}
