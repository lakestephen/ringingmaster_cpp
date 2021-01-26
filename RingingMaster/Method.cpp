// Method.cpp: implementation of the Method class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Method.h"
#include "Row.h"
#include "MethodAnalysis.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


			   
Method::Method() :
_analysis(NULL), 
_uniqueId(-1), 
_newLeadRequired(true), 
_currentLead(NULL)
{

}

Method::~Method()
{
	delete _analysis;

	for (int i=0;i<_leads.GetSize();i++)
		delete _leads.GetAt(i);
	_leads.RemoveAll();

	for (int i=0;i<_rows.GetSize();i++)
		delete _rows.GetAt(i);
	_rows.RemoveAll();
}



int Method::getUniqueId() const 
{
	return _uniqueId;
}

void Method::setUniqueId(int uniqueId) 
{
	_uniqueId = uniqueId;
}

int Method::getNumber() const
{
	if (_rows.GetSize() > 0)
		return _rows.GetAt(0)->getNumber();
	
	ASSERT(FALSE);
	return -1;
}

void Method::addRow(Row* row)
{
	if (_newLeadRequired)
		createLead();

	_rows.Add(row);		
	_currentLead->Add(row);
}			

void Method::createLead()
{
	Row* lastRow = getLastRow();
	_currentLead = new Lead;
	if (lastRow)
		_currentLead->Add(lastRow);
	_leads.Add(_currentLead); 
	_newLeadRequired = false;
}

void Method::leadFinished()
{
	_newLeadRequired = true;
}

bool Method::hasAnalysis()
{
	return getAnalysis() != NULL; //todo remove this method
}

const MethodAnalysis* Method::getAnalysis()
{
	if (_rows.GetSize() >0)
		return _analysis;

	return NULL;
}

MethodAnalysis* Method::createAnalysis()
{
	ASSERT(_rows.GetSize() == 0);
	ASSERT(_analysis == 0);
	_analysis = new MethodAnalysis(*this);

	return _analysis;
}

TerminateReason Method::getTerminateReason() const
{
	return _terminateReason;
}

void Method::setTerminateReason(TerminateReason terminateReason)
{
	_terminateReason = terminateReason;
}

int Method::getRowCount() const
{
	return _rows.GetSize();
}

int Method::getLeadCount() const
{
	return _leads.GetSize();;
}

const Lead& Method::getLead(int index) const
{
	return *_leads.GetAt(index);
}

int Method::getPartCount() const
{
	//todo
	return 1;
}

int Method::getCallCount()const
{
	//todo
	return 1;
}

//return the number of notations actually used in this method. 
//i.e. 2 notations spliced will return 2
//     non spliced will return 1
int Method::getNotationCount() const
{
	//todo	
	return 1;
}

bool Method::isSpliced() const
{
	return (getNotationCount() > 1);
}

Row* Method::getLastRow() const
{
	if (_rows.GetSize() >0)
		return _rows.GetAt(_rows.GetUpperBound());	

	return NULL;
}

Row* Method::getFirstRow() const
{
	if (_rows.GetSize() >0)
		return _rows.GetAt(0);	

	return NULL;
}

void Method::setProofTime(DWORD proofTime) 
{
	_proofTime = proofTime;
}

DWORD Method::getProofTime() const
{
	return _proofTime;
}

Row* Method::getRow(int index) const
{
	ASSERT(index < _rows.GetSize());

	if (index >= _rows.GetSize())
		return NULL;
	
	return _rows.GetAt(index);
}

Row* Method::getRow(const RowPosition &rowPosition) //todo do we need this method/
{
	//todo 
	return _rows.GetAt(0);
}
