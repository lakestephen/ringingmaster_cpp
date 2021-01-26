#if !defined(AFX_NotationCourseHead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationCourseHead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//
#include "PropList.h"

class Notation;
class NotationDlg;

class NotationCourseHead : public CBCGPPropertyPage
{

// Construction
public:
	NotationCourseHead(Notation & notation);
	~NotationCourseHead();

	void createCourseHeadPos();


// Dialog Data
	//{{AFX_DATA(NotationCourseHead)
	enum { IDD = IDD_NOTATION_COURSE_HEAD};
	CButton	_useDefaultCourseHead;
	CSpinButtonCtrl	_courseHeadSpin;
	CBCGPButton	_courseHeadButton;
	int		_courseHead;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationCourseHead)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(NotationCourseHead)
	virtual BOOL OnInitDialog();
	afx_msg void OnCourseHeadButton();
	afx_msg void OnUseDefaultCourseHead();
	//}}AFX_MSG
	afx_msg void OnUpdateDefaultCourseHeads(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
};

		  




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationCourseHead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

