#if !defined(AFX_SGEDITCTRL_H__8282BBE4_F071_11D5_B53E_000255162CF0__INCLUDED_)
#define AFX_SGEDITCTRL_H__8282BBE4_F071_11D5_B53E_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SGEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SGEditCtrl window

class SGEditCtrl : public CEdit
{
// Construction
public:
	SGEditCtrl(int iItem, int iSubItem, CString sInitText);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SGEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SGEditCtrl() {};
private:	
	int m_iItem;
	int m_iSubItem;	
	CString m_strInitText;
	BOOL  m_bVK_ESCAPE;

	// Generated message map functions
protected:
	//{{AFX_MSG(SGEditCtrl)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SGEDITCTRL_H__8282BBE4_F071_11D5_B53E_000255162CF0__INCLUDED_)
