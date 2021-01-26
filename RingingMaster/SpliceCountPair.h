// SpliceCountPair.h: interface for the SpliceCountPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLICECOUNTPAIR_H__A4B9A880_DAEE_11D5_BB7F_86C121F70573__INCLUDED_)
#define AFX_SPLICECOUNTPAIR_H__A4B9A880_DAEE_11D5_BB7F_86C121F70573__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SpliceCountPair  
{
public:
	SpliceCountPair(CString name, int number);
	virtual ~SpliceCountPair();

	CString _name;
	int _number;


};

typedef CArray<SpliceCountPair*, SpliceCountPair*> SpliceCountPairs;

#endif // !defined(AFX_SPLICECOUNTPAIR_H__A4B9A880_DAEE_11D5_BB7F_86C121F70573__INCLUDED_)
