// SpeechManager.h: interface for the SpeechManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "SpeechVoice.h"
#include "SpeechItem.h"
#include "SpeechManagerEventListener.h"
#include "SpeechResponse.h"
#include "ManagerBase.h"

class SpeechTimerPackage
{
public:

	SpeechTimerPackage(SpeechManagerEventListener* listener, 
							const SpeechResponse& response, 
							DWORD timestamp) :
	_listener(listener), 
	_response(response), 
	_timestamp(timestamp)
	{

	}

	SpeechManagerEventListener* _listener;
	SpeechResponse _response;
	DWORD _timestamp;
};


class SpeechManager : public ISpNotifyCallback, public ManagerBase
{
public:
	SpeechManager();
	virtual ~SpeechManager();


public:

	void removeEventListener(SpeechManagerEventListener* speechManagerEventListener);
	void addEventListener(SpeechManagerEventListener* speechManagerEventListener);

	void testSpeechReco(CWnd* pWnd);
	SpeechVoices& getSpeechVoices();

	void setTTSVolume(int vol);
	int getTTSVolume();

	void setTTSSpeed(int speed);
	int getTTSSpeed();

	SpeechItem captureSpeech(CWnd* pWnd, const SpeechItem& item);

	void setTTSVoiceID(CString voiceID);
	CString getTTSVoiceID();
	CString getTTSVoiceName();

	
	void speak(CString msg, bool async = true);
	BOOL isUISupported(const WCHAR* uiType, BOOL reco);

	void setUsingSpeechRecognition(BOOL usingSpeechRecognition);
	BOOL getUsingSpeechRecognition();
	BOOL isSpeechRecognitionAvailable();
	BOOL isSpeechRecognitionDLLAvailable();
	bool updateWordList();

	BOOL isTTSAvailable();			 
	BOOL isTTSDLLAvailable();			 
	BOOL getUsingTTS();
	void setUsingTTS(BOOL usingTTS);

	void displayUI(const WCHAR* uiType, BOOL reco);
	void save();
	void load();


protected:
	
	void fireSpeechRecognisedFromSeperateThread(const CString word, DWORD timestamp);
	static VOID CALLBACK SpeechManagerTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

	HRESULT STDMETHODCALLTYPE NotifyCallback(WPARAM wParam, LPARAM lParam);
	
	void enumerateSpeechVoices();
	void deleteSpeechVoices();	


	bool initTTS();
	bool initSpeechRecog();
	bool disableSpeechRecog();

	BOOL _usingSpeechRecognition;
	BOOL _notifyedFailure;
	BOOL _usingTTS;
	int _TTSVolume;
	int _TTSSpeed;
	CString _TTSVoiceID;

	SpeechVoices _speechVoices;

	virtual HRESULT recognized(CSpEvent& spEvent);


	CComPtr<ISpVoice>   _spVoice;

    CComPtr<ISpRecoContext> _spRecoContext;
	CComPtr<ISpRecognizer> _spRecognizer;
	CComPtr<ISpRecoGrammar> _spGrammar;
	CComPtr<ISpAudio> _spAudio;


	SpeechManagerEventListeners _listenerList;


};

