#if !defined(AFX_NotationInfo_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationInfo_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//
#include "PropList.h"

class Notation;
class NotationDlg;


class NotationInfo : public CBCGPPropertyPage
{
// Construction
public:
	NotationInfo(Notation & _notation);
	~NotationInfo();


// Dialog Data
	//{{AFX_DATA(NotationInfo)
	enum { IDD = IDD_NOTATION_INFO };
	PropList	_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationInfo)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(NotationInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationInfo_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

