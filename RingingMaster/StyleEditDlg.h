// StyleEditDlg.h : header file
//
#pragma once


#include "Style.h"
#include "PropertySheetBase.h"

class StyleLines;
class StyleLinesMinus;
class StyleLayout;
class StyleView;
class StyleGeneral;

enum StyleLinesType
{
	slt_normal,
	slt_drawAMethod,
	slt_preview,
	slt_callchange, 
	slt_edit,
	slt_crib,
	slt_simulator,
};

class StyleEditDlg : public PropertySheetBase
{
	DECLARE_DYNAMIC(StyleEditDlg)

// Construction
public:
	
	StyleEditDlg(Style &style, CWnd* pParent, int number,  StyleLinesType type = slt_normal, CString title = CString("Style")); 	
	
// Attributes
public:
	StyleLines* _lines;
	StyleLinesMinus* _linesMinus;
	StyleLayout* _layout;
	StyleView* _view;
	StyleGeneral* _general;

	Style _style;
	Style &_originalStyle;
	CString _originalName ;
	StyleLinesType _type;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StyleEditDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	void Apply();
	CString _title;
	virtual ~StyleEditDlg();

// Generated message map functions
protected:
	CWnd * _parent;
	CBCGPButton _applyBtn;
	//{{AFX_MSG(StyleEditDlg)
	//}}AFX_MSG

	afx_msg void OnApply();

	DECLARE_MESSAGE_MAP()
};


