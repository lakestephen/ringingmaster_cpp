#pragma once


// TowerSoundsBellInputs dialog

#include "TowerSoundsInputsBase.h"

class TowerSoundsBellInputs : public TowerSoundsInputsBase
{

// Construction
public:
	TowerSoundsBellInputs(Tower& internalTower);   // standard constructor
	~TowerSoundsBellInputs();


	enum { IDD = IDD_TOWER_SOUNDS_BELL_INPUTS};
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsBellInputs)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	SimulatorAction& getActionItem(int index) ;
	int getActionCount() ;
	CString getActionName(int index) ;
	void addActionItem(int index);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
