#pragma once

// ColumnWidthDlg dialog

class ColumnWidthDlg : public CDialog
{
	DECLARE_DYNAMIC(ColumnWidthDlg)

public:
	ColumnWidthDlg(int& width, CWnd* pParent, CString name);   // standard constructor
	virtual ~ColumnWidthDlg();

// Dialog Data
	enum { IDD = IDD_COLUMN_WIDTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int& _originalWidth;
	CString _name;

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl _widthSpin;
protected:
	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();
	int _width;
};
