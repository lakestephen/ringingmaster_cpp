// NewDocumentWizard.h : header file
//
// This class defines custom modal property sheet 
// NewDocumentWizard.
 
#ifndef __NEWTOUCHWIZARD_H__
#define __NEWTOUCHWIZARD_H__

#include "NewDocumentWizardPages.h"
#include "Notation.h"

/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizard

class NewDocumentWizardTransfer
{
public:
	BOOL isNumberFixed();
	int getNumber() const;
	bool isNotationNameUnique(const CString& name);
	bool isSpliceUnique(CString splice);
	NewDocumentWizardTransfer();

	~NewDocumentWizardTransfer();

	int _number;
	CString _name;
	bool _courseBased;
	Notations _notations;
	ViewType _viewType;

};


class NewDocumentWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(NewDocumentWizard)

// Construction
public:
	NewDocumentWizard(NewDocumentWizardTransfer * trans);

// Attributes
public:
	NewDocumentWizardPage0 m_Page0;
	NewDocumentWizardPage1 m_Page1;
	NewDocumentWizardPage2 m_Page2;
	NewDocumentWizardPage3 m_Page3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewDocumentWizard)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~NewDocumentWizard();

	NewDocumentWizardTransfer * _trans;

// Generated message map functions
protected:
	//{{AFX_MSG(NewDocumentWizard)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __NEWTOUCHWIZARD_H__
