// ProofEngine.h: interface for the ProofEngine class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
		 /*
#include "Call.h"
#include "CallNotationMap.h"
#include "CallPosition.h"
#include "Row.h"
#include "RowComment.h"
#include "Splice.h"
#include "SpliceCountPair.h"
#include "ProofInput.h"


class CallNotationMap;
class CallPositionMap;



class Lead;
class MethodPosition;





enum FalseStringType
{
	output,
	toolTip
};









typedef CMap<CString, LPCSTR, int, int> SpliceTransitions;

class ProofEngine : public CArray<Lead* , Lead*>
{
public:
	virtual void notifySplicePosPending();
	virtual void notifyCallPosPending();
	NotationCall* getNotationCall(Call *call);

	virtual WaitCallbackAction waitCallback();
	int getProofTime();
	int getCallCount();
	int getAstheticPartIndex();

	int getUniqueId() const; //todo this should be proof based / named.
	bool setMethodRequestID(ProofEngine* method);

	CString getCopyDataString();
	void calculateInCourse();
	static CString getSpliceTransitionToken(const CString& from, const CString& to);
	void createSpliceTransitions();
	virtual bool isSpliced();
	virtual int getSplicesSize();


	void addAllTheWork(Row* row, int splicePos = 0);
	void createAllTheWorkStruct();
	void getSpliceCountPairs (SpliceCountPairs & spliceCountPairs);
	//from methodinfo
	bool setNotation(CString name);
	void setupNotationsCallsAndPositions(int number);
	Notation* getCurNot();


	
	FalseRow * getFalseRow(Row * row);
	Row * getRow(const RowPosition &rowPosition);
	CString getName() {return _proofInput->getName();}
	int getNumber() const {return _number;}
	ProofEngine();
	virtual ~ProofEngine();
	int getSpliceCount() { return _spliceCount;}
	const RowArray& getCourseHeads() {return _courseHeads;}
	const RowArray& getLeadEnds() {return _leadEnds;}
	const CStringArray& getNotationStrings() {return _notationStrings;}
	BOOL getNotationSym() {return _notationSym;}

	virtual void create(ProofInput* proofInput);
	virtual void addComment(RowComment* rowComment);
	void distributePendingComments();
	int getNotationRow();
	void setNotationRow(int notationRow);
	const Notations& getNotations() const {return _proofInput->getNotations();}
	const SpliceTransitions& getSpliceTransitions() const {return _spliceTransitions;}
	const int getAstheticPartIndex() const {return _astheticPartIndex;}
	const int getCallCount() const {return _callCount;}
	const DWORD getProofTime() const {return _proofTime;}

protected:

	//only to be used in derived classes
	ProofEngine(int number);

	void init()	;

	int _callCount;	   
	int _astheticPartIndex;
	DWORD _proofTime;
	int _leadEndIndex;


	CString _lastSpliceShorthand;
	RowArray _courseHeads;
	RowArray _leadEnds;
	

	CStringArray _notationStrings;
	BOOL _notationSym;
	virtual bool shouldForceSetupNotationsCallsAndPositions();
	virtual int getCallsSize();
	virtual Call* getNextCall();
	virtual Splice* getNextSplice();
	virtual bool hasDonePrepWork();
	void incrementSpliceIndex();
	void addSpliceMade(CString splice, int partIndex);
	void wrapOverForcedSpliceMadeMatrix();
	void addCallMade(CString call, int partIndex);
	void addCourseHead(Row* row, int partIndex, int compositionRow);
	void addLeadEnd(Row* row, int partIndex, int position = -1);
	void canIncrementAstheticPartIndex(bool force = false);
	bool _circleCheckNeeded;
	bool checkTerminateCircle();
	void wrapOverForcedCallMadeMatrix();
	void completeCourseHeadMatrix();
	void completeLeadEndMatrix();
	void checkTerminate();
	void plainCourse();

	SpliceTransitions _spliceTransitions;
	int _number;
	int _spliceCount;
	int _callIndex;			//the Call we are on in the Calls array
	int _callSubIndex;		//used for agregate calls
	int _partIndex;		
	
	int _notationRow;		//the row of the notation that we are on.
	int _callRow;			//the row of the call we are on 
	Notation* _currCall;	//pointer to the current call Notation
	Notation* _curNotation;	//The curent notation

	void checkAndAddCourseHead();

	//circular touch checking
	Rows _circleCheckRows;

	//comments
	RowComments _pendingComments;
			
	bool _canChangeAstheticPartIndex;
	int _lastCompositionRow;
	int _lastPartIndex;

	bool _calculatedInCourse;

	friend class Lead;//todo remove a lot of these
		
	int _methdodRequestId;	


};

		   
		   
		   
		   
		   
		   
		   
		   
		   
		   
		   
		   
		   
		   
		   
		   
		   
		   #define EARLY_NOTIFY_SIZE_CALL 1
#define EARLY_NOTIFY_SIZE_METHOD_CHANGE 1


class Lead : public CArray<Row *, Row *>
{
public:

	void create();
	int getCommentList(RowComments& commentsList, int row, RowCommentUseType type);
	void addComment(RowComment* rowComment, RowCommentUseType type);
	void addCallMade(CString call = CString(""));
	void addSpliceMade(CString splice);
	void processSplice(Row* row);
	bool isCommentRow(int row, RowCommentUseType type);
	void getHuntBells(Ints& bells);
	Ints * getNextNotation(const Row * row);


	Lead(Method* method);
	virtual ~Lead();

	int getWorkingBellCount();
	int getWorkingBell(int index);

	int getCommentCount(RowCommentUseType type);
	RowComment * getComment(int index, RowCommentUseType type);
	void removeAllComments();
	void verifyComments(RowCommentUseType type);


protected:
	RowComments& getCommentArrayFromType(RowCommentUseType type);
	void verifyDisplayComments();
	void checkAndAddCourseHead(Row* row);
	void incrementCallIndex();
	int getInitialNotationRow();
	void checkTerminate(Row* row);
	Notation* isConditionMet(Call* call, const Row* row);
	Ints* getCallRow();
	Ints* getNotationRow();
	RowComments _displayComments;
	RowComments _soundComments;
	Method* _method;

};
*/