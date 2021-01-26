
#pragma once



/////////////////////////////////////////////////////////////////////////////
// ToolbarCombo window

class ToolbarCombo : public CComboBox	

{
// Construction
public:
	ToolbarCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ToolbarCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:


	// Generated message map functions
protected:
	//{{AFX_MSG(ToolbarCombo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
