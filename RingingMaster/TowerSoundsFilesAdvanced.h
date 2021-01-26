#if !defined(AFX_TOWERSOUNDSFILESADVANCED_H__01F05ABA_35D0_4E0F_9AE6_4A2DD1E0FE92__INCLUDED_)
#define AFX_TOWERSOUNDSFILESADVANCED_H__01F05ABA_35D0_4E0F_9AE6_4A2DD1E0FE92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TowerSoundsFilesAdvanced.h : header file
//


#include "NumericEdit.h"


class Tower;



/////////////////////////////////////////////////////////////////////////////
// TowerSoundsFilesAdvanced dialog

class TowerSoundsFilesAdvanced : public CBCGPPropertyPage

{
// Construction
public:
	TowerSoundsFilesAdvanced( Tower& internalTower);   // standard constructor

	void update();

// Dialog Data
		//{{AFX_DATA(TowerSoundsFilesAdvanced)
	enum { IDD = IDD_TOWER_SOUNDS_FILES_ADVANCED };
	CBCGPButton	_resetVol;
	CBCGPButton	_resetFreq;
	CButton	_enableCtrl;
	CBCGPButton	_editPos;
	//}}AFX_DATA

	CStatic	_pealSpeed;
	CSliderCtrl	_speedCtrl;	//to2do replace this with a NumericEdit control - with the time instead of the a number
	int		_speed;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsFilesAdvanced)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Tower& _internalTower;
	NumericEdit _freq;
	NumericEdit _vol;


	// Generated message map functions
	//{{AFX_MSG(TowerSoundsFilesAdvanced)
	afx_msg void OnEditPos();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEditPos(CCmdUI* pCmdUI);
	afx_msg void On3dEnable();
	afx_msg void OnResetVol();
	afx_msg void OnResetFreq();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnKillActive();

	virtual void OnOK();

	LRESULT OnQuerySiblings( WPARAM wParam, LPARAM lParam );

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOWERSOUNDSFILESADVANCED_H__01F05ABA_35D0_4E0F_9AE6_4A2DD1E0FE92__INCLUDED_)
