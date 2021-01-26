// StyleGeneral.h : header file
//
 
#pragma once
  
#include "AutoSelEdit.h"

class Style;
enum StyleLinesType;


class StyleGeneral : public CBCGPPropertyPage
{

// Construction
public:
	void DoShowLead();
	StyleGeneral(Style & style, StyleLinesType type);
	~StyleGeneral();
	
	Style & _style;

// Dialog Data
	//{{AFX_DATA(StyleGeneral)
	enum { IDD = IDD_STYLE_GENERAL };
	AutoSelEdit	_styleNameEdit;
	CEdit	_bellCtrl;
	CEdit	_leadsCtrl;
	CSpinButtonCtrl	_scLeadEdit;
	CBCGPColorButton  	_leadColor;
	CSpinButtonCtrl	_scBellEdit;
	CBCGPColorButton  	_bellColor;
	int		_bellEdit;
	int		_leadEdit;
	CString	_styleName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(StyleGeneral)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DoShowBell();
	StyleLinesType _type;

	// Generated message map functions
	//{{AFX_MSG(StyleGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowBell();
	afx_msg void OnShowLead();
	afx_msg void modify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

