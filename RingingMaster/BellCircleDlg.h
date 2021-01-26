#if !defined(AFX_BELLCIRCLEDLG_H__AFF98E33_9B6F_43CD_B301_23A6EED8F8F6__INCLUDED_)
#define AFX_BELLCIRCLEDLG_H__AFF98E33_9B6F_43CD_B301_23A6EED8F8F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BellCircleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BellCircleDlg dialog

class BellCircleDlg : public CDialog
{
// Construction
public:
	BellCircleDlg(CWnd* pParent, int numberOfBells);   // standard constructor

// Dialog Data
	//{{AFX_DATA(BellCircleDlg)
	enum { IDD = IDD_BELL_CIRCLE };
	CSpinButtonCtrl	_numberSpin;
	BOOL	_clockwise;
	BOOL	_userInCenter;
	int		_number;
	double	_meters;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BellCircleDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int _numberOfBells;

	// Generated message map functions
	//{{AFX_MSG(BellCircleDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BELLCIRCLEDLG_H__AFF98E33_9B6F_43CD_B301_23A6EED8F8F6__INCLUDED_)
