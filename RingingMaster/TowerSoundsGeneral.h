#if !defined(AFX_TOWERSOUNDSGENERAL_H__71CF71EB_A649_4AD5_B4A6_4DF797BEA0D5__INCLUDED_)
#define AFX_TOWERSOUNDSGENERAL_H__71CF71EB_A649_4AD5_B4A6_4DF797BEA0D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TowerSoundsGeneral.h : header file
//
class Tower;

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsGeneral dialog

class TowerSoundsGeneral : public CBCGPPropertyPage
{
// Construction
public:
	void update();
	TowerSoundsGeneral(Tower& internalTower);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TowerSoundsGeneral)
	enum { IDD = IDD_TOWER_SOUNDS_GENERAL };
	CBCGPButton	_templateBtn;
	CComboBox	_numberCtrl;
	CString	_name;
	int	_number;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Tower& _internalTower;

	// Generated message map functions
	//{{AFX_MSG(TowerSoundsGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusTowerName();
	afx_msg void OnSelchangeTowerNumber();
	afx_msg void OnTemplateButt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	LRESULT OnQuerySiblings( WPARAM wParam, LPARAM lParam );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOWERSOUNDSGENERAL_H__71CF71EB_A649_4AD5_B4A6_4DF797BEA0D5__INCLUDED_)
