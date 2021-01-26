#if !defined(AFX_OPTIONSLIBRARY_H__31021B56_606B_4297_96BF_AE0A40E3A240__INCLUDED_)
#define AFX_OPTIONSLIBRARY_H__31021B56_606B_4297_96BF_AE0A40E3A240__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsLibrary.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OptionsLibrary dialog

class OptionsLibrary : public CBCGPPropertyPage
{
// Construction
public:
	OptionsLibrary();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsLibrary)
	enum { IDD = IDD_OPTIONS_LIBRARY };
	BOOL	_CCLibLoad;
	BOOL	_nearPlaceM;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsLibrary)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsLibrary)
	virtual BOOL OnInitDialog();
	afx_msg void OnCcLibLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSLIBRARY_H__31021B56_606B_4297_96BF_AE0A40E3A240__INCLUDED_)
