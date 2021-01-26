// StrikeComboBtn.h: interface for the StrikeComboBtn class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

class StrikeComboBtn : public CBCGPToolbarComboBoxButton  
{

	DECLARE_SERIAL(StrikeComboBtn)

public:
	StrikeComboBtn ();

	virtual ~StrikeComboBtn();

protected:
	virtual CComboBox* CreateCombo (CWnd* pWndParent, const CRect& rect);	
};

