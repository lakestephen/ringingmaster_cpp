// SimulatorComboBtn.h: interface for the SimulatorComboBtn class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

class SimulatorComboBtn1 : public CBCGPToolbarComboBoxButton  
{

	DECLARE_SERIAL(SimulatorComboBtn1)

public:
	SimulatorComboBtn1 ();

	virtual ~SimulatorComboBtn1();

protected:
	virtual CComboBox* CreateCombo (CWnd* pWndParent, const CRect& rect);	
};

class SimulatorComboBtn2 : public CBCGPToolbarComboBoxButton  
{

	DECLARE_SERIAL(SimulatorComboBtn2)

public:
	SimulatorComboBtn2 ();

	virtual ~SimulatorComboBtn2();

protected:
	virtual CComboBox* CreateCombo (CWnd* pWndParent, const CRect& rect);	
};
