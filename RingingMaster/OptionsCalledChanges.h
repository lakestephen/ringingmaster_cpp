#if !defined(AFX_OPTIONSCALLEDCHANGES_H__61204B34_BD09_4894_9E33_9EA8B9A8238B__INCLUDED_)
#define AFX_OPTIONSCALLEDCHANGES_H__61204B34_BD09_4894_9E33_9EA8B9A8238B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsCalledChanges.h : header file
//




/////////////////////////////////////////////////////////////////////////////
// OptionsCalledChanges dialog

class OptionsCalledChanges : public CBCGPPropertyPage
{
// Construction
public:
	OptionsCalledChanges();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsCalledChanges)
	enum { IDD = IDD_OPTIONS_CALLED_CHANGES };
	CButton	_affect3Ctrl;
	CButton	_affect2Ctrl;
	CSpinButtonCtrl	_pullPerChangeSpin;
	BOOL	_affect2;
	BOOL	_affect3;
	int		_type;
	BOOL	_inclTenor;
	int		_pullPerChange;
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


const static CString CallChangeSection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsCalledChanges)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsCalledChanges)
	virtual BOOL OnInitDialog();
	afx_msg void OnAffect2();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSCALLEDCHANGES_H__61204B34_BD09_4894_9E33_9EA8B9A8238B__INCLUDED_)
