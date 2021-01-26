#if !defined(AFX_UNDOLISTBOX_H__3CB08857_F7B9_4B5E_B019_D2E748A2CCD4__INCLUDED_)
#define AFX_UNDOLISTBOX_H__3CB08857_F7B9_4B5E_B019_D2E748A2CCD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UndoListBox.h : header file
//

class UndoBar;

/////////////////////////////////////////////////////////////////////////////
// UndoListBox window

class UndoListBox : public CListBox
{
// Construction
public:
	UndoListBox(UndoBar& bar);

// Attributes
protected:
	UndoBar&	m_Bar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UndoListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~UndoListBox();
	virtual BOOL PreTranslateMessage( MSG* pMsg );

	// Generated message map functions
protected:
	//{{AFX_MSG(UndoListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDOLISTBOX_H__3CB08857_F7B9_4B5E_B019_D2E748A2CCD4__INCLUDED_)
