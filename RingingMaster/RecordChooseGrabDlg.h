#pragma once
// RecordChooseGrabDlg.h : header file
//	   

#include "RecordGrab.h"
#include "RecordConstants.h"

/////////////////////////////////////////////////////////////////////////////
// RecordChooseGrabDlg dialog

class RecordChooseGrabDlg : public CDialog
{
// Construction
public:
	RecordGrab* getRecordGrab();
	RecordChooseGrabDlg(CWnd* pParent, CPoint requestPosition, RecordGrab* initialGrab = NULL, RecordGrabs* recordGrabs = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RecordChooseGrabDlg)
	enum { IDD = IDD_CHOOSE_GRAB };
	CComboBox	_grabs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RecordChooseGrabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPoint _requestPosition;
	RecordGrabs* _recordGrabs;
	RecordGrab* _recordGrab;
	RecordGrab* _initialGrab;

	// Generated message map functions
	//{{AFX_MSG(RecordChooseGrabDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGrabCombo();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

