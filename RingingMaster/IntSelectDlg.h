#pragma once


#include "resource.h"
#include "IntSelectItem.h"

// IntSelectDlg dialog

class IntSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(IntSelectDlg)

public:
	IntSelectDlg(CWnd* pParent, IntSelectItem& intSelectItem, CString label, CString name = "Filter");   // standard constructor
	virtual ~IntSelectDlg();

// Dialog Data
	enum { IDD = IDD_INT_SELECT };

protected:
	IntSelectItem& _intSelectItem;
	CString _name;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
		int _type;
	afx_msg void OnBnClickedRadio();
	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();
	CEdit _editCtrl1;
	CEdit _editCtrl2;
	CSpinButtonCtrl _spinCtrl1;
	CSpinButtonCtrl _spinCtrl2;
	int _val1;
	int _val2;
	CString _typeLabel;
};
