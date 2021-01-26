
#pragma once


#include "PropList.h"
#include "afxwin.h"
#include "afxcmn.h"

class Tower;



/////////////////////////////////////////////////////////////////////////////
// TowerSoundsSimulatorDisplay dialog

class TowerSoundsSimulatorDisplay : public CBCGPPropertyPage
{
// Construction
public:
	void update();

	TowerSoundsSimulatorDisplay(Tower& internalTower);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TowerSoundsSimulatorDisplay)
	enum { IDD = IDD_TOWER_SOUNDS_SIMULATOR_DISPLAY };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsSimulatorDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Tower& _internalTower;

	// Generated message map functions
	//{{AFX_MSG(TowerSoundsSimulatorDisplay)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
public:
	CBCGPButton _directoryButt;
	CBCGPButton _bellNoFontButt;
	CBCGPButton _previewButt;
	afx_msg void OnBnClickedBellNoFont();
	afx_msg void OnBnClickedDirectory();
	CString _baseDir;
	afx_msg void OnBnClickedPreview();
	virtual void OnOK();
	BOOL _clockwise;
	int _firstSimulatedBell;
	CSpinButtonCtrl _firstSimulatedBellSpin;
	LRESULT OnQuerySiblings( WPARAM wParam, LPARAM lParam );
	virtual BOOL OnSetActive();
};

