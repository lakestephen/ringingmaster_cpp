// KeyboardManager.cpp: implementation of the KeyboardManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "KeyboardManager.h"
#include "PlayManager.h"
#include "KeyboardTestDlg.h"
#include ".\keyboardmanager.h"
#include "KeyboardSelectDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


const DWORD KeyboardManager::_NoKey = 0;
bool KeyboardManager::_DisableWindowNotification = true;

KeyboardManager::KeyboardManager() :
_idEvent(0),
_usingKeyboard(TRUE),
_interval(5),	  //to2do have a global simulator polling frequency / accuracy for lower processor speed?? 
_pKeyboard(NULL),
_pDI(NULL)
{

}

KeyboardManager::~KeyboardManager()
{
	stopTimer();
	release();
}

void KeyboardManager::addEventListener(KeyboardManagerEventListener* keyboardManagerEventListener)
{
	ASSERT(keyboardManagerEventListener != NULL);

	_listenerList.Add(keyboardManagerEventListener);

	update();
}

void KeyboardManager::removeEventListener(KeyboardManagerEventListener* keyboardManagerEventListener)
{
	ASSERT(keyboardManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == keyboardManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	update();

	ASSERT(hasRemoved == 1);
}

void KeyboardManager::release()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( _pKeyboard ) 
        _pKeyboard->Unacquire();
    
    // Release any DirectInput objects.
    if(_pKeyboard) 
	{
		_pKeyboard->Release();
		_pKeyboard = NULL;
	}

    if(_pDI) 
	{
		_pDI->Release();
		_pDI = NULL;
	}
}

void KeyboardManager::load()
{
	_usingKeyboard = AfxGetApp()->GetProfileInt("Keyboard", "Using Keyboard", TRUE);	
	_interval = AfxGetApp()->GetProfileInt("Keyboard", "Interval", 5);	

	initKeyboard();

	update();
}

void KeyboardManager::save()
{
	AfxGetApp()->WriteProfileInt("Keyboard", "Using Keyboard", _usingKeyboard);	
	AfxGetApp()->WriteProfileInt("Keyboard", "Interval", _interval);	
}

void KeyboardManager::initKeyboard()
{
    HRESULT hr;
	// Create a DInput object
    if( FAILED(hr =  DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&_pDI, NULL ) ) )
	{
		TRACE("[KeyboardManager] DirectInput8Create failed");
		return ;
	}
    
    // Obtain an interface to the system keyboard device.
    if( FAILED(hr =  _pDI->CreateDevice( GUID_SysKeyboard, &_pKeyboard, NULL ) ) )
	{
		TRACE("[KeyboardManager] CreateDevice failed");
		return ;
	}

    // Set the data format to "keyboard format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing an array
    // of 256 bytes to IDirectInputDevice::GetDeviceState.
    if( FAILED( hr =  _pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
		TRACE("[KeyboardManager] SetDataFormat failed");
		return ;
	}

    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
    hr = _pKeyboard->SetCooperativeLevel( *AfxGetMainWnd(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );
	if(hr == DIERR_UNSUPPORTED )
    {
       	release();
		TRACE("[KeyboardManager] SetCooperativeLevel failed");
		return;
    }

    if( FAILED(hr) )
        return;

    // IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
    //
    // DirectInput uses unbuffered I/O (buffer size = 0) by default.
    // If you want to read buffered data, you need to set a nonzero
    // buffer size.
    //
    // Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
    //
    // The buffer size is a DWORD property associated with the device.
    DIPROPDWORD dipdw;

    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = SAMPLE_BUFFER_SIZE; // Arbitary buffer size

    if( FAILED( hr = _pKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
        return;

}

bool KeyboardManager::isKeyboardAvailable()
{
	return (isDLLAvailable() && getUsingKeyboard());
}

BOOL KeyboardManager::isDLLAvailable()
{
	return (_pKeyboard != NULL);
}

BOOL KeyboardManager::getUsingKeyboard()
{
	return _usingKeyboard;
}

void KeyboardManager::setUsingKeyboard(BOOL usingKeyboard)
{
	_usingKeyboard = usingKeyboard;
	update();
}

void KeyboardManager::update()
{
	stopTimer();

	if (_pKeyboard)
	{
		_pKeyboard->Unacquire();

		int keyCount = updateKeyMap();

		if (keyCount > 0)
		{
			startTimer();
			// Acquire the newly created device
			_pKeyboard->Acquire();
		}
	}
}


void KeyboardManager::startTimer()
{
	_idEvent =  timeSetEvent(_interval,
							 0,
							 KeyboardPollTimerProc, 
							 (unsigned long)this,  
							 TIME_PERIODIC/*|TIME_KILL_SYNCHRONOUSDo Not Use - Causes timeSetEvent not ot work under Windows 2000*/);

}

VOID CALLBACK KeyboardManager::KeyboardPollTimerProc(UINT uID, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
	KeyboardManager* keyboardManager = (KeyboardManager*)dwUser;

	if ((keyboardManager)&&(keyboardManager->_idEvent == uID))
		keyboardManager->OnTimer();
}

void KeyboardManager::stopTimer()
{
	if (_idEvent != 0)
	{
		timeKillEvent(_idEvent);
		_idEvent = 0;
	}
}

void KeyboardManager::setInterval(int interval)
{
	_interval = interval;
	
	update();
}

int KeyboardManager::getInterval()
{	
	return _interval;
}

void KeyboardManager::OnTimer()
{
    if(!_pKeyboard) 
        return;

    DIDEVICEOBJECTDATA didod[ SAMPLE_BUFFER_SIZE ];  // Receives buffered data 
    DWORD  dwElements = SAMPLE_BUFFER_SIZE;

   
    HRESULT hr = _pKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
    if( hr != DI_OK ) 
    {
        // We got an error or we got DI_BUFFEROVERFLOW.
        //
        // Either way, it means that continuous contact with the
        // device has been lost, either due to an external
        // interruption, or because the buffer overflowed
        // and some events were lost.
        //
        // Consequently, if a button was pressed at the time
        // the buffer overflowed or the connection was broken,
        // the corresponding "up" message might have been lost.
        //
        // But since our simple sample doesn't actually have
        // any state associated with button up or down events,
        // there is no state to reset.  (In a real game, ignoring
        // the buffer overflow would result in the game thinking
        // a key was held down when in fact it isn't; it's just
        // that the "up" event got lost because the buffer
        // overflowed.)
        //
        // If we want to be cleverer, we could do a
        // GetDeviceState() and compare the current state
        // against the state we think the device is in,
        // and process all the states that are currently
        // different from our private state.
        hr = _pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = _pKeyboard->Acquire();

        // Update the dialog text 
        if( hr == DIERR_OTHERAPPHASPRIO || 
            hr == DIERR_NOTACQUIRED ) 
		{
			//error
			ASSERT(FALSE);
		}

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return; 
    }


    // extract the letter, and the down / up state, and notify listeners.	
	for(DWORD i = 0; i < dwElements; i++ ) 
    {
		//trigger only on the key down 
		fireKeyPressFromSeperateThread(didod[ i ].dwOfs,((didod[ i ].dwData & 0x80) != 0) ,didod[ i ].dwTimeStamp); 
    }
}

void KeyboardManager::fireKeyPressFromSeperateThread(DWORD scancode, bool state, DWORD timestamp)
{
	SimulatorEventData* data;
	KeyboardAction action(scancode);
	KeyboardResponse response(action, state);

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		KeyboardManagerEventListener* listener = _listenerList.GetAt(i);
		
		if (listener->_keyEvents.Lookup(action, data))
		{
			//calculate if this is valid
			response.setValid(data->eventTrigger(state));
			
			if (data->getDelay() > 0)
			{
				//package up all the information that it needed by the timer proc
				KeyboardTimerPackage* keyboardTimerPackage = 
					new KeyboardTimerPackage(listener, response);
				//start the timer
				timeSetEvent(data->getDelay(), 
							0,
							KeyboardTimerProc, 
							(unsigned long)keyboardTimerPackage,  
							TIME_ONESHOT);	 
			}
			else
			{
				listener->keyboardManager_notifyKeyPressFromSeperateThread(response, timestamp); 
			}
		}
	}
}

VOID CALLBACK KeyboardManager::KeyboardTimerProc(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
//we only want the catch block in release

#ifndef _DEBUG
	try 
	{
#endif
		KeyboardTimerPackage* keyboardTimerPackage = (KeyboardTimerPackage*)dwUser;
		
		keyboardTimerPackage->_listener->keyboardManager_notifyKeyPressFromSeperateThread(
			keyboardTimerPackage->_response, timeGetTime()); 

		delete keyboardTimerPackage;

#ifndef _DEBUG
	}
	catch (...)
	{
		TRACE("Excption in KeyboardManager::KeyboardTimerProc()") ;
		ASSERT(FALSE);
	}
#endif
}
//this is not needed at the moment. Keep for now.
char KeyboardManager::scan2char(DWORD scancode)
{
	WORD result[2];

	static HKL layout=GetKeyboardLayout(0);
	static BYTE State[256];

	if (GetKeyboardState(State))
	{
		UINT vk=MapVirtualKeyEx(scancode,1,layout);
		if (ToAsciiEx(vk,scancode,State,result,0,layout) == 1)
		{
			CString pressedCH((char)result[0]);
			pressedCH.MakeUpper();
			if (pressedCH.GetLength() > 0)
				return pressedCH.GetAt(0);
		}
	}

	return 0;
}
	 
DWORD KeyboardManager::char2scan(char ch)
{
	//NOTE: Only works with lower case strings
	if (isalpha(ch) && isupper(ch))
	{
		CString strCh(ch);
		strCh.MakeLower();
		if (strCh.GetLength() > 0)
		{
			ch = strCh.GetAt(0);
		}
		else
		{
			ASSERT(FALSE);
			return 0;
		}
	}

	static HKL layout=GetKeyboardLayout(0);

	SHORT vk  = VkKeyScanEx(_T(ch), layout);

	return MapVirtualKeyEx(vk,0,layout);
}


BOOL KeyboardManager::isOnKeyList(DWORD scancode)
{
	//we only want the down key
	SimulatorEventData* simulatorEventData;
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		KeyboardManagerEventListener* listener = _listenerList.GetAt(i);
		if (listener->_keyEvents.Lookup(scancode, simulatorEventData))
			return true;
	} 

	return false;
}

BOOL KeyboardManager::PreTranslateMessage(MSG *pMsg)
{
	switch (pMsg->message)
	{
	case WM_CHAR:
		//if we have disabled this, then allow the window to process the message
		if (_DisableWindowNotification)
			return FALSE;
		//extract the scancode bits 16-23. Mask and bit shift
		return isOnKeyList((pMsg->lParam & 0x7F8000) >> 16);
    case WM_ACTIVATE:
        if( WA_INACTIVE != pMsg->wParam && _pKeyboard )
            // Make sure the device is acquired, if we are gaining focus.
            _pKeyboard->Acquire();
		return FALSE;
	default: 
		return FALSE;
	}	
}
					 

int KeyboardManager::updateKeyMap() //to2do get all the updates in the otehr manaagers to return an int.
{
	int count = 0;
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		KeyboardManagerEventListener* listener = _listenerList.GetAt(i);
		listener->clearEventMap();
		listener->keyboardManager_notifyGetKeyList(listener->_keyEvents); 
		count += listener->_keyEvents.GetSize();
	} 

	return count;
}

void KeyboardManager::test(CWnd* wnd)
{						  		 
	KeyboardTestDlg dlg(wnd);

	dlg.DoModal();
}

KeyboardItem KeyboardManager::captureKey(CWnd* wnd, const KeyboardItem& item)
{						  		 
	KeyboardSelectDlg dlg(wnd, item);

	if (dlg.DoModal() == IDOK)
		return dlg._keyboardItem;

	return item;
}

CString KeyboardManager::scan2String(DWORD scancode)
{
	switch (scancode)
	{
	case 0:					return "";
	case DIK_ESCAPE:		return "";//we do not allow the use of escape (0)
	case DIK_MINUS:			return "Minus";
	case DIK_EQUALS:		return "Equals";
	case DIK_LBRACKET:		return "Left Bracket";
	case DIK_RBRACKET:		return "Right Bracket";
	case DIK_SEMICOLON:		return "Semicolon";
	case DIK_APOSTROPHE:	return "Apostrophe";
	case DIK_GRAVE:			return "Grave";
	case DIK_BACKSLASH:		return "Backslash";
	case DIK_COMMA:			return "Comma";
	case DIK_PERIOD:		return "Period";
	case DIK_SLASH:			return "Slash";
	case DIK_MULTIPLY:		return "Num *";
	case DIK_NUMLOCK:		return "Num Lock";
	case DIK_RCONTROL:		return "Right Ctrl";
	case DIK_DIVIDE:		return "Num Slash";
	case DIK_SYSRQ:			return "Sys Rq";
	case DIK_RMENU:			return "Alt Gr";
	case DIK_PAUSE:			return "Break";
	case DIK_HOME:			return "Home";
	case DIK_UP:			return "Up";
	case DIK_PRIOR:			return "Page Up";
	case DIK_LEFT:			return "Left";
	case DIK_RIGHT:			return "Right";
	case DIK_END:			return "End";
	case DIK_DOWN:			return "Down";
	case DIK_NEXT:			return "Page Down";
	case DIK_INSERT:		return "Insert";
	case DIK_DELETE:		return "Delete";
	case DIK_LWIN:			return "Left Window";
	case DIK_RWIN:			return "Right Window";
	case DIK_APPS:			return "Menu";
	case DIK_MUTE:			return "Mute";
	case DIK_CALCULATOR:	return "Calculator";
	case DIK_PLAYPAUSE:		return "Play / Pause";
	case DIK_MEDIASTOP:		return "Media Stop";
	case DIK_VOLUMEDOWN:	return "Volume -";
	case DIK_VOLUMEUP:		return "Volume +";
	case DIK_WEBHOME:		return "Web home";
	case DIK_NUMPADCOMMA:	return "Num ,";
	case DIK_POWER:			return "System Power";
	case DIK_SLEEP:			return "System Sleep";
	case DIK_WAKE:			return "System Wake";
	case DIK_WEBSEARCH:		return "Web Search";
	case DIK_WEBFAVORITES:	return "Web Favorites";
	case DIK_WEBREFRESH:	return "Web Refresh";
	case DIK_WEBSTOP:		return "Web Stop";
	case DIK_WEBFORWARD:	return "Web Forward";
	case DIK_WEBBACK:		return "Web Back";
	case DIK_MYCOMPUTER:	return "My Computer";
	case DIK_MAIL:			return "Mail";
	case DIK_MEDIASELECT:	return "Media Select";
	case DIK_OEM_102:       return "<>";
	case DIK_F13:           return "F13";
	case DIK_F14:           return "F14";
	case DIK_F15:           return "F15";
	case DIK_KANA:          return "Kana";
	case DIK_ABNT_C1:       return "/?";
	case DIK_CONVERT:       return "Convert";
	case DIK_NOCONVERT:     return "No Convert";
	case DIK_YEN:           return "Yen";
	case DIK_ABNT_C2:       return "Num .";
	case DIK_NUMPADEQUALS:  return "Num =";
	case DIK_PREVTRACK:     return "Previous Track";
	case DIK_AT:            return "@";
	case DIK_COLON:         return "Colon";
	case DIK_UNDERLINE:     return "Underline";
	case DIK_KANJI:         return "Kanji";
	case DIK_STOP:          return "Stop";
	case DIK_AX:            return "Ax";
	case DIK_UNLABELED:     return "Unlabeled";
	case DIK_NEXTTRACK:     return "Next Track";
	case DIK_NUMPADENTER:   return "Num Enter";

	}

	TCHAR szBuffer [50];

	DWORD scancodeCopy = (scancode << 16);
	
	if (::GetKeyNameText (scancodeCopy, szBuffer, 50) != 0)
		return GlobalFunctions::capitalString(szBuffer, false);


	//this means that the mian ones have failed, and we are using a catch all.
	ASSERT(FALSE);

	CString str;
	str.Format("Code %d", scancode);

	return str;	  
}
