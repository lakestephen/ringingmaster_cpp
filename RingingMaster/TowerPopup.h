// TowerPopup.h: interface for the TowerPopup class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


#include "AutoPopup.h"
#include "TowerPopupCombo.h"
#include "TowerManagerEventListener.h"


class TowerPopup : public AutoPopup	, public TowerManagerEventListener

{
// Construction
public:
	TowerPopup();

// Attributes
public:
	virtual BOOL Create(CWnd* pParentWnd);
    void Set(CPoint point);

 // class operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerPopup)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void towerManager_notifyUpdateTowerList();
	virtual ~TowerPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(TowerPopup)
	afx_msg void OnSelchangeOptionsTowerCombo();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


protected:
	TowerpopupCombo _comboTower;

};

