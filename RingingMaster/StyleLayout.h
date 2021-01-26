// StyleLayout.h : header file
//

#pragma once

class Style;
enum StyleLinesType;


class StyleLayout : public CBCGPPropertyPage
{

// Construction
public:
	StyleLayout(Style &style, StyleLinesType type);
	~StyleLayout();
			  
	Style& _style;

// Dialog Data
	//{{AFX_DATA(StyleLayout)
	enum { IDD = IDD_STYLE_LAYOUT };
	CStatic	_staticLeads;
	CButton	_bLeads;
	CEdit	_editLeads;
	CSpinButtonCtrl	_spWidth;
	CSpinButtonCtrl	_spTop;
	CSpinButtonCtrl	_spLeft;
	CSpinButtonCtrl	_spLeads;
	CSpinButtonCtrl	_spHeight;
	int		_height;
	int		_leads;
	int		_left;
	int		_top;
	int		_width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(StyleLayout)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:	 
	void DoLeadsCheck() ;

	StyleLinesType _type;

	// Generated message map functions
	//{{AFX_MSG(StyleLayout)
	virtual BOOL OnInitDialog();
	afx_msg void modify();
	afx_msg void OnLeadsCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};
