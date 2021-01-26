#if !defined(AFX_OPTIONSKEYBOARD_H__89BB352D_DFE4_4DB0_AB00_C18E897A420B__INCLUDED_)
#define AFX_OPTIONSKEYBOARD_H__89BB352D_DFE4_4DB0_AB00_C18E897A420B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsKeyboard.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// OptionsKeyboard dialog

class OptionsKeyboard : public CBCGPPropertyPage

{
// Construction
public:
	OptionsKeyboard();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsKeyboard)
	enum { IDD = IDD_OPTIONS_KEYBOARD };
	CBCGPButton	_testBtn;
	CSpinButtonCtrl	_freqSpin;
	int		_freq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsKeyboard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	bool _inited;

	// Generated message map functions
	//{{AFX_MSG(OptionsKeyboard)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTest();
	afx_msg void OnChangeFreq();
	afx_msg void OnUpdateGeneral(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEnable(CCmdUI* pCmdUI); 
	afx_msg void OnEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSKEYBOARD_H__89BB352D_DFE4_4DB0_AB00_C18E897A420B__INCLUDED_)
