#pragma once

#include "Tower.h"
#include "PropertySheetBase.h"



class TowerSoundsGeneral;
class TowerSoundsFiles;
class TowerSoundsFilesAdvanced;
class TowerSoundsOptions;
class TowerSoundsTTS;	
class TowerSoundsBellInputs;
class TowerSoundsCommandInputs;
class TowerSoundsMethodInputs;
class TowerSoundsCallInputs;
class TowerSoundsSimulatorDisplay;
class TowerSoundsStrikingControls;

class TowerSoundsDlg : public PropertySheetBase
{
	DECLARE_DYNAMIC(TowerSoundsDlg)

// Construction
public:
	TowerSoundsDlg(CWnd* pParent, Tower& tower, bool editing);

// Attributes
public:

	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TowerSoundsDlg();

// Generated message map functions
protected:
	TowerSoundsGeneral* _towerSoundsGeneral;
	TowerSoundsFiles* _towerSoundsFiles; 
	TowerSoundsFilesAdvanced* _towerSoundsFilesAdvanced; 
	TowerSoundsOptions* _towerSoundsOptions; 
	TowerSoundsTTS* _towerSoundsTTS; 	
	TowerSoundsBellInputs* _towerSoundsBellInputs;
	TowerSoundsCommandInputs* _towerSoundsCommandInputs;
	TowerSoundsMethodInputs* _towerSoundsMethodInputs;
	TowerSoundsCallInputs* _towerSoundsCallInputs;
	TowerSoundsSimulatorDisplay* _towerSoundsSimulatorDisplay;
	TowerSoundsStrikingControls* _towerSoundsStrikingControls;


	Tower& _origTower;
	Tower _internalTower;
	bool _editing;

	afx_msg void OnTest();
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);


	CBCGPButton	_testBtn;
	CBCGPButton	_stopBtn;

	//{{AFX_MSG(TowerSoundsDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	bool apply();

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void showTowerSoundsFilesPage();
};