#if !defined(AFX_TowerSoundsFiles_H__0A13BFAE_0A63_45E5_8F21_1B47DA2DF929__INCLUDED_)
#define AFX_TowerSoundsFiles_H__0A13BFAE_0A63_45E5_8F21_1B47DA2DF929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TowerSoundsFiles.h : header file
//

class Tower;

#include "Stroke.h"

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsFiles dialog

#define PROFILE_BELL_SOUND_CTRL_COUNT 8

class TowerSoundsFiles: public CBCGPPropertyPage
{
// Construction
public:
	TowerSoundsFiles(Tower& internalTower);   // standard constructor

	void OnTest(int bell, Stroke stroke, bool forceStop = false) ;


// Dialog Data
	//{{AFX_DATA(TowerSoundsFiles)
	enum { IDD = IDD_TOWER_SOUNDS_FILES};
	CBCGPButton	_removeAll;
	CBCGPButton	_templateBtn;

	CBCGPButton	_path0;
	CBCGPButton	_path1;
	CBCGPButton	_path2;
	CBCGPButton	_path3;
	CBCGPButton	_path4;
	CBCGPButton	_path5;
	CBCGPButton	_path6;
	CBCGPButton	_path7;

	CBCGPButton	_edit0;
	CBCGPButton	_edit1;
	CBCGPButton	_edit2;
	CBCGPButton	_edit3;
	CBCGPButton	_edit4;
	CBCGPButton	_edit5;
	CBCGPButton	_edit6;
	CBCGPButton	_edit7;

	CBCGPButton	_test0;
	CBCGPButton	_test1;
	CBCGPButton	_test2;
	CBCGPButton	_test3;
	CBCGPButton	_test4;
	CBCGPButton	_test5;
	CBCGPButton	_test6;
	CBCGPButton	_test7;

	CScrollBar	_sbLines;

	CBCGPButton	_details0;
	CBCGPButton	_details1;
	CBCGPButton	_details2;
	CBCGPButton	_details3;
	CBCGPButton	_details4;
	CBCGPButton	_details5;
	CBCGPButton	_details6;
	CBCGPButton	_details7;

	CString	_bell0;
	CString	_bell1;
	CString	_bell2;
	CString	_bell3;
	CString	_bell4;
	CString	_bell5;
	CString	_bell6;
	CString	_bell7;

	CString	_stroke0;
	CString	_stroke1;
	CString	_stroke2;
	CString	_stroke3;
	CString	_stroke4;
	CString	_stroke5;
	CString	_stroke6;
	CString	_stroke7;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsFiles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void update();
	void updateIcons();

	Tower& _internalTower;

	bool _inited;

	CBCGPButton* _path[PROFILE_BELL_SOUND_CTRL_COUNT];
	CBCGPButton* _edit[PROFILE_BELL_SOUND_CTRL_COUNT];
	CBCGPButton* _details[PROFILE_BELL_SOUND_CTRL_COUNT];
	CBCGPButton* _test[PROFILE_BELL_SOUND_CTRL_COUNT];
	CString*	_bell[PROFILE_BELL_SOUND_CTRL_COUNT];
	CString*	_stroke[PROFILE_BELL_SOUND_CTRL_COUNT];

	int _testBell;
	Stroke _testStroke;

	void ctrlsToData();
	void dataToCtrls();
	
	int rowToBell(int row);
	Stroke rowToStroke(int row);

	void OnPath(int bell, Stroke stroke);
	void OnDetails(int bell, Stroke stroke);
	void OnEdit(int bell, Stroke stroke) ;


	// Generated message map functions
	//{{AFX_MSG(TowerSoundsFiles)
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	afx_msg void OnDetails0();
	afx_msg void OnDetails1();
	afx_msg void OnDetails2();
	afx_msg void OnDetails3();
	afx_msg void OnDetails4();
	afx_msg void OnDetails5();
	afx_msg void OnDetails6();
	afx_msg void OnDetails7();

	afx_msg void OnPaint();

	afx_msg void OnPath0();
	afx_msg void OnPath1();
	afx_msg void OnPath2();
	afx_msg void OnPath3();
	afx_msg void OnPath4();
	afx_msg void OnPath5();
	afx_msg void OnPath6();
	afx_msg void OnPath7();

	afx_msg void OnTemplateButt();
	afx_msg void OnRemoveAll();

	afx_msg void OnEdit0();
	afx_msg void OnEdit1();
	afx_msg void OnEdit2();
	afx_msg void OnEdit3();
	afx_msg void OnEdit4();
	afx_msg void OnEdit5();
	afx_msg void OnEdit6();
	afx_msg void OnEdit7();

	afx_msg void OnTest0();
	afx_msg void OnTest1();
	afx_msg void OnTest2();
	afx_msg void OnTest3();
	afx_msg void OnTest4();
	afx_msg void OnTest5();
	afx_msg void OnTest6();
	afx_msg void OnTest7();

	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnChangeVal();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	LRESULT OnQuerySiblings( WPARAM wParam, LPARAM lParam );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TowerSoundsFiles_H__0A13BFAE_0A63_45E5_8F21_1B47DA2DF929__INCLUDED_)
