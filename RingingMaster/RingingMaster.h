// RingingMasterApp.h : main header file for the RingingMaster application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "MainFrame.h"
#include "resource.h"       // main symbols				

#include "ImageFormats.h"
#include "FavoritesManager.h"
#include "ManagerBase.h"


class Method;
class InternetManager;
class SerialPortManager;
class CribManager;
class MultiBellInterfaceManager;
class PlayManager;
class ProofManager;
class KeyboardManager;
class MethodManager;
class ToolbarManager;
class SerialControlManager;
class MatrixManager;
class OptionsManager;


/////////////////////////////////////////////////////////////////////////////
// RingingMasterApp:
// See RingingMaster.cpp for the implementation of this class
//



enum ViewType
{
	// Make sure the 'define below is correct

	vt_none = 0,
	vt_touch_blue_line = 1,
	vt_touch_main = 2,
	vt_tower = 3,//must stay in the same place, setialised in RecordManager
	vt_grab = 4, //must stay in the same place, setialised in RecordManager
	vt_map = 5,
	vt_internet = 6, 
	vt_image = 7, 
	vt_crib = 8, 
	vt_touch_simulator = 9,
	vt_strike = 10,

	//add new stuff to the end

	vt_null, //liave this at the end

};

#define FIRST_VIEW_TYPE  vt_none
#define LAST_VIEW_TYPE  vt_null


class RingingMasterApp : public CWinApp,
					  public CBCGPWorkspace
{
public:
	static BOOL CreateAllDirectories(char*   pszDir );
	CDocument* openDocument(ViewType viewType, CString fileName = CString(""));
	void setupTitle();
	RingingMasterApp();
	virtual ~RingingMasterApp();
	CFont _fontStd;

	CFavoritesManager	m_Favorites;
 	// Operations
	
	// Override from CBCGPWorkspace
	virtual void PreLoadState ();
	virtual void LoadCustomState();

	static const CString _RegistryBase ;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RingingMasterApp)
	public:
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(RingingMasterApp)

	afx_msg void OnFileSaveall();
	afx_msg void OnMapApplyMap(UINT wParam, LONG lParam);		
	//}}AFX_MSG

	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	void doFileNew(ViewType viewType);
	DECLARE_MESSAGE_MAP()
	
public:
	bool isInited();
	void loadFavorites();
	static int GetImageTypeFromIndex(int nIndex);
	static int GetImageIndexFromType(int nDocType);
	static CString GetImageFileTypes();
	static ENUM_CXIMAGE_FORMATS GetImageType(CString extention);
	ENUM_CXIMAGE_FORMATS GetImageTypeFromPath(CString path);

	InternetDoc* launchBrowser(CString url);
	
	CDocument* _pIdleDoc;
	ViewType _idleViewType;
	static CString getExtention(ViewType viewType);
	ViewType getViewType();
	CString getDocumentDirectory(bool forceUseDir = false);
	CString _rootPath;
	CString _rootPathFull;
	long _windowsVersion;
	UINT _wTimerRes;    


	InternetManager* internetManager()			{return _internetManager;}
	TowerManager*	towerManager()				{return _towerManager;}
	SimulatorManager*	simulatorManager()		{return _simulatorManager;}
	MultiBellInterfaceManager*	multiBellInterfaceManager()		{return _multiBellInterfaceManager;}
	SerialControlManager* serialControlManager() {return _serialControlManager;}
	PlayManager*	playManager()				{return _playManager;}
	ProofManager*	proofManager()				{return _proofManager;}
	KeyboardManager*	keyboardManager()		{return _keyboardManager;}
	SpeechManager*	speechManager()				{return _speechManager;}
	StrikeManager*	strikeManager()				{return _strikeManager;}
	SerialPortManager*	serialPortManager()		{return _serialPortManager;}
	MapManager*		mapManager()				{return _mapManager;}
	UserManager*	userManager()				{return _userManager;}
	RecordManager*	recordManager()				{return _recordManager;}
	StyleManager*	styleManager()				{return _styleManager;}
	CribManager*	cribManager()				{return _cribManager;}
	MethodManager*	methodManager()				{return _methodManager;}
	GlobalDataManager* globalDataManager()		{return _globalDataManager;}
	ToolbarManager* toolbarManager()			{return _toolbarManager;}
	MatrixManager* matrixManager()				{return _matrixManager;}
	OptionsManager* optionsManager()			{return _optionsManager;}

	ManagerBases _managers;
	

protected:

	bool _resetLayoutOnShutingDown;
	bool _inited; 

	void PreSaveState();

	void ShowTipOfTheDay(void);


	InternetManager* _internetManager;
	TowerManager* _towerManager;
	SimulatorManager* _simulatorManager;
	SpeechManager* _speechManager;
	StrikeManager* _strikeManager;
	MultiBellInterfaceManager* _multiBellInterfaceManager;
	SerialControlManager* _serialControlManager;
	PlayManager* _playManager;
	ProofManager* _proofManager;
	KeyboardManager* _keyboardManager;
	SerialPortManager* _serialPortManager;
	MapManager* _mapManager;
	UserManager* _userManager;
	RecordManager* _recordManager;
	StyleManager* _styleManager;
	CribManager* _cribManager;
	MethodManager* _methodManager;
	GlobalDataManager* _globalDataManager;
	ToolbarManager* _toolbarManager;	
	MatrixManager* _matrixManager;
	OptionsManager* _optionsManager;

public:
	afx_msg void OnNewcrib();
	afx_msg void OnNewtouch();
	afx_msg void OnWindowResetlayout();
	afx_msg void OnUpdateWindowResetlayout(CCmdUI *pCmdUI);

};


extern RingingMasterApp theApp;



inline RingingMasterApp* AFXAPI app()
	{ return (RingingMasterApp*)afxCurrentWinApp; }	  

inline MainFrame* AFXAPI mainFrame()
{ return (MainFrame*)AfxGetApp()->m_pMainWnd; }

inline StatusBar* AFXAPI statusBar()
	{ return ((MainFrame*)AfxGetApp()->m_pMainWnd)->_statusBar; }

inline TowerManager* AFXAPI towerManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->towerManager(); }	  

inline SimulatorManager* AFXAPI simulatorManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->simulatorManager(); }	  

inline SerialPortManager* AFXAPI serialPortManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->serialPortManager(); }	  

inline SpeechManager* AFXAPI speechManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->speechManager(); }	  

inline StrikeManager* AFXAPI strikeManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->strikeManager(); }	  

inline MultiBellInterfaceManager* AFXAPI multiBellInterfaceManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->multiBellInterfaceManager(); }	  

inline SerialControlManager* AFXAPI serialControlManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->serialControlManager(); }	  

inline PlayManager* AFXAPI playManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->playManager(); }	  

inline ProofManager* AFXAPI proofManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->proofManager(); }	  

inline KeyboardManager* AFXAPI keyboardManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->keyboardManager(); }	  

inline InternetManager* AFXAPI internetManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->internetManager(); }	  

inline MapManager* AFXAPI mapManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->mapManager(); }	  

inline UserManager* AFXAPI userManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->userManager(); }	  

inline StyleManager* AFXAPI styleManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->styleManager(); }	  

inline CribManager* AFXAPI cribManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->cribManager(); }	  

inline MethodManager* AFXAPI methodManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->methodManager(); }	  

inline GlobalDataManager* AFXAPI globalDataManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->globalDataManager(); }	  

inline RecordManager* AFXAPI recordManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->recordManager(); }	  

inline ToolbarManager* AFXAPI toolbarManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->toolbarManager(); }	

inline MatrixManager* AFXAPI matrixManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->matrixManager(); }	

inline OptionsManager* AFXAPI optionsManager()
	{ return ((RingingMasterApp*)afxCurrentWinApp)->optionsManager(); }	

