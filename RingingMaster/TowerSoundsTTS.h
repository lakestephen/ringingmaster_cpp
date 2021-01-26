#if !defined(AFX_TOWERSOUNDSTTS_H__32932465_AD81_48B0_8672_CB552F14229A__INCLUDED_)
#define AFX_TOWERSOUNDSTTS_H__32932465_AD81_48B0_8672_CB552F14229A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TowerSoundsTTS.h : header file
//		  

#include "PropList.h"

class Tower;



/////////////////////////////////////////////////////////////////////////////
// TowerSoundsTTS dialog

class TowerSoundsTTS : public CBCGPPropertyPage
{
// Construction
public:
	void update();

	TowerSoundsTTS(Tower& internalTower);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TowerSoundsTTS)
	enum { IDD = IDD_TOWER_SOUNDS_TTS };
	PropList	_speechList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsTTS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Tower& _internalTower;

	// Generated message map functions
	//{{AFX_MSG(TowerSoundsTTS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	afx_msg LRESULT OnEditItem(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOWERSOUNDSTTS_H__32932465_AD81_48B0_8672_CB552F14229A__INCLUDED_)
