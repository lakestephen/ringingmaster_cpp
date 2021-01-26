// RowComment.h: interface for the RowComment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROWCOMMENT_H__75063A94_F33B_11D7_B70D_000255162CF0__INCLUDED_)
#define AFX_ROWCOMMENT_H__75063A94_F33B_11D7_B70D_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
			   
#include "RowPosition.h"

class Splice;
class Notation;

enum RowCommentType
{
	//the order these are in determines precedence. top of the list is most important
	rct_go,
	rct_abort,
	rct_splice,
	rct_call,
	rct_thatsAll,
	rct_part,
	rct_comment,
	rct_none,
};

enum RowCommentUseType
{
	rcut_display,
	rcut_sound
};


class RowComment : public RowPosition
{
public:
	bool incrementRow(Method* method);
	void setGo(Notation* notation);
	void setThatsAll();
	CString getStr2();
	CString getStr1();
	void setComment(CString comment);
	void setPart(CString part, CString partNumOnly);
	void setSplice(Splice* splice);
	CString getDisplayString(bool shorthand = true);
	void setCall(CString shorthandName, CString longhandName, CString callPosition);
	void setAborted();
	
	RowComment();
	virtual ~RowComment();

//this copy constructor is just the minimum for the direct sound stuff
	RowComment(const RowComment& rowComment);

	RowCommentType getRowCommentType();
	
	bool verify(Method* method);
	
	bool operator > (RowComment & rowComment);

protected:
	CString _str1;
	CString _str2;
	CString _str3;
	RowCommentType _rowCommentType;

	int _bumped;

};

typedef CArray <RowComment*, RowComment*> RowComments;


#endif // !defined(AFX_ROWCOMMENT_H__75063A94_F33B_11D7_B70D_000255162CF0__INCLUDED_)
