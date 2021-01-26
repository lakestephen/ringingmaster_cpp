#if !defined(AFX_OPTIONSGRABS_H__CE3295BF_60AC_4E95_8468_4CC7A43FF755__INCLUDED_)
#define AFX_OPTIONSGRABS_H__CE3295BF_60AC_4E95_8468_4CC7A43FF755__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsGrabs.h : header file
//


#include "RecordManager.h"

#include "ListBoxHS.h"

/////////////////////////////////////////////////////////////////////////////
// OptionsGrabs dialog

class OptionsGrabs : public CBCGPPropertyPage
{
// Construction
public:
	void selectAvailableFields(AvailableField field);
	
	OptionsGrabs();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsGrabs)
	enum { IDD = IDD_OPTIONS_GRABS };
	CBCGPButton	_reset;
	ListBoxHS	_rightCtrl;
	ListBoxHS	_leftCtrl;
	CBCGPButton	_up;
	CBCGPButton	_right;
	CBCGPButton	_left;
	CBCGPButton	_down;
	CBCGPButton	_allRight;
	CBCGPButton	_allLeft;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsGrabs)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	BOOL isOnSelectedList(AvailableField availableField);
	BOOL isOnAvailableList(AvailableField selectedField);
	void update();

	
	// Generated message map functions
	//{{AFX_MSG(OptionsGrabs)
	virtual BOOL OnInitDialog();
	afx_msg void OnAllLeft();
	afx_msg void OnAllRight();
	afx_msg void OnDown();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg void OnUp();
	afx_msg void onReset();
	//}}AFX_MSG
	afx_msg void OnUpdateAllLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAllRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDown(CCmdUI* pCmdUI);

	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSGRABS_H__CE3295BF_60AC_4E95_8468_4CC7A43FF755__INCLUDED_)
