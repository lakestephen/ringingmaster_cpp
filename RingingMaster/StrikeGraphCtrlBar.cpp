// StrikeGraphCtrlBar.cpp: implementation of the StrikeGraphCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "StrikeGraphCtrlBar.h"
#include "StrikingData.h"
#include "StrikeDoc.h"
#include "StrikeComboBtn.h"
#include "StrikeManager.h"
#include "StrikingItem.h"
#include "StrikingRow.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StrikeGraphCtrlBar::StrikeGraphCtrlBar()
{
	strikeManager()->addEventListener(this);
}

StrikeGraphCtrlBar::~StrikeGraphCtrlBar()
{

}
   
IMPLEMENT_DYNAMIC(StrikeGraphCtrlBar, CBCGPDockingControlBar)

BEGIN_MESSAGE_MAP(StrikeGraphCtrlBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(StrikeGraphCtrlBar)
	ON_WM_CREATE()
	ON_WM_SIZE() 
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP	  
	ON_UPDATE_COMMAND_UI(ID_STRIKE_SET_BELL, OnUpdateSetBell)
	ON_CBN_SELCHANGE(ID_STRIKE_SET_BELL, OnSetBell)
					
	ON_COMMAND(ID_HAND, OnHand)
	ON_UPDATE_COMMAND_UI(ID_HAND, OnUpdateHand)
	ON_COMMAND(ID_BACK, OnBack)
	ON_UPDATE_COMMAND_UI(ID_BACK, OnUpdateBack)
	ON_COMMAND(ID_HAND_BACK, OnHandBack)
	ON_UPDATE_COMMAND_UI(ID_HAND_BACK, OnUpdateHandBack)

END_MESSAGE_MAP()



int StrikeGraphCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty ();    


	_toolBar.Create (this, dwDefaultToolbarStyle, IDR_PROP_DEFAULT);
	_toolBar.LoadToolBar (IDR_STRIKE_GENERAL, 0, 0, TRUE /* Is locked */);
	_toolBar.SetBarStyle(_toolBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY);
	_toolBar.SetBarStyle (_toolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	_toolBar.SetOwner (this);
	_toolBar.SetRouteCommandsViaFrame (FALSE);	

	_graphWnd.Create(NULL, "Graph", WS_TABSTOP, CRect(0,0,100,100),		
									this, ID_STRIKE_GRAPH_CTRL_BAR_GRAPH);

	AdjustLayout ();
	return 0;
}

void StrikeGraphCtrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void StrikeGraphCtrlBar::AdjustLayout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}       

	CRect rectClient;
	GetClientRect (rectClient);
	
	int cyTlb = _toolBar.CalcFixedLayout (FALSE, TRUE).cy;

	_toolBar.SetWindowPos (NULL,
						rectClient.left, 
						rectClient.top , 
						rectClient.Width (),
						cyTlb,
						SWP_NOACTIVATE | SWP_NOZORDER);

	_graphWnd.SetWindowPos (NULL,
									rectClient.left + 1, 
									rectClient.top + cyTlb + 1, 
									rectClient.Width () - 2,
									rectClient.Height () - (cyTlb) - 2,
									SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL StrikeGraphCtrlBar::DestroyWindow() 
{
	strikeManager()->removeEventListener(this);

	return CBCGPDockingControlBar::DestroyWindow();

}

BOOL StrikeGraphCtrlBar::OnEraseBkgnd(CDC* /*pDC*/) 
{			  	
	return TRUE;
}

void StrikeGraphCtrlBar::strikeManager_notifyUpdateStrikeList()						 
{
	update();
}

void StrikeGraphCtrlBar::update()
{
	StrikeDoc* pStrikeDoc = mainFrame()->getActiveStrikeDoc();
	assert(pStrikeDoc);
	if (pStrikeDoc)
	{
		const StrikingData& strikingData = pStrikeDoc->getStrikingData();

		_graphWnd.setCount(STATS_GRAPH_RANGE);
		_graphWnd.setXZeroPos(STATS_GRAPH_RANGE/2);

	    const StrikingDataStats& stats = strikingData.getStats(strikeManager()->getAnalysisBell());

		StrikingDataStatsStroke strikingDataStatsStroke;

		switch (strikeManager()->getStrikeAnalysisType())
		{
		case sat_handback:
			strikingDataStatsStroke = stats._handback;
			break;
		case sat_hand:
			strikingDataStatsStroke = stats._hand;
			break;
		default:
			ASSERT(FALSE);
		case sat_back:
			strikingDataStatsStroke = stats._back;
			break;
		}  

		for (int i=0;i<STATS_GRAPH_RANGE;i++)
		{
			COLORREF color = RGB(0,0,0);;
			if (i<STATS_GRAPH_RANGE/2)
				color = RGB(0,150,0);
			else if (i>STATS_GRAPH_RANGE/2)
				color = RGB(0,0,150);

			_graphWnd.setValue(i,strikingDataStatsStroke._graphVals[i], color);
		}
	}

	_graphWnd.Invalidate();
	AdjustLayout();
	
}


void StrikeGraphCtrlBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		   
	if (::IsWindow(_graphWnd.GetSafeHwnd()))
	{
		CRect rectList;
		_graphWnd.GetWindowRect (rectList);
		ScreenToClient (rectList);

		rectList.InflateRect (1, 1);
		dc.Draw3dRect (rectList, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
	}  
}    

void StrikeGraphCtrlBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	
	_graphWnd.SetFocus ();	
}

void StrikeGraphCtrlBar::toolbarReset() 
{
	_toolBar.ReplaceButton (ID_DUMMY_STRIKE_SET_BELL, StrikeComboBtn());
}

void StrikeGraphCtrlBar::OnUpdateSetBell(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void StrikeGraphCtrlBar::OnSetBell() 
{
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	CString towerName("");

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_STRIKE_SET_BELL, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); 
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo = 
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL)
			{
				if (strikeManager()->setAnalysisBell(pCombo->GetCurSel()))
					break;
			}
		}
	}	
}  

void StrikeGraphCtrlBar::OnHand()
{
	strikeManager()->setStrikeAnalysisType(sat_hand);
}

void StrikeGraphCtrlBar::OnUpdateHand(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(strikeManager()->getStrikeAnalysisType() == sat_hand);	
}

void StrikeGraphCtrlBar::OnBack()
{
	strikeManager()->setStrikeAnalysisType(sat_back);
}

void StrikeGraphCtrlBar::OnUpdateBack(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(strikeManager()->getStrikeAnalysisType() == sat_back);	
}

void StrikeGraphCtrlBar::OnHandBack()
{
	strikeManager()->setStrikeAnalysisType(sat_handback);
}

void StrikeGraphCtrlBar::OnUpdateHandBack(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(strikeManager()->getStrikeAnalysisType() == sat_handback);	
}
