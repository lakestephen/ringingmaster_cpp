#if !defined(AFX_VARIANCEDLG_H__9B794964_6C79_11D5_B5FC_009027BB3286__INCLUDED_)
#define AFX_VARIANCEDLG_H__9B794964_6C79_11D5_B5FC_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VarianceDlg.h : header file
//
class Variance;

/////////////////////////////////////////////////////////////////////////////
// VarianceDlg dialog

class VarianceDlg : public CDialog
{
// Construction
public:
	void enablePartsWindow();
	VarianceDlg(Variance* variance);   // standard constructor

// Dialog Data
	//{{AFX_DATA(VarianceDlg)
	enum { IDD = IDD_VARIANCE };
	CEdit	_partsEdit;
	CComboBox	_omitInc;
	CString	_parts;
	int		_type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(VarianceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	Variance* _variance;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(VarianceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnSpecify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARIANCEDLG_H__9B794964_6C79_11D5_B5FC_009027BB3286__INCLUDED_)
