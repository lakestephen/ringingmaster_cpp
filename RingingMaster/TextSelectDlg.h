#pragma once
// TextSelectDlg.h : header file
//

#include "TextSelectItem.h"		
  

/////////////////////////////////////////////////////////////////////////////
// TextSelectDlg dialog

class TextSelectDlg : public CDialog
{
// Construction
public:
	TextSelectDlg(CWnd* pParent, TextSelectItem& textSelectItem, CString name = "Filter");
	

// Dialog Data
	//{{AFX_DATA(TextSelectDlg)
	enum { IDD = IDD_TEXT_SELECT};
	int		_style;
	CString	_text;
	BOOL	_case;
	CEdit _textCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TextSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	TextSelectItem& _textSelectItem;
	CString _name; 
	// Generated message map functions
	//{{AFX_MSG(TextSelectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

};


