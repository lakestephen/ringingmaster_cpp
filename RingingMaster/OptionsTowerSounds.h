// OptionsTowerSounds.h : header file
//

#pragma once


#include "TowerManagerEventListener.h"
#include "CJFlatComboBox.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// OptionsTowerSounds dialog

class OptionsTowerSounds : public CBCGPPropertyPage, public TowerManagerEventListener
{
// Construction
public:
	void towerManager_notifyUpdateTowerList();

	OptionsTowerSounds();   // standard constructor
	~OptionsTowerSounds();
// Dialog Data
	//{{AFX_DATA(OptionsTowerSounds)
	enum { IDD = IDD_OPTIONS_TOWER_SOUNDS };
	CBCGPButton	_testBtn;
	CBCGPButton	_cloneBtn;
	CBCGPButton	_editBtn;
	CBCGPButton	_delBtn;
	CBCGPButton	_addBtn;
	CCJFlatComboBox	_comboTower;
	int		_tower;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsTowerSounds)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsTowerSounds)
	afx_msg void OnOptionsTowerSoundsAdd();
	afx_msg void OnOptionsTowerSoundsDel();
	afx_msg void OnOptionsTowerSoundsEdit();
	afx_msg void OnSelchangeOptionsTowerSoundsCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionsTowerSoundsClone();
	afx_msg void OnEnable();
	afx_msg void OnUpdateGeneral(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeneralSelected(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEnable(CCmdUI* pCmdUI); 
	afx_msg void OnOptionsTowerTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CBCGPButton _resetTowers;
	afx_msg void OnBnClickedResetTowers();
};
