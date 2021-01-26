// KeyboardTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "KeyboardTestDlg.h"
#include "KeyboardManager.h"
#include "WinPos.h"
#include "UserMessages.h"
#include "PLayManager.h"
#include "KeyboardResponse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KeyboardTestDlg dialog
							    

KeyboardTestDlg::KeyboardTestDlg(CWnd* pParent /*=NULL*/)
: ResizeDialog(KeyboardTestDlg::IDD, pParent),
_timestampOrigin(0)
{
	//{{AFX_DATA_INIT(KeyboardTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void KeyboardTestDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KeyboardTestDlg)
	DDX_Control(pDX, IDC_LIST, _list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KeyboardTestDlg, ResizeDialog)
	//{{AFX_MSG_MAP(KeyboardTestDlg)
	ON_MESSAGE(KEYBOARD_INTERFACE_EVENT, OnKeyboardInterfaceEvent)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KeyboardTestDlg message handlers

BOOL KeyboardTestDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
	
	AddControl(IDC_LIST, resize, resize);

	WinPos::Load(this, "KeyboardTestDlg", true);

	keyboardManager()->addEventListener(this);

	_list.SetTabStops(50);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL KeyboardTestDlg::DestroyWindow() 
{
	keyboardManager()->removeEventListener(this);

	WinPos::Save(this, "KeyboardTestDlg");
	
	return ResizeDialog::DestroyWindow();
}


void KeyboardTestDlg::keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, KEYBOARD_INTERFACE_EVENT ,(WPARAM)(&response), timestamp); 
}

LRESULT KeyboardTestDlg::OnKeyboardInterfaceEvent(WPARAM wParam, LPARAM lParam)	
{                         
	KeyboardResponse* response = (KeyboardResponse*)wParam;

	int time = lParam - _timestampOrigin;

	time = min(time, 99999);
	
	CString str;
	str.Format("%05d ms\t'%s'",time , response->getDescription());

	int nCount = _list.AddString(str);

	if (nCount >= 0)
		_list.SetCurSel(nCount);
	
	playManager()->playDefaultBell();

	_timestampOrigin = lParam;

	return 0;
}

void KeyboardTestDlg::keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) 
{
	//do not attach escape key
	for (int i=DIK_1;i<=DIK_MEDIASELECT;i++)
		keyboardActionSimulatorEventDataMap.SetAt(KeyboardAction(i),new SimulatorEventData(esigt_both));
}
