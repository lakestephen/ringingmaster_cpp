// InfoDlg.h: interface for the InfoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFODLG_H__528C9E94_C91C_11D5_B510_000255162CF0__INCLUDED_)
#define AFX_INFODLG_H__528C9E94_C91C_11D5_B510_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "genericpropdlg.h"

class Notation;

class InfoDlg : public GenericPropDlg  
{
public:
	InfoDlg(CWnd* pWnd, Notation& notation);
	virtual ~InfoDlg();

};

#endif // !defined(AFX_INFODLG_H__528C9E94_C91C_11D5_B510_000255162CF0__INCLUDED_)
