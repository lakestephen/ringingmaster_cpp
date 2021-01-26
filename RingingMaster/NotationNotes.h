#if !defined(AFX_NotationNotes_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationNotes_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//

class Notation;
class NotationDlg;


class NotationNotes : public CBCGPPropertyPage
{
// Construction
public:
	NotationNotes(Notation &notation);
	~NotationNotes();

	Notation & _notation;

// Dialog Data
	//{{AFX_DATA(NotationNotes)
	enum { IDD = IDD_NOTATION_NOTES };
	CString	_notes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationNotes)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NotationNotes)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationNotes_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

