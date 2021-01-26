#pragma once

// OptionSelectDlg dialog

class OptionSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionSelectDlg)

public:
	OptionSelectDlg(CWnd* pParent, CString trueString, CString falseString, BOOL& optionItem, CString title = "Filter");   // standard constructor
	virtual ~OptionSelectDlg();

// Dialog Data
	enum { IDD = IDD_OPTION_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString _trueString;
	CString _falseString;
	BOOL& _originalOptionItem;
	CString _title;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton _trueCtrl;
	CButton _falseCtrl;
	BOOL _option;
protected:
	virtual void OnOK();
};
