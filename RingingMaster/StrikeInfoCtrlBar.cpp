// StrikeInfoCtrlBar.cpp: implementation of the StrikeInfoCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "StrikeInfoCtrlBar.h"
#include "StrikingData.h"
#include "StrikeDoc.h"
#include "StrikeComboBtn.h"
#include "StrikeManager.h"
#include "StrikingItem.h"
#include "StrikingRow.h"
#include "PropItem.h"
#include ".\strikeinfoctrlbar.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StrikeInfoCtrlBar::StrikeInfoCtrlBar()
{
	strikeManager()->addEventListener(this);
}

StrikeInfoCtrlBar::~StrikeInfoCtrlBar()
{

}
   
IMPLEMENT_DYNAMIC(StrikeInfoCtrlBar, CBCGPDockingControlBar)

BEGIN_MESSAGE_MAP(StrikeInfoCtrlBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(StrikeInfoCtrlBar)
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



int StrikeInfoCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	BOOL result = _propList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|
								LBS_OWNERDRAWVARIABLE|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_HASSTRINGS, 
								CRect(0,0,0,0),
								this,
								ID_STRIKEINFO);	

	_propList._devider[0] = 130;	

	if (result)
		_propList.SetFont(&app()->_fontStd);

	AdjustLayout ();
	return 0;
}

void StrikeInfoCtrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void StrikeInfoCtrlBar::AdjustLayout ()
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

	_propList.SetWindowPos (NULL,
									rectClient.left + 1, 
									rectClient.top + cyTlb + 1, 
									rectClient.Width () - 2,
									rectClient.Height () - (cyTlb) - 2,
									SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL StrikeInfoCtrlBar::DestroyWindow() 
{
	strikeManager()->removeEventListener(this);

	return CBCGPDockingControlBar::DestroyWindow();

}

BOOL StrikeInfoCtrlBar::OnEraseBkgnd(CDC* /*pDC*/) 
{			  	
	return TRUE;
}

void StrikeInfoCtrlBar::strikeManager_notifyUpdateStrikeList()						 
{
	update();
}

void StrikeInfoCtrlBar::update()
{
	_propList.deleteAllPropItems();

	StrikeDoc* pStrikeDoc = mainFrame()->getActiveStrikeDoc();
	assert(pStrikeDoc);
	if (pStrikeDoc)
	{
		const StrikingData& strikingData = pStrikeDoc->getStrikingData();

        const StrikingDataStats& stats = strikingData.getStats(strikeManager()->getAnalysisBell());
		const AllBellsStrikingDataStats& allBellsStats = strikingData.getAllBellsStats();
		

		StrikingDataStatsStroke strikingDataStatsStroke;
		AllBellsStrikingDataStatsStroke allBellsStrikingDataStatsStroke;
		CString name;
		switch (strikeManager()->getStrikeAnalysisType())
		{
		case sat_handback:
			strikingDataStatsStroke = stats._handback;
			allBellsStrikingDataStatsStroke = allBellsStats._handback;
			name = "";
			break;
		case sat_hand:
			strikingDataStatsStroke = stats._hand;
			allBellsStrikingDataStatsStroke = allBellsStats._hand;
			name = "Hand";
			break;
		default:
			ASSERT(FALSE);
		case sat_back:
			strikingDataStatsStroke = stats._back;
			allBellsStrikingDataStatsStroke = allBellsStats._back;
			name = "Back";
			break;
		}         

		
		CString item, value;

		value.Format("%.2f%% %s",abs(strikingDataStatsStroke._meanPercentError*100), ((strikingDataStatsStroke._meanPercentError == 0)?"":((strikingDataStatsStroke._meanPercentError> 0)?"Late":"Early"))); 
		item.Format("Mean Error %s:", name);
		addLine(item, value);	 
		value.Format("%.2f%%",abs(strikingDataStatsStroke._standardDeviationPercent*100) ); 
		item.Format("Standard Deviation %s:", name);
		addLine(item, value);	 
		
		value.Format("%d", strikingDataStatsStroke._strikeCount); 
		item.Format("Strike Count %s:", name);	 
		addLine(item, value);	 
		value.Format("%d",strikingDataStatsStroke._lateStrikesCount); 
		item.Format("Strike Count Late (%d%%) %s:", LATE_EARLY_PERCENT, name);	 
		addLine(item, value);
		value.Format("%d",strikingDataStatsStroke._earlyStrikesCount); 
		item.Format("Strike Count Early (%d%%) %s:", LATE_EARLY_PERCENT, name);	 
		addLine(item, value);

		value.Format("%.2f%% %s ",abs(allBellsStrikingDataStatsStroke._allBellsError * 100),
			((allBellsStrikingDataStatsStroke._allBellsError == 0)?"":((allBellsStrikingDataStatsStroke._allBellsError> 0)?"Late":"Early"))); 

		item.Format("Mean Error All Bells %s:", name);	 
		addLine(item, value);	 
		value.Format("%d",allBellsStrikingDataStatsStroke._rowCount); 
		item.Format("Row Count All Bells %s:", name);	 
		addLine(item, value);		

	}

	AdjustLayout();
	
	_propList.Invalidate();
}


void StrikeInfoCtrlBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		   
	if (::IsWindow(_propList.GetSafeHwnd()))
	{
		CRect rectList;
		_propList.GetWindowRect (rectList);
		ScreenToClient (rectList);

		rectList.InflateRect (1, 1);
		dc.Draw3dRect (rectList, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
	}  
}    

void StrikeInfoCtrlBar::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPDockingControlBar::OnSetFocus(pOldWnd);
	
	_propList.SetFocus ();	
}

void StrikeInfoCtrlBar::toolbarReset() 
{
	_toolBar.ReplaceButton (ID_DUMMY_STRIKE_SET_BELL, StrikeComboBtn());
}

void StrikeInfoCtrlBar::OnUpdateSetBell(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void StrikeInfoCtrlBar::OnSetBell() 
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

void StrikeInfoCtrlBar::addLine(CString col0, CString col1, COLORREF color)
{
	PropItem* pItem; 

	pItem = _propList.findItem(col0);
	
	if (pItem == NULL)
	{
		pItem = new PropItem(PT_STATIC);

		_propList.AddPropItem(pItem);
	}
	pItem->_col[0] =  col0;
	pItem->_col[1] =  col1;

	if (color == -1)
		color = ::GetSysColor(COLOR_WINDOW);

	pItem->_backgroundColor = color;

}

void StrikeInfoCtrlBar::OnHand()
{
	strikeManager()->setStrikeAnalysisType(sat_hand);
}

void StrikeInfoCtrlBar::OnUpdateHand(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(strikeManager()->getStrikeAnalysisType() == sat_hand);	
}

void StrikeInfoCtrlBar::OnBack()
{
	strikeManager()->setStrikeAnalysisType(sat_back);
}

void StrikeInfoCtrlBar::OnUpdateBack(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(strikeManager()->getStrikeAnalysisType() == sat_back);	
}

void StrikeInfoCtrlBar::OnHandBack()
{
	strikeManager()->setStrikeAnalysisType(sat_handback);
}

void StrikeInfoCtrlBar::OnUpdateHandBack(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(strikeManager()->getStrikeAnalysisType() == sat_handback);	
}
