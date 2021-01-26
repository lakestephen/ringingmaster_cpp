#if !defined(AFX_RFTowerDistanceDlg_H__2FBFECC9_ED0C_4273_A09F_11B87ED564B7__INCLUDED_)
#define AFX_RFTowerDistanceDlg_H__2FBFECC9_ED0C_4273_A09F_11B87ED564B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RFTowerDistanceDlg.h : header file
//
#include "RFTowerDistanceData.h"
/////////////////////////////////////////////////////////////////////////////
// RFTowerDistanceDlg dialog

class RFTowerDistanceDlg : public CDialog
{
// Construction
public:
	RFTowerDistanceDlg(RFTowerDistanceData* rFTowerDistanceFilterData, CWnd* pParent );   // standard constructor

// Dialog Data
	//{{AFX_DATA(RFTowerDistanceDlg)
	enum { IDD = IDD_FILTER_DISTANCE };
	CEdit	_gridLocCtrl;
	CComboBox	_towers;
	CSpinButtonCtrl	_distanceSpin;
	int		_distanceLogic;
	CString	_gridLoc;
	int		_locationType;
	CString	_distanceLabel;
	int		_distance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RFTowerDistanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	RFTowerDistanceData* _originalRFTowerDistanceData;

	// Generated message map functions
	//{{AFX_MSG(RFTowerDistanceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLocationType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFTowerDistanceDlg_H__2FBFECC9_ED0C_4273_A09F_11B87ED564B7__INCLUDED_)
