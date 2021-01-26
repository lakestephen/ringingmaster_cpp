#pragma once


#include "DoubleSelectItem.h"

/////////////////////////////////////////////////////////////////////////////
// WeightSelectDlg dialog

class WeightSelectDlg : public CDialog
{
// Construction
public:
	void update();
	WeightSelectDlg(CWnd* pParent, DoubleSelectItem& doubleSelectItem, CString name = "Filter");   // standard constructor

// Dialog Data
	//{{AFX_DATA(WeightSelectDlg)
	enum { IDD = IDD_FILTER_WEIGHT };
	CButton	_weightCtrl2;
	CBCGPButton	_weight2;
	CBCGPButton	_weight1;
	int		_logic;
	//}}AFX_DATA

	CString _name;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WeightSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	DoubleSelectItem& _originalDoubleSelectItem;
	DoubleSelectItem _doubleSelectItem;

	// Generated message map functions
	//{{AFX_MSG(WeightSelectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnWeightLogic();
	afx_msg void OnWeight1();
	afx_msg void OnWeight2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
