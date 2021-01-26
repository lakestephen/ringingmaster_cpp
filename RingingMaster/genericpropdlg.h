#if !defined(AFX_GenericPropDlg_H__1AF13640_2F81_11D5_BB7F_D120B6716229__INCLUDED_)
#define AFX_GenericPropDlg_H__1AF13640_2F81_11D5_BB7F_D120B6716229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenericPropDlg.h : header file
//

#include "ResizeDialog.h"
#include "PropList.h"
#include "PropItem.h"
/////////////////////////////////////////////////////////////////////////////
// GenericPropDlg dialog



class GenericPropDlg : public ResizeDialog
{
// Construction
public:
	void addProp(CString name, CString value);
	GenericPropDlg(CWnd* pParent, CString name, CString regName, int devider = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GenericPropDlg)
	enum { IDD = IDD_GENERIC_PROP };
	PropList _list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GenericPropDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString _name;
	int _devider;
	PropArray _defPropItems;
	CString _regName;

	// Generated message map functions
	//{{AFX_MSG(GenericPropDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GenericPropDlg_H__1AF13640_2F81_11D5_BB7F_D120B6716229__INCLUDED_)
