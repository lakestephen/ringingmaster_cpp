// MainFrm.cpp : implementation of the MainFrame class
//

#include "stdafx.h"

//the virtual memory leak detection.
//just include this header, and it will link with the .lib
#ifdef _DEBUG
//#include "include/vld.h"
#endif

#include "RingingMaster.h"
#include "OptionsDlg.h"
#include "TouchDoc.h"
#include "TouchView.h"
#include "InternetDoc.h"
#include "InternetView.h"
#include "TowerDoc.h"
#include "GrabDoc.h"
#include "StatusBar.h"
#include "Notation.h"
#include "MethodEditDlg.h"
#include "GenericEditDlg.h"
#include "RegArchive.h"
#include "colors.h"
#include "NagDlg.h"
#include "key.h"
#include "MethodDlg.h"
#include "AboutDlg.h"
#include "PropertyCtrlBar.h"
#include "StrikeDoc.h"
#include "TaskPane.h"
#include "StrikeComboBtn.h"
#include "SimulatorComboBtn.h"
#include "StrikeInfoCtrlBar.h"
#include "RepeatingToolbar.h"




#include "TouchAnalysisStatus.h"
#include "TouchAnalysisCourseHead.h"
#include "TouchAnalysisMultiBlock.h"
#include "TouchAnalysisSplice.h"
#include "TouchAnalysisSpliceTransition.h"
#include "TouchAnalysisAllTheWork.h"
#include "TouchAnalysisMusic.h"
#include "TouchAnalysisLeadEnds.h"
#include "TouchAnalysisCallsMade.h"
#include "TouchAnalysisFalseness.h"

#include "RecordAnalysisTowerTab.h"

#include "PlayPosition.h"
#include "StrikingControls.h"
#include "TowerComboBtn.h"
#include "FilterPopup.h"
#include "Tower.h"
#include "NotationExplorerDlg.h"
#include "ExplorerFolder.h"	// Added by ClassView
#include "LeadHeadDlg.h"
#include "UserManager.h"
#include "RecordManager.h"
#include "TowerManager.h"
#include "SimulatorManager.h"
#include "SimulatorStrikingWnd.h"
#include "SimulatorInputSpyWnd.h"
#include "StrikeGraphCtrlBar.h"

#include "MultiBellInterfaceManager.h"
#include "PlayManager.h"
#include "ProofManager.h"
#include "KeyboardManager.h"
#include "SerialPortManager.h"
#include "SerialControlManager.h"
#include "SpeechManager.h"
#include "StrikeManager.h"
#include "StyleManager.h"
#include "MethodManager.h"
#include "CalledChangeEditorDlg.h"
#include "MainFrameEventListener.h"
#include "FilterComboBtn.h"
#include "MainFrame.h"
#include "UserMessages.h"
#include "MapManager.h"
#include "MapDoc.h"
#include "HistoryObj.h"
#include "CribDoc.h"
#include "CribManager.h"
#include "SplashScreenEx.h"
#include ".\mainframe.h"
#include "ToolbarManager.h"
#include "UndoBar.h"
#include "GenericCtrlBar.h"
#include "ChildFrm.h"
#include "MapDocumentInfo.h"
#include "GrabDocumentInfo.h"
#include "TowerDocumentInfo.h"
#include "InternetDocumentInfo.h"
#include "SimulatorBlueLineCtrlBar.h"
#include "SimulatorInputsMapWnd.h"
#include "MethodSimulatorWnd.h"
#include "SimulatorStrikingSingleBellCtrlBar.h"
#include "MatrixManager.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MainFrame ??

IMPLEMENT_DYNAMIC(MainFrame, CBCGPMDIFrameWnd)

const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;



MainFrame::MainFrame() :
_pMethodWnd(NULL),
_ribbonBar(NULL),
//todo_menuBar(NULL),
_trialState(ts_expired),
_trialDaysLeft(0),
_statusBar(NULL),
_taskPaneCtrlBar(NULL),
_curViewType(vt_null) //ensures that the update is processed first time in
{
	addEventListener(this);

	for (int i=FIRST_TOOLBAR;i<=LAST_CONTROLBAR;i++)
		_controlBars[i] = NULL;
}

MainFrame::~MainFrame()
{
	removeEventListener(this);

	for (int i=FIRST_TOOLBAR; i <= LAST_CONTROLBAR;i++)
	{
		delete _controlBars[i];
		_controlBars[i] = NULL;
	}

//todo	delete _menuBar;
//todo	_menuBar = NULL;

	delete _ribbonBar;
	_ribbonBar = NULL;

	delete _statusBar;
	_statusBar = NULL;

	delete _taskPaneCtrlBar;
	_taskPaneCtrlBar = NULL;

}

BEGIN_MESSAGE_MAP(MainFrame, CBCGPMDIFrameWnd)
	//{{AFX_MSG_MAP(MainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_WM_TIMER()
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseall)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSEALL, OnUpdateWindowCloseall)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEALL, OnUpdateWindowSaveall)
	ON_COMMAND(ID_SHOW_LEADHEAD, OnShowLeadhead)
	ON_COMMAND(ID_TOOLS_CALLCHANGEEDITOR, OnToolsCallchangeeditor)
	ON_COMMAND(ID_CRIB_OPTIONS, OnCribOptions)
	ON_COMMAND(ID_TOUCH_OPTIONS, OnTouchOptions)
	ON_COMMAND(ID_SIMULATOR_OPTIONS, OnSimulatorOptions)
	ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_PAGE_SETUP, OnUpdateFilePageSetup)

	//}}AFX_MSG_MAP

	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullScreen)

	//serial port comms
	ON_COMMAND_RANGE(FIRST_FAVORITE_COMMAND, LAST_FAVORITE_COMMAND, OnFavorite)

	ON_COMMAND_RANGE(FIRST_HISTORY_COMMAND, FIRST_HISTORY_COMMAND + HISTORY_LEN - 1, OnHistory)


	ON_UPDATE_COMMAND_UI(ID_ST_1, OnUpdateStatusBarPane)
	ON_UPDATE_COMMAND_UI(ID_ST_2, OnUpdateStatusBarPane)
	ON_UPDATE_COMMAND_UI(ID_ST_3, OnUpdateStatusBarPane)
	ON_UPDATE_COMMAND_UI(ID_ST_4, OnUpdateStatusBarPane)
	ON_UPDATE_COMMAND_UI(ID_ST_5, OnUpdateStatusBarPane)
	ON_UPDATE_COMMAND_UI(ID_ST_6, OnUpdateStatusBarPane)


	
	//to2do	get all the context sensitive stuff working
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelpIndex /*CMDIFrameWnd::OnHelp*/)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpIndex)

	
	ON_COMMAND(ID_HELP_KEYBOARDMAP, OnHelpKeyboardmap)//BCG specific
	ON_COMMAND(ID_HELP_FIND, OnAskQuestion) //BCG specific
	ON_REGISTERED_MESSAGE(BCGM_CUSTOMIZEHELP, OnBCGHelp)//BCG specific

	
	ON_MESSAGE(INTERNET_MESSAGE, OnInternetMessage)
	ON_MESSAGE(TOUCH_DOC_UPDATE_NUMBER, OnTouchDocUpdateNumber)
	ON_MESSAGE(TOUCH_DOC_UPDATE_STYLE_COLOR, OnTouchDocUpdateStyleColor)

	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)

	ON_COMMAND_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnToolsViewUserToolbar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnUpdateToolsViewUserToolbar)

	ON_COMMAND(ID_VIEW_STANDARD_TOOLBAR, OnViewStandard)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STANDARD_TOOLBAR, OnUpdateViewStandard)

	ON_COMMAND(ID_VIEW_TOOL_TOOLBAR, OnViewTool)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOL_TOOLBAR, OnUpdateViewTool)

	ON_COMMAND(ID_VIEW_TOUCH_MAIN_TOOLBAR, OnViewTouchMain)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOUCH_MAIN_TOOLBAR, OnUpdateViewTouchMain)

	ON_COMMAND(ID_VIEW_TOUCH_BLUE_LINE_TOOLBAR, OnViewTouchBlueLine)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOUCH_BLUE_LINE_TOOLBAR, OnUpdateViewTouchBlueLine)

	ON_COMMAND(ID_VIEW_TOUCH_SIMULATOR_TOOLBAR, OnViewSimulator)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOUCH_SIMULATOR_TOOLBAR, OnUpdateViewSimulator)

	ON_COMMAND(ID_VIEW_INTERNET_TOOLBAR, OnViewInternet)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INTERNET_TOOLBAR, OnUpdateViewInternet)

	ON_COMMAND(ID_VIEW_IMAGE_TOOLBAR, OnViewImage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_IMAGE_TOOLBAR, OnUpdateViewImage)

	ON_COMMAND(ID_VIEW_CRIB_TOOLBAR, OnViewCrib)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CRIB_TOOLBAR, OnUpdateViewCrib)

	ON_COMMAND(ID_VIEW_STRIKE_TOOLBAR, OnViewStrike)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STRIKE_TOOLBAR, OnUpdateViewStrike)

	ON_COMMAND(ID_VIEW_MAP_TOOLBAR, OnViewMap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_TOOLBAR, OnUpdateViewMap)

	ON_COMMAND(ID_VIEW_PLAY_TOOLBAR, OnViewPlay)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLAY_TOOLBAR, OnUpdateViewPlay)

	ON_COMMAND(ID_VIEW_PLAY_SIMULATOR_TOOLBAR, OnViewPlaySimulator)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLAY_SIMULATOR_TOOLBAR, OnUpdateViewPlaySimulator)

	ON_COMMAND(ID_VIEW_RECORD_TOOLBAR, OnViewRecord)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RECORD_TOOLBAR, OnUpdateViewRecord)

	ON_COMMAND(ID_VIEW_RECORD_LOGIC_TOOLBAR, OnViewRecordLogic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RECORD_LOGIC_TOOLBAR, OnUpdateViewRecordLogic)

	ON_COMMAND(ID_VIEW_TOUCH_SIMULATOR_CALLS, OnViewSimulatorCalls)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOUCH_SIMULATOR_CALLS, OnUpdateViewSimulatorCalls)

	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES, OnUpdateViewProperties)

	ON_COMMAND(ID_VIEW_TASKPANE, OnViewTaskPane)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TASKPANE, OnUpdateViewTaskPane)

	ON_COMMAND(ID_VIEW_ANALYSIS_STATUS, OnViewAnalysisStatus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_STATUS, OnUpdateViewAnalysisStatus)

	ON_COMMAND(ID_VIEW_ANALYSIS_MUSIC, OnViewAnalysisMusic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_MUSIC, OnUpdateViewAnalysisMusic)

	ON_COMMAND(ID_VIEW_ANALYSIS_COURSEHEAD, OnViewAnalysisCourseHead)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_COURSEHEAD, OnUpdateViewAnalysisCourseHead)

	ON_COMMAND(ID_VIEW_ANALYSIS_LEADEND, OnViewAnalysisLeadEnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_LEADEND, OnUpdateViewAnalysisLeadEnd)

	ON_COMMAND(ID_VIEW_ANALYSIS_ALLTHEWORK, OnViewAnalysisAllTheWork)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_ALLTHEWORK, OnUpdateViewAnalysisAllTheWork)

	ON_COMMAND(ID_VIEW_ANALYSIS_MULTIBLOCK, OnViewAnalysisMultiBlock)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_MULTIBLOCK, OnUpdateViewAnalysisMultiBlock)

	ON_COMMAND(ID_VIEW_ANALYSIS_CALLSMADE, OnViewAnalysisCallsMade)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_CALLSMADE, OnUpdateViewAnalysisCallsMade)

	ON_COMMAND(ID_VIEW_ANALYSIS_SPLICE, OnViewAnalysisSplice)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_SPLICE, OnUpdateViewAnalysisSplice)

	ON_COMMAND(ID_VIEW_ANALYSIS_SPLICETRANSITION, OnViewAnalysisSpliceTransition)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_SPLICETRANSITION, OnUpdateViewAnalysisSpliceTransition)

	ON_COMMAND(ID_VIEW_ANALYSIS_FALSENESS, OnViewAnalysisFalseness)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYSIS_FALSENESS, OnUpdateViewAnalysisFalseness)

	ON_COMMAND(ID_VIEW_SIMULATOR_METHOD, OnViewSimulatorMethod)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMULATOR_METHOD, OnUpdateViewSimulatorMethod)

	ON_COMMAND(ID_VIEW_SIMULATOR_STRIKING, OnViewSimulatorStriking)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMULATOR_STRIKING, OnUpdateViewSimulatorStriking)

	ON_COMMAND(ID_VIEW_SIMULATOR_STRIKING_SINGLE_BELL1, OnViewSimulatorStrikingSingleBell1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMULATOR_STRIKING_SINGLE_BELL1, OnUpdateViewSimulatorStrikingSingleBell1)

	ON_COMMAND(ID_VIEW_SIMULATOR_STRIKING_SINGLE_BELL2, OnViewSimulatorStrikingSingleBell2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMULATOR_STRIKING_SINGLE_BELL2, OnUpdateViewSimulatorStrikingSingleBell2)

	ON_COMMAND(ID_VIEW_SIMULATOR_INPUT_SPY, OnViewSimulatorInputSpy)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMULATOR_INPUT_SPY, OnUpdateViewSimulatorInputSpy)

	ON_COMMAND(ID_VIEW_SIMULATOR_INPUTS_MAP, OnViewSimulatorInputsMap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMULATOR_INPUTS_MAP, OnUpdateViewSimulatorInputsMap)

	ON_COMMAND(ID_VIEW_STRIKE_GRAPH, OnViewStrikeGraph)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STRIKE_GRAPH, OnUpdateViewStrikeGraph)

	ON_COMMAND(ID_VIEW_STRIKE_INFO, OnViewStrikeInfo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STRIKE_INFO, OnUpdateViewStrikeInfo)

	ON_COMMAND(ID_VIEW_STRIKING_CONTROLS, OnViewSimulatorStrikingControls)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STRIKING_CONTROLS, OnUpdateViewSimulatorStrikingControls)

	ON_COMMAND(ID_VIEW_HOME_TOWER, OnViewRecordHomeTower)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HOME_TOWER, OnUpdateViewRecordHomeTower)

	ON_COMMAND(ID_VIEW_SELECTED_TOWER, OnViewRecordSelectedTower)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTED_TOWER, OnUpdateViewRecordSelectedTower)

	ON_REGISTERED_MESSAGE(BCGM_CREATETOOLBAR, OnToolbarCreateNew)

	ON_COMMAND(ID_MDI_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDI_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDI_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDI_NEW_VERT_TAB_GROUP, OnMdiNewVertGroup)

	ON_MESSAGE(WM_APPLY_COMBINED_FILTER, OnApplyCombinedFilter)


	END_MESSAGE_MAP()





/////////////////////////////////////////////////////////////////////////////
// MainFrame construction/destruction



int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//get this going here, in case we do not get to the end. Should never fire before it gets set up again.
	SetTimer(AAA_TIMER_NAG, 400000, NULL);

	CSplashScreenEx::ShowSplashScreen();//if you pass in a parent, then the left click will not hide the splpash screen. 

	//do after loading splash screen
	CSplashScreenEx::SetTextStatic("Loading Favorites");
	app()->loadFavorites();


	// enable look:
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_ObsidianBlack);

	CBCGPDockManager::SetDockMode(DT_SMART);

	//CBCGPTabbedControlBar::EnableTabAutoColor (FALSE);

	CBCGPToolbarComboBoxButton::SetFlatMode ();

	CBCGPToolBar::EnableQuickCustomization ();

	CBCGPFontComboBox::m_bDrawUsingFont = TRUE;

	InitColorNames();

	CSplashScreenEx::SetTextStatic("Loading User Images");

	// Load toolbar user images:
	if (CBCGPToolBar::GetUserImages() == NULL)
	{
		if (!_userImages.Load (_T("UserImages.bmp")))
		{
			TRACE(_T("Failed to load user images\n"));
		//	ASSERT (FALSE);
		}
		else
		{
			CBCGPToolBar::SetUserImages (&_userImages);
		}
	}

	//--------------------------------------------
	// Menu will not take the focus on activation:
	//--------------------------------------------
	CBCGPPopupMenu::SetForceMenuFocus (FALSE);

	//------------------------------------------------------
	// Allow disabled menu items to be highligted (VS look):
	//------------------------------------------------------
	CBCGPMenuBar::HighlightDisabledItems ();

	//--------------------------------------------------------------
	// Load menu items images (not placed on the standard toolbars):
	//--------------------------------------------------------------
	CBCGPToolBar::AddToolBarForImageCollection (IDR_MENU_PICTURES1);
	CBCGPToolBar::AddToolBarForImageCollection (IDR_MENU_PICTURES2);
	CBCGPToolBar::AddToolBarForImageCollection (IDR_TOUCH_ANALYSIS_DROPDOWN);
	CBCGPToolBar::AddToolBarForImageCollection (IDR_RECORD_ANALYSIS_DROPDOWN);

	//--------------------------------------------------------------
	//load the global stuff from the managers
	//--------------------------------------------------------------
	CSplashScreenEx::SetTextStatic("Loading User Data");

	for (int i=0;i<app()->_managers.GetSize();i++)
		app()->_managers.GetAt(i)->load();

	for (int i=0;i<app()->_managers.GetSize();i++)
		app()->_managers.GetAt(i)->openUp();

	towerManager()->addEventListener(this);   //to2do add to a ManagerBase::openUp call -  will have to make mainFrame a ManagerBase 
	recordManager()->addEventListener(this);
	strikeManager()->addEventListener(this);
	simulatorManager()->addEventListener(this);

	
	CSplashScreenEx::SetTextStatic("Loading Toolbar Data");

		//--------------------------------------------------------------
	//images for the control bar icons .
	//--------------------------------------------------------------
	CImageList imagesWorkspace;
	imagesWorkspace.Create (IDR_TOUCH_MAIN, 16, 0, RGB (192, 192, 192));
	CImageList imagesTouchAnalysis;
	imagesTouchAnalysis.Create (IDR_TOUCH_ANALYSIS_DROPDOWN, 16, 0, RGB (192, 192, 192));
	CImageList imagesSimulator;
	imagesSimulator.Create (IDR_TOUCH_SIMULATOR, 16, 0, RGB (192, 192, 192));
	CImageList imagesRecordAnalysis;
	imagesRecordAnalysis.Create (IDR_RECORD_ANALYSIS_DROPDOWN, 16, 0, RGB (192, 192, 192));
	CImageList imagesStrike;
	imagesStrike.Create (IDR_STRIKE, 16, 0, RGB (192, 192, 192));
	CImageList imagesIcons;
	imagesIcons.Create (IDT_ICONS, 16, 0, RGB (192, 192, 192));

	//--------------------------------------------------------------
	// ribbon bar
	//--------------------------------------------------------------

	_ribbonBar = new CBCGPRibbonBar;
	if (!_ribbonBar->Create (this))
	{
		TRACE0("Failed to create _ribbonBar\n");
		return -1;      // fail to create
	}

	createRibbonBar();
	

	//--------------------------------------------------------------
	// menu bar
	//--------------------------------------------------------------
/*//todo	_menuBar = new CBCGPMenuBar;
	if (!_menuBar->Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
	_menuBar->SetBarStyle(_menuBar->GetBarStyle() | CBRS_SIZE_DYNAMIC);
// to2do	_menuBar->EnableHelpCombobox (ID_HELP_FIND, _T("Type a question for help"));
*/
	//--------------------------------------------------------------
	//status bar
	//--------------------------------------------------------------
	_statusBar = new StatusBar;
	if (!_statusBar->Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//--------------------------------------------------------------
	//task pane
	//--------------------------------------------------------------
	_taskPaneCtrlBar = new TaskPane;

	CRect rectDummy(0, 0, 200, 400);
	if (!_taskPaneCtrlBar->Create(_T("Task Pane"), this, CRect(0,0,200,200), TRUE, ID_VIEW_TASKPANE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_BCGP_CLOSE))
	{
		TRACE0("Failed to create task pane\n");
		return FALSE;      // fail to create
	}
	
	_taskPaneCtrlBar->SetIcon (imagesIcons.ExtractIcon(0), FALSE);
	


	//----------------------
	// Touch analysis drop down toolbar
	//----------------------
	if (!_touchAnalysisDropdownToolBar.Create(this,
		WS_CHILD|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_TOUCH_ANALYSIS_DROPDOWN) ||
		!_touchAnalysisDropdownToolBar.LoadToolBar (IDR_TOUCH_ANALYSIS_DROPDOWN))
	{
		TRACE0("Failed to create IDR_TOUCH_ANALYSIS_DROPDOWN toolbar\n");
		return FALSE;      // fail to create
	}

	//----------------------
	// Record analysis drop down toolbar
	//----------------------
	if (!_recordAnalysisDropdownToolBar.Create(this,
		WS_CHILD|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_RECORD_ANALYSIS_DROPDOWN) ||
		!_recordAnalysisDropdownToolBar.LoadToolBar (IDR_RECORD_ANALYSIS_DROPDOWN))
	{
		TRACE0("Failed to create IDR_RECORD_ANALYSIS_DROPDOWN toolbar\n");
		return FALSE;      // fail to create
	}



	//--------------------------------------------------------------
	// standard toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* standardToolbar = new CBCGPToolBar;
	_controlBars[tt_toolBarStandard] = standardToolbar;	

	if (!standardToolbar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_STANDARD_TOOLBAR) ||
		!standardToolbar->LoadToolBar(IDR_STANDARD))
	{
		TRACE0("Failed to create standard toolbar\n");
		return -1;      // fail to create
	}
	standardToolbar->SetWindowText ("Standard");

	//--------------------------------------------------------------
	// touch main toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* touchMainToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarTouchMain] = touchMainToolBar;	

	if (!touchMainToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |  CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_TOUCH_MAIN_TOOLBAR) ||
		!touchMainToolBar->LoadToolBar(IDR_TOUCH_MAIN))
	{
		TRACE0("Failed to create touch main toolbar\n");
		return -1;      // fail to create
	}
	touchMainToolBar->SetWindowText ("Touch");

	//--------------------------------------------------------------
	// touch blue line toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* touchBlueLineToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarTouchBlueLine] = touchBlueLineToolBar;	

	if (!touchBlueLineToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |  CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_TOUCH_BLUE_LINE_TOOLBAR) ||
		!touchBlueLineToolBar->LoadToolBar(IDR_TOUCH_BLUE_LINE))
	{
		TRACE0("Failed to create touch blue line toolbar\n");
		return -1;      // fail to create
	}
	touchBlueLineToolBar->SetWindowText ("Blue Line");

	//--------------------------------------------------------------
	// touch simulator toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* simulatorToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarSimulator] = simulatorToolBar;	

	if (!simulatorToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |  CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_TOUCH_SIMULATOR_TOOLBAR) ||
		!simulatorToolBar->LoadToolBar(IDR_TOUCH_SIMULATOR))
	{
		TRACE0("Failed to create touch simulator toolbar\n");
		return -1;      // fail to create
	}
	simulatorToolBar->SetWindowText ("Simulator");
	
	//--------------------------------------------------------------
	// simulator calls toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* simulatorCallsToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarSimulatorCalls] = simulatorCallsToolBar;	

	if (!simulatorCallsToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_TOUCH_SIMULATOR_CALLS) ||
		!simulatorCallsToolBar->LoadToolBar(IDT_CALLS))
	{
		TRACE0("Failed to create _calls logics toolbar\n");
		return -1;      // fail to create
	}
	simulatorCallsToolBar->SetWindowText ("Calls");

	//--------------------------------------------------------------
	// reocords toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* recordToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarRecord] = recordToolBar;	

	if (!recordToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_RECORD_TOOLBAR) ||
		!recordToolBar->LoadToolBar(IDR_RECORD))
	{
		TRACE0("Failed to create records toolbar\n");
		return -1;      // fail to create
	}
	recordToolBar->SetWindowText ("Records");

	//--------------------------------------------------------------
	// reocord logic toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* recordLogicToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarRecordLogic] = recordLogicToolBar;	

	if (!recordLogicToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_RECORD_LOGIC_TOOLBAR) ||
		!recordLogicToolBar->LoadToolBar(IDR_RECORD_LOGIC))
	{
		TRACE0("Failed to create record logics toolbar\n");
		return -1;      // fail to create
	}
	recordLogicToolBar->SetWindowText ("Record Logic");

	//--------------------------------------------------------------
	// crib toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* cribToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarCrib] = cribToolBar;	

	if (!cribToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_CRIB_TOOLBAR) ||
		!cribToolBar->LoadToolBar(IDR_CRIB))
	{
		TRACE0("Failed to create crib toolbar\n");
		return -1;      // fail to create
	}
	cribToolBar->SetWindowText ("Crib");

	//--------------------------------------------------------------
	// strike toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* strikeToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarStrike] = strikeToolBar;	

	if (!strikeToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_STRIKE_TOOLBAR) ||
		!strikeToolBar->LoadToolBar(IDR_STRIKE))
	{
		TRACE0("Failed to create strikeToolBar toolbar\n");
		return -1;      // fail to create
	}
	strikeToolBar->SetWindowText ("Strike");

	//--------------------------------------------------------------
	// internet toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* internetToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarInternet] = internetToolBar;	

	if (!internetToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |  CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_INTERNET_TOOLBAR) ||
		!internetToolBar->LoadToolBar(IDR_INTERNET))
	{
		TRACE0("Failed to create internet toolbar\n");
		return -1;      // fail to create
	}
	internetToolBar->SetWindowText ("Web");

	//--------------------------------------------------------------
	// image toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* imageToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarImage] = imageToolBar;	

	if (!imageToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |  CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_IMAGE_TOOLBAR) ||
		!imageToolBar->LoadToolBar(IDR_IMAGE))
	{
		TRACE0("Failed to create image toolbar\n");
		return -1;      // fail to create
	}
	imageToolBar->SetWindowText ("Image");

	//--------------------------------------------------------------
	// map toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* mapToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarMap] = mapToolBar;	

	if (!mapToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |  CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_MAP_TOOLBAR) ||
		!mapToolBar->LoadToolBar(IDR_MAP))
	{
		TRACE0("Failed to create map toolbar\n");
		return -1;      // fail to create
	}
	mapToolBar->SetWindowText ("Map");

	//--------------------------------------------------------------
	// play toolbar:
	//--------------------------------------------------------------
	RepeatingToolbar* playToolBar = new RepeatingToolbar;
	_controlBars[tt_toolBarPlay] = playToolBar;	

	if (!playToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_PLAY_TOOLBAR) ||
		!playToolBar->LoadToolBar(IDR_PLAY))
	{
		TRACE0("Failed to create play toolbar\n");
		return -1;      // fail to create
	}
	playToolBar->SetWindowText ("Play");
			  
	CArray<int, int> buttonIds;
	buttonIds.Add(ID_SLOW_DOWN);
	buttonIds.Add(ID_SPEED_UP);
	playToolBar->addRepeatingButtonsIds(buttonIds);	
	

	//--------------------------------------------------------------
	// play simulator toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* playSimulatorToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarPlaySimulator] = playSimulatorToolBar;

	if (!playSimulatorToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_PLAY_SIMULATOR_TOOLBAR) ||
		!playSimulatorToolBar->LoadToolBar(IDR_PLAY_SIMULATOR))
	{
		TRACE0("Failed to create play simulator toolbar\n");
		return -1;      // fail to create
	}
	playSimulatorToolBar->SetWindowText ("Play");

 	//--------------------------------------------------------------
	// tool toolbar:
	//--------------------------------------------------------------
	CBCGPToolBar* toolsToolBar = new CBCGPToolBar;
	_controlBars[tt_toolBarTools] = toolsToolBar;

	if (!toolsToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), ID_VIEW_TOOL_TOOLBAR) ||
		!toolsToolBar->LoadToolBar(IDR_TOOL))
	{
		TRACE0("Failed to create IDR_TOOL toolbar\n");
		return -1;      // fail to create
	}
	toolsToolBar->SetWindowText ("Tools");

	//--------------------------------------------------------------
	//property control bar.
	//--------------------------------------------------------------
	PropertyCtrlBar* propertyCtrlBar = new PropertyCtrlBar;
	_controlBars[tt_controlBarProperty] = propertyCtrlBar;

	if (!propertyCtrlBar->Create (_T("Property"), this, CRect (0, 0, 200, 200),
		TRUE/* Has gripper */, AAA_PROPERTY_BAR,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties bar\n");
		return -1;      // fail to create
	}
	propertyCtrlBar->SetIcon (imagesWorkspace.ExtractIcon(1), FALSE);

	//to2do split the favorits from the property bar.

	//--------------------------------------------------------------
	//touchAnalysisStatusCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisStatusCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisStatus] = touchAnalysisStatusCtrlBar;

	TouchAnalysisStatus* analysisStatus = new TouchAnalysisStatus();

	if ((!touchAnalysisStatusCtrlBar->Create (_T("Status"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_STATUS,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisStatus->Create(touchAnalysisStatusCtrlBar))||
		(!touchAnalysisStatusCtrlBar->setChild(analysisStatus)))
	{
		TRACE0("Failed to create ID_ANALYSIS_STATUS bar\n");
		return -1;      // fail to create
	}
	touchAnalysisStatusCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(0), FALSE);


	//--------------------------------------------------------------
	//touchAnalysisMusic control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisMusicCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisMusic] = touchAnalysisMusicCtrlBar;

	TouchAnalysisMusic* analysisMusic = new TouchAnalysisMusic();

	if ((!touchAnalysisMusicCtrlBar->Create (_T("Music"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_MUSIC,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisMusic->Create(touchAnalysisMusicCtrlBar))||
		(!touchAnalysisMusicCtrlBar->setChild(analysisMusic)))
	{
		TRACE0("Failed to create ID_ANALYSIS_MUSIC bar\n");
		return -1;      // fail to create
	}
	touchAnalysisMusicCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(2), FALSE);

	//--------------------------------------------------------------
	//touchAnalysisCourseHeadCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisCourseHeadCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisCourseHead] = touchAnalysisCourseHeadCtrlBar;

	TouchAnalysisCourseHead* analysisCourseHead = new TouchAnalysisCourseHead();

	if ((!touchAnalysisCourseHeadCtrlBar->Create (_T("Course Head"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_COURSE_HEAD,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisCourseHead->Create(touchAnalysisCourseHeadCtrlBar))||
		(!touchAnalysisCourseHeadCtrlBar->setChild(analysisCourseHead)))
	{
		TRACE0("Failed to create ID_ANALYSIS_COURSE_HEAD bar\n");
		return -1;      // fail to create
	}
	touchAnalysisCourseHeadCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(3), FALSE);

	//--------------------------------------------------------------
	//touchAnalysisLeadEndCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisLeadEndCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisLeadEnd] = touchAnalysisLeadEndCtrlBar;

	TouchAnalysisLeadEnds* analysisLeadEnd = new TouchAnalysisLeadEnds();

	if ((!touchAnalysisLeadEndCtrlBar->Create (_T("Lead End"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_LEAD_END,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisLeadEnd->Create(touchAnalysisLeadEndCtrlBar))||
		(!touchAnalysisLeadEndCtrlBar->setChild(analysisLeadEnd)))
	{
		TRACE0("Failed to create ID_ANALYSIS_LEAD_END bar\n");
		return -1;      // fail to create
	}
	touchAnalysisLeadEndCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(4), FALSE);

	//--------------------------------------------------------------
	//touchAnalysisAllTheWorkCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisAllTheWorkCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisAllTheWork] = touchAnalysisAllTheWorkCtrlBar;

	TouchAnalysisAllTheWork* analysisAllTheWork = new TouchAnalysisAllTheWork();

	if ((!touchAnalysisAllTheWorkCtrlBar->Create (_T("All The Work"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_ALL_THE_WORK,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisAllTheWork->Create(touchAnalysisAllTheWorkCtrlBar))||
		(!touchAnalysisAllTheWorkCtrlBar->setChild(analysisAllTheWork)))
	{
		TRACE0("Failed to create ID_ANALYSIS_ALL_THE_WORK bar\n");
		return -1;      // fail to create
	}
	touchAnalysisAllTheWorkCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(5), FALSE);

	//--------------------------------------------------------------
	//touchAnalysisCallsMadeCtrlBarcontrol bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisCallsMadeCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisCallsMade] = touchAnalysisCallsMadeCtrlBar;

	TouchAnalysisCallsMade* analysisCallsMade = new TouchAnalysisCallsMade();

	if ((!touchAnalysisCallsMadeCtrlBar->Create (_T("Calls Made"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_CALLS_MADE,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisCallsMade->Create(touchAnalysisCallsMadeCtrlBar))||
		(!touchAnalysisCallsMadeCtrlBar->setChild(analysisCallsMade)))
	{
		TRACE0("Failed to create ID_ANALYSIS_CALLS_MADE bar\n");
		return -1;      // fail to create
	}
	touchAnalysisCallsMadeCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(6), FALSE);

	//--------------------------------------------------------------
	//touchAnalysisMultiBlockCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisMultiBlockCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisMultiBlock] = touchAnalysisMultiBlockCtrlBar;

	TouchAnalysisMultiBlock* analysisMultiBlock = new TouchAnalysisMultiBlock();

	if ((!touchAnalysisMultiBlockCtrlBar->Create (_T("Multi Block"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_MULTI_BLOCK,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisMultiBlock->Create(touchAnalysisMultiBlockCtrlBar))||
		(!touchAnalysisMultiBlockCtrlBar->setChild(analysisMultiBlock)))
	{
		TRACE0("Failed to create ID_ANALYSIS_MULTI_BLOCK bar\n");
		return -1;      // fail to create
	}
	touchAnalysisMultiBlockCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(7), FALSE);

	//--------------------------------------------------------------
	//touchAnalysisSpliceCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisSpliceCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisSplice] = touchAnalysisSpliceCtrlBar;

	TouchAnalysisSplice* analysisSplice = new TouchAnalysisSplice();

	if ((!touchAnalysisSpliceCtrlBar->Create (_T("Splice"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_SPLICE,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisSplice->Create(touchAnalysisSpliceCtrlBar))||
		(!touchAnalysisSpliceCtrlBar->setChild(analysisSplice)))
	{
		TRACE0("Failed to create ID_ANALYSIS_SPLICE bar\n");
		return -1;      // fail to create
	}
	touchAnalysisSpliceCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(8), FALSE);

	//--------------------------------------------------------------
	//touchAnalysisSpliceTransitionCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisSpliceTransitionCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisSpliceTransition] = touchAnalysisSpliceTransitionCtrlBar;

	TouchAnalysisSpliceTransition* analysisSpliceTransition = new TouchAnalysisSpliceTransition();

	if ((!touchAnalysisSpliceTransitionCtrlBar->Create (_T("Splice Transition"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_SPLICE_TRANSITION,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisSpliceTransition->Create(touchAnalysisSpliceTransitionCtrlBar))||
		(!touchAnalysisSpliceTransitionCtrlBar->setChild(analysisSpliceTransition)))
	{
		TRACE0("Failed to create ID_ANALYSIS_SPLICE_TRANSITION bar\n");
		return -1;      // fail to create
	}
	touchAnalysisSpliceTransitionCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(9), FALSE);
	
	//--------------------------------------------------------------
	//touchAnalysisFalsenessCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* touchAnalysisFalsenessCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarTouchAnalysisFalseness] = touchAnalysisFalsenessCtrlBar;

	TouchAnalysisFalseness* analysisFalseness = new TouchAnalysisFalseness();

	if ((!touchAnalysisFalsenessCtrlBar->Create (_T("Falseness"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_ANALYSIS_FALSENESS,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!analysisFalseness->Create(touchAnalysisFalsenessCtrlBar))||
		(!touchAnalysisFalsenessCtrlBar->setChild(analysisFalseness)))
	{
		TRACE0("Failed to create ID_ANALYSIS_FALSENESS bar\n");
		return -1;      // fail to create
	}
	touchAnalysisFalsenessCtrlBar->SetIcon (imagesTouchAnalysis.ExtractIcon(1), FALSE);

	//--------------------------------------------------------------
	//simulatorMethodCtrlBar control bar.
	//--------------------------------------------------------------
	SimulatorBlueLineCtrlBar* simulatorMethodCtrlBar = new SimulatorBlueLineCtrlBar();
	_controlBars[tt_controlBarSimulatorMethod] = simulatorMethodCtrlBar;

	MethodSimulatorWnd* methodSimulatorWnd = new MethodSimulatorWnd();

	if ((!simulatorMethodCtrlBar->Create (_T("Method"), this,  CRect (0, 0, 200, 200),
		TRUE /* Has gripper */, ID_SIMULATOR_METHOD,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) ||
		(!methodSimulatorWnd->Create(NULL, "", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 
			CRect(0,0,0,0),  simulatorMethodCtrlBar, ID_SIMULATOR_METHOD, NULL))||
		(!simulatorMethodCtrlBar->setChild(methodSimulatorWnd)))
	{
		TRACE0("Failed to create ID_SIMULATOR_METHOD bar\n");
		return -1;      // fail to create
	}
	simulatorMethodCtrlBar->SetIcon (imagesSimulator.ExtractIcon(5), FALSE); 

	//--------------------------------------------------------------
	//simulatorStrikingCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* simulatorStrikingCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarSimulatorStriking] = simulatorStrikingCtrlBar;

	SimulatorStrikingWnd* simulatorStrikingWnd = new SimulatorStrikingWnd(sswt_all);

	if ((!simulatorStrikingCtrlBar->Create (_T("Striking"), this,  CRect (0, 0, 230, 200),
		TRUE /* Has gripper */, ID_SIMULATOR_STRIKING,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!simulatorStrikingWnd ->Create(simulatorStrikingCtrlBar, ID_SIMULATOR_STRIKING))||
		(!simulatorStrikingCtrlBar->setChild(simulatorStrikingWnd)))
	{
		TRACE0("Failed to create ID_SIMULATOR_STRIKING bar\n");
		return -1;      // fail to create
	}
	simulatorStrikingCtrlBar->SetIcon (imagesSimulator.ExtractIcon(6), FALSE); 

	//--------------------------------------------------------------
	//simulatorStrikingSingleBell1CtrlBar control bar.
	//--------------------------------------------------------------
	SimulatorStrikingSingleBellCtrlBar* simulatorStrikingSingleBell1CtrlBar = new SimulatorStrikingSingleBellCtrlBar(sswt_single_1);
	_controlBars[tt_controlBarSimulatorSingleBell1] = simulatorStrikingSingleBell1CtrlBar;

	if ((!simulatorStrikingSingleBell1CtrlBar->Create(_T("Single Bell Striking 1"), this,  CRect (0, 0, 230, 200),
		TRUE /* Has gripper */, ID_SIMULATOR_STRIKING_SINGLE_BELL1,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)))
	{
		TRACE0("Failed to create ID_SIMULATOR_STRIKING_SINGLE_BELL1 bar\n");
		return -1;      // fail to create
	}
	simulatorStrikingSingleBell1CtrlBar->SetIcon (imagesSimulator.ExtractIcon(7), FALSE);  

	//--------------------------------------------------------------
	//simulatorStrikingSingleBell2CtrlBar control bar.
	//--------------------------------------------------------------
	SimulatorStrikingSingleBellCtrlBar* simulatorStrikingSingleBell2CtrlBar = new SimulatorStrikingSingleBellCtrlBar(sswt_single_2);
	_controlBars[tt_controlBarSimulatorSingleBell2] = simulatorStrikingSingleBell2CtrlBar;

	if ((!simulatorStrikingSingleBell2CtrlBar->Create (_T("Single Bell Striking 2"), this,  CRect (0, 0, 230, 200),
		TRUE /* Has gripper */, ID_SIMULATOR_STRIKING_SINGLE_BELL2,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_BCGP_AUTO_ROLLUP| CBRS_FLOAT_MULTI)))
	{
		TRACE0("Failed to create ID_SIMULATOR_STRIKING_SINGLE_BELL2 bar\n");
		return -2;      // fail to create
	}
	simulatorStrikingSingleBell2CtrlBar->SetIcon (imagesSimulator.ExtractIcon(8), FALSE); 

    //--------------------------------------------------------------
	//simulatorInputSpyCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* simulatorInputSpyCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarSimulatorInputSpy] = simulatorInputSpyCtrlBar;

	SimulatorInputSpyWnd* simulatorInputSpyWnd = new SimulatorInputSpyWnd();

	if ((!simulatorInputSpyCtrlBar->Create (_T("Input Spy"), this,  CRect (0, 0, 230, 200),
		TRUE /* Has gripper */, ID_SIMULATOR_INPUT_SPY,
		WS_CHILD |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) ||
		(!simulatorInputSpyWnd ->Create(simulatorInputSpyCtrlBar, ID_SIMULATOR_INPUT_SPY))||
		(!simulatorInputSpyCtrlBar->setChild(simulatorInputSpyWnd)))
	{
		TRACE0("Failed to create ID_SIMULATOR_INPUT_SPY bar\n");
		return -1;      // fail to create
	}
	simulatorInputSpyCtrlBar->SetIcon (imagesSimulator.ExtractIcon(3), FALSE);

	//--------------------------------------------------------------
	//simulatorInputsMapCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* simulatorInputsMapCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarSimulatorInputsMap] = simulatorInputsMapCtrlBar;

	SimulatorInputsMapWnd* simulatorInputsMapWnd = new SimulatorInputsMapWnd();

	if ((!simulatorInputsMapCtrlBar->Create (_T("Input Map"), this,  CRect (0, 0, 230, 200),
		TRUE /* Has gripper */, ID_SIMULATOR_INPUT_MAP,
		WS_CHILD |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) ||
		(!simulatorInputsMapWnd ->Create(simulatorInputsMapCtrlBar, ID_SIMULATOR_INPUT_SPY))||
		(!simulatorInputsMapCtrlBar->setChild(simulatorInputsMapWnd)))
	{
		TRACE0("Failed to create ID_SIMULATOR_INPUT_MAP bar\n");
		return -1;      // fail to create
	}
	simulatorInputsMapCtrlBar->SetIcon (imagesSimulator.ExtractIcon(2), FALSE);

    //--------------------------------------------------------------
	//_strikeGraphCtrlBar control bar.
	//--------------------------------------------------------------
	StrikeGraphCtrlBar* strikeGraphCtrlBar = new StrikeGraphCtrlBar();
	_controlBars[tt_controlBarStrikeGraph] = strikeGraphCtrlBar;

	if ((!strikeGraphCtrlBar->Create (_T("Striking Distribution Graph"), this,  CRect (0, 0, 230, 200),
		TRUE /* Has gripper */, ID_STRIKE_GRAPH,
		WS_CHILD |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)))
		
	{
		TRACE0("Failed to create ID_STRIKE_GRAPH bar\n");
		return -1;      // fail to create		 
	}
	strikeGraphCtrlBar->SetIcon (imagesStrike.ExtractIcon(0), FALSE);

    //--------------------------------------------------------------
	//strikeInfoCtrlBar control bar.
	//--------------------------------------------------------------
	StrikeInfoCtrlBar* strikeInfoCtrlBar = new StrikeInfoCtrlBar();
	_controlBars[tt_controlBarStrikeInfo] = strikeInfoCtrlBar;

	if ((!strikeInfoCtrlBar->Create (_T("Information"), this,  CRect (0, 0, 230, 200),
		TRUE /* Has gripper */, ID_STRIKE_INFO,
		WS_CHILD |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)))
		
	{
		TRACE0("Failed to create ID_STRIKE_INFO bar\n");
		return -1;      // fail to create		 
	}
	strikeInfoCtrlBar->SetIcon (imagesStrike.ExtractIcon(1), FALSE);

	//--------------------------------------------------------------
	//_recordHomeTowerCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* recordHomeTowerCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarRecordHomeTower] = recordHomeTowerCtrlBar;

	RecordAnalysisTowerTab* recordHomeTower = new RecordAnalysisTowerTab(ratt_home);

	if ((!recordHomeTowerCtrlBar->Create (_T("Home Tower"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_RECORD_HOME_TOWER,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!recordHomeTower->Create(recordHomeTowerCtrlBar))||
		(!recordHomeTowerCtrlBar->setChild(recordHomeTower)))
	{
		TRACE0("Failed to create ID_RECORD_HOME_TOWER bar\n");
		return -1;      // fail to create
	}
	recordHomeTowerCtrlBar->SetIcon (imagesRecordAnalysis.ExtractIcon(0), FALSE);

	//--------------------------------------------------------------
	//_recordSelectedTowerCtrlBar control bar.
	//--------------------------------------------------------------
	GenericCtrlBar* recordSelectedTowerCtrlBar = new GenericCtrlBar();
	_controlBars[tt_controlBarRecordSelectedTower] = recordSelectedTowerCtrlBar;

	RecordAnalysisTowerTab* recordSelectedTower = new RecordAnalysisTowerTab(ratt_selected);

	if ((!recordSelectedTowerCtrlBar->Create (_T("Selected Tower"), this,  CRect (0, 0, 400, 200),
		TRUE /* Has gripper */, ID_RECORD_SELECTED_TOWER,
		WS_CHILD | WS_VISIBLE |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI)) ||
		(!recordSelectedTower->Create(recordSelectedTowerCtrlBar))||
		(!recordSelectedTowerCtrlBar->setChild(recordSelectedTower)))
	{
		TRACE0("Failed to create ID_RECORD_SELECTED_TOWER bar\n");
		return -1;      // fail to create
	}
	recordSelectedTowerCtrlBar->SetIcon (imagesRecordAnalysis.ExtractIcon(1), FALSE);



 	//--------------------------------------------------------------
	// Play Position
	//--------------------------------------------------------------
	PlayPosition* playPositionCtrlBar = new PlayPosition();
	_controlBars[tt_controlBarPlayPosition] = playPositionCtrlBar;


	if (!playPositionCtrlBar->Create (_T("Play Info"), this,
		CRect( 400, 90, 1000,1000), 
		TRUE ,
		AAA_PLAY_POS_BAR,
		WS_CHILD ,
		CBRS_BCGP_REGULAR_TABS ,
		CBRS_BCGP_CLOSE | CBRS_BCGP_FLOAT | CBRS_BCGP_AUTO_ROLLUP))
	{
		TRACE0("Failed to create PlayPosition bar\n");
		return -1;      // fail to create
	}

	//--------------------------------------------------------------
	// Striking Controls
	//--------------------------------------------------------------
	StrikingControls* simulatorStrikingControlsCtrlBar = new StrikingControls();
	_controlBars[tt_controlBarSimulatorStrikingControls] = simulatorStrikingControlsCtrlBar;

	if (!simulatorStrikingControlsCtrlBar->Create (_T("Striking Controls"), this,
		CRect( 500, 290, 1000,1000), 
		TRUE ,
		ID_VIEW_STRIKING_CONTROLS,
		WS_CHILD ,
		CBRS_BCGP_REGULAR_TABS ,
		CBRS_BCGP_CLOSE | CBRS_BCGP_FLOAT | CBRS_BCGP_AUTO_ROLLUP))
	{
		TRACE0("Failed to create IDR_STRIKING_CONTROLS bar\n");
		return -1;      // fail to create
	}
	simulatorStrikingControlsCtrlBar->SetIcon (imagesSimulator.ExtractIcon(4), FALSE); 

	//--------------------------------------------------------------
	// debug
	//--------------------------------------------------------------
	for (int i=FIRST_TOOLBAR;i<=LAST_CONTROLBAR;i++)
	{
		ASSERT(_controlBars[i] != 0);
	}                

	//--------------------------------------------------------------
	// allow control bars to dock
	//--------------------------------------------------------------
	for (int i = FIRST_TOOLBAR; i <= LAST_CONTROLBAR;i++)
		_controlBars[i]->EnableDocking(CBRS_ALIGN_ANY); 

//todo	_menuBar->EnableDocking(CBRS_ALIGN_ANY);
	_touchAnalysisDropdownToolBar.EnableDocking(CBRS_ALIGN_ANY);
	_recordAnalysisDropdownToolBar.EnableDocking(CBRS_ALIGN_ANY);
	_taskPaneCtrlBar->EnableDocking( CBRS_ALIGN_RIGHT); 
	playPositionCtrlBar->EnableDocking (0); //for roll up //already done above - change from default 
	simulatorStrikingControlsCtrlBar->EnableDocking (0); //for roll up //already done above - change from default


	//--------------------------------------------------------------
	// allow application to dock
	//--------------------------------------------------------------
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);


	//--------------------------------------------------------------
	// set up docking positions
	//--------------------------------------------------------------
	CSplashScreenEx::SetTextStatic("Docking Toolbars");

	//menu
//todo	DockControlBar(_menuBar);

	/*****************************************************************\
	* NOTE: toolbar docking done in MainFrame::showDefaultVisibility *
	\*****************************************************************/

	CBCGPDockingControlBar* pTabbedBar = NULL;

	//main window
	DockControlBar(_controlBars[tt_controlBarProperty]);
	DockControlBar(_taskPaneCtrlBar);

		//strike control bar
	DockControlBar(strikeInfoCtrlBar);
	strikeGraphCtrlBar->AttachToTabWnd (strikeInfoCtrlBar, DM_SHOW, TRUE, &pTabbedBar);

	//analysis 	
	DockControlBar(touchAnalysisFalsenessCtrlBar);

	touchAnalysisStatusCtrlBar->DockToWindow (touchAnalysisFalsenessCtrlBar, CBRS_ALIGN_LEFT);

	touchAnalysisMultiBlockCtrlBar->AttachToTabWnd (touchAnalysisFalsenessCtrlBar, DM_SHOW, TRUE, &pTabbedBar);
	touchAnalysisSpliceCtrlBar->AttachToTabWnd (touchAnalysisFalsenessCtrlBar, DM_SHOW, TRUE, &pTabbedBar);
	touchAnalysisSpliceTransitionCtrlBar->AttachToTabWnd (touchAnalysisFalsenessCtrlBar, DM_SHOW, TRUE, &pTabbedBar);

	touchAnalysisMusicCtrlBar->AttachToTabWnd (touchAnalysisStatusCtrlBar, DM_SHOW, TRUE, &pTabbedBar);
	touchAnalysisCourseHeadCtrlBar->AttachToTabWnd (touchAnalysisStatusCtrlBar, DM_SHOW, TRUE, &pTabbedBar);
	touchAnalysisLeadEndCtrlBar->AttachToTabWnd (touchAnalysisStatusCtrlBar, DM_SHOW, TRUE, &pTabbedBar);
	touchAnalysisAllTheWorkCtrlBar->AttachToTabWnd (touchAnalysisStatusCtrlBar, DM_SHOW, TRUE, &pTabbedBar);
	touchAnalysisCallsMadeCtrlBar->AttachToTabWnd (touchAnalysisStatusCtrlBar, DM_SHOW, TRUE, &pTabbedBar);

	//record control bars
	pTabbedBar = NULL;
	DockControlBar(recordHomeTowerCtrlBar);
	recordSelectedTowerCtrlBar->DockToWindow (recordHomeTowerCtrlBar, CBRS_ALIGN_RIGHT);



	//simulator control bars
	DockControlBar(simulatorInputSpyCtrlBar);
	pTabbedBar = NULL;
	simulatorInputsMapCtrlBar->AttachToTabWnd (simulatorInputSpyCtrlBar, DM_SHOW, TRUE, &pTabbedBar);
	simulatorMethodCtrlBar->AttachToTabWnd (simulatorInputSpyCtrlBar, DM_SHOW, TRUE, &pTabbedBar);
	DockControlBar(simulatorStrikingCtrlBar);
	DockControlBar(simulatorStrikingSingleBell2CtrlBar);
	simulatorStrikingSingleBell1CtrlBar->DockToWindow (simulatorStrikingSingleBell2CtrlBar, CBRS_ALIGN_LEFT);



	playPositionCtrlBar->FloatControlBar(CRect(400, 90,1000, 1000)); 
	playPositionCtrlBar->ShowControlBar(FALSE, FALSE, FALSE);

	simulatorStrikingControlsCtrlBar->FloatControlBar(CRect(500, 290,1000, 1000));
	simulatorStrikingControlsCtrlBar->ShowControlBar(FALSE, FALSE, FALSE);



	m_dockManager.AdjustDockingLayout();
	//--------------------------------------------------------------
	// Allow user-defined toolbars operations:
	//--------------------------------------------------------------
	InitUserToobars (NULL,
					uiFirstUserToolBarId,
					uiLastUserToolBarId);


	//--------------------------------------------------------------
	// allow customisation
	//--------------------------------------------------------------
	for (int i = FIRST_TOOLBAR; i < FIRST_CONTROLBAR;i++)
		((CBCGPToolBar*)_controlBars[i])->EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("&Customize...")); 

	//--------------------------------------------------------------
	// Enable windows manager:
	//--------------------------------------------------------------
	EnableWindowsDialog (ID_WINDOW_MANAGER, _T("Windows..."), TRUE);

	//--------------------------------------------------------------
	// Enable full screen:
	//--------------------------------------------------------------
	EnableFullScreenMode (ID_VIEW_FULLSCREEN);


	CSplashScreenEx::SetTextStatic("Creating Fonts");

	//--------------------------------------------------------------
	// Create vertical font for menu
	//--------------------------------------------------------------

	CFont font;
	if (font.CreateStockObject (DEFAULT_GUI_FONT))
	{
		LOGFONT lf;
		memset (&lf, 0, sizeof (LOGFONT));

		if (font.GetLogFont (&lf) != 0)
		{
			lf.lfOrientation = 900;
			lf.lfEscapement = 900;

			lf.lfHeight = -15;
			lf.lfWeight = 750;
			lf.lfItalic = TRUE;
			_tcscpy (lf.lfFaceName, _T("Arial"));

			_fontVertCaption.CreateFontIndirect (&lf);
		}
	}


	//timer for the map cache
#ifdef _DEBUG
	SetTimer(MAP_REQ_TIMER_EVENT, 10000, NULL); //10 seconds
#else
	SetTimer(MAP_REQ_TIMER_EVENT, 100000, NULL); //100 seconds
#endif

		//-----------------------
	// Set up Favorites menu:
	//-----------------------
	VERIFY (theApp.m_Favorites.CreateMenu (m_menuFavotites));


	//check and save the information about the trial period
	if (verifyKey() != TRUE)
	{

		///GUID	 key

/*		//version 1.07
		char startKey[] = "CLSID\\{FB179906-0ABE-11d6-BB7F-F4C7A5569667}";
		char endKey[]   = "CLSID\\{8D3F98D0-0B42-11d6-B550-000255162CF0}";

		//version 1.08
		char startKey[] = "CLSID\\{3F97ABB0-9C8B-11d6-BB83-CE0C3DBACA33}";
		char endKey[]   = "CLSID\\{4809D740-9C8B-11d6-BB83-CE0C3DBACA33}";

		//version 2.01
		char startKey[] = "CLSID\\{2B79E981-A3AC-11d6-B5F2-F4C7A5569667}";
		char endKey[]   = "CLSID\\{3FB92581-A3AC-11d6-B5F2-000255162CF0}";

		//version 3.01
		char startKey[] = "CLSID\\{9B47DB7D-E906-11d7-B704-000255162CF0}";
		char endKey[]   = "CLSID\\{C14C2CA3-E906-11d7-B704-000255162CF0}";
*/
		//version 3.02
		char startKey[] = "CLSID\\{AC06356D-93A9-45e8-A2A2-4EDE5A583513}";
		char endKey[]   = "CLSID\\{5A64CE25-3D53-4aaa-B581-6F8BB43D52ED}";


		long buffSize = 255;
		LONG retValue;
		 char startBuff[255];
		 char endBuff[255];
		retValue = RegQueryValue( HKEY_CLASSES_ROOT, startKey, /*0, &type,*/ startBuff, &buffSize );

		if (retValue == ERROR_SUCCESS)
		{
			//we have found the start date now find the end date
			RegQueryValue( HKEY_CLASSES_ROOT, endKey, /*0, &type,*/ endBuff, &buffSize );

			//recreate the ole times
			CString str;
			str = startBuff;
			double start = atof(str);
			COleDateTime retrievedStartTime(start);
			str = endBuff;
			double end = atof(str);
			COleDateTime retrievedEndTime(end);

			COleDateTime now(COleDateTime::GetCurrentTime());

			//now get the span
			COleDateTimeSpan span(retrievedStartTime - now);
			_trialDaysLeft = span.GetDays() + 30;
/*		for debug
			str.Format("%d days", _trialDaysLeft);
			AfxMessageBox(str);
  */
			//has teh clock been tampered with?
			if ((end > now)||(_trialDaysLeft > 30))
			{
				_trialState = ts_tampered;
			}
			else if(_trialDaysLeft <= 0)
			{
				_trialState = ts_expired;

			}
			else
			{
				_trialState = ts_ok;
			}
		}
		else
		{
			//we are either running for the first time, OR we cant access this part of the reg
			COleDateTime time(COleDateTime::GetCurrentTime());
			CString startDate;
			startDate.Format("%f", time.m_dt);

			//attempt to write the start date
			retValue = RegSetValue(HKEY_CLASSES_ROOT, startKey,/* 0,*/ REG_SZ,
									 (const char*)startDate.GetBuffer(startDate.GetLength()),
							 		 startDate.GetLength());
			startDate.ReleaseBuffer();

			_trialDaysLeft = 30;

			if (retValue == ERROR_SUCCESS)
			{
				//we must be running for the first time
				_trialState = ts_ok;
			}
			else
			{
				//we do not have access to the reg
				_trialState = ts_no_reg_access;
			}
		}

		if (_trialState == ts_ok)
		{
			//write the end date
			COleDateTime time(COleDateTime::GetCurrentTime());
			CString endDate;
			endDate.Format("%f", time.m_dt);

			//attempt to write the start date
			retValue = RegSetValue(HKEY_CLASSES_ROOT, endKey, /*0, */REG_SZ,
									 (const char*)endDate.GetBuffer(endDate.GetLength()),
									 endDate.GetLength());
			endDate.ReleaseBuffer();
		}
	}   

	//switch the splash screen for the tip dlg. 
	CSplashScreenEx::HideSplashScreen();

	TipDlg dlg(mainFrame());
	dlg.startAsSplash();
	
	//get the nag screen going
	OnTimer(AAA_TIMER_NAG);
	UpdateWindow();

	return 0;
}

void MainFrame::createRibbonBar() 
{

	//--------------------------------------------------------------
	// Common Setup
	//--------------------------------------------------------------
	_ribbonPanelImages.SetImageSize (CSize (16, 16));
	_ribbonPanelImages.Load (IDB_RIBBON_ICONS);




	//--------------------------------------------------------------
	// Main Button
	//--------------------------------------------------------------
	_mainButton.SetImage (IDB_RIBBON_MAIN);
	_mainButton.SetToolTipText (_T("File"));
	_mainButton.SetText (_T("\nf"));

	_ribbonBar->SetMainButton (&_mainButton, CSize (45, 45));

	CBCGPRibbonMainPanel* mainButtonCategory = _ribbonBar->AddMainCategory (
		_T("File"), IDB_RIBBON_FILESMALL, IDB_RIBBON_FILELARGE);

	mainButtonCategory->Add (new CBCGPRibbonButton (ID_FILE_NEW, _T("&New"), 0, 0));
	mainButtonCategory->Add (new CBCGPRibbonButton (ID_FILE_OPEN, _T("&Open..."), 1, 1));
	mainButtonCategory->Add (new CBCGPRibbonButton (ID_FILE_SAVE, _T("&Save"), 2, 2));
	mainButtonCategory->Add (new CBCGPRibbonButton (ID_FILE_SAVE_AS, _T("Save &As..."), 3, 3));

	CBCGPRibbonButton* printBtn = new CBCGPRibbonButton (ID_FILE_PRINT, _T("&Print"), 4, 4);
	printBtn->AddSubItem (new CBCGPRibbonLabel (_T("Preview and print the document")));
	printBtn->AddSubItem (new CBCGPRibbonButton (ID_FILE_PRINT, _T("&Print"), 4, 4, TRUE));
	printBtn->AddSubItem (new CBCGPRibbonButton (ID_FILE_PRINT_DIRECT, _T("&Quick Print"), 7, 7, TRUE));
	printBtn->AddSubItem (new CBCGPRibbonButton (ID_FILE_PRINT_PREVIEW, _T("Print Pre&view"), 6, 6, TRUE));

	printBtn->SetKeys (_T("p"), _T("w"));

	mainButtonCategory->Add (printBtn);

	mainButtonCategory->Add (new CBCGPRibbonSeparator (TRUE));
	mainButtonCategory->Add (new CBCGPRibbonButton (ID_FILE_CLOSE, _T("&Close"), 5, 5));

	mainButtonCategory->AddRecentFilesList (_T("Recent Documents"));

	mainButtonCategory->AddToBottom (new CBCGPRibbonMainPanelButton (ID_TOOLS_OPTIONS, _T("Opt&ions"), 9));
	mainButtonCategory->AddToBottom (new CBCGPRibbonMainPanelButton (ID_APP_EXIT, _T("E&xit"), 8));




	//--------------------------------------------------------------
	// Home Category
	//--------------------------------------------------------------
	CBCGPRibbonCategory* homeCategory = _ribbonBar->AddCategory (
		_T("&Home"),IDB_RIBBON_HOMESMALL,IDB_RIBBON_HOMELARGE);

	//-------------------------
	// Home Category : Clipboard Panel
	//-------------------------
	CBCGPRibbonPanel* homeClipboardPanel = homeCategory->AddPanel (
		_T("Clipboard"), _ribbonPanelImages.ExtractIcon (1));

	CBCGPRibbonButton* pBtnPaste = new CBCGPRibbonButton (ID_EDIT_PASTE, _T("Paste\nv"), 0, 0);
//todo	pBtnPaste->SetMenu (IDR_PASTE_MENU, TRUE);
	homeClipboardPanel->Add (pBtnPaste);

	homeClipboardPanel->Add (new CBCGPRibbonButton (ID_EDIT_CUT, _T("Cut\nx"), 1));
	homeClipboardPanel->Add (new CBCGPRibbonButton (ID_EDIT_COPY, _T("Copy\nc"), 2));
	homeClipboardPanel->Add (new CBCGPRibbonButton (ID_EDIT_CLEAR, _T("Erase\nr"), 3));

	homeClipboardPanel->SetKeys (_T("zc"));

	//-------------------------
	// Home Category : Module Panel
	//-------------------------
	CBCGPRibbonPanel* homeModulePanel = homeCategory->AddPanel (
		_T("Module"));

	homeModulePanel->Add (new CBCGPRibbonButton (ID_TOWER_DATABASE, _T("Towers"), 4));
	homeModulePanel->Add (new CBCGPRibbonButton (ID_GRABS, _T("Grabs"), 5));
	homeModulePanel->Add (new CBCGPRibbonButton (ID_TOOLS_CALLCHANGEEDITOR, _T("Called Changes"), 6));
	homeModulePanel->Add (new CBCGPRibbonSeparator);
	homeModulePanel->Add (new CBCGPRibbonButton (ID_TOOLS_DATABASEMAINTAINANCE, _T("Methods"), 7));
	homeModulePanel->Add (new CBCGPRibbonButton (ID_SHOW_LEADHEAD, _T("Lead Heads"), 8));

	//-------------------------
	// Home Category : Window Panel
	//-------------------------
	CBCGPRibbonPanel* homeWindowPanel = homeCategory->AddPanel (
		_T("Window"), _ribbonPanelImages.ExtractIcon (2));

	homeWindowPanel->Add (new CBCGPRibbonCheckBox (ID_VIEW_WORKSPACE, _T("View 1\n1")));
	homeWindowPanel->Add (new CBCGPRibbonCheckBox (222, _T("View 2\n2"))); //todo
	homeWindowPanel->Add (new CBCGPRibbonCheckBox (ID_VIEW_OUTPUT, _T("Output\no")));
	homeWindowPanel->Add (new CBCGPRibbonSeparator);

	CBCGPRibbonButton* windowsBtn = new CBCGPRibbonButton (0, _T("Windows\nw"), -1, 1);
	windowsBtn->SetMenu (IDR_WINDOWS_MENU, TRUE);
	homeWindowPanel->Add (windowsBtn);

	// Add the launch button at the bottom of the panel:
	homeWindowPanel->EnableLaunchButton (ID_WINDOW_MANAGER, -1, _T("l"));

	homeWindowPanel->SetKeys (_T("zw"));






	//--------------------------------------------------------------
	// Play Category
	//--------------------------------------------------------------
	CBCGPRibbonCategory* playCategory = _ribbonBar->AddCategory (
		_T("Play"), 
		0 , 
		0 );

	//TODO


	//--------------------------------------------------------------
	// Add some hidden (non-ribbon) elements:
	//--------------------------------------------------------------
	CBCGPRibbonUndoButton* editUndo = new CBCGPRibbonUndoButton (ID_EDIT_UNDO, _T("Undo"), 4);

	editUndo->AddUndoAction (_T("Undo Item 1"));
	editUndo->AddUndoAction (_T("Undo Item 2"));
	editUndo->AddUndoAction (_T("Undo Item 3"));
	editUndo->AddUndoAction (_T("Undo Item 4"));
	editUndo->AddUndoAction (_T("Undo Item 5"));

	homeCategory->AddHidden (editUndo);


	
	//--------------------------------------------------------------
	// Quick access toolbar commands
	//--------------------------------------------------------------
	CBCGPRibbonQATDefaultState qatState;

	qatState.AddCommand (ID_FILE_NEW, FALSE);
	qatState.AddCommand (ID_FILE_OPEN, FALSE);
	qatState.AddCommand (ID_FILE_SAVE);
	qatState.AddCommand (ID_FILE_PRINT_DIRECT);
	qatState.AddCommand (ID_FILE_PRINT_PREVIEW, FALSE);
	qatState.AddCommand (ID_EDIT_UNDO);

	_ribbonBar->SetQuickAccessDefaultState (qatState);

	// Add "Style" button to the right of tabs:
/* TODO?
	CBCGPRibbonButton* pStyleButton = new CBCGPRibbonButton (-1, _T("Style\ns"), -1, -1);
	pStyleButton->SetMenu (IDR_THEME_MENU, TRUE);// Right align

	_ribbonBar->AddToTabs (pStyleButton);
*/
	// Add "About" button to the right of tabs:
	_ribbonBar->AddToTabs (new CBCGPRibbonButton (ID_APP_ABOUT, _T("\na"), _ribbonPanelImages.ExtractIcon (0)));

}



/////////////////////////////////////////////////////////////////////////////
// MainFrame diagnostics

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MainFrame message handlers

void MainFrame::OnViewFullScreen()
{
	ShowFullScreen ();
}

void MainFrame::OnUpdateStatusBarPane(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

void MainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
		TRUE /* Automatic menus scaning */,
		(BCGCUSTOMIZE_MENU_SHADOWS |
		 BCGCUSTOMIZE_TEXT_LABELS |
		 BCGCUSTOMIZE_MENU_ANIMATIONS |
		 BCGCUSTOMIZE_CONTEXT_HELP  )
		);

	CBCGPToolBar::SetCustomizeMode()	;


	pDlgCust->EnableUserDefinedToolbars ();

	//-------------------------
	// Setup undo/redo buttons:
	//-------------------------
	pDlgCust->ReplaceButton (ID_EDIT_UNDO, UndoButton (ID_EDIT_UNDO, _T("&Undo")));
	pDlgCust->ReplaceButton (ID_EDIT_REDO, UndoButton (ID_EDIT_REDO, _T("&Redo")));


	//-------------------------
	// File new menu button
	//-------------------------

	CMenu menu;
	menu.LoadMenu (IDR_FILE_NEW);
	CMenu* pPopup = menu.GetSubMenu(0);

	CBCGPToolbarMenuButton btnNew((UINT)ID_FILE_NEW, // No default command
			pPopup->GetSafeHmenu (),
			CImageHash::GetImageOfCommand (ID_FILE_NEW, FALSE),
			_T("&New"));

	pDlgCust->ReplaceButton (ID_FILE_NEW,btnNew);

	//-----------------------------------
	// Add dropdown resources button:
	//-----------------------------------
	//to2do are there any more items that need putting in here?
	pDlgCust->AddButton (_T("Touch"), CBCGPDropDownToolbarButton (_T("Touch Analysis Dropdown"),  &_touchAnalysisDropdownToolBar));
	pDlgCust->AddButton (_T("Records"), CBCGPDropDownToolbarButton (_T("Records Analysis Dropdown"), &_recordAnalysisDropdownToolBar));

	//-----------------------------------
	// Remove the dummy buttons
	//-----------------------------------
	pDlgCust->RemoveButton(_T("View"),ID_VIEW_TOOLBARS)	;

	//-------------------------
	// Setup tower buttons:
	//-------------------------
	pDlgCust->ReplaceButton (ID_TOWER, TowerComboBtn());
	towerManager()->fireUpdateTowerList();

	//-------------------------
	// Setup strike buttons:
	//-------------------------
	pDlgCust->ReplaceButton(ID_DUMMY_STRIKE_SET_BELL, StrikeComboBtn());

	//-------------------------
	// Setup strike buttons:
	//-------------------------

	pDlgCust->ReplaceButton(ID_DUMMY_SIMULATOR_SET_BELL, SimulatorComboBtn1());

	//-------------------------
	// Setup filter buttons:
	//-------------------------
	pDlgCust->ReplaceButton (ID_FILTER, FilterComboBtn());
	recordManager()->fireUpdateFilterList();

	pDlgCust->Create ();

}

LRESULT MainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (BCG_GET_X_LPARAM(lp), BCG_GET_Y_LPARAM(lp));

	CMenu menu;
	loadToolbarMenu(menu);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

void MainFrame::loadToolbarMenu(CMenu &menu)
{
	ViewType viewType = app()->getViewType();

	switch (viewType)
	{
	case vt_touch_main:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_TOUCH_MAIN));
		break;
	case vt_touch_blue_line:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_TOUCH_BLUE_LINE));
		break;
	case vt_touch_simulator:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_TOUCH_SIMULATOR));
		break;
	case vt_grab:
	case vt_tower:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_RECORD));
		break;
	case vt_map:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_MAP));
		break;
	case vt_internet:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_INTERNET));
		break;
	case vt_image:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_IMAGE));
		break;
	case vt_crib:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_CRIB));
		break;
	case vt_strike:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_STRIKE));
		break;
	default:
		VERIFY(menu.LoadMenu (IDR_TOOLBAR_DEFAULT));
		break;
	}
}

void MainFrame::OnToolsViewUserToolbar (UINT uiId)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (uiId - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ShowControlBar (pUserToolBar, !(pUserToolBar->IsVisible()), FALSE, FALSE);
	RecalcLayout ();
}

void MainFrame::OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (pCmdUI->m_nID - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	pCmdUI->Enable ();
	pCmdUI->SetCheck (pUserToolBar->IsVisible());
}

LRESULT MainFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	UINT uiToolBarId = (UINT) wp;

	switch (uiToolBarId)
	{
	case IDR_STANDARD:
		{
			//-------------------------------------
			// Replace "Undo" button by menubutton:
			//-------------------------------------
			((CBCGPToolBar*)_controlBars[tt_toolBarStandard])->ReplaceButton (ID_EDIT_UNDO, UndoButton (ID_EDIT_UNDO, _T("&Undo")));
			((CBCGPToolBar*)_controlBars[tt_toolBarStandard])->ReplaceButton (ID_EDIT_REDO, UndoButton (ID_EDIT_REDO, _T("&Redo")));

			CMenu menu;
			menu.LoadMenu (IDR_FILE_NEW);
			CMenu* pPopup = menu.GetSubMenu(0);

			CBCGPToolbarMenuButton btnNew((UINT)ID_FILE_NEW, // No default command
					pPopup->GetSafeHmenu (),
					CImageHash::GetImageOfCommand (ID_FILE_NEW, FALSE),
					_T("&New"));

			((CBCGPToolBar*)_controlBars[tt_toolBarStandard])->ReplaceButton (ID_FILE_NEW, btnNew);

		}
		break;
	case IDR_PLAY:
		{
			//-------------------------------------
			// Replace "Tower" button by combo:
			//-------------------------------------
			((CBCGPToolBar*)_controlBars[tt_toolBarPlay])->ReplaceButton (ID_TOWER, TowerComboBtn());
			towerManager()->fireUpdateTowerList();
		}
		break;

	case IDR_STRIKE_GENERAL:
		{
			//-------------------------------------
			// Replace "Strike" button by combo:
			//-------------------------------------
			if (dynamic_cast<StrikeGraphCtrlBar*>(_controlBars[tt_controlBarStrikeGraph]))
				dynamic_cast<StrikeGraphCtrlBar*>(_controlBars[tt_controlBarStrikeGraph])->toolbarReset();
			if (dynamic_cast<StrikeInfoCtrlBar*>(_controlBars[tt_controlBarStrikeInfo]))
				dynamic_cast<StrikeInfoCtrlBar*>(_controlBars[tt_controlBarStrikeInfo])->toolbarReset();

		}
		break;

	case IDR_SIMULATOR_GENERAL:
		{
			//-------------------------------------
			// Replace "Strike" button by combo:
			//-------------------------------------
			if (dynamic_cast<SimulatorStrikingSingleBellCtrlBar*>(_controlBars[tt_controlBarSimulatorSingleBell1]))
				dynamic_cast<SimulatorStrikingSingleBellCtrlBar*>(_controlBars[tt_controlBarSimulatorSingleBell1])->toolbarReset();
			if (dynamic_cast<SimulatorStrikingSingleBellCtrlBar*>(_controlBars[tt_controlBarSimulatorSingleBell2]))
				dynamic_cast<SimulatorStrikingSingleBellCtrlBar*>(_controlBars[tt_controlBarSimulatorSingleBell2])->toolbarReset(); 
		}
		break;

	case IDR_PLAY_SIMULATOR:
		{
			//-------------------------------------
			// Replace "Tower" button by combo:
			//-------------------------------------
			((CBCGPToolBar*)_controlBars[tt_toolBarPlaySimulator])->ReplaceButton (ID_TOWER, TowerComboBtn());
			towerManager()->fireUpdateTowerList();
		}
		break;

	case IDR_INTERNET:
		{
			//to2do do this in OnViewCustomise also

			CString str;

			// Replace "Back" and "Forward" buttons by the menu buttons
			// with the history lists:

			CMenu menuHistory;
			menuHistory.LoadMenu (IDR_HISTORY_POPUP);

			str = "Back";
			((CBCGPToolBar*)_controlBars[tt_toolBarInternet])->ReplaceButton (ID_GO_BACK,
				CBCGPToolbarMenuButton (ID_GO_BACK, menuHistory,
							CImageHash::GetImageOfCommand (ID_GO_BACK), str));

			str = "Forward";
			((CBCGPToolBar*)_controlBars[tt_toolBarInternet])->ReplaceButton (ID_GO_FORWARD,
				CBCGPToolbarMenuButton (ID_GO_FORWARD, menuHistory,
							CImageHash::GetImageOfCommand (ID_GO_FORWARD), str));

			// Setup "Favorites" menu button:
			CMenu menuFavorites;
			menuFavorites.LoadMenu (IDR_FAVORITES_POPUP);

			str = "Favorites";
			((CBCGPToolBar*)_controlBars[tt_toolBarInternet])->ReplaceButton (ID_FAVORITS_DUMMY,
				CBCGPToolbarMenuButton ((UINT)-1, menuFavorites,
							CImageHash::GetImageOfCommand (ID_FAVORITS_DUMMY), str));

			// Setup "Fonts" menu button:
			CMenu menuFonts;
			menuFonts.LoadMenu (IDR_FONT_POPUP);

			str = "Font";
			((CBCGPToolBar*)_controlBars[tt_toolBarInternet])->ReplaceButton (ID_FONT_DROPDOWN,
				CBCGPToolbarMenuButton ((UINT)-1, *menuFonts.GetSubMenu (0),
							CImageHash::GetImageOfCommand (ID_FONT_DROPDOWN), str));
		}
		break;
	case IDR_TOUCH_BLUE_LINE:
		{
			((CBCGPToolBar*)_controlBars[tt_toolBarTouchBlueLine])->ReplaceButton (ID_VIEW_ANALYSIS_DUMMY,
				CBCGPDropDownToolbarButton (_T("Touch Analysis"), &_touchAnalysisDropdownToolBar));
		}
		break;
	case IDR_TOUCH_MAIN:
		{
			((CBCGPToolBar*)_controlBars[tt_toolBarTouchMain])->ReplaceButton (ID_VIEW_ANALYSIS_DUMMY,
				CBCGPDropDownToolbarButton (_T("Touch Analysis"), &_touchAnalysisDropdownToolBar));
		}
		break;

	case IDR_RECORD:
		{
			((CBCGPToolBar*)_controlBars[tt_toolBarRecord])->ReplaceButton (ID_FILTER, FilterComboBtn());
			((CBCGPToolBar*)_controlBars[tt_toolBarRecord])->ReplaceButton (ID_VIEW_ANALYSIS_DUMMY,
				CBCGPDropDownToolbarButton (_T("Record Analysis"), &_recordAnalysisDropdownToolBar));
			recordManager()->fireUpdateFilterList();
		}
		break;

	}
	return 0;
}

BOOL MainFrame::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
    CBCGPMDIFrameWnd::OnShowPopupMenu (pMenuPopup);

    if (pMenuPopup == NULL)
	{
		return TRUE;
	}

	CBCGPPopupMenuBar* pMenuBar = pMenuPopup->GetMenuBar ();
	ASSERT_VALID (pMenuBar);

	for (int i = 0; i < pMenuBar->GetCount (); i ++)
	{
		CBCGPToolbarButton* pButton = pMenuBar->GetButton (i);
		ASSERT_VALID (pButton);

		if (pButton->m_nID == ID_FAVORITS_DUMMY)
		{
			if (CBCGPToolBar::IsCustomizeMode ())
			{
				return FALSE;
			}

			pMenuBar->ImportFromMenu (m_menuFavotites);
			pMenuPopup->SetMaxWidth (300);

			return TRUE;
		}
	}

	CBCGPToolbarMenuButton* pParentButton = pMenuPopup->GetParentButton ();
	if (pParentButton != NULL)
	{
		switch (pParentButton->m_nID)
		{
		case ID_GO_BACK:
		case ID_GO_FORWARD:
			{
				if (CBCGPToolBar::IsCustomizeMode ())
				{
					return FALSE;
				}

				InternetDoc* pDoc = getActiveInternetDoc();
				ASSERT_VALID(pDoc);

				_T_HistotyList lst;

				if (pParentButton->m_nID == ID_GO_BACK)
				{
					pDoc->GetBackList (lst);
				}
				else
				{
					pDoc->GetFrwdList (lst);
				}

				if (!lst.IsEmpty ())
				{
					pMenuPopup->RemoveAllItems ();

					for (POSITION pos = lst.GetHeadPosition (); pos != NULL;)
					{
						HistoryObj* pObj = lst.GetNext (pos);
						ASSERT (pObj != NULL);

						pMenuPopup->InsertItem (
							CBCGPToolbarMenuButton (pObj->GetCommand (), NULL, -1,
													pObj->GetTitle ()));
					}
				}
			}
		}
	}

	//----------------------------------------------------------------
	// The window menu - can be on any of the main menus.
	//----------------------------------------------------------------

    if (pMenuPopup->GetMenuBar ()->CommandToIndex (ID_WINDOW_CLOSEALL) >= 0)
	{
		TRACE("ID_WINDOW_CLOSEALL");

		if (CBCGPToolBar::IsCustomizeMode ())
		{
			return FALSE;
		}

		DWORD dwAllowedItems = GetMDITabsContextMenuAllowedItems();

		for (int i=0;i<pMenuPopup->GetMenuItemCount();i++)
		{
			CBCGPToolbarMenuButton* menuItem = pMenuPopup->GetMenuItem(i);
			if (menuItem->m_nID == ID_MDI_NEW_HORZ_TAB_GROUP)
			{
				if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
					pMenuPopup->RemoveItem(i--);
			}
			if (menuItem->m_nID == ID_MDI_NEW_VERT_TAB_GROUP)
			{
				if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
					pMenuPopup->RemoveItem(i--);
			}
			if (menuItem->m_nID == ID_MDI_MOVE_TO_NEXT_GROUP)
			{
				if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
					pMenuPopup->RemoveItem(i--);
			}
			if (menuItem->m_nID == ID_MDI_MOVE_TO_PREV_GROUP)
			{
				if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
					pMenuPopup->RemoveItem(i--);
			}
		}
	}

	//----------------------------------------------------------------
	// The touch menu - can be on any of the three touch menus.
	//----------------------------------------------------------------
    if (pMenuPopup->GetMenuBar ()->CommandToIndex (ID_TOUCH_PROVE) >= 0)
	{
		TRACE("ID_TOUCH_PROVE");

		if (CBCGPToolBar::IsCustomizeMode ())
		{
			return FALSE;
		}

		TouchDoc* pTouchDoc = getActiveTouchDoc();

		if (!pTouchDoc)
			pMenuPopup->RemoveAllItems ();
		else
	    {
			CMenu pPopup;
			pPopup.Attach((HMENU)pMenuPopup->GetMenu());
			pTouchDoc->setupTouchMenu(&pPopup);

			pMenuPopup->GetMenuBar ()->ImportFromMenu (pPopup, TRUE);

		}
	}

	//----------------------------------------------------------------
	// The play menu - can be on any of the three touch menus.
	//----------------------------------------------------------------
    if (pMenuPopup->GetMenuBar ()->CommandToIndex (ID_PLAY) >= 0)
	{
		TRACE("ID_PLAY");

		if (CBCGPToolBar::IsCustomizeMode ())
		{
			return FALSE;
		}

		TouchDoc* pTouchDoc = getActiveTouchDoc();

		if (pTouchDoc)
	    {
			CMenu pPopup;
			pPopup.Attach((HMENU)pMenuPopup->GetMenu());
			pTouchDoc->setupPlayMenu(&pPopup);

			pMenuPopup->GetMenuBar ()->ImportFromMenu (pPopup, TRUE);
		}
	}

    if (pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
    {
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;

		loadToolbarMenu(menu);

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);
		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
    }

	//sort the style menu
	if (pMenuPopup->GetMenuBar ()->CommandToIndex (ID_METHOD_STYLE_DUMMY) >= 0)
    {

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_METHOD_STYLE_DEFAULT));

		CMenu* pPopup = menu.GetSubMenu(0)->GetSubMenu(0);
		ASSERT(pPopup != NULL);

		styleManager()->setupStyleMenu(pPopup);
		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
 	}

	int iIndex = -1;
/*//todo	if (_menuBar->GetDroppedDownMenu (&iIndex) ==
		pMenuPopup->GetParentButton () &&
		iIndex == (_menuBar->GetSystemMenu () == NULL ? 0 : 1))	// "File" menu
	{
		pMenuPopup->EnableMenuLogo (22);
	}
*/
 	if (pMenuPopup != NULL)
	{
		AdjustColorsMenu (pMenuPopup, ID_LINECOLOR, "Line");
		AdjustColorsMenu (pMenuPopup, ID_TEXTCOLOR, "Text");
	}

	return TRUE;
}

void MainFrame::OnViewStandard()
{
	ShowControlBar (_controlBars[tt_toolBarStandard],
					!(_controlBars[tt_toolBarStandard]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewStandard(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarStandard]->IsVisible());
}

void MainFrame::OnViewTouchMain()
{
	ShowControlBar (_controlBars[tt_toolBarTouchMain],
					!(_controlBars[tt_toolBarTouchMain]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewTouchMain(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarTouchMain]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_main);
}

void MainFrame::OnViewTouchBlueLine()
{
	ShowControlBar (_controlBars[tt_toolBarTouchBlueLine],
					!(_controlBars[tt_toolBarTouchBlueLine]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewTouchBlueLine(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarTouchBlueLine]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_blue_line);
}

void MainFrame::OnViewSimulator()
{
	ShowControlBar (_controlBars[tt_toolBarSimulator],
					!(_controlBars[tt_toolBarSimulator]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulator(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarSimulator]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewSimulatorCalls()
{
	ShowControlBar (_controlBars[tt_toolBarSimulatorCalls],
					!(_controlBars[tt_toolBarSimulatorCalls]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulatorCalls(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarSimulatorCalls]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewRecord()
{
	ShowControlBar (_controlBars[tt_toolBarRecord],
					!(_controlBars[tt_toolBarRecord]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewRecord(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarRecord]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_tower)||(_curViewType == vt_grab));
}

void MainFrame::OnViewRecordLogic()
{
	ShowControlBar (_controlBars[tt_toolBarRecordLogic],
					!(_controlBars[tt_toolBarRecordLogic]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewRecordLogic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarRecordLogic]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_tower)||(_curViewType == vt_grab));
}

void MainFrame::OnViewCrib()
{
	ShowControlBar (_controlBars[tt_toolBarCrib],
					!(_controlBars[tt_toolBarCrib]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewCrib(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarCrib]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_crib);
}

void MainFrame::OnViewStrike()
{
	ShowControlBar (_controlBars[tt_toolBarStrike],
					!(_controlBars[tt_toolBarStrike]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewStrike(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarStrike]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_strike);
}
                      
void MainFrame::OnViewInternet()
{
	ShowControlBar (_controlBars[tt_toolBarInternet],
					!(_controlBars[tt_toolBarInternet]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewInternet(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarInternet]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_internet);
}

void MainFrame::OnViewImage()
{
	ShowControlBar (_controlBars[tt_toolBarImage],
					!(_controlBars[tt_toolBarImage]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewImage(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarImage]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_image);
}

void MainFrame::OnViewMap()
{
	ShowControlBar (_controlBars[tt_toolBarMap],
					!(_controlBars[tt_toolBarMap]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewMap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarMap]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_map);
}

void MainFrame::OnViewPlay()
{
	ShowControlBar (_controlBars[tt_toolBarPlay],
					!(_controlBars[tt_toolBarPlay]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewPlay(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarPlay]->IsVisible());
	pCmdUI->Enable(_curViewType != vt_touch_simulator);
}

void MainFrame::OnViewPlaySimulator()
{
	ShowControlBar (_controlBars[tt_toolBarPlaySimulator],
					!(_controlBars[tt_toolBarPlaySimulator]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewPlaySimulator(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarPlaySimulator]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewTool()
{
	ShowControlBar (_controlBars[tt_toolBarTools],
					!(_controlBars[tt_toolBarTools]->IsVisible()),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewTool(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_toolBarTools]->IsVisible());
}

void MainFrame::OnViewProperties()
{
	ShowControlBar (_controlBars[tt_controlBarProperty],
					!_controlBars[tt_controlBarProperty]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarProperty]->IsVisible());
	pCmdUI->Enable((_curViewType != vt_none)&&(_curViewType != vt_image));   
}


void MainFrame::OnViewAnalysisStatus()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisStatus],
					!_controlBars[tt_controlBarTouchAnalysisStatus]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisStatus(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisStatus]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}

void MainFrame::OnViewAnalysisMusic()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisMusic],
					!_controlBars[tt_controlBarTouchAnalysisMusic]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisMusic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisMusic]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}

void MainFrame::OnViewAnalysisCourseHead()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisCourseHead],
					!_controlBars[tt_controlBarTouchAnalysisCourseHead]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisCourseHead(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisCourseHead]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}

void MainFrame::OnViewAnalysisLeadEnd()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisLeadEnd],
					!_controlBars[tt_controlBarTouchAnalysisLeadEnd]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisLeadEnd(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisLeadEnd]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}

void MainFrame::OnViewAnalysisAllTheWork()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisAllTheWork],
					!_controlBars[tt_controlBarTouchAnalysisAllTheWork]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisAllTheWork(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisAllTheWork]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}

void MainFrame::OnViewAnalysisMultiBlock()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisMultiBlock],
					!_controlBars[tt_controlBarTouchAnalysisMultiBlock]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisMultiBlock(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisMultiBlock]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}

void MainFrame::OnViewAnalysisCallsMade()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisCallsMade],
					!_controlBars[tt_controlBarTouchAnalysisCallsMade]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisCallsMade(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisCallsMade]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main)||(_curViewType == vt_touch_simulator));
}

void MainFrame::OnViewAnalysisSplice()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisSplice],
					!_controlBars[tt_controlBarTouchAnalysisSplice]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisSplice(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisSplice]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}

void MainFrame::OnViewAnalysisSpliceTransition()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisSpliceTransition],
					!_controlBars[tt_controlBarTouchAnalysisSpliceTransition]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisSpliceTransition(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisSpliceTransition]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}

void MainFrame::OnViewAnalysisFalseness()
{
	ShowControlBar (_controlBars[tt_controlBarTouchAnalysisFalseness],
					!_controlBars[tt_controlBarTouchAnalysisFalseness]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewAnalysisFalseness(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarTouchAnalysisFalseness]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_touch_blue_line)||(_curViewType == vt_touch_main));
}


void MainFrame::OnViewSimulatorMethod()
{
	ShowControlBar (_controlBars[tt_controlBarSimulatorMethod],
					!_controlBars[tt_controlBarSimulatorMethod]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulatorMethod(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarSimulatorMethod]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewSimulatorStriking()
{
	ShowControlBar (_controlBars[tt_controlBarSimulatorStriking],
					!_controlBars[tt_controlBarSimulatorStriking]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulatorStriking(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarSimulatorStriking]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewSimulatorStrikingSingleBell1()
{
	ShowControlBar (_controlBars[tt_controlBarSimulatorSingleBell1],
					!_controlBars[tt_controlBarSimulatorSingleBell1]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulatorStrikingSingleBell1(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarSimulatorSingleBell1]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewSimulatorStrikingSingleBell2()
{
	ShowControlBar (_controlBars[tt_controlBarSimulatorSingleBell2],
					!_controlBars[tt_controlBarSimulatorSingleBell2]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulatorStrikingSingleBell2(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarSimulatorSingleBell2]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewSimulatorInputSpy()
{
	ShowControlBar (_controlBars[tt_controlBarSimulatorInputSpy],
					!_controlBars[tt_controlBarSimulatorInputSpy]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulatorInputSpy(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarSimulatorInputSpy]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewSimulatorInputsMap()
{
	ShowControlBar (_controlBars[tt_controlBarSimulatorInputsMap],
			!_controlBars[tt_controlBarSimulatorInputsMap]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulatorInputsMap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarSimulatorInputsMap]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewSimulatorStrikingControls()
{
	ShowControlBar (_controlBars[tt_controlBarSimulatorStrikingControls],
					!_controlBars[tt_controlBarSimulatorStrikingControls]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewSimulatorStrikingControls(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarSimulatorStrikingControls]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_touch_simulator);
}

void MainFrame::OnViewStrikeGraph()
{
	ShowControlBar (_controlBars[tt_controlBarStrikeGraph],
					!_controlBars[tt_controlBarStrikeGraph]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewStrikeGraph(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarStrikeGraph]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_strike);
}

void MainFrame::OnViewStrikeInfo()
{
	ShowControlBar (_controlBars[tt_controlBarStrikeInfo],
					!_controlBars[tt_controlBarStrikeInfo]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewStrikeInfo(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarStrikeInfo]->IsVisible());
	pCmdUI->Enable(_curViewType == vt_strike);
}

void MainFrame::OnViewRecordHomeTower()
{
	ShowControlBar (_controlBars[tt_controlBarRecordHomeTower],
					!_controlBars[tt_controlBarRecordHomeTower]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewRecordHomeTower(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarRecordHomeTower]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_grab)||(_curViewType == vt_tower)||(_curViewType == vt_map));
}

void MainFrame::OnViewRecordSelectedTower()
{
	ShowControlBar (_controlBars[tt_controlBarRecordSelectedTower],
					!_controlBars[tt_controlBarRecordSelectedTower]->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewRecordSelectedTower(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_controlBars[tt_controlBarRecordSelectedTower]->IsVisible());
	pCmdUI->Enable((_curViewType == vt_grab)||(_curViewType == vt_tower)||(_curViewType == vt_map));
}




void MainFrame::OnViewTaskPane()
{
	ShowControlBar (_taskPaneCtrlBar,
					!_taskPaneCtrlBar->IsVisible (),
					FALSE, TRUE);
	RecalcLayout ();
}

void MainFrame::OnUpdateViewTaskPane(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (_taskPaneCtrlBar->IsVisible());
	pCmdUI->Enable(TRUE);   
}


void MainFrame::UpdateCaption(CString str)
{
	m_strTitle = str;
	OnUpdateFrameTitle(TRUE);
}

void MainFrame::OnToolsOptions()
{
	OptionsDisplayFilter filter;
	OptionsDlg propSheet(this, filter);
	propSheet.DoModal();
}


void MainFrame::OnCribOptions()
{
	doCribOptions(this)	;
}

void MainFrame::doCribOptions(CWnd* pWnd)
{
	OptionsDisplayFilter filter(false);
	filter._method= true;
	filter._crib = true;
	filter._displayScreenType = os_Crib;
	OptionsDlg propSheet(pWnd, filter);
	propSheet.DoModal();
}


void MainFrame::OnTouchOptions()
{
	OptionsDisplayFilter filter(false);
	filter._touchStyle = true;
	OptionsDlg propSheet(this, filter);
	propSheet.DoModal();
}

void MainFrame::OnSimulatorOptions()
{
	doSimulatorOptions(true, this);
}

void MainFrame::doSimulatorOptions(bool towerSounds, CWnd* parent)
{
	OptionsDisplayFilter filter(false);
	filter._keyboard= true;
	filter._serialControl= true;
	filter._microphone= true;
	filter._multiBellInterface= true;
	filter._TTS= true;
	filter._towerSounds= towerSounds;
	OptionsDlg propSheet(parent, filter);
	propSheet.DoModal();
}

void MainFrame::OnClose()
{
	// DO NOT DO ANYTHING DESTRUCTIVE HERE IN CASE THE USER DOES NOT
	//  EXIT BY PRESSING CANCLE ON A DO YOU WANT TO SAVE MESSAGE BOX
	//  IN SHORT THIS MAY BE CALLED A NUMBER OF TIMES. USE DestroyWindow()

	for (int i=0;i<app()->_managers.GetSize();i++)
		app()->_managers.GetAt(i)->save();   


	// Because of problems that BCG will not solve, we
	// have to loop around all documents, and create the info
	// objects for serialisation. If we do it on the fly,
	// CArchive foules up as it does not serialise second objects
	// of the same type at the same memory address, when calling SaveMDIState
	app()->SetRegistryBase (RingingMasterApp::_RegistryBase);

	const CObList& lstTabbedGroups = m_wndClientArea.GetMDITabGroups();
	for (POSITION pos = lstTabbedGroups.GetHeadPosition (); pos != NULL;)
	{
		CBCGPTabWnd* tabWnd = DYNAMIC_DOWNCAST (CBCGPTabWnd, lstTabbedGroups.GetNext (pos));

		for (int i=0;i<tabWnd->GetTabsNum ();i++)
		{
			ChildFrame* pNextWnd = DYNAMIC_DOWNCAST (ChildFrame, tabWnd->GetTabWnd (i));
			ASSERT_VALID (pNextWnd);

			if (pNextWnd)
				pNextWnd->createDocumentObject();
		}
	}
	SaveMDIState (app()->GetRegSectionPath());

	//save the last state
	saveCurrentVolatileToolbarVisibility(_curViewType);
	saveCurrentWindowPositionState(toolbarManager()->getStateType(_curViewType)); 

	//allow the usual user save documents to happen.
	CBCGPMDIFrameWnd::OnClose();


}

void MainFrame::addEventListener(MainFrameEventListener* mainFrameEventListener)
{
	ASSERT(mainFrameEventListener != NULL);

	_listenerList.Add(mainFrameEventListener);
}

void MainFrame::removeEventListener(MainFrameEventListener* mainFrameEventListener)
{
	ASSERT(mainFrameEventListener!= NULL);

	int hasRemoved = 0;

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == mainFrameEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	}

	ASSERT(hasRemoved == 1);
}

void MainFrame::fireDocViewChanged(ViewType oldViewType, ViewType newViewType, CDocument* oldDoc, CDocument* newDoc)
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->mainFrame_notifyDocViewChanged(oldViewType, newViewType, oldDoc, newDoc);
	}
}

void MainFrame::fireUpdate() //todo reduce the amount this is used, and the scope of mainFrame_notifyUpdate by creating better more local updates. Look at the change document notification.
{
//	TRACE("Update\r\n");
	ViewType viewType = app()->getViewType();
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->mainFrame_notifyUpdate(viewType);
	}
}

void MainFrame::fireIdle()
{
	showHidePlayInfo();
	showHideTaskBar();

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->mainFrame_notifyIdle();
	}
}

void MainFrame::showStrikingControls(Tower* tower, int bell)
{
	if (_controlBars[tt_controlBarSimulatorStrikingControls] && 
		::IsWindow(_controlBars[tt_controlBarSimulatorStrikingControls]->GetSafeHwnd()))
	{
		((StrikingControls*)_controlBars[tt_controlBarSimulatorStrikingControls])->setTower(tower, bell);
		ShowControlBar (_controlBars[tt_controlBarSimulatorStrikingControls],TRUE,FALSE, FALSE);
	}
}

void MainFrame::showHidePlayInfo(bool forceHide)
{
	if (forceHide)
		ShowControlBar (_controlBars[tt_controlBarPlayPosition],FALSE,FALSE, FALSE);
	else
	{
		BOOL shouldShow = playManager()->shouldShowPlayInfo();

		BOOL isShowing = _controlBars[tt_controlBarPlayPosition]->IsVisible();

		if (shouldShow != isShowing)
			ShowControlBar (_controlBars[tt_controlBarPlayPosition],shouldShow,FALSE, FALSE);
	}
}

void MainFrame::showHideTaskBar()
{
	BOOL shouldShow = (getActiveDocument() == NULL);

	BOOL isShowing = _taskPaneCtrlBar->IsVisible();

	if (shouldShow && !isShowing)
		ShowControlBar (_taskPaneCtrlBar,TRUE,FALSE, TRUE);
}




void MainFrame::mainFrame_notifyUpdate(ViewType newViewType)
{
	if (!app()->isInited())	//This stops us changing state a lot on start up.
		return;
								   
	//are we changing viewType?
	if (_curViewType != newViewType)
	{
		CWaitCursor wait;

		StateType curStateType = toolbarManager()->getStateType(_curViewType);
		StateType newStateType = toolbarManager()->getStateType(newViewType);

		ASSERT(newViewType != vt_null);
		TRACE("Changing to view type:%s state type: %s\r\n",
			toolbarManager()->getViewName(newViewType),toolbarManager()->getStateName(newStateType));

		//allways save the toolbar state
		saveCurrentVolatileToolbarVisibility(_curViewType); 

		//have we changed state?
		if (_curViewType == vt_null || (curStateType != newStateType))
		{
			//we haev changed view type and state
			LockWindowUpdate();

			saveCurrentWindowPositionState(curStateType);
		
			//keep the type
			_curViewType = newViewType;

			//to2do stop the toolbars saveing customisation
			//to2do check if accellerators are saved and reloaded.

			//load the new state only if we have one saved.
			if (app()->GetProfileInt(toolbarManager()->getConfigLocation(newStateType), "saved", FALSE))
			{
				//dock the toobars to ensure that they have been are docked before loading the state. 
				dockToolbars(newStateType);

				loadCurrentWindowPositionState(newStateType);
			}
			else
			{
				//load an initial state for the view type
				showDefaultVisibility(newViewType);
				//dock the toobars after the visibility has been set. 
				dockToolbars(newStateType);
			}

			CMDIChildWnd* activeChild = MDIGetActive();
			if (activeChild)
				activeChild->SetFocus();

			UnlockWindowUpdate();
		}
		else
		{
			//we have changed view type but not state. 
			//keep the type
			_curViewType = newViewType;

		}

		//allways load the toolbar state
		loadCurrentVolatileToolbarVisibility(newViewType);

		//do the updating required to recreate the dynamic data on toolbars, that has just been loaded in fromthe LoadState
		recordManager()->fireUpdateFilterList();
		towerManager()->fireUpdateTowerList();
	}

	//hide all non allowed control / tool bars.
	hideNonAllowedToolbars(newViewType);
}

void MainFrame::loadCurrentWindowPositionState(StateType curStateType)
{
	if (curStateType != st_null)
	{
		theApp.m_bLoadUserToolbars = FALSE;
		theApp.LoadState (this, toolbarManager()->getConfigLocation(curStateType));
		app()->SetRegistryBase (RingingMasterApp::_RegistryBase); //this is needed to stop the resistry base being set by LoadState
		RecalcLayout ();
	}
}

void MainFrame::saveCurrentWindowPositionState(StateType curStateType)
{
	//save the current state
	if (curStateType != st_null)
	{
		CString location = toolbarManager()->getConfigLocation(curStateType);

		theApp.SaveState (this, location);//to2do why do we archive errors here?
		app()->SetRegistryBase (RingingMasterApp::_RegistryBase); //this is needed to stop the resistry base being set by LoadState
		app()->WriteProfileInt(location, "saved", TRUE);
	}
}

//save the visible state of the volitile toolbars that will 
//change when changing view type but not changing state
void MainFrame::saveCurrentVolatileToolbarVisibility(ViewType viewType)
{	 

	//todo only do when needed
	if (viewType != vt_null)
	{
		CString location = toolbarManager()->getConfigLocation(viewType);
		app()->WriteProfileInt(location, "saved", TRUE);

		ToolbarFileState stateNow;
		getCurrentToolbarState(stateNow);

		RegArchive ar(CArchive::store,location,"visible");   
		stateNow.Serialize(ar);
	}  
}

void MainFrame::loadCurrentVolatileToolbarVisibility(ViewType viewType)
{		 
	if (viewType != vt_null)
	{
		CString location = toolbarManager()->getConfigLocation(viewType);
		if (app()->GetProfileInt(location, "saved", FALSE))
		{                    
		   	ToolbarFileState stateWanted;
		
			RegArchive ar(CArchive::load,location,"visible");   
			stateWanted.Serialize(ar);
			doShowHideToolbars(viewType, stateWanted);
		}
		else
		{
			showDefaultVisibility(viewType);
		}								 
	}	 
}

void MainFrame::showDefaultVisibility(ViewType viewType)
{
	//what items can possibly be showing? 
	const ToolbarFileState& defaultState = toolbarManager()->getDefaultState(viewType);
	const ToolbarFileState& activateState = toolbarManager()->getActivateState();

	doShowHideToolbars(viewType, defaultState, activateState);    
}

void MainFrame::dockToolbars(StateType stateType)
{
	//dock the toolbars

	//first row
	switch (stateType)
	{
	case st_none:
		DockControlBar(_controlBars[tt_toolBarStandard]);
		break;
	case st_touch_main:
		DockControlBar(_controlBars[tt_toolBarTouchMain]);
		DockControlBarLeftOf(_controlBars[tt_toolBarTouchBlueLine], _controlBars[tt_toolBarTouchMain]);
		DockControlBarLeftOf(_controlBars[tt_toolBarStandard], _controlBars[tt_toolBarTouchBlueLine]);
		break;
	case st_touch_simulator:	  
		DockControlBar(_controlBars[tt_toolBarSimulator]);
		DockControlBarLeftOf(_controlBars[tt_toolBarStandard], _controlBars[tt_toolBarSimulator]);
		break;
	case st_crib:
		DockControlBar(_controlBars[tt_toolBarCrib]);
		DockControlBarLeftOf(_controlBars[tt_toolBarStandard], _controlBars[tt_toolBarCrib]);
		break;
	case st_records:
		DockControlBar(_controlBars[tt_toolBarMap]);
		DockControlBarLeftOf(_controlBars[tt_toolBarRecord], _controlBars[tt_toolBarMap]);
		DockControlBarLeftOf(_controlBars[tt_toolBarStandard], _controlBars[tt_toolBarRecord]);
		break;
	case st_internet:
		DockControlBar(_controlBars[tt_toolBarInternet]);
		DockControlBarLeftOf(_controlBars[tt_toolBarStandard], _controlBars[tt_toolBarInternet]);
		break;
	case st_image:
		DockControlBar(_controlBars[tt_toolBarImage]);
		DockControlBarLeftOf(_controlBars[tt_toolBarStandard], _controlBars[tt_toolBarImage]);
		break;
	case st_strike:
		DockControlBar(_controlBars[tt_toolBarStrike]);
		DockControlBarLeftOf(_controlBars[tt_toolBarStandard], _controlBars[tt_toolBarStrike]);
		break;
	default:
		ASSERT(FALSE);
	}                   


	//second row
	switch (stateType)
	{
	case st_none:
	case st_crib:
	case st_touch_main:
	case st_internet:
	case st_image:
	case st_strike:
		DockControlBar(_controlBars[tt_toolBarTools]);
		DockControlBarLeftOf(_controlBars[tt_toolBarPlay], _controlBars[tt_toolBarTools]);
		break;
	case st_records:
		DockControlBar(_controlBars[tt_toolBarRecordLogic]);
		DockControlBarLeftOf(_controlBars[tt_toolBarTools], _controlBars[tt_toolBarRecordLogic]);
		DockControlBarLeftOf(_controlBars[tt_toolBarPlay], _controlBars[tt_toolBarTools]);
		break;
	case st_touch_simulator:
		DockControlBar(_controlBars[tt_toolBarSimulatorCalls]);
		DockControlBarLeftOf(_controlBars[tt_toolBarTools], _controlBars[tt_toolBarSimulatorCalls]);
		DockControlBarLeftOf(_controlBars[tt_toolBarPlaySimulator], _controlBars[tt_toolBarTools]);
		break;

	default:
		ASSERT(FALSE);
	}

}


void MainFrame::getCurrentToolbarState(ToolbarFileState& stateNow)
{
	for  (int i=FIRST_TOOLBAR;i<=LAST_CONTROLBAR;i++)
	{
		stateNow.setState((ToolbarTypes)i, _controlBars[i]->IsVisible()?tfs_SHOW:tfs_HIDE);
	}
}


void MainFrame::hideNonAllowedToolbars(ViewType viewType)
{
	//create a ToolbarFileState with all set to true
	ToolbarFileState stateWanted(tfs_DONTCARE);
	doShowHideToolbars(viewType, stateWanted);
}

void MainFrame::doShowHideToolbars(const ViewType viewType, const ToolbarFileState& stateWanted)
{
	//create an all off activateState
	ToolbarFileState activateState;

	doShowHideToolbars(viewType, stateWanted, activateState);	
}

void MainFrame::doShowHideToolbars(const ViewType viewType, const ToolbarFileState& stateWanted, const ToolbarFileState& activateState)
{
	const ToolbarFileState& stateAllowed = toolbarManager()->getStateAllowed(viewType);

	ToolbarFileState stateNow;
	getCurrentToolbarState(stateNow);

	//hide unwanted windows that are not wanted 
	for  (int i=FIRST_TOOLBAR;i<=LAST_CONTROLBAR;i++)
	{
		ToolbarTypes toolbarType = (ToolbarTypes)i;
		
		if ((stateNow.getState(toolbarType) == tfs_SHOW) && 
			((stateAllowed.getState(toolbarType) == tfs_HIDE)||
			 (stateWanted.getState(toolbarType) == tfs_HIDE)))
		{
			BOOL activate = (activateState.getState(toolbarType) == tfs_SHOW);
			ShowControlBar(_controlBars[i], FALSE, FALSE, activate);
		}
	}


	//show wanted windows that are not showing
	for  (int i=FIRST_TOOLBAR;i<=LAST_CONTROLBAR;i++)
	{
		ToolbarTypes toolbarType = (ToolbarTypes)i;
		
		if ((stateNow.getState(toolbarType) == tfs_HIDE) && 
			((stateAllowed.getState(toolbarType) == tfs_SHOW)&&
			 (stateWanted.getState(toolbarType) == tfs_SHOW)))
		{
			BOOL activate = (activateState.getState(toolbarType) == tfs_SHOW);
			ShowControlBar(_controlBars[i], TRUE, FALSE, activate);
		}
	}

}


void MainFrame::OnDrawMenuLogo (CDC* pDC, CBCGPPopupMenu* /*pMenu*/, const CRect& rectLogo)
{
	ASSERT_VALID (pDC);

	//------------------------------------------------------
	// Fill menu side logo by gradient color:
	// (this code was copied from BCGSizingControlBar.cpp
	// Copyright (C) 1998, 1999 by Cristi Posea)
	//------------------------------------------------------

    // this will make 2^6 = 64 fountain steps
    int nShift = 6;
    int nSteps = 1 << nShift;

	COLORREF clrCptnRight = RGB(0,255,0);
	COLORREF clrCptn = RGB (0, 0, 0);

    for (int i = 0; i < nSteps; i++)
    {
        // do a little alpha blending
        BYTE bR = (BYTE) ((GetRValue(clrCptn) * (nSteps - i) +
                   GetRValue(clrCptnRight) * i) >> nShift);
        BYTE bG = (BYTE) ((GetGValue(clrCptn) * (nSteps - i) +
                   GetGValue(clrCptnRight) * i) >> nShift);
        BYTE bB = (BYTE) ((GetBValue(clrCptn) * (nSteps - i) +
                   GetBValue(clrCptnRight) * i) >> nShift);

		CBrush br (RGB(bR, bG, bB));

        // then paint with the resulting color
        CRect r2 = rectLogo;

		r2.bottom = rectLogo.bottom -
               ((i * rectLogo.Height()) >> nShift);
        r2.top = rectLogo.bottom -
            (((i + 1) * rectLogo.Height()) >> nShift);
        if (r2.Height() > 0)
           pDC->FillRect(r2, &br);
    }

	pDC->Draw3dRect (rectLogo, ::GetSysColor (COLOR_3DLIGHT), ::GetSysColor (COLOR_3DSHADOW));

    int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
    COLORREF clrOldText = pDC->SetTextColor (RGB (255, 255, 255));

    CFont* pOldFont = pDC->SelectObject (&_fontVertCaption);
    CString sTitle;
	sTitle.LoadString(AFX_IDS_APP_TITLE);
	sTitle += "® 4.01";

    CPoint ptOrg = CPoint(rectLogo.left + 1, rectLogo.bottom - 10);
    pDC->ExtTextOut (ptOrg.x, ptOrg.y,ETO_CLIPPED, rectLogo, sTitle, NULL);

    pDC->SelectObject(pOldFont);
    pDC->SetBkMode(nOldBkMode);
    pDC->SetTextColor(clrOldText);

}

void MainFrame::OnWindowManager()
{
	ShowWindowsDialog ();
}

void MainFrame::AdjustColorsMenu (CBCGPPopupMenu* pMenuPopup, UINT uiID, CString menuText)
{
	CBCGPPopupMenuBar* pMenuBar = pMenuPopup->GetMenuBar ();
	ASSERT (pMenuBar != NULL);

	int iIndex = pMenuBar->CommandToIndex (uiID);
	if (iIndex < 0)
	{
		return;
	}

	if (DYNAMIC_DOWNCAST (CBCGPColorMenuButton, pMenuBar->GetButton (iIndex)) != NULL)
	{
		return;
	}

	CBCGPColorMenuButton* pColorButton = new
		CBCGPColorMenuButton (uiID, menuText + " Color", NULL);


	//get the starting colors
	if ((_pMethodWnd)&&
		(IsWindow(_pMethodWnd->m_hWnd)&&
		(_pMethodWnd->IsKindOf(RUNTIME_CLASS(MethodWnd)))))

	{
		MethodWnd * pMethod = (MethodWnd*)_pMethodWnd;
		MethodPosition pos;

		if(pMethod->rowHitTest(pMethod->getContectMouseDown(), &pos))
		{
			COLORREF color = ULONG_MAX;
			if (uiID ==  ID_LINECOLOR)
				color = pMethod->_style.getLine(pos.getBell()-1, pMethod->getNumber())->_lineColor;
			else if (uiID ==  ID_TEXTCOLOR)
				color = pMethod->_style.getLine(pos.getBell()-1, pMethod->getNumber())->_textColor;

			pColorButton->SetColor(color);
			pColorButton->EnableAutomaticButton ("No " + menuText, RGB (0, 0, 0));
			pColorButton->EnableOtherButton (_T("More Colors..."));

			pMenuBar->ReplaceButton (uiID, *pColorButton, TRUE);

		}
	}

	delete pColorButton;
}


void MainFrame::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == AAA_TIMER_NAG)
	{
		KillTimer(AAA_TIMER_NAG);
		//if we have registered then dont time again
		if (verifyKey() == TRUE)
			return;

#ifndef _DEBUG
		NagDlg dlg(this);
		dlg.DoModal();

		if (hasTrialExpired())
		{
			CString str;
			if (_trialState == ts_tampered)
				str = "Your 30-day trial has been terminated due to the system date being put back.\nPlease purchase a full licence and register, or remove Ringing Master from your machine.\nTo register, please restart and press the register button on the nag screen";
			else
				str = "Your 30-day trial has expired.\nPlease purchase a full licence and register, or remove Ringing Master from your machine.\nTo register, please restart and press the register button on the nag screen";
			AfxMessageBox(str);
			PostMessage(WM_CLOSE, 0,0);
			return;
		}  
#endif 
		//15 min == 900000 10 min == 600000 7.5 min = 450000
		//set a shorter time for no reg access
		SetTimer(AAA_TIMER_NAG,(_trialState == ts_no_reg_access)?400000:800000, NULL);
		app()->setupTitle();
	}
	else if (nIDEvent == MAP_REQ_TIMER_EVENT)
	{
		if (mapManager())
		{
			mapManager()->cacheTimerEvent();
		}
	}

}

bool MainFrame::hasTrialExpired()
{
	if (verifyKey() == TRUE)
		//the user must have registered - great - loads a cash!!! ;-)
		return false;
	//we are unregistered - do the check!
	return (_trialState != ts_ok);
}

BOOL MainFrame::verifyKey(CString user, CString key)
{
	//setup version range
	Ints versions;
	versions.Add(1070);//1.07
	versions.Add(1080);//1.08
	versions.Add(2010);//2.01
	versions.Add(3010);//3.01
	versions.Add(3020);//3.02
	versions.Add(4010);//4.01

	//get from registry
	if (user.IsEmpty()&&key.IsEmpty())
	{
		user = app()->GetProfileString("MainFrame","User Name", "");
		key = app()->GetProfileString("MainFrame","User Key", "");
		if (Key::verifyKeyRange(user, &versions, key) == KR_OK)
		{
			return TRUE;
		}
		else
		{
			app()->WriteProfileString("MainFrame","User Name", "");
			app()->WriteProfileString("MainFrame","User Key", "");
			return FALSE;
		}
	}
	else //write to registry
	{
		app()->WriteProfileString("MainFrame","User Name", user);
		app()->WriteProfileString("MainFrame","User Key", key);
		KeyRetType val = Key::verifyKeyRange(user, &versions, key);
		if (val == KR_OK)
		{
			return TRUE;
		}
		else
		{
			AfxMessageBox(Key::getErrorString(val));
			return FALSE;
		}
	}
}

void MainFrame::OnAppAbout()
{
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL MainFrame::DestroyWindow()
{
	towerManager()->removeEventListener(this);  //to2do add to a ManagerBase::closeDown call -  will have to make mainFrame a ManagerBase 
	recordManager()->removeEventListener(this);
	strikeManager()->removeEventListener(this);
	simulatorManager()->removeEventListener(this);

	KillTimer(AAA_TIMER_NAG);

	//allows the managers to disconnect listeners from each other
	for (int i=0;i<app()->_managers.GetSize();i++)
		app()->_managers.GetAt(i)->closeDown();

	return CBCGPMDIFrameWnd::DestroyWindow();
}

BOOL MainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//pump through managers
	if ((simulatorManager())&&(simulatorManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((strikeManager())&&(strikeManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((proofManager())&&(proofManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((playManager())&&(playManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((towerManager())&&(towerManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((methodManager())&&(methodManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((recordManager())&&(recordManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((mapManager())&&(mapManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((userManager())&&(userManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	if ((matrixManager())&&(matrixManager()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;


	//NOTE: if you override this, then please check that you are not getting some
	// odd tooltips when mousing over the menu items. It appears the BCG does not like
	// having command routing through the control bars

	return CBCGPMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

TouchDoc * MainFrame::getActiveTouchDoc()
{
	return dynamic_cast<TouchDoc*>(getActiveDocument());
}

MatrixDoc * MainFrame::getActiveMatrixDoc()
{
	return dynamic_cast<MatrixDoc*>(getActiveDocument());
}

StrikeDoc * MainFrame::getActiveStrikeDoc()
{
	return dynamic_cast<StrikeDoc*>(getActiveDocument());
}

MapDoc * MainFrame::getActiveMapDoc()
{
	return dynamic_cast<MapDoc*>(getActiveDocument());
}

CribDoc * MainFrame::getActiveCribDoc()
{
	return dynamic_cast<CribDoc*>(getActiveDocument());
}

InternetDoc * MainFrame::getActiveInternetDoc()
{
	return dynamic_cast<InternetDoc*>(getActiveDocument());
}

CDocument* MainFrame::getActiveDocument()
{
	//get the active document
	CDocument* pDocument = NULL;
	if (MDIGetActive())
	{
		pDocument = MDIGetActive()->GetActiveDocument();

		if (!pDocument) //have another go
		{
			CWnd* child = MDIGetActive()->GetWindow(GW_CHILD);
			TouchDoc* pTouchDoc = NULL;
			while ((child)&&(!pTouchDoc)&&IsWindow(child->m_hWnd))
			{
				CView* pView = dynamic_cast<CView*>(child);
				if (pView) pTouchDoc = dynamic_cast<TouchDoc*>(pView->GetDocument());
				child = child->GetWindow(GW_HWNDNEXT);//needed for the while loop
			}
			pDocument = pTouchDoc;
		}
	}

	return pDocument;
}

void MainFrame::OnWindowCloseall()
{
	if (app()->SaveAllModified())
		app()->CloseAllDocuments(FALSE);

}

void MainFrame::OnUpdateWindowSaveall(CCmdUI* pCmdUI)
{
	//update all views attached to docs.
	POSITION TemplatePos = app()->GetFirstDocTemplatePosition();

	while(TemplatePos)//end itterate doc templates
	{
		CDocTemplate * pTemplate = app()->GetNextDocTemplate(TemplatePos);
		POSITION DocumentPos = pTemplate->GetFirstDocPosition(); ///get the doc position
		while (DocumentPos)
		{
			CDocument * pDoc = pTemplate->GetNextDoc(DocumentPos); //get the document

			TouchDoc* pTouchDoc = dynamic_cast<TouchDoc*>(pDoc);
			CribDoc* pCribDoc = dynamic_cast<CribDoc*>(pDoc);
			StrikeDoc* pStrikeDoc = dynamic_cast<StrikeDoc*>(pDoc);

			if (pTouchDoc)
			{
				if (pTouchDoc->IsModified())
				{
					pCmdUI->Enable(TRUE);
					return;
				}
			} 
			else if (pCribDoc)
			{
				if (pCribDoc->IsModified())
				{
					pCmdUI->Enable(TRUE);
					return;
				}
			}
			else if (pStrikeDoc)
			{
				if (pStrikeDoc->IsModified())
				{
					pCmdUI->Enable(TRUE);
					return;
				}
			}
		}
	}

	//none are dirty
	pCmdUI->Enable(FALSE);
}

void MainFrame::OnUpdateWindowCloseall(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(getActiveDocument() != NULL);
}

LRESULT MainFrame::OnBCGHelp(WPARAM wParam, LPARAM /*lParam*/)
{
	HtmlHelp((DWORD)wParam + 1000);
	return 0;
}




void MainFrame::OnHelpKeyboardmap()
{
	CBCGPKeyMapDlg dlg(this, TRUE);
	dlg.DoModal();
}

void MainFrame::OnAskQuestion()
{
/*	TODO CBCGPToolbarComboBoxButton* pHelpCombo = _menuBar->GetHelpCombobox ();
	if (pHelpCombo == NULL)
	{
		return;
	}

	CString strQuestion = pHelpCombo->GetText ();
	strQuestion.TrimLeft ();
	strQuestion.TrimRight ();

	if (strQuestion.IsEmpty ())
	{
		return;
	}

	pHelpCombo->AddItem (strQuestion);

	CString str;
	str.Format (_T("The question is: %s"), strQuestion); //to2do

	//to2do feed into the help system.
	MessageBox (str);

	SetFocus ();*/
}

void MainFrame::towerManager_notifyUpdateTowerList()
{
//	TRACE("Listener MainFrame::towerManager_notifyUpdateTowerList() \r\n");

	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_TOWER_COMBO, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition ();
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo =
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL)
			{
				pCombo->RemoveAllItems();
				for (int i=0;i<towerManager()->getTowerCount();i++)
				{
					pCombo->AddItem(towerManager()->getTower(i)->getDisplayName());
				}
				pCombo->SelectItem(towerManager()->getActiveTowerIndex());
			}
		}
	}
}

void MainFrame::OnShowLeadhead()
{
	doShowLeadHead(this);
}

void MainFrame::doShowLeadHead(CWnd *parent)
{
	UINT IDD = IDD_LEAD_HEAD;

	while (IDD != IDCANCEL)
	{
		LeadHeadDlg dlg(parent, IDD);
		IDD = dlg.DoModal();
	}
}


void MainFrame::OnToolsCallchangeeditor()
{
	CalledChangeEditorDlg dlg(this);
	dlg.DoModal();
}

void MainFrame::postLoadState()
{
	fireUpdate();
	RecalcLayout();
}


void MainFrame::OnFilePageSetup()
{
	CPageSetupDialog dlg(PSD_MARGINS | PSD_INWININIINTLMEASURE );//|PSD_DISABLEPAPER |PSD_DISABLEPRINTER       );

	//measurements in 100th of mm
	CRect rect;
	rect.left = app()->GetProfileInt("Print", "Touch Left", 1000);
	rect.top  = app()->GetProfileInt("Print", "Touch Top", 1000);
	rect.right = app()->GetProfileInt("Print", "Touch Right", 1000);
	rect.bottom  = app()->GetProfileInt("Print", "Touch Bottom", 1000);


	dlg.m_psd.rtMargin = rect;

	if (dlg.DoModal() == IDOK)
	{
		rect = dlg.m_psd.rtMargin ;
		app()->WriteProfileInt("Print", "Touch Left", rect.left );
		app()->WriteProfileInt("Print", "Touch Top", rect.top);
		app()->WriteProfileInt("Print", "Touch Right", rect.right);
		app()->WriteProfileInt("Print", "Touch Bottom", rect.bottom);

		CDC* pDC = GetDC();

		CPage::SetPrinterMode(pDC, ((dlg.m_psd.ptPaperSize.x > dlg.m_psd.ptPaperSize.y)?DMORIENT_LANDSCAPE:DMORIENT_PORTRAIT));

		ReleaseDC(pDC);
	}
}

void MainFrame::OnUpdateFilePageSetup(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(getActiveDocument() != NULL);
}


BOOL MainFrame::OnDrawMenuImage (CDC* pDC,
								const CBCGPToolbarMenuButton* pMenuButton,
								const CRect& rectImage)
{
	if (theApp.m_Favorites.GetSysImages () == NULL)
	{
		return FALSE;
	}


	ASSERT_VALID (pDC);
	ASSERT_VALID (pMenuButton);

	int iIcon = -1;

	switch (pMenuButton->m_nID)
	{
	case ID_HELP_MICROSOFT_ON_THE_WEB_FREE_STUFF:
	case ID_HELP_MICROSOFT_ON_THE_WEB_GET_FASTER_INTERNET_ACCESS:
	case ID_HELP_MICROSOFT_ON_THE_WEB_FREQUENTLY_ASKED_QUESTIONS:
	case ID_HELP_MICROSOFT_ON_THE_WEB_INTERNET_START_PAGE:
	case ID_HELP_MICROSOFT_ON_THE_WEB_SEND_FEEDBACK:
	case ID_HELP_MICROSOFT_ON_THE_WEB_BEST_OF_THE_WEB:
	case ID_HELP_MICROSOFT_ON_THE_WEB_SEARCH_THE_WEB:
	case ID_HELP_MICROSOFT_ON_THE_WEB_MICROSOFT_HOME_PAGE:
		iIcon = theApp.m_Favorites.GetInternetShortcutIcon ();
		break;

	default:
		if (pMenuButton->m_nID >= FIRST_FAVORITE_COMMAND &&
			pMenuButton->m_nID <= LAST_FAVORITE_COMMAND)
		{
			iIcon = theApp.m_Favorites.GetIDIcon (pMenuButton->m_nID);
		}
		else if (IsFavoritesMenu (pMenuButton))	// Maybe, favorits folder?
		{
			iIcon = theApp.m_Favorites.GetFolderIcon ();
		}
	}

	if (iIcon == -1)
	{
		return FALSE;	// Don't draw it!
	}

	::ImageList_Draw (theApp.m_Favorites.GetSysImages (), iIcon,
		pDC->GetSafeHdc (),
		rectImage.left + (rectImage.Width () - theApp.m_Favorites.GetSysImageSize ().cx) / 2,
		rectImage.top + (rectImage.Height () - theApp.m_Favorites.GetSysImageSize ().cy) / 2, ILD_TRANSPARENT);

	return TRUE;
}

void MainFrame::OnFavorite(UINT nID)
{
	SetFocus ();
	InternetDoc* doc = getActiveInternetDoc();
	doc->setInitialData(theApp.m_Favorites.GetURLofID (nID));
}


BOOL MainFrame::OnMenuButtonToolHitTest (CBCGPToolbarButton* pButton, TOOLINFO* pTI)
{
	ASSERT_VALID (pButton);

	if (pButton->m_nID < FIRST_FAVORITE_COMMAND ||
		pButton->m_nID > LAST_FAVORITE_COMMAND)
	{
		return FALSE;
	}

	ASSERT (pTI != NULL);

	CString strText = theApp.m_Favorites.GetURLofID (pButton->m_nID);

	pTI->lpszText = (LPTSTR) ::calloc ((strText.GetLength () + 1), sizeof (TCHAR));
	_tcscpy (pTI->lpszText, strText);

	return TRUE;
}


BOOL MainFrame::IsFavoritesMenu (const CBCGPToolbarMenuButton* pMenuButton)
{
	if (pMenuButton == NULL || pMenuButton->m_nID != (UINT) -1)
	{
		return FALSE;
	}

	ASSERT_VALID (pMenuButton);
	const CObList& lstCommands = pMenuButton->GetCommands ();

	for (POSITION pos = lstCommands.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolbarButton* pCmd = (CBCGPToolbarButton*) lstCommands.GetNext (pos);
		ASSERT_VALID (pCmd);

		if ((pCmd->m_nID >= FIRST_FAVORITE_COMMAND &&
			pCmd->m_nID <= LAST_FAVORITE_COMMAND) ||
			IsFavoritesMenu (DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pCmd)))
		{
			return TRUE;
		}
	}

	return FALSE;
}


void MainFrame::OnHistory(UINT nID)
{

	InternetDoc* pDoc = getActiveInternetDoc();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		HistoryObj* pObj = pDoc->Go (nID);
		ASSERT (pObj != NULL);

		pDoc->setInitialData(pObj->GetURL ());
	}
}

void MainFrame::SetAddress(LPCTSTR /*lpszUrl*/)
{
	// This is called when the browser has completely loaded the new location,
	// so make sure the text in the address bar is up to date and stop the
	// animation.
/*	m_wndAddress.SetWindowText(lpszUrl);
	*///to2do
}


void MainFrame::getAllUsedMaps(Ints& ints)
{
	ints.RemoveAll();

	//update all views attached to docs.
	POSITION TemplatePos = app()->GetFirstDocTemplatePosition();

	while(TemplatePos)//end itterate doc templates
	{
		CDocTemplate * pTemplate = app()->GetNextDocTemplate(TemplatePos);
		POSITION DocumentPos = pTemplate->GetFirstDocPosition(); ///get the doc position
		while (DocumentPos)
		{
			CDocument * pDoc = pTemplate->GetNextDoc(DocumentPos); //get the document

			MapDoc* pMapDoc = dynamic_cast<MapDoc*>(pDoc);

			if (pMapDoc)
			{
				int mapId = pMapDoc->getMapRefId();
				if (mapId >= 0)
					ints.Add(mapId);
			}
		}
	}

}

void MainFrame::closeAllMapDocuments()
{
	//update all views attached to docs.
	POSITION TemplatePos = app()->GetFirstDocTemplatePosition();

	while(TemplatePos)//end itterate doc templates
	{
		CDocTemplate * pTemplate = app()->GetNextDocTemplate(TemplatePos);
		POSITION DocumentPos = pTemplate->GetFirstDocPosition(); ///get the doc position
		while (DocumentPos)
		{
			CDocument * pDoc = pTemplate->GetNextDoc(DocumentPos); //get the document

			MapDoc* pMapDoc = dynamic_cast<MapDoc*>(pDoc);

			if (pMapDoc)
			{
				pMapDoc->OnCloseDocument();
			}
		}
	}

}

void MainFrame::setInternetMessage(CString message)
{
	//as this could be called from a thread, post meassage in this function

	_internetMessage = message;

	PostMessage(INTERNET_MESSAGE, 0,0);
}

LRESULT MainFrame::OnInternetMessage(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (!_internetMessage.IsEmpty())
		AfxMessageBox(_internetMessage);

		return 0;
}

LRESULT MainFrame::OnTouchDocUpdateNumber(WPARAM wParam, LPARAM lParam)
{
	TouchDoc* pTouchDoc = (TouchDoc*)wParam;
	int number = (int)lParam;

	//just make sure that the docment still exists.
	if (getActiveTouchDoc() == pTouchDoc)
		pTouchDoc->setNumber(number);
	
	return 0;
}

LRESULT MainFrame::OnTouchDocUpdateStyleColor(WPARAM wParam, LPARAM lParam)
{
	TouchDoc* pTouchDoc = (TouchDoc*)wParam;
	StyleColorPackage* package = (StyleColorPackage*)lParam;

	//just make sure that the docment still exists.
	if (getActiveTouchDoc() == pTouchDoc)
		pTouchDoc->setStyleColor(package->_type, package->_color);
	
	return 0;
}

void MainFrame::tidyMenu(CMenu *menu)
{
	bool lastItemSeperator = false;

	//first we go through all the menu items to see if they should be deleted
	for (unsigned int i=0;i<menu->GetMenuItemCount();i++)
	{
		//do we have a sub menu in this position?
		CMenu* sub = menu->GetSubMenu(i);
		if (sub != NULL)
		{
			tidyMenu(sub);
			if (sub->GetMenuItemCount() == 0)
			{
				menu->DeleteMenu(i,MF_BYPOSITION);
				i--;
			}
			lastItemSeperator=false;
		}
		else
		{
			//NOTE: GetMenuItemID() returns 0 for a seperator
			UINT menuItemID = menu->GetMenuItemID(i);

			//this caters for the first menu item(s) being aeperator
			if (menuItemID == 0)
			{
				if ((i==0)||(lastItemSeperator))
				{
					menu->DeleteMenu(i,MF_BYPOSITION);
					i--;
				}
				else
				{
					lastItemSeperator = true;
				}
			}
			else
			{
				lastItemSeperator = false;
			}
		}
	}

	//deal with last menu item being seperator
	while ((menu->GetMenuItemCount() > 0)&&(menu->GetMenuItemID(menu->GetMenuItemCount()-1) == 0))
	{
		menu->DeleteMenu(menu->GetMenuItemCount()-1,MF_BYPOSITION);
	}
}


BOOL MainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	if (!CBCGPMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	//to2do add my new weight conversion tool.

	// Add some tools for example....
	CBCGPUserToolsManager* pUserToolsManager = theApp.GetUserToolsManager ();
	if (pUserToolsManager != NULL &&
		pUserToolsManager->GetUserTools ().IsEmpty ())
	{
		CBCGPUserTool* pTool1 = pUserToolsManager->CreateNewTool ();
		pTool1->m_strLabel = _T("Notepad");
		pTool1->SetCommand (_T("notepad.exe"));

		CBCGPUserTool* pTool2 = pUserToolsManager->CreateNewTool ();
		pTool2->m_strLabel = _T("Paint Brush");
		pTool2->SetCommand (_T("mspaint.exe"));

		CBCGPUserTool* pTool3 = pUserToolsManager->CreateNewTool ();
		pTool3->m_strLabel = _T("Windows Explorer");
		pTool3->SetCommand (_T("explorer.exe"));

		CBCGPUserTool* pTool4 = pUserToolsManager->CreateNewTool ();
		pTool4->m_strLabel = _T("Ringing Master On-Line");
		pTool4->SetCommand (_T("http://www.ringingmaster.co.uk"));
	}

	//----------------------------------------------------
	// Enable customization button fore all USER toolbars:
	//----------------------------------------------------
	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CBCGPToolBar* pUserToolbar = GetUserBarByIndex (i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));
		}
	}


	CBCGPMDITabParams cBCGPMDITabParams;
	cBCGPMDITabParams.m_bAutoColor = TRUE;
	cBCGPMDITabParams.m_bDocumentMenu = TRUE;
	cBCGPMDITabParams.m_bEnableTabSwap = TRUE;
	cBCGPMDITabParams.m_bTabCloseButton = TRUE;
	cBCGPMDITabParams.m_bTabIcons = TRUE;
	cBCGPMDITabParams.m_nTabBorderSize = 1;
	cBCGPMDITabParams.m_tabLocation = CBCGPTabWnd::LOCATION_TOP;
	cBCGPMDITabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
	EnableMDITabbedGroups(TRUE, cBCGPMDITabParams);


	return TRUE;
}

BOOL MainFrame::OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop)
{
	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_MDITABS));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if (bDrop)
	{
		pPopup->DeleteMenu (ID_FILE_SAVE, MF_BYCOMMAND);
		pPopup->DeleteMenu (ID_FILE_CLOSE, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_HORZ_TAB_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_VERT_TAB_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_NEXT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_PREV_GROUP, MF_BYCOMMAND);
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->SetAutoDestroy (FALSE);
	pPopupMenu->Create (this, point.x, point.y, pPopup->GetSafeHmenu ());

	return TRUE;

}



LRESULT MainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CBCGPMDIFrameWnd::OnToolbarCreateNew (wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CBCGPToolBar* pUserToolbar = (CBCGPToolBar*) lres;
	ASSERT_VALID (pUserToolbar);

	pUserToolbar->EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));
	return lres;
}

void MainFrame::OnMdiMoveToNextGroup()
{
	MDITabMoveToNextGroup ();
}

void MainFrame::OnMdiMoveToPrevGroup()
{
	MDITabMoveToNextGroup (FALSE);
}

void MainFrame::OnMdiNewHorzTabGroup()
{
	MDITabNewGroup (FALSE);
}

void MainFrame::OnMdiNewVertGroup()
{
	MDITabNewGroup ();
}

CBCGPMDIChildWnd* MainFrame::CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* pObj)
{
	if (lpcszDocName != NULL && lpcszDocName [0] != '\0')
	{
		CDocument* pDoc = NULL;

#ifndef _DEBUG
		try
		{
#endif // _DEBUG
			//create the documents for the non disk serialised views
			if (CString(lpcszDocName) == ChildFrame::_TowerDocumentToken)
			{
				pDoc = app()->openDocument(vt_tower);
				TowerDoc* towerDoc = dynamic_cast<TowerDoc*>(pDoc);
				TowerDocumentInfo* info = DYNAMIC_DOWNCAST(TowerDocumentInfo, pObj);
				if ((towerDoc)&&(info))
					towerDoc->setTowerDocInfo(info);
			}
			else if (CString(lpcszDocName) == ChildFrame::_GrabDocumentToken)
			{
				pDoc = app()->openDocument(vt_grab);
				GrabDoc* grabDoc = dynamic_cast<GrabDoc*>(pDoc);
				GrabDocumentInfo* info = DYNAMIC_DOWNCAST(GrabDocumentInfo, pObj);
				if ((grabDoc)&&(info))
					grabDoc->setGrabDocInfo(info);
			}
			else if (CString(lpcszDocName) == ChildFrame::_MapDocumentToken)
			{
				MapDocumentInfo* info = DYNAMIC_DOWNCAST(MapDocumentInfo, pObj);
				pDoc = mapManager()->showTowerMapDoc(info);
			}
			else if (CString(lpcszDocName) == ChildFrame::_InternetDocumentToken)
			{
				InternetDocumentInfo* info = DYNAMIC_DOWNCAST(InternetDocumentInfo, pObj);
				pDoc = app()->launchBrowser(info->getURL());
			}
			else
				pDoc = AfxGetApp()->OpenDocumentFile (lpcszDocName);
#ifndef _DEBUG
		}
		catch(...)
		{
			//do nothing
		}
#endif // _DEBUG

		ASSERT(pDoc);

		if (pDoc != NULL)
		{
			POSITION pos = pDoc->GetFirstViewPosition();

			if (pos != NULL)
			{
				CView* pView = pDoc->GetNextView (pos);
				return DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, pView->GetParent ());
			}
		}
	}

	ASSERT(FALSE);
	return NULL;
}

void MainFrame::UpdateMRUFilesList ()
{
	ASSERT(_taskPaneCtrlBar);
	if (_taskPaneCtrlBar)
		_taskPaneCtrlBar->UpdateMRUFilesList ();
}

void MainFrame::simulatorManager_notifyNumberChanged(int number)
{
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_SIMULATOR_SET_BELL1, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition ();
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo =
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL)
			{
				pCombo->RemoveAllItems();
				for (int i=0;i<number;i++)
				{
					CString str;
					str.Format("Bell %c", GlobalFunctions::bellNumbersToChar(i+1));
					pCombo->AddItem(str, i);
				}

				pCombo->SelectItem(simulatorManager()->getSimulatorStrikingSngleBell(sswt_single_1)); 
			}
		}
	}	
	
	if (CBCGPToolBar::GetCommandButtons (ID_SIMULATOR_SET_BELL2, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition ();
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo =
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL)
			{
				pCombo->RemoveAllItems();
				for (int i=0;i<number;i++)
				{
					CString str;
					str.Format("Bell %c", GlobalFunctions::bellNumbersToChar(i+1));
					pCombo->AddItem(str, i);
				}


				pCombo->SelectItem(simulatorManager()->getSimulatorStrikingSngleBell(sswt_single_2)); 
			}
		}
	}	
}

void MainFrame::strikeManager_notifyUpdateStrikeList()
{
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

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
				pCombo->RemoveAllItems();
				for (int i=0;i<strikeManager()->getMaxBells();i++)
				{
					CString str;
					str.Format("Bell %c", GlobalFunctions::bellNumbersToChar(i+1));
					pCombo->AddItem(str, i);
				}
				pCombo->SelectItem(strikeManager()->getAnalysisBell());
			}
		}
	}	
}

void MainFrame::hideVisualFeedbackForSimulator()
{
	ShowControlBar (_controlBars[tt_controlBarSimulatorStrikingControls],FALSE,FALSE, FALSE);
	ShowControlBar (_controlBars[tt_controlBarSimulatorSingleBell1],FALSE,FALSE, FALSE);
	ShowControlBar (_controlBars[tt_controlBarSimulatorSingleBell2],FALSE,FALSE, FALSE);
	ShowControlBar (_controlBars[tt_controlBarSimulatorStriking],FALSE,FALSE, FALSE);
}

LRESULT MainFrame::OnApplyCombinedFilter(WPARAM wParam, LPARAM /*lParam*/)
{
	
	matrixManager()->applyCombinedFilter(wParam);
	return 0;
}
