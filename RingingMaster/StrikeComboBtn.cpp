// StrikeComboBtn.cpp: implementation of the StrikeComboBtn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "StrikeComboBtn.h"
#include "ToolbarCombo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(StrikeComboBtn, CBCGPToolbarComboBoxButton, 1)

StrikeComboBtn::StrikeComboBtn():
CBCGPToolbarComboBoxButton (ID_STRIKE_SET_BELL, 
				CImageHash::GetImageOfCommand (ID_DUMMY_STRIKE_SET_BELL, FALSE),
				CBS_DROPDOWNLIST,
				60)
{

}

StrikeComboBtn::~StrikeComboBtn()
{

}

CComboBox* StrikeComboBtn::CreateCombo (CWnd* pWndParent, const CRect& rect)	
{
	ToolbarCombo* pToolbarCombo = new ToolbarCombo;

	if (!pToolbarCombo->Create (m_dwStyle, rect, pWndParent, m_nID))
	{
		delete pToolbarCombo;
		return NULL;
	}

	return pToolbarCombo;
}
