#if !defined(AFX_RSTOWERDIRECTIONDLG_H__D02DF740_5871_11D7_B696_000255162CF0__INCLUDED_)
#define AFX_RSTOWERDIRECTIONDLG_H__D02DF740_5871_11D7_B696_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RSTowerDirectionDlg.h : header file
//

#include "RFTowerDirectionData.h"

/////////////////////////////////////////////////////////////////////////////
// RSTowerDirectionDlg dialog

class RSTowerDirectionDlg : public CDialog

{
// Construction
public:
	RSTowerDirectionDlg(RFTowerDirectionData* rFTowerDirectionData, BOOL& ascending, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RSTowerDirectionDlg)
	enum { IDD = IDD_SORT_DISTANCE };
	CEdit	_gridLocCtrl;
	CComboBox	_towers;
	CString	_gridLoc;
	int		_locationType;
	int		_ascending;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RSTowerDirectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	RFTowerDirectionData* _originalRFTowerDirectionData;
	BOOL& _originalAscending;

	// Generated message map functions
	//{{AFX_MSG(RSTowerDirectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLocationType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSTOWERDIRECTIONDLG_H__D02DF740_5871_11D7_B696_000255162CF0__INCLUDED_)
