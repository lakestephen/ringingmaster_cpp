#if !defined(AFX_SEARCHDLG_H__183F92E1_C723_11D5_BB7F_82F42DA9DA7C__INCLUDED_)
#define AFX_SEARCHDLG_H__183F92E1_C723_11D5_BB7F_82F42DA9DA7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchDlg.h : header file
//

#include "ComboCompletion.h"
#include "NotationLite.h"
/////////////////////////////////////////////////////////////////////////////
// SearchDlg dialog

class SearchDlg : public CDialog
{
// Construction
public:
	bool doIndividualSearch(CString& searchStr, CString searchableStr);
	void doSearch();

	BOOL _advanced;
	void setDialogSize();
	SearchDlg(NotationLites& selection,int number, bool noSelect = false);   // standard constructor
	~SearchDlg();

// Dialog Data
	//{{AFX_DATA(SearchDlg)
	enum { IDD = IDD_SEARCH };
	CStatic	_findWhat;
	CComboCompletion	_numberCtrl;
	CStatic	_lineStatic;
	CButton	_advancedBtn;
	CComboCompletion	_name;
	BOOL	_location;
	BOOL	_CCC;
	BOOL	_dateTower;
	BOOL	_dateHand;
	BOOL	_LE;
	BOOL	_MUG;
	BOOL	_notation;
	BOOL	_RW_Ref;
	BOOL	_PMM;
	BOOL	_shorthand;
	BOOL	_TDMM;
	BOOL	_LH;
	CString	_searchStr;
	BOOL	_nameMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SearchDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool doAdvancedSearch(NotationLite* notation, CString& searchStr, int searchNumber);
	bool doNormalSearch(NotationLite* notation, CString& searchStr, int searchNumber);

	int _number;
	NotationLites& _selection;
	bool _noSelect;


	// Generated message map functions
	//{{AFX_MSG(SearchDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAdvanced();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHDLG_H__183F92E1_C723_11D5_BB7F_82F42DA9DA7C__INCLUDED_)
