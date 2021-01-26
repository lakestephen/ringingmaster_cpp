#if !defined(AFX_REPEATINGTOOLBAR_H__B23A869A_6AD0_11D5_B6A6_0090275C4742__INCLUDED_)
#define AFX_REPEATINGTOOLBAR_H__B23A869A_6AD0_11D5_B6A6_0090275C4742__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// repeatingtoolbar.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// RepeatingToolbar window

class RepeatingToolbar : public CBCGPToolBar
{
// Construction
public:
	RepeatingToolbar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RepeatingToolbar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~RepeatingToolbar();
	void addRepeatingButtonsIds(CArray<int,int>& buttonIds);

	// Generated message map functions
protected:
	//{{AFX_MSG(RepeatingToolbar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	int _cycles;
	UINT _timerId;
	int _commandId;
	CArray<int, int> _buttonIds;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPEATINGTOOLBAR_H__B23A869A_6AD0_11D5_B6A6_0090275C4742__INCLUDED_)
