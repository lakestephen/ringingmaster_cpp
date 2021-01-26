#if !defined(AFX_OPTIONSUSER_H__E054E306_4B2A_4B5E_A1BB_1DB6976FD378__INCLUDED_)
#define AFX_OPTIONSUSER_H__E054E306_4B2A_4B5E_A1BB_1DB6976FD378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsUser.h : header file
//

#include "CJFlatComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// OptionsUser dialog

class OptionsUser : public CBCGPPropertyPage
{
// Construction
public:
	OptionsUser();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsUser)
	enum { IDD = IDD_OPTIONS_USER };
	CBCGPButton	_delBtn;
	CBCGPButton	_addBtn;
	CBCGPButton _workingDirBut;
	CCJFlatComboBox	_comboUsers;
	CString	_workingDir;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void updateUsersList();

	// Generated message map functions
	//{{AFX_MSG(OptionsUser)
	afx_msg void OnOptionsUsersAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeOptionsUserCombo();
	afx_msg void OnWorkingDirButt();
	afx_msg void OnChangeWorkingDir();
	afx_msg void OnOptionsUsersDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSUSER_H__E054E306_4B2A_4B5E_A1BB_1DB6976FD378__INCLUDED_)
