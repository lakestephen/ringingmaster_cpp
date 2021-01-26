// PlayManager.h: interface for the PlayManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYMANAGER_H__7B0EDFC9_32E1_47D3_828F_C10E84BE5E86__INCLUDED_)
#define AFX_PLAYMANAGER_H__7B0EDFC9_32E1_47D3_828F_C10E84BE5E86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TowerManagerEventListener.h"
#include "PlayManagerEventListener.h"
#include "PlayRequest.h"
#include "PlayBuffer.h"
#include "TowerBell.h"
#include "Lead.h"
#include "ManagerBase.h"


enum PlayingState
{
	ps_playing,
	ps_paused,
	ps_stopped
};

enum SoundMapType
{
	smt_tower_hand,
	smt_tower_back,
	smt_temp,
};

enum WaitingForLeadIncDec
{
	wflid_inc,
	wflid_dec,
};

class RowPositionChangedPackage
{
public:
	int _playRequestId;
	RowPosition _rowPosition;
	BOOL _tracking;
    DWORD _timestamp;
	Stroke _stroke;
	int _speed;
	Row* _row;
};

class PlayBellPackage
{
public:	
	PlayBellPackage(PlayManager* playManager, const CString& comment) :
	_playManager(playManager), 
	_place(0), 
	_bell(-1), 
	_stroke(hand), 
	_perfectTimestamp(0),
	_actualTimestamp(0), 
	_mute(false), 
	_rowIndex(0), 
	_comment(comment)
	{

	}

	PlayBellPackage(PlayManager* playManager, int place, int bell, Stroke stroke,	
						DWORD perfectTimestamp, DWORD actualTimestamp, bool mute, int rowIndex) :
	_playManager(playManager), 
	_place(place), 
	_bell(bell), 
	_stroke(stroke), 
	_perfectTimestamp(perfectTimestamp),
	_actualTimestamp(actualTimestamp), 
	_mute(mute), 
	_rowIndex(rowIndex)
	{

	}
       
	PlayManager* _playManager;
	int _place;
	int _bell;
	CString _comment;
	Stroke _stroke;
	DWORD _perfectTimestamp;
	DWORD _actualTimestamp;
	bool _mute;
	int _rowIndex;
};

class PlayManager : public CCmdTarget, public ManagerBase, TowerManagerEventListener
{


public:
	
	bool isMute();
	int getNumberInclCover();

	bool doTowerChecksLite(int number);
	void extractCommentForSpeech(RowComment* rowComment, CString& comment);
	bool extractCommentForSpeech(CString str2, RowCommentType rowCommentType, CString& comment);

	PlayRequest* getPlayRequest();

	void test(bool allowStopping);
	bool isPlayAvailable();
	BOOL isDLLAvailable();

	BOOL getUsingSound();
	void setUsingSound(BOOL usingSound);

	void release();
	BOOL playBell(int bellNumber, Stroke stroke);
	void playDefaultBell();

	BOOL isTracking();
	void toggleTracking();

	void addEventListener(PlayManagerEventListener* playManagerEventListener);
	void removeEventListener(PlayManagerEventListener* playManagerEventListener);

	void addRow(RowExtra* rowExtra, int methodId);
	void removeRow(int methodRequestID);
	void save();
	void load();
	void openUp();
	void closeDown();

	bool isPlaying() const;
	bool isStopped() const;
	bool isPaused() const; 


	PlayManager();
	virtual ~PlayManager();

	void towerManager_notifyUpdateTowerList() ;

	bool shouldShowPlayInfo();
	void setPullsPerChange(int pullsPerChange);
	void play(PlayRequest* request, int delay = 0);
	BOOL playTowerBell(TowerBell towerBell);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PlayManager)
	public:
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(PlayManager)
	afx_msg void OnUpdatePauseMethod(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStopMethod(CCmdUI* pCmdUI);	
	afx_msg void OnUpdateDecLead(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIncLead(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSpeedUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSlowDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOverallFrequencyUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOverallFrequencyDown(CCmdUI* pCmdUI);
	afx_msg void OnPlayStop();
	afx_msg void OnPlayPause();
	afx_msg void OnIncLead();
	afx_msg void OnDecLead();
	afx_msg void OnSlowDown();
	afx_msg void OnSpeedUp();
	afx_msg void OnOverallFrequencyUp();
	afx_msg void OnOverallFrequencyDown();
	//}}AFX_MSG
	
	afx_msg void OnPlayPlay();

	afx_msg void OnUpdatePlayPlay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCcPlayLive(CCmdUI* pCmdUI, int number, BOOL enoughRows); 

	DECLARE_MESSAGE_MAP()

protected:
	PlayManagerEventListeners _listenerList;
	BOOL _tracking;




// Implementation
protected:	  

	void speak(CString str, bool async = true );

	Method* getActiveMethod();

	BOOL _usingSound;
	BOOL _mute;
	void initSound();
	PlayBuffer* getPlayBuffer(SoundMapType type, int index = 0);
	BOOL removePlayBuffer(SoundMapType type, int index = 0);
	BOOL playPlayBuffer(SoundMapType type, int index = 0);
	BOOL updatePlayBuffer(TowerBell towerBell, SoundMapType type, int index = 0);
	int getSoundMapIndex(SoundMapType type, int index = 0);

	void getComment(CString& comment);

	void doPause();
	void doRestart();
	bool doPlay(PlayRequest* request, int delay);
	void doStop(int delay = 500);

	bool doTowerChecks();

	void reconsilePendingRows();

	void fireNotifyRowPositionChanged(DWORD timestamp = 0);
	bool incrementPlace();
	bool decrementPlace();

	int getLastRowRepeat();

	void stopTimers();

	void removeAllPendingRows();
	RowExtraList _pendingRows;
	
	static UINT StrikingDataThread(LPVOID pParam);
	UINT strikingDataThread();

	void startInitialTimer(int delay);
	static VOID CALLBACK InitialTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	static VOID CALLBACK InitialTimerProcDelay(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void initialTimerProc();
	void initialTimerProcDelay();
	UINT _initialEvent;

	void startEveryHandstrokeTimer();
	void stopEveryHandstrokeTimer();
	static VOID CALLBACK EveryHandstrokeTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void everyHandstrokeTimerProc();
	UINT _everyHandstrokeEvent;

	void startRestartEveryHandstrokeTimer();
	void stopRestartEveryHandstrokeTimer();
	static VOID CALLBACK RestartEveryHandstrokeTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void restartEveryHandstrokeTimerProc();
	UINT _restartEveryHandstrokeEvent;


	void startBellExactTimer(UINT delay);
	void stopBellExactTimer();	
	static VOID CALLBACK BellExactTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void bellExactTimerProc();
	UINT _bellExactEvent;
	
	void startBellUnevenTimer(PlayBellPackage* package, UINT delay);
	static VOID CALLBACK BellUnevenTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void bellUnevenTimerProc(PlayBellPackage* playBellPackage);

	PlayingState _state;

	PlayRequest* _playRequest;

	int _pullsRemainingOnChange;

	
	LPDIRECTSOUND	_pDS;			// DirectSound Object
	PlayBufferMap _playBufferMap;

	Tower* _tower; 
	
	//waiting for restart stuff
	CCriticalSection _waitingForRestartCriticalSection;
	Tower* _waitingTowerReload;

	CArray<WaitingForLeadIncDec, WaitingForLeadIncDec> _waitingForLeadIncDec;

	void unLoadTower();
	bool loadTower(const Tower& originalTower);
	bool loadTower(Tower* tower);

	int _row;
	int _place;


	int _numberInclCover;
	int _coverCount;
	int _offset;

	Stroke _stroke;//can we get this from row?

	bool _doneLastRow;
public:
	afx_msg void OnPlayMute();
	afx_msg void OnUpdatePlayMute(CCmdUI *pCmdUI);
};

#endif // !defined(AFX_PLAYMANAGER_H__7B0EDFC9_32E1_47D3_828F_C10E84BE5E86__INCLUDED_)
