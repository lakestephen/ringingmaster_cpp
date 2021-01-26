// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsDlg.h"	

#include "OptionsMethod.h"
#include "OptionsCrib.h"
#include "OptionsLibrary.h"
#include "OptionsTouchStyle.h"
#include "OptionsMiscellaneous.h"	
#include "OptionsTowerSounds.h"	
#include "OptionsPrint.h"	
#include "OptionsCalledChanges.h"	
#include "OptionsMap.h"	
#include "OptionsTower.h"	
#include "OptionsUser.h"	 
#include "OptionsGrabs.h"
#include "OptionsScales.h"
#include "OptionsInternet.h"
#include "OptionsMultiBellInterface.h"
#include "OptionsSerialControl.h"
#include "OptionsMicrophone.h"
#include "OptionsKeyboard.h"
#include "OptionsTTS.h"
#include "WinPos.h"

#include "OptionsManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsDlg dialog


OptionsDlg::OptionsDlg(CWnd* pParent , OptionsDisplayFilter optionsDisplayFilter)
	: PropertySheetBase("Ringing Master Options", pParent, 154)
{
	//set up the dialog stuff	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	_touchStyle = new 	OptionsTouchStyle();                             
	_method = new 	OptionsMethod();                            
	_crib = new 	OptionsCrib();                            
	_print = new 	OptionsPrint();                             
	_library = new 	OptionsLibrary();                           
      
	_users = new 	OptionsUser();                              
	_tower = new 	OptionsTower();                              
	_grabs = new 	OptionsGrabs();                             
	_scales = new 	OptionsScales();        
	_map = new 	OptionsMap();        
      
	_multiBellInterface = new 	OptionsMultiBellInterface();                             
	_serialControl = new 	OptionsSerialControl();                             
	_microphone = new 	OptionsMicrophone();                             
	_keyboard = new 	OptionsKeyboard();                             
	_TTS  = new 	OptionsTTS();                             
	_towerSounds = new 	OptionsTowerSounds();                             
	_calledChanges = new 	OptionsCalledChanges();                     
      
	_miscellaneous = new 	OptionsMiscellaneous();                           
	_internet= new 	OptionsInternet();                           



	if ((optionsDisplayFilter._touchStyle)||
		(optionsDisplayFilter._method)||
		(optionsDisplayFilter._print)||
		(optionsDisplayFilter._library)||
		(optionsDisplayFilter._library))
	{
		CBCGPPropSheetCategory* pTouchProving = AddTreeCategory (_T("Touch Proving"), 0, 1);
	
		if (optionsDisplayFilter._touchStyle)	AddPageToTree(pTouchProving, _touchStyle,	-1, 2);
		if (optionsDisplayFilter._method)		AddPageToTree(pTouchProving, _method,		-1, 2);
		if (optionsDisplayFilter._crib)			AddPageToTree(pTouchProving, _crib,			-1, 2);
		if (optionsDisplayFilter._print)		AddPageToTree(pTouchProving, _print,		-1, 2);
		if (optionsDisplayFilter._library)		AddPageToTree(pTouchProving, _library,		-1, 2);
	}

	if ((optionsDisplayFilter._towerSounds)||
		(optionsDisplayFilter._calledChanges)||
		(optionsDisplayFilter._multiBellInterface)||
		(optionsDisplayFilter._TTS)||
		(optionsDisplayFilter._microphone)||
		(optionsDisplayFilter._serialControl)||
		(optionsDisplayFilter._keyboard))
	{
		CBCGPPropSheetCategory* pSimulation = AddTreeCategory (_T("Simulation"), 0, 1);
		
		if (optionsDisplayFilter._towerSounds)			AddPageToTree(pSimulation, _towerSounds,		-1, 2);	
		if (optionsDisplayFilter._keyboard)				AddPageToTree(pSimulation, _keyboard,			-1, 2);	
		if (optionsDisplayFilter._multiBellInterface)	AddPageToTree(pSimulation, _multiBellInterface,	-1, 2);	
		if (optionsDisplayFilter._serialControl)		AddPageToTree(pSimulation, _serialControl,		-1, 2);	
		if (optionsDisplayFilter._microphone)			AddPageToTree(pSimulation, _microphone,			-1, 2);
		if (optionsDisplayFilter._TTS)					AddPageToTree(pSimulation, _TTS,				-1, 2);	
		if (optionsDisplayFilter._calledChanges)		AddPageToTree(pSimulation, _calledChanges,		-1, 2);
	}

	if ((optionsDisplayFilter._users)||
		(optionsDisplayFilter._tower)||
		(optionsDisplayFilter._grabs)||
		(optionsDisplayFilter._scales)||
		(optionsDisplayFilter._map))
	{
		CBCGPPropSheetCategory* pRecordKeeping = AddTreeCategory (_T("Record Keeping"), 0, 1);

		if (optionsDisplayFilter._users)	AddPageToTree(pRecordKeeping, _users,	-1, 2);
		if (optionsDisplayFilter._tower)	AddPageToTree(pRecordKeeping, _tower,	-1, 2);
		if (optionsDisplayFilter._grabs)	AddPageToTree(pRecordKeeping, _grabs,	-1, 2);
		if (optionsDisplayFilter._scales)	AddPageToTree(pRecordKeeping, _scales,	-1, 2);
		if (optionsDisplayFilter._map)		AddPageToTree(pRecordKeeping, _map,		-1, 2);
	}  

	if ((optionsDisplayFilter._micl)||
		(optionsDisplayFilter._internet))
	{
		CBCGPPropSheetCategory* pGeneral = AddTreeCategory (_T("General"), 0, 1);

		if (optionsDisplayFilter._micl)		AddPageToTree(pGeneral, _miscellaneous,	-1, 2);
		if (optionsDisplayFilter._internet) AddPageToTree(pGeneral, _internet,		-1, 2);
	}

	switch(optionsDisplayFilter._displayScreenType)
	{
	case os_TouchStyle:		SetActivePage(_touchStyle);		break;                 
	case os_Method:			SetActivePage(_method);			break;                
	case os_Crib:			SetActivePage(_crib);			break;                
	case os_Print:			SetActivePage(_print);			break;                 
	case os_Library:		SetActivePage(_library);			break;               
	case os_User:			SetActivePage(_users);			break;                  
	case os_Tower:			SetActivePage(_tower);			break;                  
	case os_Grabs:			SetActivePage(_grabs);			break;                 
	case os_Map:			SetActivePage(_map);				break;                   
	case os_TowerSounds:	SetActivePage(_towerSounds);		break;                 
	case os_Microphone:		SetActivePage(_microphone);		break;                 
	case os_Keyboard:		SetActivePage(_keyboard);		break;                 
	case os_MultiBellInterface:	SetActivePage(_multiBellInterface);break;                 
	case os_SerialCommands:	SetActivePage(_serialControl);break;                 
	case os_CalledChanges:	SetActivePage(_calledChanges);	break;         
	case os_Miscellaneous:	SetActivePage(_miscellaneous);	break;               
	case os_Scales:			SetActivePage(_scales);			break;  
	case os_Internet:		SetActivePage(_internet);		break;  
	case os_none:
	default:
		//do nothing break
		break;
	}

}



OptionsDlg::~OptionsDlg()
{
	delete _touchStyle;			   
	delete _method;
	delete _crib;
 	delete _print;
	delete _library;

	delete _users;
	delete _tower;
	delete _grabs;
	delete _scales;
	delete _map;

	delete _microphone;
	delete _serialControl;
	delete _keyboard;
	delete _multiBellInterface;
	delete _towerSounds;
	delete _calledChanges;
	delete _TTS;

 	delete _miscellaneous;
 	delete _internet;

}


BEGIN_MESSAGE_MAP(OptionsDlg, PropertySheetBase)
	//{{AFX_MSG_MAP(OptionsDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_MESSAGE(OPTIONS_SELECT_USER_PAGE, OnSelectUserPage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsDlg message handlers


void OptionsDlg::OnCancel() 
{
	OnOK();
}

void OptionsDlg::OnOK() 
{
	PressButton(PSBTN_OK);
	optionsManager()->fireOptionsDlgClosed();
}	  

LRESULT OptionsDlg::OnSelectUserPage(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	SetActivePage(_users);	

	return 0;
}

BOOL OptionsDlg::OnInitDialog() 
{
	BOOL ret = PropertySheetBase::OnInitDialog();

	WinPos::Load(this, "OptionsDlg", false); 

	_ok.ShowWindow(SW_HIDE);
	_cancle.SetWindowText("Close");
	_cancle.SetTooltip("Close");

	return ret;
}


void OptionsDlg::OnDestroy() 
{
	WinPos::Save(this, "OptionsDlg");

	PropertySheetBase::OnDestroy();	
}
