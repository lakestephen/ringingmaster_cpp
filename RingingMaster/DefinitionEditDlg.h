#if !defined(AFX_DEFINITIONEDITDLG_H__C76D41F4_7AB1_11D5_B60A_009027BB3286__INCLUDED_)
#define AFX_DEFINITIONEDITDLG_H__C76D41F4_7AB1_11D5_B60A_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefinitionEditDlg.h : header file
//

class Definition;
class TouchDoc;
/////////////////////////////////////////////////////////////////////////////
// DefinitionEditDlg dialog

class DefinitionEditDlg : public CDialog
{
// Construction
public:
	DefinitionEditDlg(Definition* definition, bool editing, TouchDoc* touchDoc);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DefinitionEditDlg)
	enum { IDD = IDD_DEFINITION };
	CString	_definition;
	CString	_name;
	int		_type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DefinitionEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
		   
	Definition* _origDefinition;
	bool _editing;
	TouchDoc* _touchDoc;

	// Generated message map functions
	//{{AFX_MSG(DefinitionEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINITIONEDITDLG_H__C76D41F4_7AB1_11D5_B60A_009027BB3286__INCLUDED_)
