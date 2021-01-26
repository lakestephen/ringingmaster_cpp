#if !defined(AFX_EDITBLOCKLETTERDLG_H__65F39700_67B2_11D5_BB7F_F7E2E9E8687E__INCLUDED_)
#define AFX_EDITBLOCKLETTERDLG_H__65F39700_67B2_11D5_BB7F_F7E2E9E8687E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditBlockLetterDlg.h : header file
//
#include "TouchBlock.h"

/////////////////////////////////////////////////////////////////////////////
// EditBlockLetterDlg dialog

class EditBlockLetterDlg : public CDialog
{
// Construction
public:
	EditBlockLetterDlg(CPoint point, CString letter, TouchBlockArray& touchBlocks,
						int startLine, int endLine, int max, int min);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EditBlockLetterDlg)
	enum { IDD = IDD_EDIT_BLOCK_LETTER };
	CSpinButtonCtrl	_topSpin;
	CSpinButtonCtrl	_bottomSpin;
	CString	_blockLetter;
	int		_top;
	int		_bottom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditBlockLetterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString _originalLetter;
	TouchBlockArray& _touchBlocks;
	CPoint _point;

	int _max;
	int _min;
	// Generated message map functions
	//{{AFX_MSG(EditBlockLetterDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void AFXAPI DDV_MinMaxChars(CDataExchange* pDX, CString const& value, int nCharsMin, int nCharsMax);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITBLOCKLETTERDLG_H__65F39700_67B2_11D5_BB7F_F7E2E9E8687E__INCLUDED_)
