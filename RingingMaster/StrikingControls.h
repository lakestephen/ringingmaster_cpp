// StrikingControls.h: interface for the StrikingControls class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "NumericEdit.h"
#include "TowerManagerEventListener.h"
#include "SimulatorManagerEventListener.h"

//todo rename to SimulatorStrikingControls

class StrikingControls : public CBCGPDockingControlBar, TowerManagerEventListener, SimulatorManagerEventListener
{
public:
	StrikingControls();
	virtual ~StrikingControls();

	DECLARE_DYNAMIC(StrikingControls)

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StrikingControls)
	public:
	//}}AFX_VIRTUAL

	void setTower(Tower* tower, int bell);

protected:

	void towerManager_notifyUpdateTowerList();
	void simulatorManager_notifyDocumentChanged();
	void applyTowerChange();
	void update();

	//{{AFX_MSG(StrikingControls)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	Tower* _tower;
	int _bell;
	bool _updatingCtrls;

	CComboBox _bells;
	CButton _useStrikingControls;
	CButton _quiet;
	CButton _followPreviousBellsError;
	CButton _lockHandAndBack;
	CBCGPButton _applyAll;
	CBCGPButton _removeAll;
	CBCGPButton _reset;
	CBCGPButton _resetAll;

	CStatic _handstroke;
	NumericEdit _handLateness;
	NumericEdit _handQuality;
	CStatic _handearlylate;
	CStatic _handerratic;

	CStatic _backstroke;
	NumericEdit _backLateness;
	NumericEdit _backQuality;
	CStatic _backearlylate;
	CStatic _backerratic;

	DECLARE_MESSAGE_MAP()


	afx_msg void OnBnClickedApplyAll();
	afx_msg void OnBnClickedRemoveAll();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedResetAll();
	afx_msg void OnBnClickedQuiet();
	afx_msg void OnBnClickedPrevError();
	afx_msg void OnBnClickedUseStrikingErrors();
	afx_msg void OnBnClickedLockHandBack();
	afx_msg void OnCbnSelchangeBell();
	afx_msg LRESULT OnNumericEditChange(WPARAM,LPARAM);

	afx_msg void OnUpdateOn(CCmdUI* pCmdUI);

};
