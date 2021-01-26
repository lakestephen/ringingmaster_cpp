// Met.h: interface for the Met class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MET_H__5F4D2580_B373_11D5_BB7F_A392F5A3C375__INCLUDED_)
#define AFX_MET_H__5F4D2580_B373_11D5_BB7F_A392F5A3C375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Met  
{
public:
	CString getColumn(int index);
	Met();
	virtual ~Met();
	
	CStringArray _strs;

};

typedef CArray<Met*, Met*> Mets;

#endif // !defined(AFX_MET_H__5F4D2580_B373_11D5_BB7F_A392F5A3C375__INCLUDED_)
