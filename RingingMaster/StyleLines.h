#pragma once


class Style;
enum StyleLinesType;

class StyleLines : public CBCGPPropertyPage
{
public:

	int _number;
	void ctrlsToData();
	StyleLines(Style &style, int number, StyleLinesType type);
	~StyleLines();


	CBCGPColorButton  * _btnTextColor[8];
	CComboBox			* _cbLineWidth[8];
	CBCGPColorButton  * _btnLineColor[8];
	CStatic				* _staticLinesNum[8];

	Style & _style;

// Dialog Data
	//{{AFX_DATA(StyleLines)
	enum { IDD = IDD_STYLE_LINES };
	CStatic	m_tenorNote;
	CStatic	_staticLinesNum0;
	CStatic	_staticLinesNum1;
	CStatic	_staticLinesNum2;
	CStatic	_staticLinesNum3;
	CStatic	_staticLinesNum4;
	CStatic	_staticLinesNum5;
	CStatic	_staticLinesNum6;
	CStatic	_staticLinesNum7;
	CScrollBar	_sbLines;
	CBCGPColorButton  	_btnTextColor0;
	CBCGPColorButton  	_btnTextColor1;
	CBCGPColorButton  	_btnTextColor2;
	CBCGPColorButton  	_btnTextColor3;
	CBCGPColorButton  	_btnTextColor4;
	CBCGPColorButton  	_btnTextColor5;
	CBCGPColorButton  	_btnTextColor6;
	CBCGPColorButton  	_btnTextColor7;
	CComboBox	_cbLineWidth0;
	CComboBox	_cbLineWidth1;
	CComboBox	_cbLineWidth2;
	CComboBox	_cbLineWidth3;
	CComboBox	_cbLineWidth4;
	CComboBox	_cbLineWidth5;
	CComboBox	_cbLineWidth6;
	CComboBox	_cbLineWidth7;
	CBCGPColorButton  	_btnLineColor0;
	CBCGPColorButton  	_btnLineColor1;
	CBCGPColorButton  	_btnLineColor2;
	CBCGPColorButton  	_btnLineColor3;
	CBCGPColorButton  	_btnLineColor4;
	CBCGPColorButton  	_btnLineColor5;
	CBCGPColorButton  	_btnLineColor6;
	CBCGPColorButton  	_btnLineColor7;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(StyleLines)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void modify();


// Implementation
protected:
	StyleLinesType _type;
	void hideLine(int line);
	void updateTooltips();
	void dataToCtrls();

	// Generated message map functions
	//{{AFX_MSG(StyleLines)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnEditLineFont();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnUpdateLineWidth(CCmdUI* pCmdUI);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()

public:
	CBCGPButton _lineFontBtn;
};
