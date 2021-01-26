// RingingMaster.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RingingMaster.h"

#include "MainFrame.h"
#include "TouchDoc.h"
#include "TouchHoldView.h"
#include "TowerDoc.h"
#include "TowerView.h"
#include "StrikeDoc.h"
#include "MatrixDoc.h"
#include "StrikeView.h"
#include "GrabDoc.h"
#include "GrabView.h"
#include "MapDoc.h"
#include "MapView.h"
#include "InternetDoc.h"
#include "InternetView.h"
#include "resource.h"
#include "AboutDlg.h"
#include "Tower.h"
#include "UserMessages.h"
#include "TowerManager.h"
#include "SimulatorManager.h"
#include "MultiBellInterfaceManager.h"
#include "SerialControlManager.h"
#include "PlayManager.h"
#include "ProofManager.h"
#include "KeyboardManager.h"
#include "SpeechManager.h"
#include "StrikeManager.h"
#include "MapManager.h"
#include "CribManager.h"
#include "UserManager.h"
#include "RecordManager.h"
#include "SerialPortManager.h"
#include "StyleManager.h"
#include "MatrixManager.h"
#include "OptionsManager.h"
#include "MethodManager.h"
#include "InternetManager.h"
#include "InternetDoc.h"
#include "AutoPopup.h"
#include "CribDoc.h"
#include "CribView.h"
#include "NewDocumentWizard.h"
#include "ChildFrmSplitter.h"


#include "ImageDoc.h"
#include "ImageView.h"
#include "SplashScreenEx.h"
#include "ChildFrm.h"
#include "ToolbarManager.h"
#include ".\ringingmaster.h"
#include "DocManagerEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ImageFormats imageFormats[] =
{
#if CXIMAGE_SUPPORT_BMP
	{ CXIMAGE_FORMAT_BMP, "Windows Bitmap", "*.bmp" },
#endif
#if CXIMAGE_SUPPORT_GIF
	{ CXIMAGE_FORMAT_GIF, "Graphics Interchange Format files", "*.gif" },
#endif
#if CXIMAGE_SUPPORT_JPG
	{ CXIMAGE_FORMAT_JPG, "JPG files", "*.jpg;*.jpeg" },
#endif
#if CXIMAGE_SUPPORT_PNG
	{ CXIMAGE_FORMAT_PNG, "PNG files", "*.png" },
#endif
#if CXIMAGE_SUPPORT_PCX
	{ CXIMAGE_FORMAT_PCX, "PCX files", "*.pcx" },
#endif


};

const CString RingingMasterApp::_RegistryBase  = "Settings";

//todo put in the crash analyis code. 

/////////////////////////////////////////////////////////////////////////////
// RingingMasterApp

BEGIN_MESSAGE_MAP(RingingMasterApp, CWinApp)
	ON_COMMAND(ID_HELP_TIPOFTHEDAY, ShowTipOfTheDay)
	//{{AFX_MSG_MAP(RingingMasterApp)
	ON_COMMAND(ID_FILE_SAVEALL, OnFileSaveall)
	ON_THREAD_MESSAGE(MAP_REQ_APPLY_MAP, OnMapApplyMap)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)


	ON_COMMAND(ID_NEWCRIB, OnNewcrib)
	ON_COMMAND(ID_NEWTOUCH, OnNewtouch)

	ON_COMMAND(ID_WINDOW_RESETLAYOUT, OnWindowResetlayout)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_RESETLAYOUT, OnUpdateWindowResetlayout)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// RingingMasterApp construction

RingingMasterApp::RingingMasterApp() :
CBCGPWorkspace(TRUE),//for smart update of resources
_windowsVersion(VER_PLATFORM_WIN32_WINDOWS),
_wTimerRes(INT_MAX),
_pIdleDoc(NULL),
_idleViewType(vt_none),
_inited(false), 
_resetLayoutOnShutingDown(false)
{

	EnableHtmlHelp();

#ifndef _DEBUG
	//only allow a single instance of ringingmaster per machine
	::CreateMutex(NULL, FALSE, "RingingMasterAppMultipleInstanceMutex");

	if(::GetLastError() == ERROR_ALREADY_EXISTS){
		// dont load this instance.
		AfxMessageBox("You already have Ringing Master open", MB_ICONEXCLAMATION);
		exit(0);
	}

#endif

	_internetManager = new InternetManager;
	_managers.Add(_internetManager);

	_towerManager = new TowerManager;
	_managers.Add(_towerManager);

	_userManager = new UserManager;//must be done after the tower save
	_managers.Add(_userManager);

	_recordManager = new RecordManager;
	_managers.Add(_recordManager);

	_simulatorManager = new SimulatorManager;
	_managers.Add(_simulatorManager);

	_multiBellInterfaceManager = new MultiBellInterfaceManager;
	_managers.Add(_multiBellInterfaceManager);

	_serialControlManager = new SerialControlManager;
	_managers.Add(_serialControlManager);

	_playManager = new PlayManager;
	_managers.Add(_playManager);

	_proofManager = new ProofManager;
	_managers.Add(_proofManager);

	_keyboardManager = new KeyboardManager;
	_managers.Add(_keyboardManager);

	_speechManager = new SpeechManager;
	_managers.Add(_speechManager);

	_strikeManager = new StrikeManager;
	_managers.Add(_strikeManager);

	_serialPortManager = new SerialPortManager;
	_managers.Add(_serialPortManager);

	_mapManager = new MapManager;
	_managers.Add(_mapManager);

	_styleManager = new StyleManager;
	_managers.Add(_styleManager);

	_cribManager = new CribManager;
	_managers.Add(_cribManager);

	_methodManager = new MethodManager;
	_managers.Add(_methodManager);

	_globalDataManager = new GlobalDataManager;
	_managers.Add(_globalDataManager);

	_toolbarManager = new ToolbarManager;
	_managers.Add(_toolbarManager);

	_matrixManager = new MatrixManager;
	_managers.Add(_matrixManager);

	_optionsManager = new OptionsManager;
	_managers.Add(_optionsManager);

}

RingingMasterApp::~RingingMasterApp()
{

}

/////////////////////////////////////////////////////////////////////////////
// The one and only RingingMasterApp object

RingingMasterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// RingingMasterApp initialization


BOOL RingingMasterApp::InitInstance()
{

#ifdef _DEBUG
//	CString regkey;
//	regkey.LoadString(AFX_IDS_APP_TITLE);

	//CBCGPRegistry reg(FALSE, FALSE);
	//reg.DeleteKey("Software\\Sensation Software\\" + regkey);
#endif //_DEBUG


		

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	setlocale(LC_ALL, "");
    
	//get the platform info
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionEx (&osvi);
	_windowsVersion = osvi.dwPlatformId;

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox("OLE init failed");
		return FALSE;
	}

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//start the splash screan
	CSplashScreenEx::EnableSplashScreen(cmdInfo.m_bShowSplash);

	// older versions of Windows* (NT 3.51 for instance)
	// fail with DEFAULT_GUI_FONT
	if (!_fontStd.CreateStockObject(DEFAULT_GUI_FONT))
		if (!_fontStd.CreatePointFont(80, "MS Sans Serif"))
			return -1;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


	CSplashScreenEx::SetTextStatic("Loading");
	//reg stuff
	SetRegistryKey(_T("Sensation Software"));
	LoadStdProfileSettings(10);

	SetRegistryBase (RingingMasterApp::_RegistryBase);


	//check version and reset if needed.
	const int thisVersion = 3020;
	int regVersion = GetProfileInt("MainFrame", "Last opened version", 0);

	if (regVersion > thisVersion)
	{
		//do not allow any further interaction
		CString str;
		str.Format("You have installed version %d. There is no backwards compatability. Please remove this version (%d).", regVersion, thisVersion);
		AfxMessageBox(str);
		exit(0);
	}

	WriteProfileInt("MainFrame", "Last opened version", thisVersion);

	if (regVersion != thisVersion)
	{
		CBCGPRegistry reg(FALSE, FALSE);
		CString regkey;
		regkey.LoadString(AFX_IDS_APP_TITLE);

		reg.DeleteKey("Software\\Sensation Software\\" + regkey + "\\" + RingingMasterApp::_RegistryBase);
		TRACE("Deleting Software\\Sensation Software\\" + regkey + "\\" + RingingMasterApp::_RegistryBase + "\r\n");

		reg.DeleteKey("Software\\Sensation Software\\" + regkey + "\\" + "Towers");
		TRACE("Deleting Software\\Sensation Software\\" + regkey + "\\" + "Towers" + "\r\n");
	}


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	//create an instance of my overridden doc manager.
	//This will only register some of teh document types.
	ASSERT(m_pDocManager == NULL);
	m_pDocManager = new CDocManagerEx;

	CMultiDocTemplate* pDocTemplate;

	//NOTE: ID's must be in order to register the icons correctly
	// also have to get the order correct in the rc file.
	pDocTemplate = new CMultiDocTemplate(
		IDR_TOUCHTYPE,   //2
		RUNTIME_CLASS(TouchDoc),			    
		RUNTIME_CLASS(ChildFrame),		   //to2do use splitter
		RUNTIME_CLASS(TouchHoldView));
	AddDocTemplate(pDocTemplate);
	
	pDocTemplate = new CMultiDocTemplate(
		IDR_CRIBTYPE,  //3
		RUNTIME_CLASS(CribDoc),
		RUNTIME_CLASS(ChildFrame), //to2do use splitter (make sure it is using its own scroll bars)
		RUNTIME_CLASS(CribView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
		IDR_STRIKETYPE,  //4
		RUNTIME_CLASS(StrikeDoc),
		RUNTIME_CLASS(ChildFrameSplitter12),	
		RUNTIME_CLASS(StrikeView));
	AddDocTemplate(pDocTemplate);
  
	pDocTemplate = new CMultiDocTemplate(
		IDR_TOWERTYPE,   //5
		RUNTIME_CLASS(TowerDoc),
		RUNTIME_CLASS(ChildFrame),	//to2do scroll bars different - use view scroll bars - problem then with header
		RUNTIME_CLASS(TowerView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
		IDR_GRABTYPE,   //6
		RUNTIME_CLASS(GrabDoc),
		RUNTIME_CLASS(ChildFrame),   //to2do scroll bars different - use view scroll bars - problem then with header
		RUNTIME_CLASS(GrabView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
		IDR_MAPTYPE, //7
		RUNTIME_CLASS(MapDoc),
		RUNTIME_CLASS(ChildFrameSplitter22),   
		RUNTIME_CLASS(MapView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(		   
		IDR_INTERNETTYPE,   //8
		RUNTIME_CLASS(InternetDoc),
		RUNTIME_CLASS(ChildFrame),
		RUNTIME_CLASS(InternetView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
		IDR_IMAGETYPE,	//9
		RUNTIME_CLASS(ImageDoc),
		RUNTIME_CLASS(ChildFrameSplitter22),   
		RUNTIME_CLASS(ImageView));
	AddDocTemplate(pDocTemplate);




	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager();

	InitTooltipManager();

	CBCGPToolTipParams params;
	params.m_bVislManagerTheme = TRUE;

	theApp.GetTooltipManager ()->SetTooltipParams (
		0xFFFF,
		RUNTIME_CLASS (CBCGPToolTipCtrl),
		&params);

	EnableUserTools (ID_TOOLS_ENTRY, ID_USER_TOOL1, ID_USER_TOOL10,
			RUNTIME_CLASS (CBCGPUserTool));



	//extract the path

	if (osvi.dwMajorVersion > 4)
	{
		char szAppPath[2000];
		char szDrive[200];
		char szDir[200];

		VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, 2000) != 0); //IN WindowsME this does not work correctly.

		_rootPathFull = szAppPath;

		_splitpath(szAppPath, szDrive, szDir, NULL, NULL);

		_rootPath += szDrive;
		_rootPath += szDir;
	}
	else
	{
		//extract the path
		_rootPathFull =  __targv[0];
		_rootPath =  __targv[0];
		_rootPath.Replace("\\RingingMaster.exe", "\\");
		_rootPath.Replace("\\RingingMaster.EXE", "\\");
	}

	WriteProfileString("MainFrame", "Last opened path", _rootPath);


	// create main MDI Frame window
	MainFrame* pMainFrame = NULL;
	m_pMainWnd = pMainFrame = new MainFrame;

	EnableLoadWindowPlacement (FALSE);

	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	CSplashScreenEx::SetTextStatic("Verifying Licence Key");

	BOOL autoOpen = app()->GetProfileInt("MainFrame", "Auto Open Last Project", 1);

	//during standard start up, do not create new child.
#ifndef _DEBUG
	if (mainFrame()->verifyKey())
	{
#endif
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		{

			if (autoOpen)
			{
				if (!pMainFrame->LoadMDIState (GetRegSectionPath ()))
				{
					//open any specific files. e.g. the visual studio start page
				}
			}
		}
		else
		{
			// Dispatch commands specified on the command line
			if (!ProcessShellCommand(cmdInfo))
				return FALSE;
		}
#ifndef _DEBUG
	}
#endif

	//enable drag drop
	m_pMainWnd->DragAcceptFiles();

	// The main window has been initialized, so show and update it.
	if (!ReloadWindowPlacement (pMainFrame))
	{
		pMainFrame->ShowWindow(m_nCmdShow);
		pMainFrame->UpdateWindow();
	}


	// Enable DDE Execute open
	EnableShellOpen();
	UnregisterShellFileTypes();
	RegisterShellFileTypes(TRUE);

	LoadCustomState ();

	CBCGPDockingControlBar::SetCaptionStyle(TRUE, TRUE);

	CSplashScreenEx::SetTextStatic("Showing Window");

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	setupTitle();


	//set tp teh multi media timer resolution
	#define TARGET_RESOLUTION 1         // 1-millisecond target resolution

	TIMECAPS tc;
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
	{
		// Error; application can't continue.
	}
	else
	{
		_wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
		timeBeginPeriod(_wTimerRes);
	}

	// Enable open from command line
	CString cmdLine = m_lpCmdLine;
	if (!cmdLine.IsEmpty())
	{
		CSplashScreenEx::SetTextStatic("Opening Document");
		OpenDocumentFile(cmdLine);
	}



	CSplashScreenEx::SetTextStatic("Setting Up Towers");
	//set up the towers first time around
	towerManager()->initialTowerSetup();


	//update now that all windows are created and registered
	CSplashScreenEx::SetTextStatic("Updating Windows");
	mainFrame()->fireUpdate();

	CSplashScreenEx::SetTextStatic("");

	CSplashScreenEx::HideSplashScreen();

	_inited = true;
	pMainFrame->fireUpdate();


	return TRUE;
}

void RingingMasterApp::PreSaveState()
{
	//hide this if it is shown, to stop it reappearing again on
	// next startup when stopped in the middle of playing.
	mainFrame()->showHidePlayInfo(true);
}



BOOL RingingMasterApp::PreTranslateMessage(MSG* pMsg)
{
	if ((keyboardManager())&&
		(keyboardManager()->PreTranslateMessage(pMsg)))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// RingingMasterApp message handlers

int RingingMasterApp::ExitInstance()
{
	if (_resetLayoutOnShutingDown)
	{
		CBCGPRegistry reg(FALSE, FALSE);
		CString regkey;
		regkey.LoadString(AFX_IDS_APP_TITLE);
		reg.DeleteKey("Software\\Sensation Software\\" + regkey + "\\" + RingingMasterApp::_RegistryBase);
		TRACE("Deleting Software\\Sensation Software\\" + regkey + "\\" + RingingMasterApp::_RegistryBase + "\r\n");
	}

	for (int i=0;i<_managers.GetSize();i++)
		delete _managers.GetAt(i);

	_simulatorManager = NULL;
	_speechManager = NULL;
	_strikeManager = NULL;
	_userManager = NULL;
	_recordManager = NULL;
	_playManager = NULL;
	_proofManager = NULL;
	_towerManager = NULL;
	_keyboardManager = NULL;
	_multiBellInterfaceManager = NULL;
	_serialControlManager = NULL;
	_serialPortManager = NULL;
	_mapManager = NULL;
	_styleManager = NULL;
	_cribManager = NULL;
	_methodManager = NULL;
	_globalDataManager = NULL;
	_toolbarManager = NULL;
	_internetManager= NULL;
	_matrixManager = NULL;

	BCGCBProCleanUp();

	AutoPopup::CleanUp();

	return CWinApp::ExitInstance();
}




void RingingMasterApp::PreLoadState ()
{
	GetMouseManager()->AddView (IDR_TOUCHTYPE, _T("Touch"), IDR_TOUCHTYPE);
	GetMouseManager()->AddView (IDR_CRIBTYPE, _T("Crib"), IDR_CRIBTYPE);
	GetMouseManager()->AddView (IDR_STRIKETYPE, _T("Strike"), IDR_STRIKETYPE);
	GetMouseManager()->AddView (IDR_TOWERTYPE, _T("Tower"), IDR_TOWERTYPE);
	GetMouseManager()->AddView (IDR_GRABTYPE, _T("Grab"), IDR_GRABTYPE);
	GetMouseManager()->AddView (IDR_MAPTYPE, _T("Map"), IDR_MAPTYPE);
	GetMouseManager()->AddView (IDR_IMAGETYPE, _T("Image"), IDR_IMAGETYPE);
	GetMouseManager()->AddView (IDR_INTERNETTYPE, _T("Web"), IDR_INTERNETTYPE);

	GetContextMenuManager()->AddMenu (_T("Touch Grid"), IDR_TOUCH_CONTEXT_GRID);
	GetContextMenuManager()->AddMenu (_T("Touch Defined Blocks"), IDR_TOUCH_CONTEXT_BLOCKS);
	GetContextMenuManager()->AddMenu (_T("Touch"), IDR_TOUCH_CONTEXT_OFF);
	GetContextMenuManager()->AddMenu (_T("Simulator"), IDR_SIMULATOR);
	GetContextMenuManager()->AddMenu (_T("Method"), IDR_METHOD_STYLE_BLUE_LINE);
	GetContextMenuManager()->AddMenu (_T("Map"), IDR_MAP);
	GetContextMenuManager()->AddMenu (_T("Crib"), IDR_METHOD_CRIB);

	//to2do should all menus be loaded here?
}

// called after everything is loaded
void RingingMasterApp::LoadCustomState()
{
	mainFrame()->postLoadState();
}

/////////////////////////////////////////////////////////////////////////////
// RingingMasterApp message handlers

CString RingingMasterApp::getDocumentDirectory(bool forceUseDir)
{
	BOOL useDir = GetProfileInt("MainFrame",
								"Use Working Directory", TRUE);

	if (forceUseDir ||  useDir)
	{
		CString dir = GetProfileString("MainFrame",
									"Working Directory", "");
		if (dir.IsEmpty())
			dir = _rootPath + "Examples\\";

		CFileFind fileFind;
		if (!fileFind.FindFile(dir + "."))
		{
			dir = _rootPath;
		}

		return dir;
	}
	else
	{
		return "";
	}
}


void RingingMasterApp::OnFileNew()
{
	doFileNew(vt_none);
}

void RingingMasterApp::OnNewtouch()
{
	doFileNew(vt_touch_main);
}

void RingingMasterApp::OnNewcrib()
{
	doFileNew(vt_crib);
}

void RingingMasterApp::doFileNew(ViewType viewType)
{
	NewDocumentWizardTransfer trans;

	trans._viewType = viewType;

	NewDocumentWizard dlg(&trans);
	if (dlg.DoModal() != ID_WIZFINISH )
		return;

	ASSERT((trans._viewType == vt_touch_main)||
		   (trans._viewType == vt_crib));

	if ((trans._viewType == vt_touch_main))
	{
		TouchDoc* pDoc = dynamic_cast<TouchDoc*>(openDocument(vt_touch_main));
		if (pDoc)
		{
			pDoc->setInitialData(trans);
		}
	}

	if (trans._viewType == vt_crib)
	{
		for (int i=0;i<trans._notations.GetSize();i++)
		{
			trans._notations.GetAt(i)->crib();
		}
	}
}

CDocument* RingingMasterApp::openDocument(ViewType viewType, CString fileName)
{

	if (m_pDocManager != NULL)
	{
		POSITION TemplatePos = GetFirstDocTemplatePosition();
		while(TemplatePos)//end itterate doc templates
		{
			CDocTemplate * pTemplate = GetNextDocTemplate(TemplatePos);

			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) && strExt == getExtention(viewType))
			{
				CDocument* pDocument = NULL;
				if (fileName.IsEmpty())
					pDocument = pTemplate->OpenDocumentFile(NULL);
				else
					pDocument = pTemplate->OpenDocumentFile(fileName);

				mainFrame()->fireUpdate();
				recordManager()->fireUpdateFilterList();
				towerManager()->fireUpdateTowerList();

				return pDocument;
			}
		}
	}


	return NULL;
}

void RingingMasterApp::OnFileOpen()
{

	CString strFilter;
	strFilter += "Ringing Master Files (*.tch;*.crb;*.strk)|*.tch;*.crb;*.strk|";
	strFilter += "Touch Files (*.tch)|*.tch|";
	strFilter += "Crib Files (*.crb)|*.crb|";
	strFilter += "Strike Files (*.strk)|*.strk|";

	strFilter += GetImageFileTypes();


	CString allFilter;

	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += "|*.*||";


	CFileDialog dlg(TRUE,
					"",NULL,
					OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
					strFilter,NULL);

	//to2do internet files

	dlg.m_ofn.lpstrTitle = "Open";


	//declare my own buffer to get around the 256 char limit
	CString strFileName;
	//set a limit -in character-
	int maxChar = 5000;
	//give dialog a hint
	dlg.m_ofn.lpstrFile = strFileName.GetBuffer(maxChar);
	dlg.m_ofn.nMaxFile = maxChar;

	//set initiall directory
	CString temp = getDocumentDirectory();
	if (!temp.IsEmpty())
		dlg.m_ofn.lpstrInitialDir = temp;


	if(dlg.DoModal()  != IDOK)
	{
		strFileName.ReleaseBuffer();
		return;
	}
	else
	{
		strFileName.ReleaseBuffer();
		//do the actual opening
		POSITION pos = dlg.GetStartPosition();

		while (pos != NULL)
		{
			CString fileName = dlg.GetNextPathName(pos);

			OpenDocumentFile(fileName);
		}
	}
}

CDocument* RingingMasterApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	//extract the path
	char szDrive[200];
	char szDir[200];
	char szFileName[200];
	char szExt[200];

	_splitpath(lpszFileName, szDrive, szDir, szFileName, szExt);

	//are we an image type?
	if (GetImageType(szExt) != CXIMAGE_FORMAT_UNKNOWN)
	{
		return openDocument(vt_image, lpszFileName);
	}


	return CWinApp::OpenDocumentFile(lpszFileName);
}


void RingingMasterApp::ShowTipOfTheDay(void)
{
	// CG: This function added by 'Tip of the Day' component.

	TipDlg dlg(mainFrame());
	dlg.startAsMenu();
}


void RingingMasterApp::setupTitle()
{
	//change the app name, so that the message boxes do noyt show the version number
	free((void*)m_pszAppName);

	CString title =  "Ringing Master ";

	mainFrame()->verifyKey();

	CString user = GetProfileString("MainFrame","User Name", "");
	if (user.IsEmpty())
		title += "(Unregistered) ";
	else
	{
		title += "(Registered to ";
		title += user;
		title += ") ";
	}

#ifdef _DEBUG
	title += "- DEBUG";
#endif

	m_pszAppName=_tcsdup(title);
	mainFrame()->UpdateCaption(title);

}


void RingingMasterApp::OnFileSaveall()
{
	SaveAllModified();
}

void RingingMasterApp::OnMapApplyMap(UINT /*wParam*/, LONG lParam)
{
	int mapId = (int)lParam;
	if (mapManager())
	{
		mapManager()->mapLoadedFromThread(mapId);
	}
}

BOOL RingingMasterApp::OnIdle(LONG lCount)
{
	if (lCount == 0)
	{
		//this copes with a document change
		CDocument* pDoc = mainFrame()->getActiveDocument();
		ViewType viewType = getViewType();

		if ((pDoc != _pIdleDoc)||(viewType != _idleViewType))
		{
			CDocument* oldDoc = _pIdleDoc;
			_pIdleDoc = pDoc;
			ViewType oldViewType = _idleViewType;
			_idleViewType = viewType;

			mainFrame()->fireDocViewChanged(oldViewType, viewType, oldDoc, pDoc);
			mainFrame()->fireUpdate();//todo remove when happy that everything is relying on 
		}

		//general idle processing
		mainFrame()->fireIdle();
	}

	return CWinApp::OnIdle(lCount);
}

ViewType RingingMasterApp::getViewType()
{
	//get the active document
	CDocument * pDoc = NULL;
	if ((pDoc = mainFrame()->getActiveDocument()) != NULL)
	{
		if (dynamic_cast<TouchDoc*>(pDoc))
		{
			TouchDoc* pTouchDoc = dynamic_cast<TouchDoc*>(pDoc);
			if (pTouchDoc)
			{
				if (pTouchDoc->getTouchHoldView()->GetActiveView() == (CView*)pTouchDoc->getTouchView())
					return vt_touch_main;
				else if (pTouchDoc->getTouchHoldView()->GetActiveView() == (CView*)pTouchDoc->getBlueLineView())
					return vt_touch_blue_line;
				else if (pTouchDoc->getTouchHoldView()->GetActiveView() == (CView*)pTouchDoc->getTouchSimulatorView())
					return vt_touch_simulator;
			}
		}
		else if (dynamic_cast<TowerDoc*> (pDoc)) return vt_tower;
		else if (dynamic_cast<GrabDoc*> (pDoc)) return vt_grab;
		else if (dynamic_cast<MapDoc*>  (pDoc)) return vt_map;
		else if (dynamic_cast<InternetDoc*>  (pDoc)) return vt_internet;
		else if (dynamic_cast<ImageDoc*>  (pDoc)) return vt_image;
		else if (dynamic_cast<CribDoc*>  (pDoc)) return vt_crib;
		else if (dynamic_cast<StrikeDoc*>  (pDoc)) return vt_strike;
		else ASSERT(FALSE);
	}

	return vt_none;
}

CString RingingMasterApp::getExtention(ViewType viewType)
{
	switch (viewType)
	{
	case vt_touch_main:
	case vt_touch_blue_line:
	case vt_touch_simulator:
						return ".tch";
	case vt_tower:		return ".tow";
	case vt_strike:		return ".strk";
	case vt_grab:		return ".grb";
	case vt_map:		return ".map";
	case vt_internet:   return ".htm";
	case vt_image:		return ".bmp";
	case vt_crib:		return ".crb";

	case vt_none:
	default:
		return "";
	}
}



InternetDoc* RingingMasterApp::launchBrowser(CString url)
{

	BOOL launchStyle = app()->GetProfileInt("Internet",
									"Launch Type", 0);

	//internal launch
	if (launchStyle == 0)
	{
		CDocument* pDoc = openDocument(vt_internet);

		InternetDoc* internetDoc = dynamic_cast<InternetDoc*>(pDoc);


		if (internetDoc)
		{
			internetDoc->setInitialData(url);
			return internetDoc;
		}
		else if (pDoc)
		{
			pDoc->OnCloseDocument();
			delete pDoc;
		}
	}


	//external launch

	CString path = "http\\shell\\open\\command";
	HKEY key;
	if( (RegOpenKeyEx(HKEY_CLASSES_ROOT, path, NULL, KEY_QUERY_VALUE, &key))==ERROR_SUCCESS)
	{
		DWORD type = REG_SZ;
		BYTE value[1000];
		DWORD length = sizeof(value);

		if(RegQueryValueEx(key,NULL,NULL,&type,value,&length)==ERROR_SUCCESS)
		{
			CString iepath(value);
			iepath.MakeLower();
			iepath.Replace("\"", ""); //remove all quotes
			int pos = iepath.Find(".exe",0);
			if (pos != -1)
			{
				iepath = iepath.Left(pos+4);
				//This spawnl call executes an .exe file and runs passes in the
				// command linew aguements. by convention, the name of the app is
				// passed in as the first item.
				_spawnl( _P_NOWAIT, iepath , "iexplore.exe", url, NULL );
				RegCloseKey(key);
				return NULL;
			}
		}
		RegCloseKey(key);
	}

	// Shell execute version - reused browsers,
	// but if cant get the path of IE due to
	// reg access problems this is second best.
	SHELLEXECUTEINFO info;
	memset(&info, 0, sizeof(info));
	info.cbSize = sizeof(info);
	info.hwnd = m_pMainWnd->m_hWnd;
	info.lpFile = url;
	info.nShow = SW_SHOW;
	::ShellExecuteEx(&info);

	return NULL;
}


BOOL RingingMasterApp::CreateAllDirectories(char*   pszDir )
{
    char*   pszLastSlash;
    char    cTmp;

    if( _access( pszDir, 0 ) != -1 )
    {
        // it already exists
        return TRUE;
    }

    pszLastSlash = strrchr( pszDir, '\\' );
    if ( pszLastSlash )
    {
        cTmp = *pszLastSlash;
        *pszLastSlash = '\0';

        // try again with one less dir

        CreateAllDirectories( pszDir );

        *pszLastSlash = cTmp;
    }


    if ( _mkdir( pszDir ) == -1 )
    {
        return FALSE;
    }

    return TRUE;
}

void RingingMasterApp::loadFavorites()
{

	// Initialize favorites:
#ifdef _DEBUG
	if (false)
#endif
		m_Favorites.Load ();
}

int RingingMasterApp::GetImageIndexFromType(int nDocType)
{
	int nCnt = 0;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		{
			if (imageFormats[i].nID == nDocType) return nCnt;
			nCnt++;
		}
	}
	return -1;
}

int RingingMasterApp::GetImageTypeFromIndex(int nIndex)
{
	int nCnt = 0;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		{
			if (nCnt == nIndex)
//              return i; // PJO - Buglet ?
                return imageFormats[i].nID;
			nCnt++;
		}
	}
	ASSERT(FALSE);
	return -1;
}
//


CString RingingMasterApp::GetImageFileTypes()
{
	CString str, temp;
	for (int i=0;i<sizeof(imageFormats)/sizeof(ImageFormats);i++)
	{
		temp.Format("%s(%s)|%s|",	imageFormats[i].description,
									imageFormats[i].ext,
									imageFormats[i].ext);
		str += temp;
	}
	return str;
}

ENUM_CXIMAGE_FORMATS RingingMasterApp::GetImageType(CString extention)
{
	ENUM_CXIMAGE_FORMATS format = CXIMAGE_FORMAT_UNKNOWN;

	CString str, temp;
	extention.MakeLower();
	for (int i=0;i<sizeof(imageFormats)/sizeof(ImageFormats);i++)
	{
		CString thisExt = imageFormats[i].ext;
		thisExt.MakeLower();

		if (thisExt.Find(extention) != -1)
		{
			format = (ENUM_CXIMAGE_FORMATS)GetImageTypeFromIndex(i);
			break;
		}
	}

	return format;
}

ENUM_CXIMAGE_FORMATS RingingMasterApp::GetImageTypeFromPath(CString path)
{
	ENUM_CXIMAGE_FORMATS format = CXIMAGE_FORMAT_UNKNOWN;

	CString str, temp;
	path.MakeLower();
	for (int i=0;i<sizeof(imageFormats)/sizeof(ImageFormats);i++)
	{
		CString thisExt = imageFormats[i].ext;
		thisExt.MakeLower();
		thisExt.Replace("*", "");

		if (path.Find(thisExt) != -1)
		{
			format = (ENUM_CXIMAGE_FORMATS)GetImageTypeFromIndex(i);
			break;
		}
	}

	return format;
}

bool RingingMasterApp::isInited()
{
	return _inited;
}


void RingingMasterApp::OnWindowResetlayout()
{
	if (_resetLayoutOnShutingDown)
		_resetLayoutOnShutingDown = false;
	else if (AfxMessageBox("This action will reset the screen layout next time you start the program. Do you wish to continue?",
		MB_ICONHAND|MB_YESNO) == IDYES)
	{
		_resetLayoutOnShutingDown = true;		
	}
}

void RingingMasterApp::OnUpdateWindowResetlayout(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_resetLayoutOnShutingDown);
}