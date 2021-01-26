#pragma once


class Style;
enum StyleLinesType;

/////////////////////////////////////////////////////////////////////////////
class StyleView : public CBCGPPropertyPage
{

	// Construction
public:
	StyleView(Style &style, StyleLinesType type);
	~StyleView();

	Style& _style;

	// Dialog Data
	//{{AFX_DATA(StyleView)
	enum { IDD = IDD_STYLE_VIEW };
	CButton	_showInCourseCtrl;
	CBCGPColorButton 	_musicRowsColor;
	CBCGPButton	_strokeFont;
	CButton	_showHandstrokeCtrl;
	CButton	_showBackstrokeCtrl;
	CButton	_showCallsCtrl;
	CBCGPButton	_callsFont;
	CBCGPColorButton 	_falseRowsColor;
	CBCGPButton	_titleFont;
	CButton	_showTitleCtrl;
	CButton	_showStartCtrl;
	CBCGPButton	_startFont;
	CButton	_startsBlobsCtrl;
	CButton	_showNotationCtrl;
	CBCGPButton	_notationFont;
	BOOL	_showTitle;
	BOOL	_showStart;
	BOOL	_showNotation;
	BOOL	_showStartBlobs;
	BOOL	_showCalls;
	BOOL	_showBackstroke;
	BOOL	_showHandstroke;
	BOOL	_showInCourse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(StyleView)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	StyleLinesType _type;

	void DoShowNotation();
	void DoShowStarts();
	void DoShowTitle();
	void DoShowCalls();
	void DoShowStroke();

	void updateTooltips();

	// Generated message map functions
	//{{AFX_MSG(StyleView)
	virtual BOOL OnInitDialog();
	afx_msg void modify();
	afx_msg void OnNotationFont();
	afx_msg void OnShowTitle();
	afx_msg void OnShowNotation();
	afx_msg void OnShowStarts();
	afx_msg void OnStartFont();
	afx_msg void OnTitleFont();
	afx_msg void OnShowCalls();
	afx_msg void OnCallsFont();
	afx_msg void OnStrokeFont();
	afx_msg void OnShowStroke();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
