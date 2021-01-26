#pragma once

class Tower;

#include "NumericEdit.h"

// TowerSoundsStrikingControls dialog

class TowerSoundsStrikingControls : public CBCGPPropertyPage
{
	DECLARE_DYNAMIC(TowerSoundsStrikingControls)

public:
	TowerSoundsStrikingControls(Tower& internalTower);   // standard constructor
	virtual ~TowerSoundsStrikingControls();



// Dialog Data
	enum { IDD = IDD_TOWER_SOUNDS_STRIKE_CONTROLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	void update();
	

	Tower& _internalTower;

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
public:
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
