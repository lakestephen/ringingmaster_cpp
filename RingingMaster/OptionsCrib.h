#if !defined(AFX_OPTIONSCRIB_H__D69BCD14_ABC0_11D7_B6E2_000255162CF0__INCLUDED_)
#define AFX_OPTIONSCRIB_H__D69BCD14_ABC0_11D7_B6E2_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsCrib.h : header file
//

#include "CJFlatComboBox.h"


/////////////////////////////////////////////////////////////////////////////
// OptionsCrib dialog

class OptionsCrib : public CBCGPPropertyPage
{
// Construction
public:
	OptionsCrib();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsCrib)
	enum { IDD = IDD_OPTIONS_CRIB };
	CBCGPColorButton 	_notesFontColor;
	CBCGPColorButton 	_cribFontColor;
	CCJFlatComboBox	_notesFontSize;
	CCJFlatComboBox	_cribFontSize;
	CBCGPFontComboBox	_notesFont;
	CBCGPFontComboBox		_cribFont;
	CBCGPButton	_editBtn;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsCrib)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsCrib)
	afx_msg void OnOptionsLinesEdit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSCRIB_H__D69BCD14_ABC0_11D7_B6E2_000255162CF0__INCLUDED_)
