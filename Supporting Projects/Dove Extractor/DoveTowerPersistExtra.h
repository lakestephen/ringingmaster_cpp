// DoveTowerPersistExtra.h: interface for the DoveTowerPersistExtra class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOVETOWERPERSISTEXTRA_H__454A0FD3_134B_11D7_B657_000255162CF0__INCLUDED_)
#define AFX_DOVETOWERPERSISTEXTRA_H__454A0FD3_134B_11D7_B657_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\RingingMaster\TowerDataStatic.h"

class DoveTowerPersistExtra : public TowerDataStatic  
{
public:
	void calculateWeight();
	DoveTowerPersistExtra();
	virtual ~DoveTowerPersistExtra();

	int _cwt;
	int _quarters;
	int _lb;
	CString _strIndex;

};
		  
typedef CArray<DoveTowerPersistExtra*, DoveTowerPersistExtra*> DoveTowerPersistExtras;

#endif // !defined(AFX_DOVETOWERPERSISTEXTRA_H__454A0FD3_134B_11D7_B657_000255162CF0__INCLUDED_)
