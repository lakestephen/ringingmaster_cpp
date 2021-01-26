#pragma once

#include "KeyboardManagerEventListener.h"
#include "KeyboardResponse.h"

// KeyboardCaptureDlg dialog

class KeyboardCaptureDlg : public CDialog, KeyboardManagerEventListener
{
	DECLARE_DYNAMIC(KeyboardCaptureDlg)

public:
	KeyboardCaptureDlg(CWnd* pParent = NULL);   // standard constructor
	
	void keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp);
	void keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) ;

	virtual ~KeyboardCaptureDlg();

// Dialog Data
	enum { IDD = IDD_KEYBOARD_CAPTURE };
								    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	KeyboardResponse _response;
	afx_msg LRESULT OnKeyboardInterfaceEvent(WPARAM wParam, LPARAM lParam);

};
