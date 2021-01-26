#pragma once

#include "DateTimeSelectItem.h"
#include "resource.h"

class DateTimeSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(DateTimeSelectDlg)
										  
public:
	DateTimeSelectDlg(CWnd* pParent, DateTimeSelectItem& dateTimeSelectItem, CString name = "Filter");   // standard constructor
	virtual ~DateTimeSelectDlg();

// Dialog Data
	enum { IDD = IDD_DATE_TIME_SELECT };

protected:
	
	DateTimeSelectItem& _dateTimeSelectItem;
	CString _name;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBCGPDateTimeCtrl _dateCtrl1;
	CBCGPDateTimeCtrl _dateCtrl2;
	virtual BOOL OnInitDialog();
	int _type;
	afx_msg void OnBnClickedRadio();


protected:
	virtual void OnOK();
};
