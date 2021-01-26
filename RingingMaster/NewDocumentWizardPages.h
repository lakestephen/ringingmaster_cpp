// NewDocumentWizardPages.h : header file
//

#ifndef __NEWTOUCHWIZARDPAGES_H__
#define __NEWTOUCHWIZARDPAGES_H__

/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizardPage1 dialog
#include "PropList.h"
#include "RingingMaster.h"


class NewDocumentWizardPage0 : public CPropertyPage
{
	//DECLARE_DYNCREATE(NewDocumentWizardPage0)

// Construction
public:
	NewDocumentWizardPage0(ViewType viewType);
	~NewDocumentWizardPage0();

// Dialog Data
	//{{AFX_DATA(NewDocumentWizardPage0)
	enum { IDD = IDD_NEW_METHOD_WIZ_0 };
	int		_documentType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NewDocumentWizardPage0)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NewDocumentWizardPage0)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


class NewDocumentWizardPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(NewDocumentWizardPage1)

// Construction
public:
	NewDocumentWizardPage1();
	~NewDocumentWizardPage1();

// Dialog Data
	//{{AFX_DATA(NewDocumentWizardPage1)
	enum { IDD = IDD_NEW_METHOD_WIZ_1 };
	CString	_name;
	int		_courseBased;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NewDocumentWizardPage1)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NewDocumentWizardPage1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizardPage2 dialog

class NewDocumentWizardPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(NewDocumentWizardPage2)

// Construction
public:
	NewDocumentWizardPage2();
	~NewDocumentWizardPage2();

// Dialog Data
	//{{AFX_DATA(NewDocumentWizardPage2)
	enum { IDD = IDD_NEW_METHOD_WIZ_2 };
	CBCGPButton	_find;
	CBCGPButton	_explore;
	CBCGPButton	_delete;
	CBCGPButton	_add;
	PropList	_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NewDocumentWizardPage2)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardBack();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void sortNotations();
	void update();
	// Generated message map functions
	//{{AFX_MSG(NewDocumentWizardPage2)
	afx_msg void OnAddMethod();
	afx_msg void OnDeleteMethod();
	afx_msg void OnFindMethod();
	afx_msg void OnExploreMethod();
	//}}AFX_MSG
	afx_msg LRESULT OnEditNotation(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizardPage3 dialog

class NewDocumentWizardPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(NewDocumentWizardPage3)

// Construction
public:
	NewDocumentWizardPage3();
	~NewDocumentWizardPage3();

// Dialog Data
	//{{AFX_DATA(NewDocumentWizardPage3)
	enum { IDD = IDD_NEW_METHOD_WIZ_3 };
	CComboBox	_number;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NewDocumentWizardPage3)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(NewDocumentWizardPage3)
	afx_msg void OnSelchangeNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


#endif // __NEWTOUCHWIZARDPAGES_H__
