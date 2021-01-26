#include "afxcmn.h"
#if !defined(AFX_TOWERSOUNDSOPTIONS_H__E3E1FDCA_DF16_4108_B897_2D1644CA84AA__INCLUDED_)
#define AFX_TOWERSOUNDSOPTIONS_H__E3E1FDCA_DF16_4108_B897_2D1644CA84AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TowerSoundsOptions.h : header file
//
#include "NumericEdit.h"

class Tower;

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsOptions dialog

class TowerSoundsOptions : public CBCGPPropertyPage
{
// Construction
public:

	TowerSoundsOptions( Tower& internalTower);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TowerSoundsOptions)
	enum { IDD = IDD_TOWER_SOUNDS_OPTIONS };
	CButton	_extraCoverForOddBells;
	CSpinButtonCtrl	_coverCountSpin;
	CSpinButtonCtrl	_repeatSpinStart;
	CSpinButtonCtrl	_repeatSpinEnd;
	CComboBox	_smallerTrebleNumCtrl;
	int		_smaller;
	int		_repeatStart;
	int _coverCount;
	int		_repeatEnd;
	//}}AFX_DATA

	NumericEdit _handLeadGap;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Tower& _internalTower;

	// Generated message map functions
	//{{AFX_MSG(TowerSoundsOptions)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateTowerCover(CCmdUI* pCmdUI);
	afx_msg void OnTowerCover();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOWERSOUNDSOPTIONS_H__E3E1FDCA_DF16_4108_B897_2D1644CA84AA__INCLUDED_)
