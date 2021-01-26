#pragma once


// TowerSoundsCommandInputs dialog

#include "TowerSoundsInputsBase.h"

class TowerSoundsCommandInputs : public TowerSoundsInputsBase
{

// Construction
public:
	TowerSoundsCommandInputs(Tower& internalTower);   // standard constructor
	~TowerSoundsCommandInputs();


// Dialog Data
	//{{AFX_DATA(TowerSoundsCommandInputs)
	enum { IDD = IDD_TOWER_SOUNDS_COMMAND_INPUTS};
	//}}AFX_DATA



// Implementation
protected:

	SimulatorAction& getActionItem(int index) ;
	CString getActionName(int index) ;
	void addActionItem(int index);

	int getActionCount() ;

	// Generated message map functions
	//{{AFX_MSG(TowerSoundsCommandInputs)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
};

