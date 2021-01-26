#if !defined(AFX_OPTIONSSCALES_H__61CA5076_26EE_11D7_B664_000255162CF0__INCLUDED_)
#define AFX_OPTIONSSCALES_H__61CA5076_26EE_11D7_B664_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsScales.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// OptionsScales dialog

class OptionsScales : public CBCGPPropertyPage
{
// Construction
public:
	OptionsScales();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsScales)
	enum { IDD = IDD_OPTIONS_SCALES };
	int		_weight;
	int		_distance;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsScales)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsScales)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSSCALES_H__61CA5076_26EE_11D7_B664_000255162CF0__INCLUDED_)
