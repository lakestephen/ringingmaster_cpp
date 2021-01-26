#pragma once
#include "Stroke.h"
#include "Notation.h"
#include "Call.h"
#include "Splice.h"
#include "MusicRule.h"

enum ProofEngineType
{
	pet_noCalls,
	pet_courseBased, 
	pet_leadBased
};

class ProofInput
{
public:
	ProofInput(int number); 
	~ProofInput(void);

public:
	int getNumber() const; 

	void setName(CString name); 
	CString getName() const; 

	void setPlainLeadLetter(const CString& plainLeadLetter); 
	const CString& getPlainLeadLetter(); 
	
	ProofEngineType getProofEngineType() const; 

	void setStartRow(const CString& startRow); 
	const CString& getStartRow() const; 

	void setEndRow(const CString& endRow); 
	const CString& getEndRow() const; 

	void setStartStroke(Stroke startStroke); 
	Stroke getStartStroke() const; 

	void setStartAtRow(int startAtRow); 
	int getStartAtRow() const; 

	void setCallFrom(int callFrom); 
	int getCallFrom() const; 

	void setPerformAnalysis(); 
	bool getPerformAnalysis() const; 

	void setStartNotation(const Notation& notation); 
	Notation& getStartNotation(); 

	void addNotation(const Notations& notations);
	void addNotation(const Notation& notation);
	const Notations& getNotations() const; 

	void addCall(const Call& call);
	void addCall(const Calls& calls);
	int getCallCount() const;
	const Call* getCall(int index) const ;

	void addSplice(const Splices& splices);
	void addSplice(const Splice& splice);
	int getSpliceCount() const;
	const Splice* getSplice(int index) const ;

	void addMusicRule(const MusicRules& musicRules);
	const MusicRules& getMusicRules() const; 

	void setBlockCount(int blockCount); 
	int getBlockCount() const; 

	void setRowLimit(int rowLimit);
	int getRowLimit() const;

	void setLeadLimit(int leadLimit);
	int getLeadLimit() const;

	void setPartLimit(int partLimit);
	int getPartLimit() const;
	
	void setCircularCourseLimit(int circularCourseLimit);
	int getCircularCourseLimit() const;

	void setWrapCalls(bool wrapCalls);
	bool getWrapCalls() const;

	int getUniqueId() const;


protected:

	//general
	int _uniqueId;
	int _number;
	CString _name;
	CString _plainLeadLetter;
	
	CString _startRow; 
	Stroke _startStroke;
	int _startAtRow;

	int _blockCount;
	int _callFrom;
	Notation _startNotation; 
	bool _performAnalysis;

	Notations _notations;	
	Calls _calls;
	Splices _splices;
	MusicRules _musicRules;

	//limits	
	int _rowLimit;
	int _leadLimit; 
	int _partLimit;
	int _circularCourseLimit;
	CString _endRow;
	bool _wrapCalls;
};
