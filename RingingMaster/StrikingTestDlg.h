#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "NumericEdit.h"


// StrikingTestDlg dialog

class StrikingTestDlg : public CDialog
{
	DECLARE_DYNAMIC(StrikingTestDlg)

public:
	StrikingTestDlg(CWnd* pParent, int maxNumber);   // standard constructor
	virtual ~StrikingTestDlg();

// Dialog Data
	enum { IDD = IDD_STRIKING_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int _bellsAffected;
	CEdit _bellsAffectedCtrl;
	CSpinButtonCtrl _bellsAffectedSpinCtrl;
	BOOL _allowFllowOn;
	BOOL _hideVisualFeedback;
	double _maxErrorValue;
	virtual BOOL OnInitDialog();
	int _maxNumber;
	NumericEdit _maxError;
protected:
	virtual void OnOK();
public:
	BOOL _lockHandBack;
};
