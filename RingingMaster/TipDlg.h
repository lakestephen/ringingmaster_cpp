#if !defined(TIPDLG_H_INCLUDED_)
#define TIPDLG_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// TipDlg dialog

class TipDlg : public CDialog
{
// Construction
public:

	TipDlg(CWnd* pParent);	 // standard constructor
	~TipDlg();

// Dialog Data
	//{{AFX_DATA(TipDlg)
	BOOL	_show;
	CString _strTip;
	//}}AFX_DATA

	int _index;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void startAsMenu();
	void startAsSplash();

protected:
	// Generated message map functions
	//{{AFX_MSG(TipDlg)
	afx_msg void OnNextTip();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(TIPDLG_H_INCLUDED_)
