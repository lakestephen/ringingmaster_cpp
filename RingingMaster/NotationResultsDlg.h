#if !defined(AFX_NOTATIONRESULTSDLG_H__B9957701_B773_11D5_BB7F_F333FB19907B__INCLUDED_)
#define AFX_NOTATIONRESULTSDLG_H__B9957701_B773_11D5_BB7F_F333FB19907B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationResultsDlg.h : header file
//

#include "NotationResultList.h"
#include "ResizeDialog.h"
#include "NotationLite.h"



/////////////////////////////////////////////
////////////////////////////////
// NotationResultsDlg dialog

class NotationResultsDlg : public ResizeDialog
{
// Construction
public:
	void updateStatus();
	void makeSelection();
	NotationResultsDlg(NotationLites& lites, CWnd* pParent, bool noSelect = false);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NotationResultsDlg)
	enum { IDD = IDD_NOTATION_RESULTS };
	NotationResultList	_list;
	CString	_status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NotationResultsDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	NotationLites& _lites;
	bool _noSelect;

	// Generated message map functions
	//{{AFX_MSG(NotationResultsDlg)
	virtual BOOL OnInitDialog();	
	virtual void OnOK();
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class NotationResultList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTATIONRESULTSDLG_H__B9957701_B773_11D5_BB7F_F333FB19907B__INCLUDED_)
