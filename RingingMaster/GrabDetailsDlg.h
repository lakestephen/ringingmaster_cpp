#if !defined(AFX_GRABDETAILSDLG_H__14708F78_92B8_40A0_A8AF_B08097FE300D__INCLUDED_)
#define AFX_GRABDETAILSDLG_H__14708F78_92B8_40A0_A8AF_B08097FE300D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrabDetailsDlg.h : header file
//
#include "ComboCompletion.h"

class RecordGrab;

/////////////////////////////////////////////////////////////////////////////
// GrabDetailsDlg dialog

class GrabDetailsDlg : public CDialog
{
// Construction
public:
	GrabDetailsDlg(RecordGrab& recordGrab);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GrabDetailsDlg)
	enum { IDD = IDD_GRAB_DETAILS };
	CComboCompletion	_commentsCtrl;
	CComboBox	_towers;
	CComboBox	_bellSound;
	CComboBox	_bellGo;
	COleDateTime	_dateOle;
	CString	_comments;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GrabDetailsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	RecordGrab& _originalRecordGrab;
	// Generated message map functions
	//{{AFX_MSG(GrabDetailsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRABDETAILSDLG_H__14708F78_92B8_40A0_A8AF_B08097FE300D__INCLUDED_)
