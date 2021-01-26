// KeyboardCaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "KeyboardCaptureDlg.h"
#include "keyboardmanager.h"
#include "winpos.h"
#include "usermessages.h"
#include "PlayManager.h"



// KeyboardCaptureDlg dialog

IMPLEMENT_DYNAMIC(KeyboardCaptureDlg, CDialog)
KeyboardCaptureDlg::KeyboardCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KeyboardCaptureDlg::IDD, pParent)
{
}

KeyboardCaptureDlg::~KeyboardCaptureDlg()
{
}

void KeyboardCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KeyboardCaptureDlg, CDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(KEYBOARD_INTERFACE_EVENT, OnKeyboardInterfaceEvent)
END_MESSAGE_MAP()


// KeyboardCaptureDlg message handlers

BOOL KeyboardCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WinPos::Load(this, "KeyboardCaptureDlg", false);

	keyboardManager()->addEventListener(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void KeyboardCaptureDlg::OnDestroy()
{
	CDialog::OnDestroy();

	keyboardManager()->removeEventListener(this);

	WinPos::Save(this, "KeyboardTestDlg");
}

void KeyboardCaptureDlg::keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, KEYBOARD_INTERFACE_EVENT,(WPARAM)(&response), timestamp); 
}

LRESULT KeyboardCaptureDlg::OnKeyboardInterfaceEvent(WPARAM wParam, LPARAM /*lParam*/)	
{
	KeyboardResponse* response = (KeyboardResponse*)wParam;
	
	_response = *response;

	playManager()->playDefaultBell();

	EndDialog(IDOK);

	return 0;
}

void KeyboardCaptureDlg::keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) 
{												   
	//do not attach escape key
	for (int i=DIK_1;i<=DIK_MEDIASELECT;i++)
		keyboardActionSimulatorEventDataMap.SetAt(KeyboardAction(i),new SimulatorEventData(esigt_both));
}