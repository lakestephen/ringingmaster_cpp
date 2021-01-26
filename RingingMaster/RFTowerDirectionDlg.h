#if !defined(AFX_RFTOWERDIRECTIONDLG_H__3E77E08A_7BA9_4A0B_A3E3_053922182085__INCLUDED_)
#define AFX_RFTOWERDIRECTIONDLG_H__3E77E08A_7BA9_4A0B_A3E3_053922182085__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RFTowerDirectionDlg.h : header file
//

#include "ListSelectDlg.h"
#include "ListSelectItem.h"
#include "RFTowerDirectionData.h"
/////////////////////////////////////////////////////////////////////////////
// RFTowerDirectionDlg dialog


class RFTowerDirectionDlg : public ListSelectDlg
{
// Construction
public:
	RFTowerDirectionDlg(CWnd* pParent, ListSelectItems& available, ListSelectItems& selected, RFTowerDirectionData& rFTowerDirectionData);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RFTowerDirectionDlg)
	enum { IDD = IDD_FILTER_DIRECTION };
	CEdit	_gridLocCtrl;
	CComboBox	_towers;   
	CString	_gridLoc;
	int		_locationType;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RFTowerDirectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	RFTowerDirectionData& _originalRFTowerDirectionData;

	// Generated message map functions
	//{{AFX_MSG(RFTowerDirectionDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLocationType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFTOWERDIRECTIONDLG_H__3E77E08A_7BA9_4A0B_A3E3_053922182085__INCLUDED_)
