// KeyboardManager.h: interface for the KeyboardManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDMANAGER_H__7534DB5E_0935_49F3_B0EB_76C178EE6824__INCLUDED_)
#define AFX_KEYBOARDMANAGER_H__7534DB5E_0935_49F3_B0EB_76C178EE6824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define SAMPLE_BUFFER_SIZE 8  // arbitrary number of buffer elements

#include "KeyboardManagerEventListener.h"
#include "KeyboardItem.h"
#include "KeyboardResponse.h"
#include "ManagerBase.h"

class KeyboardTimerPackage
{
public:

	KeyboardTimerPackage(KeyboardManagerEventListener* listener, 
							const KeyboardResponse& response) :
	_listener(listener), 
	_response(response)
	{

	}

	KeyboardManagerEventListener* _listener;
	KeyboardResponse _response;
};

class KeyboardManager : public ManagerBase
{
public:
	void test(CWnd* wnd);
	KeyboardItem captureKey(CWnd* wnd, const KeyboardItem& item);
	int updateKeyMap();
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnTimer();
	int getInterval();
	void setInterval(int interval);
	void setUsingKeyboard(BOOL usingKeyboard);
	void initKeyboard();
	BOOL getUsingKeyboard();
	BOOL isDLLAvailable();
	bool isKeyboardAvailable();
	void save();
	void load();
	static bool _DisableWindowNotification;
	KeyboardManager();
	virtual ~KeyboardManager();
	
	static char scan2char(DWORD scancode);
	static DWORD char2scan(char ch);
	static CString scan2String(DWORD scancode);

	void removeEventListener(KeyboardManagerEventListener* keyboardManagerEventListener);
	void addEventListener(KeyboardManagerEventListener* keyboardManagerEventListener);

	static const DWORD _NoKey;

protected:

	void fireKeyPressFromSeperateThread(DWORD scancode, bool state, DWORD timestamp);

	void stopTimer();
	void startTimer();
	BOOL isOnKeyList(DWORD scancode);

	static VOID CALLBACK KeyboardPollTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	static VOID CALLBACK KeyboardTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

	void release();
	void update();

	BOOL _usingKeyboard;
	int _interval; //milliseconds
	UINT _idEvent;

	KeyboardManagerEventListeners _listenerList;
	
	LPDIRECTINPUT8       _pDI       ; // The DirectInput object         
	LPDIRECTINPUTDEVICE8 _pKeyboard ; // The keyboard device 

};

#endif // !defined(AFX_KEYBOARDMANAGER_H__7534DB5E_0935_49F3_B0EB_76C178EE6824__INCLUDED_)
