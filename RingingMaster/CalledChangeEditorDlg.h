
#if !defined(AFX_CALLEDCHANGEEDITORDLG_H__451E29F5_B106_11D6_B5FF_000255162CF0__INCLUDED_)
#define AFX_CALLEDCHANGEEDITORDLG_H__451E29F5_B106_11D6_B5FF_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalledChangeEditorDlg.h : header file
//

#include "ResizeDialog.h"
#include "MethodCallChange.h"	   
#include "CJListBox.h"
#include "CJFlatComboBox.h"
#include "ListBoxHS.h"
#include "MethodCallChangeWnd.h"



/////////////////////////////////////////////////////////////////////////////
// CalledChangeEditorDlg dialog

class CalledChangeEditorDlg : public ResizeDialog, public TowerManagerEventListener
{
// Construction
public:

	void update();
	void towerManager_notifyUpdateTowerList();
	CalledChangeEditorDlg(CWnd* pParent);   // standard constructor
	~CalledChangeEditorDlg();

// Dialog Data
	//{{AFX_DATA(CalledChangeEditorDlg)
	enum { IDD = IDD_CALL_CAHNGES_EDITOR };
	CStatic	_messageCtrl;
	CBCGPButton	_style;
	CBCGPButton	_seek;
	CBCGPButton	_rewind;
	CBCGPButton	_copy;
	CCJFlatComboBox	_numberCombo;
	CBCGPButton	_towerDetails;
	CBCGPButton	_towerOptions;
	CCJFlatComboBox	_comboTower;
	CBCGPButton	_undo;
	CBCGPButton	_stop;
	CBCGPButton	_playLive;
	CBCGPButton	_startAgain;
	CBCGPButton	_options;
	//}}AFX_DATA

	MethodCallChangeWnd _methodList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CalledChangeEditorDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool removeLastRow();
	void setMessage();
	CString getNumberName(int number);
	bool _inited;
	CString getCallText(Row* row1, Row* row2);
	CString getCallText(Row* row1, Row* row2, BOOL affect2, BOOL affect3);
	void addRow(Row* row);
	void updateComments();

	CString _seekRow;
	void setupSwapButtons();
	void setToolTips();
	void paintNumbers(CDC* pDC);
	Row* getLastRow();
	Lead* getLead();

	int _number;
	CBCGPButton _buttons[MAXBELLS];
	MethodCallChange* _method;

	// Generated message map functions
	//{{AFX_MSG(CalledChangeEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnCcOptions();	 
	afx_msg void OnCcStatrAgain();
	afx_msg void OnCcPlayLive();
	afx_msg void OnCcStop();
	afx_msg void OnCcUndo();
	afx_msg void OnSelchangeOptionsTowerCombo();
	afx_msg void OnSelchangeNumber();
	afx_msg void OnCcRewind();
	afx_msg void OnCcSeek();
	afx_msg void OnCcCopy();
	afx_msg void OnCcStyle();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	//}}AFX_MSG

	void doCcStatrAgain(bool doStop = true);	

	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg void OnUpdateCcPlayLive(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCcStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCcUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCcStartAgain(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCcRewind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsTowerCombo(CCmdUI* pCmdUI);
	afx_msg LRESULT OnSetMethodPos(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnSwap( UINT nID );
	afx_msg void OnUpdateSwap(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLEDCHANGEEDITORDLG_H__451E29F5_B106_11D6_B5FF_000255162CF0__INCLUDED_)
