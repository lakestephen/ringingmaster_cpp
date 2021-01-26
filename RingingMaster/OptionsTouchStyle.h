#if !defined(AFX_OptionsTouchStyle_H__98B7AE9A_F0AA_4B02_8C7C_4140A0B1CC39__INCLUDED_)
#define AFX_OptionsTouchStyle_H__98B7AE9A_F0AA_4B02_8C7C_4140A0B1CC39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsTouchStyle.h : header file
//
#include "CJFlatComboBox.h"
#include "afxwin.h"



/////////////////////////////////////////////////////////////////////////////
// OptionsTouchStyle dialog

class OptionsTouchStyle : public CBCGPPropertyPage
{
// Construction
public:
	OptionsTouchStyle();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsTouchStyle)
	enum { IDD = IDD_OPTIONS_TOUCH };
	CCJFlatComboBox	_notesFontSize;
	CBCGPColorButton 	_notesColor;
	CBCGPColorButton 	_plainLeadTokenColor;
	CBCGPColorButton 	_groupColor;
	CBCGPColorButton 	_definitionColor;
	CBCGPColorButton 	_varColor;
	CBCGPColorButton 	_blockDefColor;
	CBCGPColorButton 	_unchangedCHColor;
	CBCGPColorButton 	_methodSplicedColor;
	CBCGPColorButton 	_changedCHColor;
	CBCGPColorButton 	_callPosColor;
	CBCGPColorButton 	_callColor;
	//}}AFX_DATA
					 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsTouchStyle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsTouchStyle)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void updateTooltips();
public:
	virtual BOOL OnKillActive();
	CBCGPButton _titleFont;
	CBCGPButton _authorFont;
	CBCGPButton _mainFont;
	CBCGPButton _notesFont;
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedTitleFont();
	afx_msg void OnBnClickedAuthorFont();
	afx_msg void OnBnClickedMainFont();
	afx_msg void OnBnClickedNotesFont();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OptionsTouchStyle_H__98B7AE9A_F0AA_4B02_8C7C_4140A0B1CC39__INCLUDED_)
