// SimulatorComboBtn1.cpp: implementation of the SimulatorComboBtn1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorComboBtn.h"
#include "ToolbarCombo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(SimulatorComboBtn1, CBCGPToolbarComboBoxButton, 1)

SimulatorComboBtn1::SimulatorComboBtn1():
CBCGPToolbarComboBoxButton (ID_SIMULATOR_SET_BELL1, 
				CImageHash::GetImageOfCommand (ID_DUMMY_SIMULATOR_SET_BELL, FALSE),
				CBS_DROPDOWNLIST,
				60)
{

}

SimulatorComboBtn1::~SimulatorComboBtn1()
{

}

CComboBox* SimulatorComboBtn1::CreateCombo (CWnd* pWndParent, const CRect& rect)	
{
	ToolbarCombo* pToolbarCombo = new ToolbarCombo;

	if (!pToolbarCombo->Create (m_dwStyle, rect, pWndParent, m_nID))
	{
		delete pToolbarCombo;
		return NULL;
	}

	return pToolbarCombo;
}


IMPLEMENT_SERIAL(SimulatorComboBtn2, CBCGPToolbarComboBoxButton, 1)

SimulatorComboBtn2::SimulatorComboBtn2():
CBCGPToolbarComboBoxButton (ID_SIMULATOR_SET_BELL2, 
				CImageHash::GetImageOfCommand (ID_DUMMY_SIMULATOR_SET_BELL, FALSE),
				CBS_DROPDOWNLIST,
				60)
{

}

SimulatorComboBtn2::~SimulatorComboBtn2()
{

}

CComboBox* SimulatorComboBtn2::CreateCombo (CWnd* pWndParent, const CRect& rect)	
{
	ToolbarCombo* pToolbarCombo = new ToolbarCombo;

	if (!pToolbarCombo->Create (m_dwStyle, rect, pWndParent, m_nID))
	{
		delete pToolbarCombo;
		return NULL;
	}

	return pToolbarCombo;
}
