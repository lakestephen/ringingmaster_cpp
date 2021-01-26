// NewDocumentWizard.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "NewDocumentWizard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizard

IMPLEMENT_DYNAMIC(NewDocumentWizard, CPropertySheet)

NewDocumentWizard::NewDocumentWizard(NewDocumentWizardTransfer * trans)
: CPropertySheet(IDS_NEW_TOUCH_CAPTION, NULL),
_trans(trans), 
m_Page0(trans->_viewType)
{
	AddPage(&m_Page0);
	AddPage(&m_Page1);
	AddPage(&m_Page2);	
	AddPage(&m_Page3);

	switch (trans->_viewType)
	{
	case vt_none:
		SetActivePage(&m_Page0);
		break;
	case vt_touch_main:
		SetActivePage(&m_Page1);
		break;
	case vt_crib:
		SetActivePage(&m_Page2);
		break;
	}

	SetWizardMode();


}

NewDocumentWizard::~NewDocumentWizard()
{
}


BEGIN_MESSAGE_MAP(NewDocumentWizard, CPropertySheet)
	//{{AFX_MSG_MAP(NewDocumentWizard)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizard message handlers

NewDocumentWizardTransfer::NewDocumentWizardTransfer() :
_number(8),
_courseBased(true), 
_viewType(vt_touch_main)
{
}

NewDocumentWizardTransfer::~NewDocumentWizardTransfer() 
{
	for (int i=0;i<_notations.GetSize();i++)
	{
		delete _notations.GetAt(i);
	}
	_notations.RemoveAll();
}

int NewDocumentWizardTransfer::getNumber() const
{
	if (_notations.GetSize() > 0)
	{
		return _notations.GetAt(0)->getNumber();
	}
	else 
	{
		return _number;
	}
}

BOOL NewDocumentWizardTransfer::isNumberFixed()
{
	if (_notations.GetSize() > 0)
	{
		return true;
	}
	else 
		return false;
}

bool NewDocumentWizardTransfer::isNotationNameUnique(const CString& name)
{
	bool unique = true;
	for (int i=0;i<_notations.GetSize();i++)
	{
		if (_notations.GetAt(i)->getName(true) == name)
			 unique = false;
	}
	return unique;
}

bool NewDocumentWizardTransfer::isSpliceUnique(CString splice)
{
	if (splice.IsEmpty()) return true;
	bool unique = true;
	for (int i=0;i<_notations.GetSize();i++)
	{
		if (_notations.GetAt(i)->getSpliceLetter() == splice)
			 unique = false;
	}
	return unique;
}