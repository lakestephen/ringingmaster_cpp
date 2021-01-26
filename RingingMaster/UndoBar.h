#if !defined(AFX_UNDOBAR_H__F88FEBBF_A4D0_4BEC_80DE_0CC1D1C8B073__INCLUDED_)
#define AFX_UNDOBAR_H__F88FEBBF_A4D0_4BEC_80DE_0CC1D1C8B073__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UndoBar.h : header file
//

#include "UndoListBox.h"

class UndoButton;

/////////////////////////////////////////////////////////////////////////////
// UndoBar window

class UndoBar : public CBCGPPopupMenuBar
{
	DECLARE_SERIAL(UndoBar)

// Construction
public:
	UndoBar();

// Attributes
protected:
	UndoListBox	m_wndList;
	CRect			m_rectLabel;
	int				m_nLabelHeight;
	CString			m_strLabel;

// Operations
public:
	void DoUndo ();
	void SetLabel (const CString& strLabel);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UndoBar)
	//}}AFX_VIRTUAL

	virtual void OnFillBackground (CDC* pDC);
	virtual CSize CalcSize (BOOL bVertDock);

// Implementation
public:
	virtual ~UndoBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(UndoBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	UndoButton* GetParentButton () const;
};

//////////////////////////////////////////////////////////////////////////////
// UndoMenu

class UndoMenu : public CBCGPPopupMenu
{
	DECLARE_SERIAL(UndoMenu)

	virtual CBCGPPopupMenuBar* GetMenuBar ()
	{
		return &m_wndUndoBar;
	}

	UndoBar	m_wndUndoBar;

    // Generated message map functions
protected:
    //{{AFX_MSG(UndoMenu)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////
// UndoButton

class UndoButton : public CBCGPToolbarMenuButton
{
	friend class UndoBar;

	DECLARE_SERIAL(UndoButton)

public:
	UndoButton()
	{
	}

	UndoButton(UINT uiCmdID, LPCTSTR lpszText) :
		CBCGPToolbarMenuButton (uiCmdID, NULL,
			CImageHash::GetImageOfCommand (uiCmdID, FALSE), lpszText)
	{
	}

	CStringList m_lstActions;

protected:
	virtual CBCGPPopupMenu* CreatePopupMenu ();

	virtual BOOL IsEmptyMenuAllowed () const
	{
		return TRUE;
	}

	virtual void OnChangeParentWnd (CWnd* pWndParent)
	{
		CBCGPToolbarMenuButton::OnChangeParentWnd (pWndParent);
		m_bDrawDownArrow = TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDOBAR_H__F88FEBBF_A4D0_4BEC_80DE_0CC1D1C8B073__INCLUDED_)
