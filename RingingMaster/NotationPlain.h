#if !defined(AFX_NotationPlain_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationPlain_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//

class Notation;
class NotationDlg;


class NotationPlain : public CBCGPPropertyPage
{
// Construction
public:
	NotationPlain(Notation & notation, BOOL fixNumber);
	~NotationPlain();
	
	Notation & _notation;

// Dialog Data
	//{{AFX_DATA(NotationPlain)
	enum { IDD = IDD_NOTATION_PLAIN };
	CBCGPButton	_methodExplorer;
	CBCGPButton	_visualEdit;
	CBCGPButton	_searchNotation;
	CBCGPButton	_searchName;
	CStatic	_notationStaticCtrl;
	CEdit	_notationCtrl;
	CEdit	_leadEndCtrl;
	CStatic	_leadEndTitleStaticCtrl;
	CStatic	_leadEndStaticCtrl;
	CComboBox	_number;
	CString	_leadEnd;
	CString	_leadEndStatic;
	CString	_name;
	CString	_notationStatic;
	CString	_splice;
	CString	_not;
	BOOL	_asym;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationPlain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL _fixNumber;
	void setUpData();
	void verify();
	void setNumber(int number) {_number.SetCurSel(number - 3);}
	CRect _notRect[2];
	CRect _notStaticRect[2];

	// Generated message map functions
	//{{AFX_MSG(NotationPlain)
	virtual BOOL OnInitDialog();
	afx_msg void OnAsym();
	afx_msg void OnMethodExplorer();
	afx_msg void OnSearchNotation();
	afx_msg void OnSearchName();
	afx_msg void OnVisualEdit();
	afx_msg void OnKillfocusName();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
};




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationPlain_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

