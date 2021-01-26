#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// GenericNumberSpinDlg dialog

class GenericNumberSpinDlg : public CDialog
{
	DECLARE_DYNAMIC(GenericNumberSpinDlg)

public:
	GenericNumberSpinDlg(CWnd* pParent, int& val, int min, int max, CString dlgName, CString hint);   // standard constructor
	virtual ~GenericNumberSpinDlg();

// Dialog Data
	enum { IDD = IDD_GENERIC_NUMBER_SPIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int& _val;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CSpinButtonCtrl _valSpin;
	int _valEdit;
	const int _min;
	const int _max;

	CString _dlgName;
	CString _hint;
protected:
	virtual void OnOK();
public:
	CStatic _hintStatic;
};
