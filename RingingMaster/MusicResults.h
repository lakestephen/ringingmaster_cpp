// MusicResults.h: interface for the MusicResults class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSICRESULTS_H__CCD17862_BB2D_11D5_BB7F_EAB537D5F87E__INCLUDED_)
#define AFX_MUSICRESULTS_H__CCD17862_BB2D_11D5_BB7F_EAB537D5F87E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MusicRule.h"
#include "RowPosition.h"

class MusicResultRowPosition : public RowPosition
{

public:
	CString getChange();
	MusicResultRowPosition(CString change);

protected:
	CString _change;

};

typedef CArray<MusicResultRowPosition*, MusicResultRowPosition*> MusicResultRowPositions;



class MusicResult : public MusicResultRowPositions, public MusicRule
{
public:
	MusicResult(MusicRule& musicRule, CString name = CString(""));	
	virtual ~MusicResult();
};						   

typedef CArray<MusicResult*, MusicResult*> MusicResults;

#endif // !defined(AFX_MUSICRESULTS_H__CCD17862_BB2D_11D5_BB7F_EAB537D5F87E__INCLUDED_)
