#if !defined(AFX_OPTIONSMETHOD_H__9B68BED7_F3E6_4232_B993_0E5F5FF78442__INCLUDED_)
#define AFX_OPTIONSMETHOD_H__9B68BED7_F3E6_4232_B993_0E5F5FF78442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsMethod.h : header file
//
#include "CJFlatComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// OptionsMethod dialog

class OptionsMethod : public CBCGPPropertyPage
{
// Construction
public:
	OptionsMethod();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsMethod)
	enum { IDD = IDD_OPTIONS_METHOD };
	CBCGPButton	_editBtn;
	CBCGPButton	_delBtn;
	CBCGPButton	_addBtn;
	CCJFlatComboBox	_seperatorCombo;
	CCJFlatComboBox	_allChangeCombo;
	CCJFlatComboBox	_methodCreator;
	CCJFlatComboBox	_methodEditor;
	CCJFlatComboBox	_comboStyle;
	int		_allChange;
	int		_seperator;
	int		_rowNoType;
	BOOL	_saveStyleWithTouch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsMethod)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsMethod)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionsMethodAdd();
	afx_msg void OnOptionsMethodDel();
	afx_msg void OnOptionsMethodEdit();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
	int _startAtRow0;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSMETHOD_H__9B68BED7_F3E6_4232_B993_0E5F5FF78442__INCLUDED_)
