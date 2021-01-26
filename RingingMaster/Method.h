#pragma once

#include "TerminateReason.h"
#include "Lead.h"

class Row;
class RowPosition;
class MethodAnalysis;



class Method 
{
public:
	Method();	
	virtual ~Method();	

	int getUniqueId() const;
	void setUniqueId(int uniqueId);

	bool hasAnalysis();
	const MethodAnalysis* getAnalysis();
	MethodAnalysis* createAnalysis();
	TerminateReason getTerminateReason() const;
	void setTerminateReason(TerminateReason terminateReason);

	void addRow(Row* row);
	void leadFinished();

	int getNumber() const;

	int getPartCount() const;//todo should be in analysis
	int getLeadCount() const ;
	const Lead& getLead(int index) const;
	int getRowCount() const;
	int getCallCount() const;//todo should be in analysis
	int getNotationCount() const;//todo should be in analysis
	bool isSpliced() const ; //todo should be in analysis

	Row* getLastRow() const;	 //todo can we make these const ref returns
	Row* getFirstRow() const;

	DWORD getProofTime() const;
	void setProofTime(DWORD proofTime);	  //todo should be in analysis

	//todo return a ref?
	Row* getRow(const RowPosition &rowPosition); //todo do we still want this to use RowPosition? 
	Row* getRow(int index) const; 



protected:
	
	void createLead();

	//core information
	CArray<Row* , Row*> _rows;
	CArray<Lead*, Lead*> _leads; //this is just a container. The rows are the authority.
	int _uniqueId;

	//information  generated while proving
	TerminateReason _terminateReason;
	DWORD _proofTime;

	//optional additional analysis
	MethodAnalysis* _analysis;

	//internal use items
	Lead* _currentLead;
	bool _newLeadRequired;
	
};

