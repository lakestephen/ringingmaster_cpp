#if !defined(AFX_RSTowerDistanceDlg_H__3E1ED3E1_EAE8_46CB_BCB3_F1E37A8256D9__INCLUDED_)
#define AFX_RSTowerDistanceDlg_H__3E1ED3E1_EAE8_46CB_BCB3_F1E37A8256D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RSTowerDistanceDlg.h : header file
//
#include "RFTowerDistanceData.h"
/////////////////////////////////////////////////////////////////////////////
// RSTowerDistanceDlg dialog

class RSTowerDistanceDlg : public CDialog
{
// Construction
public:
	RSTowerDistanceDlg(RFTowerDistanceData* rFTowerDistanceData, BOOL& ascending, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RSTowerDistanceDlg)
	enum { IDD = IDD_SORT_DISTANCE };
	CEdit	_gridLocCtrl;
	CComboBox	_towers;
	CString	_gridLoc;
	int		_locationType;
	int		_ascending;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RSTowerDistanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	RFTowerDistanceData* _originalRFTowerDistanceData;
	BOOL& _originalAscending;

	// Generated message map functions
	//{{AFX_MSG(RSTowerDistanceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLocationType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSTowerDistanceDlg_H__3E1ED3E1_EAE8_46CB_BCB3_F1E37A8256D9__INCLUDED_)
