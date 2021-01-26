#pragma once


// TowerSoundsCallInputs dialog

#include "TowerSoundsInputsBase.h"

class TowerSoundsCallInputs : public TowerSoundsInputsBase
{

// Construction
public:
	TowerSoundsCallInputs(Tower& internalTower);   // standard constructor
	~TowerSoundsCallInputs();


// Dialog Data
	//{{AFX_DATA(TowerSoundsCallInputs)
	enum { IDD = IDD_TOWER_SOUNDS_CALL_INPUTS};
	//}}AFX_DATA



// Implementation
protected:

	SimulatorAction& getActionItem(int index) ;
	CString getActionName(int index) ;
	void addActionItem(int index);

	int getActionCount() ;

	// Generated message map functions
	//{{AFX_MSG(TowerSoundsCallInputs)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
};

