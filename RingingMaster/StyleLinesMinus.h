#pragma once


class Style;
enum StyleLinesType;

class StyleLinesMinus : public CBCGPPropertyPage
{
public:

	int _number;
	void ctrlsToData();
	StyleLinesMinus(Style &style, int number, StyleLinesType type);
	~StyleLinesMinus();


	CBCGPColorButton  * _btnTextColor[4];
	CComboBox			* _cbLineWidth[4];
	CBCGPColorButton  * _btnLineColor[4];
	CButton             * _overrideMinus[4];

	Style & _style;

// Dialog Data
	//{{AFX_DATA(StyleLinesMinus)
	enum { IDD = IDD_STYLE_LINES_MINUS };
	CStatic	m_tenorNote;
	CButton 	_overrideMinus0;
	CButton 	_overrideMinus1;
	CButton 	_overrideMinus2;
	CButton 	_overrideMinus3;
	CBCGPColorButton  	_btnTextColor0;
	CBCGPColorButton  	_btnTextColor1;
	CBCGPColorButton  	_btnTextColor2;
	CBCGPColorButton  	_btnTextColor3;
	CComboBox	_cbLineWidth0;
	CComboBox	_cbLineWidth1;
	CComboBox	_cbLineWidth2;
	CComboBox	_cbLineWidth3;
	CBCGPColorButton  	_btnLineColor0;
	CBCGPColorButton  	_btnLineColor1;
	CBCGPColorButton  	_btnLineColor2;
	CBCGPColorButton  	_btnLineColor3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(StyleLinesMinus)
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

	void dataToCtrls();

	// Generated message map functions
	//{{AFX_MSG(StyleLinesMinus)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	
	afx_msg void OnUpdateLineWidth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTextColor(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

};
