// FilterComboBtn.h: interface for the FilterComboBtn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTERCOMBOBTN_H__72D967AF_7D47_4810_9273_6AACA031CD63__INCLUDED_)
#define AFX_FILTERCOMBOBTN_H__72D967AF_7D47_4810_9273_6AACA031CD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FilterComboBtn : public CBCGPToolbarComboBoxButton  
{

	DECLARE_SERIAL(FilterComboBtn)

public:
	FilterComboBtn ();

	virtual ~FilterComboBtn();

protected:
	virtual CComboBox* CreateCombo (CWnd* pWndParent, const CRect& rect);	
};

#endif // !defined(AFX_FILTERCOMBOBTN_H__72D967AF_7D47_4810_9273_6AACA031CD63__INCLUDED_)
