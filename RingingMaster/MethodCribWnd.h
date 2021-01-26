#pragma once

#include "MethodWnd.h"

class MethodCrib;


/////////////////////////////////////////////////////////////////////////////
// MethodCribWnd window

class MethodCribWnd : public MethodWnd
{
// Construction
public:
	MethodCribWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodCribWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	CSize getSize();
	int getStyleMenuIndex();
	virtual ~MethodCribWnd();

	// Generated message map functions
protected:
	CSize _statSize;

	void doToolsStyle();

	CRect getNotesRect();
	CRect getCribRect();
	CRect getMethodRect();
	void drawCrib(CDC *pDC, MethodCrib* methodCrib );
	void drawNotes(CDC *pDC, MethodCrib* methodCrib );

	UINT getContectMenuId();

	void OnDraw(CDC* pDC);

	//{{AFX_MSG(MethodCribWnd)
		afx_msg void OnCribOptions();
	afx_msg void OnPopupEditnotes();
	afx_msg void OnPopupEditmethod();
	afx_msg void OnFilePrintMenu();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CString getCopyDataString();

	friend class CribView;
};

