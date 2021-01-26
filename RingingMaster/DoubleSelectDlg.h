#pragma once


#include "resource.h"
#include "DoubleSelectItem.h"

// DoubleSelectDlg dialog

class DoubleSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(DoubleSelectDlg)

public:
	DoubleSelectDlg(CWnd* pParent, DoubleSelectItem& doubleSelectItem, CString label, CString name = "Filter");   // standard constructor
	virtual ~DoubleSelectDlg();

// Dialog Data
	enum { IDD = IDD_DOUBLE_SELECT };

protected:
	DoubleSelectItem& _doubleSelectItem;
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
	double _val1;
	double _val2;
	CString _typeLabel;
};
