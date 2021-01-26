// SpeechManager.cpp: implementation of the SpeechManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SpeechManager.h"
#include "UserMessages.h"
#include "GenericEditDlg.h"
#include "KeyboardManager.h"
#include "SpeechChooseWordDlg.h"
#include "SplashScreenEx.h"

#include "SpeechRecoTestDlg.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SpeechManager::SpeechManager()   :
_notifyedFailure(FALSE)
{
	
}
 
SpeechManager::~SpeechManager()
{
	deleteSpeechVoices();
	_spVoice.Detach();	
	_spRecoContext.Detach();	
}


void SpeechManager::addEventListener(SpeechManagerEventListener* speechManagerEventListener)
{
	ASSERT(speechManagerEventListener != NULL);

	_listenerList.Add(speechManagerEventListener);

	updateWordList();
}


void SpeechManager::removeEventListener(SpeechManagerEventListener* speechManagerEventListener)
{
	ASSERT(speechManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == speechManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	updateWordList();

	ASSERT(hasRemoved == 1);
}




HRESULT STDMETHODCALLTYPE SpeechManager::NotifyCallback(WPARAM /*wParam*/, LPARAM /*lParam*/) 
{
	CSpEvent spEvent;	 
	while(spEvent.GetFrom(_spRecoContext)==S_OK) 
	{
		switch(spEvent.eEventId) 
		{
		case SPEI_RECOGNITION:
			recognized(spEvent);
			break;
		}
	}  
		 
	return S_OK;
}

HRESULT SpeechManager::recognized(CSpEvent& spEvent) 
{
	USES_CONVERSION;
	HRESULT result;
	
	CSpDynamicString text;
	if(FAILED(result=spEvent.RecoResult()->GetText((ULONG)SP_GETWHOLEPHRASE, (ULONG)SP_GETWHOLEPHRASE, TRUE, &text, NULL))) {
		return result;
	}

	SPRECORESULTTIMES times;
	if(FAILED(result=spEvent.RecoResult()->GetResultTimes(&times))) {
		return result;
	}        
	
	CString strWord(W2T(text));
	TRACE("detected " +  strWord + "\r\n");

	//fire the update listener
	fireSpeechRecognisedFromSeperateThread(strWord, times.dwTickCount);

	return S_OK;
}	

void SpeechManager::fireSpeechRecognisedFromSeperateThread(const CString word, DWORD timestamp)
{
	SimulatorEventData* data;
	SpeechAction action(word);
	SpeechResponse response(action);

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		SpeechManagerEventListener* listener = _listenerList.GetAt(i);
		
		if (listener->_speechEvents.Lookup(action, data))
		{
			//calculate if this is valid
			response.setValid(data->eventTrigger());
			
			if (data->getDelay() > 0)
			{
				//package up all the information that it needed by the timer proc
				//NOTE: we are using the timestamp, as it will be the start 
				// of the recognition rather than now.
				SpeechTimerPackage* speechTimerPackage = 
					new SpeechTimerPackage(listener, response, timestamp + data->getDelay());
				//start the timer
				timeSetEvent(data->getDelay(), 
							0, 
							SpeechManagerTimerProc, 
							(unsigned long)speechTimerPackage,  
							TIME_ONESHOT);	 
			}
			else
			{
				//NOTE: we are using the timestamp, as it will be the start 
				// of the recognition rather than now.
				listener->speechManager_notifySpeechRecognisedFromSeperateThread(response, timestamp); 
			}
		}
	}
}

VOID CALLBACK SpeechManager::SpeechManagerTimerProc(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{					
//we only want the catch block in release

#ifndef _DEBUG
	try 
	{
#endif
		SpeechTimerPackage* speechTimerPackage = (SpeechTimerPackage*)dwUser;
		
		speechTimerPackage->_listener->speechManager_notifySpeechRecognisedFromSeperateThread(
			speechTimerPackage->_response, speechTimerPackage->_timestamp); 

		delete speechTimerPackage;

#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in SpeechManager::SpeechManagerTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}




void SpeechManager::load()
{
	initTTS();

	_usingSpeechRecognition = AfxGetApp()->GetProfileInt("Speech", "Using Speech Recognition", TRUE);	
	_usingTTS = AfxGetApp()->GetProfileInt("Speech", "Using TTS", TRUE);	
	setTTSVolume(AfxGetApp()->GetProfileInt("Speech", "TTS Volume", 100));	
	setTTSSpeed(AfxGetApp()->GetProfileInt("Speech", "TTS Speed", 3));	
	setTTSVoiceID(AfxGetApp()->GetProfileString("Speech", "TTS Voice", "" ));	

	initSpeechRecog();

	enumerateSpeechVoices();
}

void SpeechManager::save()
{
	AfxGetApp()->WriteProfileInt("Speech", "Using Speech Recognition", _usingSpeechRecognition );	
	AfxGetApp()->WriteProfileInt("Speech", "Using TTS", _usingTTS);	
	AfxGetApp()->WriteProfileInt("Speech", "TTS Volume", _TTSVolume);
	AfxGetApp()->WriteProfileInt("Speech", "TTS Speed", _TTSSpeed);
	AfxGetApp()->WriteProfileString("Speech", "TTS Voice", _TTSVoiceID);
}

bool SpeechManager::initTTS()
{
	_spVoice.CoCreateInstance( CLSID_SpVoice );

	return true;
}

const ULONGLONG interest= SPFEI(SPEI_RECOGNITION);

bool SpeechManager::disableSpeechRecog()
{
	CSplashScreenEx::HideSplashScreen();

	if (_usingSpeechRecognition && !_notifyedFailure)
	{
		_notifyedFailure = true;
		if (AfxMessageBox("Speech recognition initialisation failure! \nYou need to install the speech package \"Microsoft Speech English V51.msi\". \nThis can be found on the Ringing Master CD-ROM, or on www.ringingmaster.co.uk \nDo you want to disable the speech recognition feature?", MB_YESNO) == IDYES)
		{																																																												
			_usingSpeechRecognition = FALSE;
			AfxMessageBox("Speech recongition has been disabled. Use the [File]>[Options]>[Simulator]>[Microphone] tab to reinstate.");
		}
	}

	return false;
}

bool SpeechManager::initSpeechRecog()
{
	if(FAILED(_spRecognizer.CoCreateInstance(CLSID_SpInprocRecognizer))) 
		return disableSpeechRecog();
	
	if((!_spRecognizer)||FAILED(_spRecognizer->CreateRecoContext(&_spRecoContext))) 
		return disableSpeechRecog();
	
	if((!_spRecoContext)||FAILED(_spRecoContext->SetNotifyCallbackInterface(this, 0, 0))) 
		return disableSpeechRecog();


	if((!_spRecoContext)||FAILED(_spRecoContext->SetInterest(interest, interest))) 
		return disableSpeechRecog();
	
	if(FAILED(SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &_spAudio))) 
		return disableSpeechRecog();
	
	if((!_spRecognizer)||FAILED(_spRecognizer->SetInput(_spAudio, TRUE))) 
		return disableSpeechRecog();
	
	if((!_spRecoContext)||FAILED(_spRecoContext->CreateGrammar(0, &_spGrammar)))
		return disableSpeechRecog();
	
	//load a default file from the resource. This will be overwritten, bit it needs something to go on.
	if((!_spGrammar)||FAILED(_spGrammar->LoadCmdFromResource(NULL, 
											(const WCHAR*)MAKEINTRESOURCE(IDR_GRAMMAR_MAIN),
											L"GRAMMAR", 
											MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL),
											SPLO_DYNAMIC))) 
		return disableSpeechRecog();									  	
	
	return updateWordList();
}

bool SpeechManager::updateWordList()
{
	SPSTATEHANDLE hRule; 

	//deactivate the existing rules
	if((!_spRecognizer)||FAILED(_spRecognizer->SetRecoState(SPRST_INACTIVE))) 
		return disableSpeechRecog();
	if((!_spGrammar)||FAILED(_spGrammar->SetRuleState(NULL, NULL, SPRS_INACTIVE))) 
		return disableSpeechRecog();									  	

	//get and clear the rule
	if((!_spGrammar)||FAILED(_spGrammar->GetRule(L"MAIN", NULL, SPRAF_Active, TRUE, &hRule))) 
		return disableSpeechRecog();			
	if((!_spGrammar)||FAILED(_spGrammar->ClearRule(hRule)))
		return disableSpeechRecog();
	

	//get the complete list of words
	StringMap words;
	SpeechAction key;
	SimulatorEventData* value;

	words.RemoveAll();

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		SpeechManagerEventListener* listener = _listenerList.GetAt(i);
		listener->clearEventMap();
		listener->speechManager_notifyGetSpeechList(listener->_speechEvents); 

		POSITION pos = listener->_speechEvents.GetStartPosition();
		while (pos)
		{
			listener->_speechEvents.GetNextAssoc(pos, key, value);
			words.SetAt(key.getWord(),0);
		}	                       		
 
	}

	//do we have any words?
	if (words.GetCount() > 0)
	{  
		//add each string
		CString word;
		int iDummy;
		POSITION pos = words.GetStartPosition();
		while (pos)
		{
			words.GetNextAssoc(pos, word, iDummy);

			if (!word.IsEmpty())
			{
				TRACE("Adding to speech Reco " + word + "\r\n");

				wchar_t* buff = new wchar_t[word.GetLength()+2];
														
				swprintf(buff, L"%S", word);

				if((!_spRecognizer)||FAILED(_spGrammar->AddWordTransition(hRule, NULL, buff, NULL, SPWT_LEXICAL, 1, NULL)))
				{
					AfxMessageBox("Cant Add the word " + word + " to the speech recognition list.", MB_ICONWARNING);
				}

				delete buff;
			}
		}
		
		if((!_spGrammar)||FAILED(_spGrammar->Commit(NULL)))
			return disableSpeechRecog();									  	
		
		if((!_spGrammar)||FAILED(_spGrammar->SetGrammarState(SPGS_ENABLED)))
			return disableSpeechRecog();									  	
		
		if((!_spGrammar)||FAILED(_spGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE))) 		
			return disableSpeechRecog();									  	
		
		if((!_spRecognizer)||FAILED(_spRecognizer->SetRecoState(SPRST_ACTIVE))) 
			return disableSpeechRecog();
	}

	return true;
}

BOOL SpeechManager::isSpeechRecognitionDLLAvailable()
{
	return ( _spRecoContext != NULL);
}

BOOL SpeechManager::isSpeechRecognitionAvailable()
{
	return (isSpeechRecognitionDLLAvailable() && getUsingSpeechRecognition());
}

BOOL SpeechManager::getUsingSpeechRecognition()
{
	return _usingSpeechRecognition;
}

void SpeechManager::setUsingSpeechRecognition(BOOL usingSpeechRecognition)
{
	//try and set a wordList
	if ((!usingSpeechRecognition)||(updateWordList()))
		_usingSpeechRecognition = usingSpeechRecognition;
}

BOOL SpeechManager::isTTSDLLAvailable()
{
	return (_spVoice != NULL);
}

BOOL SpeechManager::isTTSAvailable()
{
	return (isTTSDLLAvailable() && getUsingTTS());
}

BOOL SpeechManager::getUsingTTS()
{
	return _usingTTS;
}

void SpeechManager::setUsingTTS(BOOL usingTTS)
{
	_usingTTS = usingTTS;
}

void SpeechManager::displayUI(const WCHAR* uiType, BOOL reco)
{
	if (reco)
	{
		if (isSpeechRecognitionDLLAvailable())
		{
			CComPtr<ISpRecognizer> spRecognizer;
		
			_spRecoContext->GetRecognizer(&spRecognizer);

			spRecognizer->DisplayUI(mainFrame()->GetSafeHwnd(), L"Ringing Master", uiType, NULL, NULL);
		}
	}
	else
	{
		if (isTTSDLLAvailable())
		{
			_spVoice->DisplayUI(mainFrame()->GetSafeHwnd(), L"Ringing Master", uiType, NULL, NULL);
		}	
	}		
}

BOOL SpeechManager::isUISupported(const WCHAR* uiType, BOOL reco)
{
	BOOL fSupported = FALSE;

	if (reco)
	{
		if (isSpeechRecognitionDLLAvailable())
		{
			CComPtr<ISpRecognizer> spRecognizer;
		
			_spRecoContext->GetRecognizer(&spRecognizer);

			spRecognizer->IsUISupported(uiType, NULL, NULL, &fSupported);
		}
	}
	else
	{
		if (isTTSDLLAvailable())
		{
			_spVoice->IsUISupported(uiType, NULL, NULL, &fSupported);
		}	
	}		

	return fSupported;
}
			
void SpeechManager::speak(CString msg, bool async)
{
	if ((isTTSAvailable())&&(!msg.IsEmpty()))
	{

		CString finalMsg;
		finalMsg.Format("<rate absspeed=\"%d\"/>%s", _TTSSpeed, msg);

		wchar_t* buff = new wchar_t[finalMsg.GetLength()+2];
												
		swprintf(buff, L"%S", finalMsg);

		TRACE("[SpeechManager::speak]   " + finalMsg + "\r\n");

		_spVoice->Speak( buff, async?(SPF_DEFAULT | SPF_ASYNC):(SPF_DEFAULT) ,  0 );


		delete[] buff;
	}
}

int SpeechManager::getTTSSpeed()
{
	return _TTSSpeed;
}

void SpeechManager::setTTSSpeed(int speed)
{
	_TTSSpeed = speed;
}


int SpeechManager::getTTSVolume()
{
	return _TTSVolume;
}

void SpeechManager::setTTSVolume(int vol)
{
	_TTSVolume = vol;

	if (_spVoice)
		_spVoice->SetVolume((unsigned short)_TTSVolume);
}

CString SpeechManager::getTTSVoiceName()
{
	CString name = getTTSVoiceID();

	for (int i=0;i<_speechVoices.GetSize();i++)
	{
		SpeechVoice* item = _speechVoices.GetAt(i);
		if (name == item->getID())
			return item->getName();
	}

	return "computers default voice";
}


CString SpeechManager::getTTSVoiceID()
{
	return _TTSVoiceID;
}

void SpeechManager::setTTSVoiceID(CString voiceID)
{
	_TTSVoiceID = voiceID;

	if (_spVoice)
	{
		if (_TTSVoiceID.IsEmpty())
		{
			_spVoice->SetVoice(NULL);
		}
		else
		{
			HRESULT                             hr = S_OK;
			CComPtr<ISpObjectToken>             cpVoiceToken;
			CComPtr<IEnumSpObjectTokens>        cpEnum;
			ULONG                               ulCount = 0;


			//Enumerate the available voices 
			if(SUCCEEDED(hr))
				hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);

			//Get the number of voices
			if(SUCCEEDED(hr))
				hr = cpEnum->GetCount(&ulCount);

			// Obtain a list of available voice tokens, set the voice to the token, and call Speak
			while (SUCCEEDED(hr) && ulCount -- )
			{
				cpVoiceToken.Release();
				if(SUCCEEDED(hr))
					hr = cpEnum->Next( 1, &cpVoiceToken, NULL );

				WCHAR* buff = NULL;

				cpVoiceToken->GetId(&buff);
				CString id(buff);

				if((id == voiceID)&&(SUCCEEDED(hr)))
				{
					hr = _spVoice->SetVoice(cpVoiceToken);
					break;
				}
			}	
		}
	}
}


void SpeechManager::enumerateSpeechVoices()
{	  
	deleteSpeechVoices();

	HRESULT                             hr = S_OK;
	CComPtr<ISpObjectToken>             cpVoiceToken;
	CComPtr<IEnumSpObjectTokens>        cpEnum;
	ULONG                               ulCount = 0;


	//Enumerate the available voices 
	if(SUCCEEDED(hr))
		hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);

	//Get the number of voices
	if(SUCCEEDED(hr))
		hr = cpEnum->GetCount(&ulCount);

	// Obtain a list of available voice tokens, set the voice to the token, and call Speak
	while (SUCCEEDED(hr) && ulCount -- )
	{
		cpVoiceToken.Release();
		if(SUCCEEDED(hr))
			hr = cpEnum->Next( 1, &cpVoiceToken, NULL );

		WCHAR* buff = NULL;

		cpVoiceToken->GetId(&buff);
		CString id(buff);

		SpGetDescription(cpVoiceToken, &buff );
		CString name(buff);

		_speechVoices.Add(new SpeechVoice(id, name));
	}	
}



void SpeechManager::deleteSpeechVoices()
{
	for (int i=0;i<_speechVoices.GetSize();i++)
	{
		delete _speechVoices.GetAt(i);	
	}					   

	_speechVoices.RemoveAll();
}

SpeechVoices& SpeechManager::getSpeechVoices()
{
	return _speechVoices;
}



void SpeechManager::testSpeechReco(CWnd* pWnd)
{
	CStringArray str;
	str.Add("bob");
	str.Add("single");
	str.Add("go");
	str.Add("stop");
	str.Add("cambridge");
	str.Add("london");
	str.Add("plain");

	SpeechRecoTestDlg dlg(pWnd, str);
	dlg.DoModal();					 
}


SpeechItem SpeechManager::captureSpeech(CWnd* pWnd, const SpeechItem& item)
{				 
	SpeechChooseWordDlg dlg(pWnd, item);

	if (dlg.DoModal() == IDOK)
	{
		return dlg._speechItem;
	}

	return item;
}

