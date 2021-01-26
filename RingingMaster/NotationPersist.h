// NotationPersist.h: interface for the NotationPersist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NotationPersist_H__A5546CD3_F837_11D4_B576_009027BB3286__INCLUDED_)
#define AFX_NotationPersist_H__A5546CD3_F837_11D4_B576_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NotationCallPosAgregate.h"


//i.e. bob, single
class NotationCall : public CObject
{
public:
	CString getDisplayName();
	NotationCall();
	NotationCall(NotationCall & notationCall);
	virtual void Serialize( CArchive& ar );
	void deserialize_v1(CArchive& ar);
	void deserialize_v2(CArchive& ar);
	CString _nameShorthand;
	CString _nameLonghand;
	CString _notation;
};

//i.e. Wrong Before Middle Etc
class NotationCallPosMethod : public CObject
{
public:
	NotationCallPosMethod();
	NotationCallPosMethod(NotationCallPosMethod & notationCallPosMethod);
	virtual void Serialize( CArchive& ar );

	int _lead;
	int _pos;
	CString _name;
};

enum NotationCCInfoType
{
	cc_none,
	cc_towerBells,		//date first rung or spliced on tower bells
	cc_place,			//place first rung
	cc_handbells,		//date first rung or spliced on hand bells
	cc_CCC,				//ref of earlier Central Council Collection
	cc_PMM,				//ref on Collection of Plain Minor Methods
	cc_TDMM,			//ref on Treble Dodging Minor Methods
	cc_MUG,				//code for the 360 plain triples methods rung by the Manchester Uni guild
	cc_RW				//ringing World Ref
};

class NotationCCInfo 
{
public:
	NotationCCInfo();
	NotationCCInfo(NotationCCInfo & notationCCInfo);
	virtual void Serialize( CArchive& ar );

	NotationCCInfoType _type;
	CString _text;
};


class NotationPersist : public CObject
{
public:
	void init();
	NotationPersist();
	NotationPersist(const NotationPersist & notationPersist);
	virtual ~NotationPersist();

	void operator =(const NotationPersist &notationPersist);
	virtual void Serialize( CArchive& ar );


protected:
	//main method data
	unsigned long _folder;
	int _number;
	CString _name;
	BOOL _symmetric;   //true for symetric NotationPersists (i.e. have a half lead)
	CString _notation;
	CString _leadEnd;	
	CString _leadHead; //classification letter
	CArray<NotationCall*,NotationCall*> _calls;
	CString _standardCall;  //string of the call that gets made in place of a number
	Ints _callPosLead; //call pos in single lead
	Ints _splice; //splice in single lead
	CArray<NotationCallPosMethod*, NotationCallPosMethod*> _callPosMethod; //calling pos withina plain course (W B M H Etc)
	CArray<NotationCallPosAgregate*, NotationCallPosAgregate*> _callPosAgregate;
	Ints _leadLines;
	CString _spliceLetter;
	CString _notes;
	int _courseHeadPos;

	//default flags
	BOOL _defaultCalls;
	BOOL _defaultCallPosLead;
	BOOL _defaultCallPosMethod;
	BOOL _defaultSplice;
	BOOL _defaultLeadLines;
	BOOL _defaultCourseHeadPos;

	BOOL _call;	//states if this notation is a call rather than a method

	//CC stuf
	CArray<NotationCCInfo*, NotationCCInfo*> _info;
};

#endif // !defined(AFX_NotationPersist_H__A5546CD3_F837_11D4_B576_009027BB3286__INCLUDED_)
