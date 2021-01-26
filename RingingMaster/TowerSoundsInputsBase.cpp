// TowerSoundsInputsBase.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsInputsBase.h"
#include "PlayManager.h"
#include "Tower.h"
#include "KeyboardManager.h"
#include "KeyboardItem.h"
#include "KeyboardResponse.h"
#include "UserMessages.h"
#include "MultiBellInterfaceManager.h"
#include "SpeechManager.h"
#include "SerialControlManager.h"
#include "SerialControlItem.h"
#include "SpeechResponse.h"
#include ".\towersoundsinputsbase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsInputsBase dialog


TowerSoundsInputsBase::TowerSoundsInputsBase(UINT nIDTemplate, Tower& internalTower)
	: CBCGPPropertyPage(nIDTemplate), 
	_internalTower(internalTower), 
	_setup(false)
{
	//{{AFX_DATA_INIT(TowerSoundsInputsBase)
	//}}AFX_DATA_INIT			 
	
}

TowerSoundsInputsBase::~TowerSoundsInputsBase()
{
	desetup();
}


void TowerSoundsInputsBase::setup()
{
	if (!_setup)
	{
		_internalTower.setupForEditing();
		keyboardManager()->addEventListener(this);
		multiBellInterfaceManager()->addEventListener(this);
		speechManager()->addEventListener(this);
		serialControlManager()->addEventListener(this);
		_setup = true;
	}
}

void TowerSoundsInputsBase::desetup()
{
	if (_setup)
	{
		keyboardManager()->removeEventListener(this);
		multiBellInterfaceManager()->removeEventListener(this);
		speechManager()->removeEventListener(this);
		serialControlManager()->removeEventListener(this);
	}	
}

void TowerSoundsInputsBase::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TowerSoundsInputsBase)
		DDX_Control(pDX, IDC_INPUTS_SCROLL, _sbLines);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TowerSoundsInputsBase, CBCGPPropertyPage)
	ON_WM_VSCROLL()
	ON_MESSAGE(ID_SIMULATOR_ADD_BTN,OnSimulatroAddBtn)
	ON_MESSAGE(ID_SIMULATOR_UPDATE_INPUT,OnSimulatroUpdateInput)
	
	ON_MESSAGE(KEYBOARD_INTERFACE_EVENT, OnKeyboardInterfaceEvent)
	ON_MESSAGE(MULTI_BELL_INTERFACE_EVENT, OnMultiBellInterfaceEvent)
	ON_MESSAGE(SERIAL_CONTROL_INTERFACE_EVENT, OnSerialControlInterfaceEvent)	
	ON_MESSAGE(SPEECH_EVENT, OnSpeechEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsInputsBase message handlers

BOOL TowerSoundsInputsBase::OnInitDialog() 
{
	setup();

	CBCGPPropertyPage::OnInitDialog();
	
	UpdateData(FALSE);

	for (int i=0;i<getPROFILE_BELL_CTRL_COUNT();i++)
		_ipSelectWnd[i].Create(CPoint(30,10 + i*21),this, TOWER_SOUNDS_WINDOW_BASE+i);

	updateScroll();
	update();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void TowerSoundsInputsBase::updateScroll(int scrollPos)
{
	//set up the scroll bar
	SCROLLINFO ScrollInfo;

	ScrollInfo.cbSize = sizeof(ScrollInfo);     // size of this structure
	ScrollInfo.fMask = SIF_ALL;                 // parameters to set
	ScrollInfo.nMin = 0;                        // minimum scrolling position
	ScrollInfo.nMax = max(getInputCount() - getPROFILE_BELL_CTRL_COUNT()+1,0);			// maximum scrolling position
	ScrollInfo.nPage = 2;						// the page size of the scroll box
	ScrollInfo.nPos = scrollPos;						// initial position of the scroll box
	ScrollInfo.nTrackPos = 0;                   // immediate position of a scroll box that the user is dragging
	_sbLines.SetScrollInfo(&ScrollInfo);
}

void TowerSoundsInputsBase::update(void)
{    
   	const int inputOffset =_sbLines.GetScrollPos();

	for (int windowIndex=0; windowIndex<getPROFILE_BELL_CTRL_COUNT(); windowIndex++)
	{
		int actionIndex = -1;
		int actionInputIndex = -1;
		if (getActionIndex(windowIndex + inputOffset, actionIndex, actionInputIndex))
		{
			SimulatorAction& action = getActionItem(actionIndex);
			CString str = getActionName(actionIndex);

			//special case for no inputs
			if (action.getInputCount() <= 0)
			{
				ASSERT(FALSE);
				break;
			}
			else
			{
				_ipSelectWnd[windowIndex].setSimulatorInput(actionIndex,
						action.getInput(actionInputIndex), 
						(actionInputIndex==0 || windowIndex==0)?str:"",  
						GetSysColor(COLOR_WINDOWTEXT),
						(action.getInputCount()-1 == actionInputIndex && action.getNoInputCount() ==0), 
						(action.getInputCount()-1 == actionInputIndex));				
			}
		}
		else
		{
			_ipSelectWnd[windowIndex].setSimulatorInput(0,NULL, "",RGB(0,0,0), false, false); 
		}
	}
}
    

void TowerSoundsInputsBase::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO ScrollInfo;
	_sbLines.GetScrollInfo(&ScrollInfo);  // get information about the scroll
	int pos = _sbLines.GetScrollPos() ;
	switch(nSBCode)  
	{ 
    case SB_LINEDOWN:       //Scrolls one line down. 
		pos++;  
		break;
    case SB_LINEUP:         //Scrolls one line up. 
		pos--;      
		break;
    case SB_PAGEDOWN:       //Scrolls one page down. 
		pos += ScrollInfo.nPage;      
		break;
    case SB_PAGEUP:         //Scrolls one page up. 
		pos -= ScrollInfo.nPage;      
		break;
    case SB_THUMBTRACK:     //The user is dragging the scroll box. This message is sent repeatedly until the user releases the mouse button. The nPos parameter indicates the position that the scroll box has been dragged to. 
		pos = nPos;      
		break;
	}
	_sbLines.SetScrollPos(pos);

	TRACE("POS: %d", pos);

	update();
	
	CBCGPPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT TowerSoundsInputsBase::OnSimulatroAddBtn(WPARAM wParam, LPARAM /*lParam*/)
{
	int id = wParam;
	
	addActionItem(id);
	
	updateScroll(_sbLines.GetScrollPos());

	update();

	return 0;
}

LRESULT TowerSoundsInputsBase::OnSimulatroUpdateInput(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	_internalTower.cleanupAfterEditing();
	_internalTower.setupForEditing();

	updateScroll(_sbLines.GetScrollPos());
	update();

	keyboardManager()->updateKeyMap();
	speechManager()->updateWordList();
	multiBellInterfaceManager()->updateMBIList();
	serialControlManager()->updateSerialControlList();

	return 0;
}


void TowerSoundsInputsBase::keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, KEYBOARD_INTERFACE_EVENT ,(WPARAM)(&response), timestamp); 
}

LRESULT TowerSoundsInputsBase::OnKeyboardInterfaceEvent(WPARAM wParam, LPARAM /*lParam*/)	
{
	KeyboardResponse* response = (KeyboardResponse*)wParam;

	//play the bell even though the input might nopt be on the screen.
	for (int i=0; i<getActionCount();i++)
	{
		SimulatorAction& bell = getActionItem(i);

		//special case for no inputs
		ASSERT(bell.getInputCount() > 0);

		for (int j=0;j<bell.getInputCount();j++)
		{
			if (bell.getInput(j)->isKeyboard())
			{
				KeyboardAction* action = bell.getInput(j)->getKeyboard();
				if ((action) && 
					(*action == *((KeyboardAction*)response)) && 
					(response->isValid()))
				{
					playManager()->playDefaultBell();		 
				}
			}
		}
	}

	//hilight the button that has been triggered
   	const int inputOffset =_sbLines.GetScrollPos();
	for (int windowIndex=0; windowIndex<getPROFILE_BELL_CTRL_COUNT(); windowIndex++)
	{
		int actionIndex = -1;
		int actionInputIndex = -1;
		if (getActionIndex(windowIndex + inputOffset, actionIndex, actionInputIndex))
		{
			SimulatorAction& bell = getActionItem(actionIndex);

			if (bell.getInput(actionInputIndex)->isKeyboard())
			{
				KeyboardAction* action = bell.getInput(actionInputIndex)->getKeyboard();
				if (action && *action == *((KeyboardAction*)response))
				{                                         
					_ipSelectWnd[windowIndex].setActiveIcon();
				}  
			}      
		}
	}       			

	return 0;
}

void TowerSoundsInputsBase::keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) 
{
	for (int i=0;i<getActionCount();i++)
	{
		SimulatorAction& bell = getActionItem(i);
		for (int j=0;j<bell.getInputCount();j++)
		{
			if (bell.getInput(j)->isKeyboard())
			{
				KeyboardAction* keyboard = bell.getInput(j)->getKeyboard();
				ASSERT(keyboard);
				KeyboardItem* item = dynamic_cast<KeyboardItem*>(keyboard);
				ASSERT(item);
				if (item)
				{
					keyboardActionSimulatorEventDataMap.SetAt(*keyboard,new SimulatorEventData(item->getSimulatorEventData()));
				}
			}
		}    
	}  
}

void TowerSoundsInputsBase::multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, MULTI_BELL_INTERFACE_EVENT,(WPARAM)(&multiBellResponse), timestamp); 
}

LRESULT TowerSoundsInputsBase::OnMultiBellInterfaceEvent(WPARAM wParam, LPARAM /*lParam*/)	
{
	MultiBellResponse* response = (MultiBellResponse*)wParam;

	//play the bell even though the input might nopt be on the screen.
	for (int i=0; i<getActionCount();i++)
	{
		SimulatorAction& bell = getActionItem(i);

		//special case for no inputs
		ASSERT(bell.getInputCount() > 0);

		for (int j=0;j<bell.getInputCount();j++)
		{
			if (bell.getInput(j)->isMultiBellInterface())
			{
				MultiBellAction* action = bell.getInput(j)->getMultiBellInterface();
				if ((action) && 
					(*action == *((MultiBellAction*)response)) && 
					(response->isValid()))
				{
					playManager()->playDefaultBell();		 
				}
			}
		}
	}

   	const int inputOffset =_sbLines.GetScrollPos();
	for (int windowIndex=0; windowIndex<getPROFILE_BELL_CTRL_COUNT(); windowIndex++)
	{
		int actionIndex = -1;
		int actionInputIndex = -1;
		if (getActionIndex(windowIndex + inputOffset, actionIndex, actionInputIndex))
		{
			SimulatorAction& bell = getActionItem(actionIndex);

			if (bell.getInput(actionInputIndex)->isMultiBellInterface())
			{
				MultiBellAction* action = bell.getInput(actionInputIndex)->getMultiBellInterface();
				if (action && *action == *((MultiBellAction*)response))
				{                                         
					_ipSelectWnd[windowIndex].setActiveIcon();
				}  
			}      
		}
	} 

	return 0;
}

void TowerSoundsInputsBase::multiBellInterfaceManager_notifyMBIStatusEvent(bool /*portOK*/, bool /*MBIOK*/)
{

}

void TowerSoundsInputsBase::multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap)
{
	for (int i=0;i<getActionCount();i++)
	{
		SimulatorAction& bell = getActionItem(i);
		for (int j=0;j<bell.getInputCount();j++)
		{
			if (bell.getInput(j)->isMultiBellInterface())
			{
				MultiBellAction* multiBell = bell.getInput(j)->getMultiBellInterface();
				ASSERT(multiBell);
				MultiBellItem* item = dynamic_cast<MultiBellItem*>(multiBell);
				ASSERT(item);
				if (item)
				{
					multiBellActionSimulatorEventDataMap.SetAt(*multiBell,new SimulatorEventData(item->getSimulatorEventData()));
				}
			}
		}    
	}
}

void TowerSoundsInputsBase::speechManager_notifySpeechRecognisedFromSeperateThread(const SpeechResponse& response, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, SPEECH_EVENT,(WPARAM)(&response), timestamp); 
}

LRESULT TowerSoundsInputsBase::OnSpeechEvent(WPARAM wParam, LPARAM /*lParam*/)	
{

	SpeechResponse* response = (SpeechResponse*)wParam;

	//play the bell even though the input might nopt be on the screen.
	for (int i=0; i<getActionCount();i++)
	{
		SimulatorAction& bell = getActionItem(i);

		//special case for no inputs
		ASSERT(bell.getInputCount() > 0);

		for (int j=0;j<bell.getInputCount();j++)
		{
			if (bell.getInput(j)->isSpeech())
			{
				SpeechAction* action = bell.getInput(j)->getSpeech();
				if ((action) && 
					(*action == *((SpeechAction*)response)) && 
					(response->isValid()))
				{
					playManager()->playDefaultBell();		 
				}
			}
		}
	}


	//hilight the button that has been triggered
   	const int inputOffset =_sbLines.GetScrollPos();
	for (int windowIndex=0; windowIndex<getPROFILE_BELL_CTRL_COUNT(); windowIndex++)
	{
		int actionIndex = -1;
		int actionInputIndex = -1;
		if (getActionIndex(windowIndex + inputOffset, actionIndex, actionInputIndex))
		{
			SimulatorAction& bell = getActionItem(actionIndex);

			if (bell.getInput(actionInputIndex)->isSpeech())
			{
				SpeechAction* action = bell.getInput(actionInputIndex)->getSpeech();
				if (action && *action == *((SpeechAction*)response))
				{                                         
					_ipSelectWnd[windowIndex].setActiveIcon();
				}  
			}      
		}
	}  

	return 0;
}

void TowerSoundsInputsBase::speechManager_notifyGetSpeechList(SpeechActionEventDataMap& speechActionEventDataMap)
{
	for (int i=0;i<getActionCount();i++)
	{
		SimulatorAction& bell = getActionItem(i);
		for (int j=0;j<bell.getInputCount();j++)
		{
			if (bell.getInput(j)->isSpeech())
			{
				SpeechAction* speech = bell.getInput(j)->getSpeech();
				ASSERT(speech);
				SpeechItem* item = dynamic_cast<SpeechItem*>(speech);
				ASSERT(item);
				if (item)
				{
					speechActionEventDataMap.SetAt(*speech,new SimulatorEventData(item->getSimulatorEventData()));
				}
			}
		}    
	}
}

void TowerSoundsInputsBase::serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp)
{
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, SERIAL_CONTROL_INTERFACE_EVENT,(WPARAM)(&response), timestamp); 
}

LRESULT TowerSoundsInputsBase::OnSerialControlInterfaceEvent(WPARAM wParam, LPARAM /*lParam*/)	
{
	SerialControlResponse* response = (SerialControlResponse*)wParam;

	for (int i=0; i<getActionCount();i++)
	{
		SimulatorAction& bell = getActionItem(i);

		//special case for no inputs
		ASSERT(bell.getInputCount() > 0);

		for (int j=0;j<bell.getInputCount();j++)
		{
			if (bell.getInput(j)->isSerialControl())
			{
				SerialControlAction* action = bell.getInput(j)->getSerialControl();;
				if ((action) && 
					(*action == *((SerialControlAction*)response)) && 
					(response->isValid()))
				{
					playManager()->playDefaultBell();		 
				}
			}
		}
	}


	//hilight the button that has been triggered
   	const int inputOffset =_sbLines.GetScrollPos();
	for (int windowIndex=0; windowIndex<getPROFILE_BELL_CTRL_COUNT(); windowIndex++)
	{
		int actionIndex = -1;
		int actionInputIndex = -1;
		if (getActionIndex(windowIndex + inputOffset, actionIndex, actionInputIndex))
		{
			SimulatorAction& bell = getActionItem(actionIndex);

			if (bell.getInput(actionInputIndex)->isSerialControl())
			{
				SerialControlAction* action = bell.getInput(actionInputIndex)->getSerialControl();
				if (action && *action == *((SerialControlAction*)response))
				{                                         
					_ipSelectWnd[windowIndex].setActiveIcon();
				}  
			}      
		}
	}  

	return 0;
}

void TowerSoundsInputsBase::serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& serialControlResponseSimulatorEventDataMap)
{
	for (int i=0;i<getActionCount();i++)
	{
		SimulatorAction& bell = getActionItem(i);
		for (int j=0;j<bell.getInputCount();j++)
		{
			if (bell.getInput(j)->isSerialControl())
			{
				SerialControlAction* serialControl = bell.getInput(j)->getSerialControl();
				ASSERT(serialControl);
				SerialControlItem* item = dynamic_cast<SerialControlItem*>(serialControl);
				ASSERT(item);
				if (item)
				{
					serialControlResponseSimulatorEventDataMap.SetAt(*item,new SimulatorEventData(item->getSimulatorEventData()));
				}
			}
		}    
	}
}


BOOL TowerSoundsInputsBase::OnSetActive()
{
	updateScroll();
	update();

	return __super::OnSetActive();
}

bool TowerSoundsInputsBase::getActionIndex(int inputIndex, int& actionIndex, int& actionInputInex)
{
	int count = 0;
	for (int i=0;i<getActionCount();i++)
	{
		SimulatorAction& action = getActionItem(i);
		for (int j=0;j<action.getInputCount();j++)
		{
			if (count == inputIndex)
			{
				actionIndex = i;
				actionInputInex = j;
				return true;
			}
			count++;

		}
	}

	return false;
}

int TowerSoundsInputsBase::getInputCount()
{
	int count = 0;
	for (int i=0;i<getActionCount();i++)
	{
		count += getActionItem(i).getInputCount();
	}
	return count;
}
