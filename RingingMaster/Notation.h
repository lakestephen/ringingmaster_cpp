// Notation.h: interface for the Notation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTATION_H__2F64DC24_F8DF_11D4_B577_009027BB3286__INCLUDED_)
#define AFX_NOTATION_H__2F64DC24_F8DF_11D4_B577_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NotationPersist.h"  
#include "GlobalDataManager.h"  
#include "RingingMaster.h"


class Method;
class NotationLite;
class AllTheWorkBell;


enum CallError
{
	CE_OK,
	CE_OK_DEFAULT,	
	CE_DUPLICATE_SHORTHAND_NAME,
	CE_DUPLICATE_LONGHAND_NAME,
	CE_DUPLICATE_NOTATION,
	CE_INVALID_SHORTHAND_NAME,
	CE_INVALID_LONGHAND_NAME,
	CE_INVALID_NOTATION,
	CE_ERROR
};

enum CallPosLeadError
{
	CPLE_OK,
	CPLE_OK_DEFAULT,	
	CPLE_DUPLICATE_POS,
	CPLE_INVALID_POS,
	CPLE_ERROR
};


enum CallPosMethodError
{
	CPME_OK,
	CPME_OK_DEFAULT,	
	CPME_DUPLICATE_POS,
	CPME_DUPLICATE_NAME,
	CPME_INVALID_LEAD,
	CPME_INVALID_POS,
	CPME_INVALID_NAME,
	CPME_ERROR
};

enum CallPosAgregateError
{
	CPAE_OK,
	CPAE_DUPLICATE_AGREGATE,
	CPAE_DUPLICATE_NAME,
	CPAE_ERROR
};


enum LeadLinesError
{
	LLE_OK,
	LLE_OK_DEFAULT,	
	LLE_DUPLICATE_LINE,
	LLE_INVALID_LINE,
	LLE_ERROR
};

enum SpliceError
{
	SPE_OK,
	SPE_OK_DEFAULT,	
	SPE_DUPLICATE_POS,
	SPE_INVALID_POS,
	SPE_ERROR
};





class Notation : public NotationPersist  
{
public:
	void crib();
	bool isSpliceLetter(CString name);
	//verification
	bool verify(CString &msg);
	bool verifyNonEssential(CString &msg);

	//CC info stuff
	void setCCInfo(NotationCCInfoType type, CString info);
	CString getCCInfo(NotationCCInfoType type);
	
	//notes stuff
	void setNotes(CString & notes);
	CString getNotes();	

	//standard (default) call stuff
	bool  hasStandardCall();
	NotationCall* getStandardCall();
	void setStandardCall(const CString& str);


	//lead lines stuff
	LeadLinesError changeLeadLine(int index, int leadLine);
	bool deleteLeadLine(int index);
	BOOL getDefaultLeadLines();
	int getLeadLineCount();
	int getLeadLine(int index);
	int findLeadLine(int leadLine);
	void setDefaultLeadLines(BOOL state = -1);
	CString getAddLeadLineErrorText(LeadLinesError err);
	LeadLinesError addLeadLine(int leadLine);
	void sortLeadLine();
	void addLeadLineDefault();

	//call pos method stuff
	void setDefaultCallPosMethod(BOOL state = -1);
	CallPosMethodError changeCallPosMethod(int index, int lead, int pos, CString name);
	bool deleteCallPosMethod(int index);
	CString getAddCallPosMethodErrorText(CallPosMethodError err);
	BOOL getDefaultCallPosMethod();
	int getCallPosMethodCount();
	NotationCallPosMethod * getCallPosMethod(int index);
	int findCallPosMethod(int lead, int pos);
	CallPosMethodError addCallPosMethod(int lead, int pos, CString name, bool overwrite = false);
	void sortCallPosMethod();
	bool getCallingPosInOrder(CStringArray& strArr, BOOL homeLast);
	void addCallPosMethodDefault(bool overwrite);
	void addCallPosMethodEnum(bool overwrite);

	//call pos lead stuff
	CallPosLeadError changeCallPosLead(int index, int pos);
	bool deleteCallPosLead(int index);
	BOOL getDefaultCallPosLead();
	int getCallPosLeadCount();
	int getCallPosLead(int index);
	int findCallPosLead(int pos);
	void setDefaultCallPosLead(BOOL state = -1);
	CString getAddCallPosLeadErrorText(CallPosLeadError err);
	CallPosLeadError addCallPosLead(int pos);
	void sortCallPosLead();
	void addCallPosLeadDefault();
	
	//call pos agregate stuff
	CallPosAgregateError addCallPosAgregate(NotationCallPosAgregate notationCallPosAgregate);
	CString getAddCallPosAgregateErrorText(CallPosAgregateError err);
	int findCallPosAgregate(CString name);
	NotationCallPosAgregate* getCallPosAgregate(int index);
	int getCallPosAgregateCount();
	bool deleteCallPosAgregate(int index);
	void sortCallPosAgregate();
	CallPosAgregateError changeCallPosAgregate(int index,NotationCallPosAgregate notationCallPosAgregate );

	//splice stuff
	SpliceError changeSplicePosition(int index, int pos);
	bool deleteSplicePosition(int index);
	BOOL getDefaultSplicePosition();
	int getSplicePositionCount();
	int getSplicePosition(int index);
	int findSplicePosition(int pos);
	void setDefaultSplicePosition(BOOL state = -1);
	CString getAddSplicePositionErrorText(SpliceError err);
	SpliceError addSplicePosition(int pos);
	void sortSplicePositions();
	void addSplicePositionDefault();

	//lead head position stuff
	BOOL getDefaultCourseHeadPos();
	void setDefaultCourseHeadPos(BOOL state = -1);
	BOOL getCourseHeadPos();
	void setCourseHeadPos(int courseHeadPos);


	//call stuff
	void setDefaultCalls(BOOL state = -1);
	CallError changeCall(int index, CString nameShorthand, CString nameLonghand, CString notation);
	bool deleteCall(int index);
	CString getAddCallErrorText(CallError err);
	BOOL getDefaultCalls();
	int getCallCount();
	NotationCall * getCall(int index);
	int findCall(CString name);
	CallError addCall(CString nameShorthand, CString nameLonghand,  CString notation);
	void sortCalls();
	void addCallDefault(bool nearCall);

	//general stuff
	void Serialize(CArchive &ar);
	void calculateLeadHead();
	void convertToAsym();
	bool checkForSymmetry(bool takeAction);
	CString getSpliceLetter() const;
	void setSpliceLetter(CString spliceLetter);
	void setName(CString name);
	bool contains(int row, int place);
	void init(bool deep = true);
	Ints * getRow(int row) const;
	CString getRowStr(int row);
	void prepare();
	int getChangesPerPlainLead() const;
	CString getNotation(bool showSymmetry = false, SeperationStyle seperationStyle = SEP_NONE) const;
	CString getLeadHead() {return _leadHead;}
	CString getLeadEnd();
	int getNumber() const;
	void create(BOOL call, CString name, int number, bool symmetric, CString notation, CString leadEnd = "", CString spliceLetter = "", bool bInit = true);
	BOOL isSymmetric();
	CString getName(bool showNumber = false, bool showSpliceLetter = false) const;
	Notation();
	Notation(const Notation & notation);
	virtual ~Notation();
	void operator =(const NotationLite& notation);
	void operator =(const Notation& notation);
	
	//todo this is only used by method. convienent storage space
	int _spliceRowCount;
	AllTheWorkBell* _allTheWork;

protected:
	void doSetDefaultLeadLine();
	void doAddCallDefault(bool nearCall);
	void doAddCallPosMethodDefault(bool overwrite = false);
	void calculateCallPosMethod(Method* bobbedLead, Method *plainCourse, int pos, int place, CString callPosName, bool overwrite);
	Ints * copyRowArray(Ints * rowArr);
	Ints * getRowArray(CString row);
	CString validateRow(CString row);
	CString validateNotation(CString not);
	CString getDisplayString(const CString &str, SeperationStyle seperationStyle) const ;
	CArray <Ints*, Ints*> _plainLead;

};


typedef CArray<Notation*, Notation*> Notations;


#endif // !defined(AFX_NOTATION_H__2F64DC24_F8DF_11D4_B577_009027BB3286__INCLUDED_)
