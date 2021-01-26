// RowPositionition.cpp: implementation of the RowPositionition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "RowPosition.h"
#include "Method.h"
#include "Lead.h"
#include "Row.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RowPosition::RowPosition()  :
_lead(-1),
_leadRow(-1),
_methodRow(-1),
_valid(rpvs_invalid)
{
}

void RowPosition::setRow(int lead, int leadRow) 
{
	//set it, but allow a validation step later
	_lead = lead;
	_leadRow = leadRow;
	ASSERT(_leadRow >= 0);
	_valid = rpvs_leadRowSet;
}

bool RowPosition::setRow(const Method* method, int lead, int leadRow, bool rollOver) 
{
	ASSERT(method);
	setInvalid();
	//is it in range?
	if ((method)&&
		(lead     >= 0)&&
		(lead     <  method->getLeadCount())&& 
		(leadRow  >= 0)&&
		(leadRow  <  method->getLead(lead).GetSize()))
	{
		//should we roll over?
		if ((rollOver)&&
			(leadRow == method->getLead(lead).GetUpperBound())&&
			(lead < method->getLeadCount()-1))
		{
			lead++;
			leadRow = 0;
		}

		//set the values
		_lead = lead;
		_leadRow = leadRow;
		_methodRow = method->getLead(lead).GetAt(leadRow)->getRowIndex();
		_valid = rpvs_valid;
	}
	else
	{
		ASSERT(FALSE);
	}

	return isValid();
}

bool RowPosition::setRow(const Method* method, int methodRow) 
{
	ASSERT(method);			  //TODO can we make this faster with new method structure? Try getting the rw index of the first row in each lead.
	
	setInvalid();
	if ((method)&&(methodRow >=0))
	{
		int row = methodRow;

		int leadSize = 0;
		int i;
		for (i=0;i<method->getLeadCount();i++)
		{
			leadSize = method->getLead(i).GetUpperBound();
			if ((row - leadSize) < 0)
			{
				_lead = i;
				_leadRow = row;
				_methodRow = methodRow;
				_valid = rpvs_valid;
				return isValid();
			}
			else 
				row -= leadSize;
		}
		if (row == 0)
		{
			_lead = i-1;
			_leadRow = leadSize;
			_methodRow = methodRow;
			_valid = rpvs_valid; 
			return isValid();
		}
	}
	   

	return isValid();
}

RowPosition::~RowPosition()
{

}

void RowPosition::setInvalid()
{
	_valid = rpvs_invalid;
	_methodRow = -1;
	_leadRow = -1;
	_lead = -1;
}

int RowPosition::getLead(bool relaxedValidity) const
{	  
	ASSERT(isValid(relaxedValidity));
	if (isValid(relaxedValidity))
		return _lead;
	else
		return -1;
}

int RowPosition::getLeadRow(bool relaxedValidity) const
{
	ASSERT(isValid(relaxedValidity));
	if (isValid(relaxedValidity))
		return _leadRow;
	else
		return -1;
}

int RowPosition::getMethodRow(bool relaxedValidity)const
{
	ASSERT(isValid(relaxedValidity));
	if (isValid(relaxedValidity))
		return _methodRow;
	else
		return -1;
}

//returns the MethodRow or LeadRow depending on the globalData Manager
int RowPosition::getRowDisplay(bool relaxedValidity) const
{
	if (globalDataManager()->isMethodRowType() && _valid != rpvs_leadRowSet)
		return getMethodRow(relaxedValidity);
	else
		return getLeadRow(relaxedValidity);
}

CString RowPosition::format(RowPositionFormatType type)
{
	ASSERT(isValid());					 

	CString display;

	if (isValid())
	{
		switch(type)
		{
		case rpft_full:
			if(globalDataManager()->isMethodRowType())
				display.Format("Row %d", _methodRow + (globalDataManager()->isStartAtRowZero()?0:1));
			else
				display.Format("Lead %d/Row %d", _lead + 1, _leadRow + (globalDataManager()->isStartAtRowZero()?0:1));
			break;
		case rpft_short:
			if(globalDataManager()->isMethodRowType())
				display.Format("%d", _methodRow + (globalDataManager()->isStartAtRowZero()?0:1));
			else
				display.Format("%d/%d", _lead + 1, _leadRow + (globalDataManager()->isStartAtRowZero()?0:1));
			break;
		case rpft_lead:
			display.Format("Lead %d", _lead + 1);
			break;
		case rpft_rowDisplay:
			display.Format("Row %d", getRowDisplay() + (globalDataManager()->isStartAtRowZero()?0:1));
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}
	return display;
}

bool RowPosition::isValid(bool relaxedValidity) const
{
	if (relaxedValidity)
		return _valid != rpvs_invalid;
	else
		return _valid == rpvs_valid;
}

bool RowPosition::verify(Method* method, bool& didRollOver)
{
	didRollOver = false;

	switch (_valid)
	{
	case rpvs_invalid:
		//nothing we can do.
		break;
	case rpvs_valid:
	case rpvs_leadRowSet:
		{
			int lead = _lead;
			setRow(method, _lead, _leadRow);
			didRollOver = ((_leadRow == 0)&&(lead != _lead));
		}
		break;
	case rpvs_methodRowSet:
		ASSERT(FALSE);//not yet implemented
	}
	return isValid();

}

//returns true if decrement successfull
bool RowPosition::decrementRow(Method *method)
{
	//move back a row if we can
	int methodRow = _methodRow;
	if (setRow(method, _methodRow -1))
	{
		return true;
	}
	else
	{	//the setRow has failed. return ro original state. should succeed
		VERIFY(setRow(method, methodRow) == true);
		return false;
	}
}

bool RowPosition::incrementRow(Method *method)
{
	int methodRow = _methodRow;
	if (setRow(method, _methodRow +1))
	{
		return true;
	}
	else
	{
		//the setRow has failed. return ro original state. should succeed
		VERIFY(setRow(method, methodRow) == true);
		return false;
	}
}

int RowPosition::compare( const void * arg1, const void * arg2 )
{
	RowPosition* mrrp1 = (*(RowPosition**)arg1);
	RowPosition* mrrp2 = (*(RowPosition**)arg2);

	ASSERT((mrrp1->isValid())&&(mrrp2->isValid()));

	if (mrrp1->_methodRow > mrrp2->_methodRow)
		return 1;
	else if (mrrp1->_methodRow < mrrp2->_methodRow)
		return -1;
	else
		return 1;

}

bool RowPosition::operator !=(const RowPosition & rowPosition)
{
	ASSERT(isValid());
	return (_methodRow != rowPosition._methodRow);
}

bool RowPosition::operator ==(const RowPosition & rowPosition)
{
	ASSERT(isValid());
	return (_methodRow == rowPosition._methodRow);
}