// MainFrm.h : interface of the MainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BAD8B19C_D196_11D4_B554_009027BB3286__INCLUDED_)
#define AFX_MAINFRM_H__BAD8B19C_D196_11D4_B554_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TowerManagerEventListener.h"
#include "MainFrameEventListener.h"
#include "RecordManagerEventListener.h"
#include "StrikeManagerEventListener.h"
#include "SimulatorManagerEventListener.h"
#include "ToolbarTypes.h"


class PlayPosition;
class Music;					 
class TouchDoc;
class ExplorerFolder;
class NotationLite;
class Tower;
class TowerDoc;
class MapDoc;
class GrabDoc;
class UserManager;
class RecordManager;
class TowerManager;
class SimulatorManager;
class MultiBellInterfaceManager;
class PlayManager;
class SpeechManager;
class StrikeManager;
class MapManager;
class StyleManager;
class GlobalDataManager;
class StatusBar;
class InternetDoc;
class CribDoc;
class MatrixDoc;

class StrikingControls;
class TaskPane;
class StrikeDoc;

class ToolbarFileState;

enum StateType;


class MainFrame : public CBCGPMDIFrameWnd, 
	public TowerManagerEventListener, 
	public MainFrameEventListener, 
	public RecordManagerEventListener,
	public StrikeManagerEventListener, 
	public SimulatorManagerEventListener

{
	DECLARE_DYNAMIC(MainFrame)
public:
	MainFrame();

	void hideVisualFeedbackForSimulator();

	void addEventListener(MainFrameEventListener* mainFrameEventListener);
	void removeEventListener(MainFrameEventListener* mainFrameEventListener);
	void fireUpdate();
	void fireDocViewChanged(ViewType oldViewType, ViewType newViewType, CDocument* oldDoc, CDocument* newDoc);
	void fireIdle();
	void mainFrame_notifyUpdate(ViewType viewType);

	void strikeManager_notifyUpdateStrikeList();
	void simulatorManager_notifyNumberChanged(int number);

	void showStrikingControls(Tower* tower, int bell);
	
// Attributes
public:

// Operations
public:

	CMenu	m_menuFavotites;


	virtual BOOL OnDrawMenuImage (CDC* pDC,
									const CBCGPToolbarMenuButton* pMenuButton, 
									const CRect& rectImage);	   

	virtual BOOL OnMenuButtonToolHitTest (CBCGPToolbarButton* pButton, TOOLINFO* pTI);
	BOOL IsFavoritesMenu (const CBCGPToolbarMenuButton* pMenuButton);
	void SetAddress(LPCTSTR lpszUrl);


// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainFrame)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:



	void showHidePlayInfo(bool forceHide = false);
	void showHideTaskBar();

	MatrixDoc* getActiveMatrixDoc();

	TouchDoc * getActiveTouchDoc();
	StrikeDoc * getActiveStrikeDoc();
	MapDoc * getActiveMapDoc();
	CribDoc * getActiveCribDoc();
	InternetDoc * getActiveInternetDoc();

	CDocument* getActiveDocument();
	CWnd * _pMethodWnd;
	
	BOOL verifyKey(CString user = "", CString key = "");
	bool hasTrialExpired();

	void UpdateCaption(CString str);

	virtual ~MainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void hideNonAllowedToolbars(ViewType viewType);
	void doShowHideToolbars(const ViewType viewType, const ToolbarFileState& stateWanted, const ToolbarFileState& activateState);
	void doShowHideToolbars(const ViewType viewType, const ToolbarFileState& stateWanted);	
	void getCurrentToolbarState(ToolbarFileState& stateNow);
	void showDefaultVisibility(ViewType viewType);
	void dockToolbars(StateType stateType);

	 // control bars 
	CBCGPControlBar*	_controlBars[LAST_CONTROLBAR+1];

	//ribbon bar 
	CBCGPRibbonBar*			_ribbonBar;
	CBCGPRibbonMainButton	_mainButton;
	CBCGPToolBarImages		_ribbonPanelImages;


	//non show hide control bars
	//CBCGPMenuBar*		 _menuBar; //todo
	StatusBar*			 _statusBar;
	TaskPane*			_taskPaneCtrlBar;

	//dropdown control bars
	//should not be in _controlBars array 
	CBCGPDropDownToolBar _touchAnalysisDropdownToolBar; 
	CBCGPDropDownToolBar _recordAnalysisDropdownToolBar; 

	//GUI components
	CFont				_fontVertCaption;
	CBCGPToolBarImages	_userImages;


// Generated message map functions
public:
	static void tidyMenu(CMenu *menu);
	void setInternetMessage(CString message);
	void getAllUsedMaps(Ints& ints);
	void closeAllMapDocuments();
	void postLoadState();
	void loadToolbarMenu(CMenu& menu);

	void towerManager_notifyUpdateTowerList();
	void doShowLeadHead(CWnd* parent);
	enum trialState
	{
		ts_ok,
		ts_expired,
		ts_no_reg_access,
		ts_tampered,

	} _trialState;

	int _trialDaysLeft;


	void UpdateMRUFilesList ();

	//{{AFX_MSG(MainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolsOptions();
	afx_msg void OnClose();
	afx_msg void OnWindowManager();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAppAbout();
	afx_msg void OnWindowCloseall();
	afx_msg void OnUpdateWindowCloseall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowSaveall(CCmdUI* pCmdUI);
	afx_msg void OnHelpKeyboardmap();
	afx_msg void OnUpdateStatusBarPane(CCmdUI* pCmdUI);
	afx_msg void OnShowLeadhead();
	afx_msg void OnToolsCallchangeeditor();
	afx_msg void OnCribOptions();
	afx_msg void OnTouchOptions();
	afx_msg void OnSimulatorOptions();
	afx_msg void OnFilePageSetup();
	afx_msg void OnUpdateFilePageSetup(CCmdUI* pCmdUI);
	afx_msg void OnHistory(UINT nID);
	//}}AFX_MSG
	 
	void doSimulatorOptions(bool towerSounds, CWnd* parent);
	afx_msg void OnViewFullScreen();

	void doCribOptions(CWnd* pWnd);

	afx_msg void OnFavorite(UINT nID);


	afx_msg LRESULT OnInternetMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTouchDocUpdateNumber(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTouchDocUpdateStyleColor(WPARAM wParam, LPARAM lParam);
	

	afx_msg LRESULT OnBCGHelp(WPARAM wParam, LPARAM lParam);
	

	
	void AdjustColorsMenu (CBCGPPopupMenu* pMenuPopup, UINT uiId, CString menuText);

	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnViewStandard();
	afx_msg void OnUpdateViewStandard(CCmdUI* pCmdUI);
	afx_msg void OnViewTool();
	afx_msg void OnUpdateViewTool(CCmdUI* pCmdUI);
	afx_msg void OnViewTouchMain();
	afx_msg void OnUpdateViewTouchMain(CCmdUI* pCmdUI);
	afx_msg void OnViewTouchBlueLine();
	afx_msg void OnUpdateViewTouchBlueLine(CCmdUI* pCmdUI);
	afx_msg void OnViewSimulator();
	afx_msg void OnUpdateViewSimulator(CCmdUI* pCmdUI);
	afx_msg void OnViewInternet();
	afx_msg void OnUpdateViewInternet(CCmdUI* pCmdUI);
	afx_msg void OnViewImage();
	afx_msg void OnUpdateViewImage(CCmdUI* pCmdUI);
	afx_msg void OnViewMap();
	afx_msg void OnUpdateViewMap(CCmdUI* pCmdUI);
	afx_msg void OnViewCrib();
	afx_msg void OnUpdateViewCrib(CCmdUI* pCmdUI);
	afx_msg void OnViewStrike();
	afx_msg void OnUpdateViewStrike(CCmdUI* pCmdUI);
	afx_msg void OnViewProperties();
	afx_msg void OnUpdateViewProperties(CCmdUI* pCmdUI);
	afx_msg void OnViewTaskPane();
	afx_msg void OnUpdateViewTaskPane(CCmdUI* pCmdUI);
	
	afx_msg void OnViewAnalysisStatus();
	afx_msg void OnUpdateViewAnalysisStatus(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisMusic();
	afx_msg void OnUpdateViewAnalysisMusic(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisCourseHead();
	afx_msg void OnUpdateViewAnalysisCourseHead(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisLeadEnd();
	afx_msg void OnUpdateViewAnalysisLeadEnd(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisAllTheWork();
	afx_msg void OnUpdateViewAnalysisAllTheWork(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisMultiBlock();
	afx_msg void OnUpdateViewAnalysisMultiBlock(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisCallsMade();
	afx_msg void OnUpdateViewAnalysisCallsMade(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisSplice();
	afx_msg void OnUpdateViewAnalysisSplice(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisSpliceTransition();
	afx_msg void OnUpdateViewAnalysisSpliceTransition(CCmdUI* pCmdUI);
	afx_msg void OnViewAnalysisFalseness();
	afx_msg void OnUpdateViewAnalysisFalseness(CCmdUI* pCmdUI);
	afx_msg void OnViewSimulatorMethod();
	afx_msg void OnUpdateViewSimulatorMethod(CCmdUI* pCmdUI);
	afx_msg void OnViewSimulatorStriking();
	afx_msg void OnUpdateViewSimulatorStriking(CCmdUI* pCmdUI);           
	afx_msg void OnViewSimulatorStrikingSingleBell1();
	afx_msg void OnUpdateViewSimulatorStrikingSingleBell1(CCmdUI* pCmdUI);
	afx_msg void OnViewSimulatorStrikingSingleBell2();
	afx_msg void OnUpdateViewSimulatorStrikingSingleBell2(CCmdUI* pCmdUI);
	afx_msg void OnViewSimulatorInputSpy();
	afx_msg void OnUpdateViewSimulatorInputSpy(CCmdUI* pCmdUI);
	afx_msg void OnViewSimulatorInputsMap();
	afx_msg void OnUpdateViewSimulatorInputsMap(CCmdUI* pCmdUI);
	afx_msg void OnViewStrikeGraph();
	afx_msg void OnUpdateViewStrikeGraph(CCmdUI* pCmdUI);
	afx_msg void OnViewStrikeInfo();
	afx_msg void OnUpdateViewStrikeInfo(CCmdUI* pCmdUI);
	afx_msg void OnViewSimulatorStrikingControls();
	afx_msg void OnUpdateViewSimulatorStrikingControls(CCmdUI* pCmdUI);
	afx_msg void OnViewRecordHomeTower();
	afx_msg void OnUpdateViewRecordHomeTower(CCmdUI* pCmdUI);
	afx_msg void OnViewRecordSelectedTower();
	afx_msg void OnUpdateViewRecordSelectedTower(CCmdUI* pCmdUI);

	afx_msg void OnViewPlay();
	afx_msg void OnUpdateViewPlay(CCmdUI* pCmdUI);
	afx_msg void OnViewPlaySimulator();
	afx_msg void OnUpdateViewPlaySimulator(CCmdUI* pCmdUI);
	afx_msg void OnViewRecord();
	afx_msg void OnUpdateViewRecord(CCmdUI* pCmdUI);
	afx_msg void OnViewRecordLogic();
	afx_msg void OnUpdateViewRecordLogic(CCmdUI* pCmdUI);
	afx_msg void OnViewSimulatorCalls();
	afx_msg void OnUpdateViewSimulatorCalls(CCmdUI* pCmdUI);

	afx_msg void OnAskQuestion();

	LRESULT OnToolbarCreateNew(WPARAM wp,LPARAM lp);
	BOOL OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop);


	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();

	afx_msg LRESULT OnApplyCombinedFilter(WPARAM wParam, LPARAM lParam);
	
	CBCGPMDIChildWnd* CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* pObj);


	DECLARE_MESSAGE_MAP()


	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);  
	virtual void OnDrawMenuLogo (CDC* pDC, CBCGPPopupMenu* pMenu, const CRect& rectLogo);
	void createRibbonBar() ;

protected:
	CString _internetMessage;

	MainFrameEventListeners _listenerList;
	ViewType _curViewType;


	void saveCurrentWindowPositionState(StateType curStateType);
	void loadCurrentWindowPositionState(StateType curStateType);
	
	void loadCurrentVolatileToolbarVisibility(ViewType viewType);
	void saveCurrentVolatileToolbarVisibility(ViewType viewType);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BAD8B19C_D196_11D4_B554_009027BB3286__INCLUDED_)



