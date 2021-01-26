#if !defined(AFX_OPTIONSTOWER_H__7B90DCE8_18F3_4D49_BD3D_13A2663C2333__INCLUDED_)
#define AFX_OPTIONSTOWER_H__7B90DCE8_18F3_4D49_BD3D_13A2663C2333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsTower.h : header file
//

#include "RecordManager.h"
#include "ListBoxHS.h"


/////////////////////////////////////////////////////////////////////////////
// OptionsTower dialog

class OptionsTower : public CBCGPPropertyPage
{
// Construction
public:
	void selectAvailableFields(AvailableField field);
	OptionsTower();   // standard constructor



// Dialog Data
	//{{AFX_DATA(OptionsTower)
	enum { IDD = IDD_OPTIONS_TOWER };
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
	//{{AFX_VIRTUAL(OptionsTower)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL isOnSelectedList(AvailableField availableField);
	BOOL isOnAvailableList(AvailableField selectedField);
	void update();


	// Generated message map functions
	//{{AFX_MSG(OptionsTower)
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

#endif // !defined(AFX_OPTIONSTOWER_H__7B90DCE8_18F3_4D49_BD3D_13A2663C2333__INCLUDED_)
