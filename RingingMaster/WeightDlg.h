#if !defined(AFX_WEIGHTDLG_H__2DCF6C91_7990_11D7_B6AE_000255162CF0__INCLUDED_)
#define AFX_WEIGHTDLG_H__2DCF6C91_7990_11D7_B6AE_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WeightDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WeightDlg dialog

class WeightDlg : public CDialog
{
// Construction
public:
	double getWeightInKG();
	WeightDlg(double weightKG, bool useApprox, BOOL approx = TRUE, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(WeightDlg)
	enum { IDD = IDD_WEIGHT };
	CButton	_approxCtrl;
	CSpinButtonCtrl	_spinQR;
	CSpinButtonCtrl	_spinLB;
	CSpinButtonCtrl	_spinCWT;
	int		_qr;
	double	_kg;
	int		_cwt;
	int		_lb;
	BOOL	_approx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WeightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	double _originalWeightKG;
	bool _useApprox;
	// Generated message map functions
	//{{AFX_MSG(WeightDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEIGHTDLG_H__2DCF6C91_7990_11D7_B6AE_000255162CF0__INCLUDED_)
