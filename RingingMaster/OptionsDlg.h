#if !defined(AFX_OPTIONSDLG_H__36D94431_F58E_4817_81DC_F325EB52E8D2__INCLUDED_)
#define AFX_OPTIONSDLG_H__36D94431_F58E_4817_81DC_F325EB52E8D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

#include "PropertySheetBase.h"

class OptionsTouchStyle;                 
class OptionsMethod;                
class OptionsCrib;                
class OptionsPrint;                 
class OptionsLibrary;               
class OptionsUser;                  
class OptionsTower;                  
class OptionsGrabs;                 
class OptionsMap;                   
class OptionsTowerSounds;                 
class OptionsCalledChanges;         
class OptionsMiscellaneous;               
class OptionsScales;   
class OptionsInternet;             
class OptionsMultiBellInterface;    
class OptionsSerialControl;    
class OptionsMicrophone;  
class OptionsKeyboard;  
class OptionsTTS;

enum OptionsScreensType
{
	os_none,
	os_TouchStyle,                 
	os_Method,                
	os_Crib,                
	os_Print,                 
	os_Library,               
	os_User,                  
	os_Tower,                  
	os_Grabs,                 
	os_Map,                   
	os_TowerSounds,                 
	os_CalledChanges,         
	os_Microphone,
	os_Keyboard,
	os_MultiBellInterface,
	os_SerialCommands,
	os_TTS,
	os_Miscellaneous,               
	os_Scales,  
	os_Internet,  
};

/////////////////////////////////////////////////////////////////////////////
// OptionsDlg dialog

class OptionsDisplayFilter
{
public:
	OptionsDisplayFilter(bool state = true) :
	_displayScreenType(os_TouchStyle),
	_method(state),
	_crib(state),
	_library(state),
 	_micl(state),
 	_touchStyle(state),
 	_towerSounds(state),
 	_print(state),
	_calledChanges(state),
	_map(state),
	_tower(state),
	_users(state),
	_grabs(state),
	_scales(state),
	_internet(state),
	_multiBellInterface(state),
	_serialControl(state),
	_microphone(state),
	_keyboard(state),
	_TTS(state)
	{}
	bool _method;
	bool _crib;
	bool _library;
 	bool _micl;
 	bool _touchStyle;
 	bool _towerSounds;
 	bool _print;
	bool _calledChanges;
	bool _map;
	bool _tower;
	bool _grabs;
	bool _users;
	bool _scales;
	bool _internet;
	bool _multiBellInterface;
	bool _serialControl;
	bool _microphone;
	bool _keyboard;
	bool _TTS;

	OptionsScreensType _displayScreenType;
};

class OptionsDlg : public PropertySheetBase
{


// Construction
public:
	OptionsDlg(CWnd* pParent , OptionsDisplayFilter optionsDisplayFilter);   // standard constructor
	~OptionsDlg();

	// Dialog Data
	//{{AFX_DATA(OptionsDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:

	OptionsTouchStyle* _touchStyle;
	OptionsMethod* _method;
	OptionsCrib* _crib;
 	OptionsPrint* _print;
	OptionsLibrary* _library;

	OptionsUser* _users;
	OptionsTower* _tower;
	OptionsGrabs* _grabs;
	OptionsScales* _scales;
	OptionsMap* _map;


	OptionsMicrophone* _microphone;
	OptionsSerialControl* _serialControl;
	OptionsKeyboard* _keyboard;
	OptionsMultiBellInterface* _multiBellInterface;
	OptionsTTS* _TTS;
	OptionsTowerSounds* _towerSounds;
	OptionsCalledChanges* _calledChanges;

 	OptionsMiscellaneous* _miscellaneous;
	OptionsInternet* _internet;


	// Generated message map functions
	//{{AFX_MSG(OptionsDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg LRESULT OnSelectUserPage(WPARAM wParam, LPARAM lParam);

	

	DECLARE_MESSAGE_MAP()

	friend class OptionsTower;
	friend class OptionsGrabs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__36D94431_F58E_4817_81DC_F325EB52E8D2__INCLUDED_)

