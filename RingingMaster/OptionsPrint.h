#if !defined(AFX_OPTIONSPRINT_H__93253CD3_B488_4435_9E38_797CE3F3E312__INCLUDED_)
#define AFX_OPTIONSPRINT_H__93253CD3_B488_4435_9E38_797CE3F3E312__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsPrint.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OptionsPrint dialog

class OptionsPrint : public CBCGPPropertyPage
{
// Construction
public:
	OptionsPrint();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsPrint)
	enum { IDD = IDD_OPTIONS_PRINT };
			BOOL	_printColumns;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsPrint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsPrint)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSPRINT_H__93253CD3_B488_4435_9E38_797CE3F3E312__INCLUDED_)
