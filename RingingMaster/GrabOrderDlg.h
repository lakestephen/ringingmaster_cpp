
#pragma once
// GrabOrderDlg.h : header file
//

#include "ListBoxHS.h"
#include "ResizeDialog.h"
#include "RecordConstants.h"

class RecordGrab;

/////////////////////////////////////////////////////////////////////////////
// GrabOrderDlg dialog

class GrabOrderDlg : public ResizeDialog
{
// Construction
public:
	void update();
	void sortGrabs();
	GrabOrderDlg(RecordGrabs& recordGrabs);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GrabOrderDlg)
	enum { IDD = IDD_GRAB_ORDER };
	ListBoxHS	_list;
	CBCGPButton _up;
	CBCGPButton	_down;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GrabOrderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	static int compareGrabs( const void * arg1, const void * arg2 );


	RecordGrabs& _recordGrabs;

	// Generated message map functions
	//{{AFX_MSG(GrabOrderDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDown();
	afx_msg void OnUp();
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg void OnUpdateUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDown(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

