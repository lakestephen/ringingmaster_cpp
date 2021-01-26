// StatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "StatusBar.h"
#include "TouchDoc.h"
#include "RecordManager.h"
#include "Method.h"
#include "TouchHoldView.h"
#include "MapDoc.h"
#include "CribDoc.h"
#include "ProofManager.h"
#include "MultiBellInterfaceManager.h"
#include "KeyboardManager.h"
#include "SerialCOntrolManager.h"
#include "SpeechManager.h"
#include "MethodAnalysis.h"
#include "MatrixManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// StatusBar

StatusBar::StatusBar() : 
_state(	ANIST_CLEAR),
_lastViewType(vt_none),
_progressCurr(0),
_progressTotal(-1)
{
	_proofAni.Create (IDB_PROOF_ANI, 26, 0, RGB (255, 255, 255));
	_bmpTick.LoadBitmap (IDB_PROOF_TICK);
	_bmpCross.LoadBitmap (IDB_PROOF_CROSS);
	_bmpClear.LoadBitmap (IDB_PROOF_CLEAR);

	_bmpMBI.LoadBitmap(IDB_MBI);
	_bmpMBICross.LoadBitmap(IDB_MBI_CROSS);

	_bmpKeyboard.LoadBitmap(IDB_KEYBOARD);
	_bmpKeyboardCross.LoadBitmap(IDB_KEYBOARD_CROSS);

	_bmpSpeech.LoadBitmap(IDB_MIKE);
	_bmpSpeechCross.LoadBitmap(IDB_MIKE_CROSS);

	_bmpSerialControl.LoadBitmap(IDB_SERIAL);
	_bmpSerialControlCross.LoadBitmap(IDB_SERIAL_CROSS);

	_bmpStatus.LoadBitmap (IDB_STATUS_READY);
	_imlStatusAnimation.Create (IDB_STATUS_ANIMATION, 11, 0, RGB (255, 0, 255));
	
}

StatusBar::~StatusBar()
{

}


BEGIN_MESSAGE_MAP(StatusBar, CBCGPStatusBar )
	//{{AFX_MSG_MAP(StatusBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StatusBar message handlers

void StatusBar::startProofAnimation()
{
	if (getSize() > 0)
	{
		SetPaneAnimation(getSize(), _proofAni, 150);
		Invalidate();
	}
}

void StatusBar::stopProofAnimation(BOOL showTick)
{
	if (getSize() > 0)
	{
		SetPaneAnimation (getSize(), NULL);
		SetPaneIcon(getSize(), showTick?_bmpTick:_bmpCross, RGB(255,255,255));
		Invalidate();
	}
}

void StatusBar::clearProofAnimation()
{
	if (getSize() > 1)
	{
		SetPaneAnimation (getSize(), NULL);
		SetPaneIcon(getSize(), _bmpClear, RGB(255,255,255));
		Invalidate();
	}
}

void StatusBar::updateAnimation(BOOL force)
{
//	TRACE("UpdateAnimation\r\n");
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	StatusBarAniState state = ANIST_CLEAR;

	if (pTouchDoc)
	{
		if (proofManager()->isCalculatingProof())
		{
			state = ANIST_ANI;
		}
		else
		{
			Method* method = pTouchDoc->getMethod();

			if (method)
			{
				if (method->getAnalysis()->isTrue())
					state = ANIST_TICK;
				else
					state = ANIST_CROSS;
			}
		}
	}							 					   
	
	if ((state != _state)||(force))
	{
		_state = state;

		switch (_state)
		{
		case ANIST_TICK:
			stopProofAnimation(TRUE);
			break;
		case ANIST_CROSS:
			stopProofAnimation(FALSE);
			break;
		case ANIST_ANI:
			startProofAnimation();
			break;
		case ANIST_CLEAR:
		default:
			clearProofAnimation();
			break;
			
		}
	}
}

static UINT indicators_touch_main[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_ST_1,
	ID_ST_2,
	ID_ST_3,
	ID_ST_4,
};

static UINT indicators_touch_blue_line[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_ST_1,
	ID_ST_2,
	ID_ST_3,
	ID_ST_4,
	ID_ST_5,
	ID_ST_6,
	ID_ST_7,
};

static UINT indicators_touch_simulator[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_ST_1,
	ID_ST_2,
	ID_ST_3,
	ID_ST_4,
	ID_ST_5,
	ID_ST_6,
	ID_ST_7,
	ID_ST_8,
	ID_ST_9,
	ID_ST_10,
};
static UINT indicators_crib[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_ST_1,
	ID_ST_2,
	ID_ST_3,
	ID_ST_4,
	ID_ST_5,
	ID_ST_6,
};

static UINT indicators_record[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_ST_1,
	ID_ST_2,
	ID_ST_3,
};

static UINT indicators_map[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_ST_1,
};

static UINT indicators_internet[] =
{
	ID_INDICATOR_ICON,
	ID_SEPARATOR,           // status line indicator
	ID_ST_1,
};

static UINT indicators_none[] =
{
	ID_SEPARATOR,           // status line indicator
};



void StatusBar::clearAllPaneText()
{
	int size = getSize();

	for (int i=1;i<size;i++)
		SetPaneText(i,"");
}


void StatusBar::mainFrame_notifyUpdate(ViewType /*ViewType*/)
{

	ViewType viewType = app()->getViewType();
	if (viewType != _lastViewType)
	{

		//remove a BCG memory leak in SetIndicators
		//to2do do we still need this?
		for (int i=0;i<GetCount();i++)
			SetTipText(i, NULL); 		

		_lastViewType = viewType;

		const int pealTimeWidth = 47;

		switch (_lastViewType)
		{
		case vt_touch_main:
			SetIndicators(indicators_touch_main, sizeof(indicators_touch_main)/sizeof(UINT));
			SetPaneWidth (1, 45);
			SetPaneWidth (2, pealTimeWidth);
			SetPaneWidth (3, 35);
			updateAnimation(TRUE); //SetPaneWidth (4, 50); //do not set the animation pane
			break;
		case vt_touch_blue_line:
			SetIndicators(indicators_touch_blue_line, sizeof(indicators_touch_blue_line)/sizeof(UINT));
			SetPaneWidth (1, 45);
			SetPaneWidth (2, 55);
			SetPaneWidth (3, 41);
			SetPaneWidth (4, 27);
			SetPaneWidth (5, 55);
			SetPaneWidth (6, pealTimeWidth);
			updateAnimation(TRUE); //SetPaneWidth (7, 50); //do not set the animation pane
			break;
		case vt_touch_simulator:
			SetIndicators(indicators_touch_simulator, sizeof(indicators_touch_simulator)/sizeof(UINT));
			SetPaneWidth (1, 45);
			SetPaneWidth (2, 55);
			SetPaneWidth (3, 55);
			SetPaneWidth (4, 150);
			SetPaneWidth (5, pealTimeWidth);
			SetPaneWidth (6, 0);
			SetPaneWidth (7, 0);
			SetPaneWidth (8, 0);
			SetPaneWidth (9, 0);
			updateAnimation(TRUE); //SetPaneWidth (10, 50);//do not set the animation pane
			break;
		case vt_crib:
			SetIndicators(indicators_crib, sizeof(indicators_crib)/sizeof(UINT));
			SetPaneWidth (1, 45);
			SetPaneWidth (2, 55);
			SetPaneWidth (3, 41);
			SetPaneWidth (4, 27);
			SetPaneWidth (5, 65);
			SetPaneWidth (6, pealTimeWidth);
			break;
		case vt_tower:
		case vt_grab:
			SetIndicators(indicators_record, sizeof(indicators_record)/sizeof(UINT));
			SetPaneWidth (1, 200);
			SetPaneWidth (2, 100);
			SetPaneWidth (3, 100);
			break;
		case vt_map:
			SetIndicators(indicators_map, sizeof(indicators_map)/sizeof(UINT));
			SetPaneWidth (1, 90);
			break;
		case vt_internet:
			SetIndicators(indicators_internet, sizeof(indicators_internet)/sizeof(UINT));
			SetPaneWidth (0, 0);
			SetPaneStyle (0, SBPS_NOBORDERS);
			SetPaneStyle (1, SBPS_STRETCH | SBPS_NOBORDERS);
			SetPaneWidth (2, 80);
			break;
		case vt_none:
		default:
			SetIndicators(indicators_none, sizeof(indicators_none)/sizeof(UINT));
			break;
		}
	}


	//now do updates that are needed every time
	switch (_lastViewType)
	{
	case vt_internet:
		EnablePaneProgressBar (2, _progressTotal);	 
		break;
	}
}

int StatusBar::getSize()
{
	return m_nCount -1;
}

void StatusBar::mainFrame_notifyIdle()
{
	updateAnimation(FALSE);

	switch (app()->getViewType())
	{
	case vt_touch_main:
		{
			TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
			if (pTouchDoc)
			{
				CString pane1,pane2,pane3;
				pTouchDoc->getMainStatusBarText(pane1,pane2,pane3);
				
				setPaneText(1,pane1);
				SetTipText(1, "Touch grid position");
				setPaneText(2,pane2);
				SetTipText(2, "Peal Speed h:m:s");
				setPaneText(3,pane3);
				SetTipText(3, "Insert / Overstrike");
				SetTipText(4, "Touch proof status");
			}
		}
		break;
	case vt_touch_blue_line:
		{
			TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
			if (pTouchDoc)
			{
				CString pane1,pane2,pane3,pane4,pane5,pane6;
				pTouchDoc->getBlueLineStatusBarText(pane1,pane2,pane3,pane4,pane5,pane6);
				setPaneText(1,pane1);
				SetTipText(1, "Lead");
				setPaneText(2,pane2);
				SetTipText(2, "Row");
				setPaneText(3,pane3);
				SetTipText(3, "Place");
				setPaneText(4,pane4);
				SetTipText(4, "Bell");
				setPaneText(5,pane5);
				SetTipText(5, "Stroke");
				setPaneText(6,pane6);
				SetTipText(6, "Peal Speed h:m:s");
				SetTipText(7, "Touch proof status");
			}
		}
		break;
	case vt_touch_simulator:
		{
			TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
			if (pTouchDoc)
			{
				CString pane1,pane2,pane3,pane4,pane5;
				pTouchDoc->getSimulatorStatusBarText(pane1,pane2,pane3,pane4,pane5);
				setPaneText(1,pane1);
				SetTipText(1, "Lead");
				setPaneText(2,pane2);
				SetTipText(2, "Row");
				setPaneText(3,pane3);
				SetTipText(3, "Stroke");
				setPaneText(4,pane4);
				SetTipText(4, "Current Method");
				setPaneText(5,pane5);
				SetTipText(5, "Peal Speed h:m:s");
				setPaneText(6, " ");
				setPaneText(7, " ");
				setPaneText(8, " ");
				setPaneText(9, " ");
				SetTipText(10, "Touch proof status");


				if (keyboardManager()->isKeyboardAvailable())
				{
					SetPaneIcon(6, _bmpKeyboard, RGB(192,192,192));
					SetTipText(6, "Keyboard system is connected.");
				}
				else
				{
					SetPaneIcon(6, _bmpKeyboardCross, RGB(192,192,192));
					SetTipText(6, "Keyboard sistem is not connected.");
				}

				if (multiBellInterfaceManager()->isMBIConnected())
				{
					SetPaneIcon(7, _bmpMBI, RGB(192,192,192));
					CString str;
					str.Format("Multi Bell Interface connected on COM %d.", multiBellInterfaceManager()->getPortID());
					SetTipText(7, str);
				}
				else
				{
					SetPaneIcon(7, _bmpMBICross, RGB(192,192,192));
					SetTipText(7, "Multi Bell Interface not connected.");
				}

				if (serialControlManager()->isComPortConnected())
				{
					bool allOK = true;
					CString str("Serial Control is connected on");
					for (int i=1;i<=MAX_PORTS;i++)
					{
						if (serialControlManager()->isComPortActive(i) && 
							serialControlManager()->isComPortConnected(i))
						{
							CString temp;
							temp.Format(" COM %d", i); 
							str += temp;
						}
					}

					for (int i=1;i<=MAX_PORTS;i++)
					{
						if (serialControlManager()->isComPortActive(i) && 
							!serialControlManager()->isComPortConnected(i))
						{
							CString temp;
							temp.Format(". Can't connect to COM %d", i); 
							str += temp;
							allOK = false;
						}
					}

					SetPaneIcon(8, allOK?_bmpSerialControl:_bmpSerialControlCross, RGB(192,192,192));
					SetTipText(8, str + ".");
				}
				else
				{				
					SetPaneIcon(8, _bmpSerialControlCross, RGB(192,192,192));
					SetTipText(8, "Serial Control is not connected.");
				}

				if (speechManager()->isSpeechRecognitionAvailable())
				{
 					SetPaneIcon(9, _bmpSpeech, RGB(192,192,192));
					SetTipText(9, "Microphone system is connected.");
				}
				else
				{
 					SetPaneIcon(10, _bmpSpeechCross, RGB(192,192,192));
					SetTipText(10, "Microphone system is not connected.");
				}

				Invalidate();
			}
		}
		break;
	case vt_crib:
		{
			CribDoc* pCribDoc = mainFrame()->getActiveCribDoc();
			if (pCribDoc)
			{
				CString pane1,pane2,pane3,pane4,pane5,pane6;
				pCribDoc->getBlueLineStatusBarText(pane1,pane2,pane3,pane4,pane5,pane6);
				setPaneText(1,pane1);
				SetTipText(1, "Lead");
				setPaneText(2,pane2);
				SetTipText(2, "Row");
				setPaneText(3,pane3);
				SetTipText(3, "Place");
				setPaneText(4,pane4);
				SetTipText(4, "Bell");
				setPaneText(5,pane5);
				SetTipText(5, "Stroke");
				setPaneText(6,pane6);
				SetTipText(6, "Peal Speed h:m:s");
			}
		}
		break;
	case vt_tower:
		{
			CString pane1, pane2, pane3;
			matrixManager()->getStatusBarText(pane1, pane2, pane3);
			setPaneText(1, pane1);
			SetTipText(1, "Home Tower");
			setPaneText(2, pane2);
			SetTipText(2, "Memory Tower Count");
			setPaneText(3, pane3);
			SetTipText(3, "Screen Tower Count");
		}
		break;
	case vt_grab:
		{
			CString pane1, pane2, pane3;
			matrixManager()->getStatusBarText(pane1, pane2, pane3);
			setPaneText(1, pane1);
			SetTipText(1, "Home Tower");
			setPaneText(2, pane2);
			SetTipText(2, "Memory Grab Count");
			setPaneText(3, pane3);
			SetTipText(3, "Screen Grab Count");
		}
		break;
	case vt_map:
		{
			MapDoc* pMapDoc = mainFrame()->getActiveMapDoc();

			if (pMapDoc)
			{
				CString pane1, pane2, pane3;
				pMapDoc->getStatusBarText(pane1);
				setPaneText(1, pane1);
				SetTipText(1, "Grid Reference");
			}
		}
		break;
	case vt_internet:
		{
			if (_progressTotal >= 0)
				SetPaneProgress (2, min (_progressTotal, max (0, _progressCurr)));
			else
				RedrawWindow();

			setPaneText (0, "");
			SetTipText(0, "");
		}
		break;
	default:
	case vt_none:
		break;
	}

}


void StatusBar::setPaneText(int index, CString str)
{
	if (index < 0 || index >= m_nCount)
		return;
	if (str.IsEmpty())
		SetPaneText(index,NULL);
	else
		SetPaneText(index,str);
}

void StatusBar::SetProgress(long nCurr, long nTotal)
{   	
	_progressCurr = nCurr;
	_progressTotal = nTotal;

	mainFrame_notifyUpdate(vt_none);//file type gets ignored
}

void StatusBar::startInternetAnimation()
{
	if (m_nCount > 0)
		SetPaneAnimation (0, _imlStatusAnimation);
}

void StatusBar::stopInternetAnimation()
{
	if (m_nCount > 0)
		SetPaneIcon (0, _bmpStatus);
}



BOOL StatusBar::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}

int StatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	mainFrame()->addEventListener(this);

	return 0;
}

