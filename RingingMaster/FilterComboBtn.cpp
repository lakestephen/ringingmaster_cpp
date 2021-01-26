// FilterComboBtn.cpp: implementation of the FilterComboBtn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "FilterComboBtn.h"
#include "FilterCombo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL(FilterComboBtn, CBCGPToolbarComboBoxButton, 1)

FilterComboBtn::FilterComboBtn():
CBCGPToolbarComboBoxButton (ID_FILTER_COMBO, 
				CImageHash::GetImageOfCommand (ID_FILTER, FALSE),
				CBS_DROPDOWNLIST,
				150)
{

}

FilterComboBtn::~FilterComboBtn()
{

}

CComboBox* FilterComboBtn::CreateCombo (CWnd* pWndParent, const CRect& rect)	
{
	FilterCombo* pFilterCombo = new FilterCombo(this);

	if (!pFilterCombo->Create ((m_dwStyle|CBS_OWNERDRAWVARIABLE|CBS_HASSTRINGS|CBS_DROPDOWNLIST), rect, pWndParent, m_nID))
	{
		delete pFilterCombo;
		return NULL;
	}

	return pFilterCombo;
}
