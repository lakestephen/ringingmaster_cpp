#pragma once


// TowerSoundsMethodInputs dialog

#include "TowerSoundsInputsBase.h"

class TowerSoundsMethodInputs : public TowerSoundsInputsBase
{

// Construction
public:
	TowerSoundsMethodInputs(Tower& internalTower);   // standard constructor
	~TowerSoundsMethodInputs();


// Dialog Data
	//{{AFX_DATA(TowerSoundsMethodInputs)
	enum { IDD = IDD_TOWER_SOUNDS_METHOD_INPUTS};
	//}}AFX_DATA



// Implementation
protected:

	BOOL _microphoneNames;
	BOOL _keyboardShortcuts;

	SimulatorAction& getActionItem(int index) ;
	CString getActionName(int index) ;
	void addActionItem(int index);
	virtual int getPROFILE_BELL_CTRL_COUNT() {return PROFILE_BELL_CTRL_COUNT-2;}

	int getActionCount() ;

	// Generated message map functions
	//{{AFX_MSG(TowerSoundsMethodInputs)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	virtual void OnOK();
};

