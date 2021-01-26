// NotationCallPosAgregate.h: interface for the NotationCallPosAgregate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NotationCallPosAgregate_H__6F48F4C2_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_)
#define AFX_NotationCallPosAgregate_H__6F48F4C2_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class NotationCallPosAgregate  
{
public:
	public:
	CString getDisplay() const;
	NotationCallPosAgregate();
	NotationCallPosAgregate(NotationCallPosAgregate & notationCallPosAgregate);
	
	NotationCallPosAgregate& operator=(NotationCallPosAgregate& NotationCallPosAgregate);

	virtual ~NotationCallPosAgregate();

	virtual void Serialize( CArchive& ar );

	CStringArray _callPosMethods;
	CString _name;

};

#endif // !defined(AFX_NotationCallPosAgregate_H__6F48F4C2_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_)
