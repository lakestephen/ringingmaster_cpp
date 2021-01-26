#if !defined(AFX_PROPERTYTREEINTERNET_H__E03EE5EA_A160_11D7_B6D7_000255162CF0__INCLUDED_)
#define AFX_PROPERTYTREEINTERNET_H__E03EE5EA_A160_11D7_B6D7_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InternetPropList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// InternetPropList window

class InternetPropList : public CTreeCtrl			
{
// Construction
public:
	InternetPropList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InternetPropList)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	void setup();
	virtual ~InternetPropList();

	// Generated message map functions
protected:
	//{{AFX_MSG(InternetPropList)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYTREEINTERNET_H__E03EE5EA_A160_11D7_B6D7_000255162CF0__INCLUDED_)
