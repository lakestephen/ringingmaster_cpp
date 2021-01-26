#if !defined(AFX_RecordChooseTowerDlg_H__42376386_E569_4D51_A69E_38E2BA8A4115__INCLUDED_)
#define AFX_RecordChooseTowerDlg_H__42376386_E569_4D51_A69E_38E2BA8A4115__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordChooseTowerDlg.h : header file
//

#include "RecordTower.h"

/////////////////////////////////////////////////////////////////////////////
// RecordChooseTowerDlg dialog

class RecordChooseTowerDlg : public CDialog
{
// Construction
public:
	RecordTower* getRecordTower();
	RecordChooseTowerDlg(CWnd* pParent, CPoint requestPosition, RecordTower* initialTower = NULL, RecordTowers* recordTowers = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RecordChooseTowerDlg)
	enum { IDD = IDD_CHOOSE_TOWER };
	CComboBox	_towers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RecordChooseTowerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPoint _requestPosition;
	RecordTowers* _recordTowers;
	RecordTower* _recordTower;
	RecordTower* _initialTower;

	// Generated message map functions
	//{{AFX_MSG(RecordChooseTowerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTowerCombo();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RecordChooseTowerDlg_H__42376386_E569_4D51_A69E_38E2BA8A4115__INCLUDED_)
