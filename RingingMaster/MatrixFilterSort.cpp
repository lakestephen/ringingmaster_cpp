#include "stdafx.h"
#include "MatrixFilterSort.h"
#include "RingingMaster.h"



#include "MatrixEntityBase.h"
#include "MatrixManager.h"
#include "MatrixManagerColumnFactory.h"
#include "MatrixFilterSort.h"
#include "MatrixItemBase.h"
#include "MatrixFilterDescriptionItem.h"
#include "LogicDlg.h"

long MatrixFilterSort::_lastUniqueID = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//todo this file was copied direct fromscheduler check that there are not bits that we do not need.


MatrixFilterSort::MatrixFilterSort(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken) :
_entityToken(entityToken), 
_matrixFilterSectionToken(matrixFilterSectionToken),
_logicType(lt_and)
{
	generateUniqueID();
}

void MatrixFilterSort::generateUniqueID()
{
	//todo time_t is 64 bit!! _uniqueID is no
	time_t timer; 
	time(&timer);
	_uniqueID = (long)timer;

	//just in case we create more than one in a millisecond - this can happen when creating in code.
	if (_uniqueID <= _lastUniqueID)
	{
		_uniqueID = _lastUniqueID +1;
	}
	
	_lastUniqueID = _uniqueID;
}

MatrixFilterSort::~MatrixFilterSort()
{
	clearSorts();
	for (int i=0;i<_filter.GetSize();i++)
		delete _filter.GetAt(i);
	clearColumns();
}

MatrixFilterSort::MatrixFilterSort(const MatrixFilterSort& matrixFilterSort, bool newID, 
								   bool setMatrixFilterSectionToken, MatrixFilterSectionToken matrixFilterSectionToken) 
{
	operator =(matrixFilterSort);

	if (newID)
		generateUniqueID();

	if (setMatrixFilterSectionToken)
		_matrixFilterSectionToken = matrixFilterSectionToken;
}

MatrixFilterSort& MatrixFilterSort::operator = (const MatrixFilterSort& matrixFilterSort) 
{
	//clear down the old items
	for (int i=0;i<_sort.GetSize();i++)
		delete _sort.GetAt(i);
	_sort.RemoveAll();

	for (int i=0;i<_filter.GetSize();i++)
		delete _filter.GetAt(i);
	_filter.RemoveAll();

	for (int i=0;i<_column.GetSize();i++)
		delete _column.GetAt(i);
	_column.RemoveAll();

	_entityToken = matrixFilterSort._entityToken;
	_matrixFilterSectionToken = matrixFilterSort._matrixFilterSectionToken;
	_logicType = matrixFilterSort._logicType;
	_name = matrixFilterSort._name;

	_uniqueID = matrixFilterSort._uniqueID;

	//copy filters
	for (int i=0;i<matrixFilterSort._filter.GetSize();i++)
	{
		MatrixItemBase* item = matrixFilterSort._filter.GetAt(i)->clone();
		_filter.Add(item);
	}

	//copy sorts
	for (int i=0;i<matrixFilterSort._sort.GetSize();i++)
	{
		MatrixItemBase* item = matrixFilterSort._sort.GetAt(i)->clone();
		_sort.Add(item);
	}

	for (int i=0;i<matrixFilterSort._column.GetSize();i++)
	{
		MatrixItemBase* item = matrixFilterSort._column.GetAt(i)->clone();
		_column.Add(item);
	}

	return *this;
}

MatrixEntityToken MatrixFilterSort::getMatrixEntityToken()
{
	return _entityToken;
}

MatrixFilterSectionToken MatrixFilterSort::getMatrixFilterSectionToken()
{
	return _matrixFilterSectionToken;
}

CString MatrixFilterSort::getDescription()
{
	CString desc;

	CString EntityName = getEntityName();
	EntityName.MakeLower();

	//first write out the filter.
	if (getFilterCount() > 0)
	{
		desc += "Show ";
		desc += EntityName;

		desc += "'s";
		
		for (int i=0;i<getFilterCount();i++)
		{
			MatrixFilterDescriptionItems& matrixFilterDescriptionItems = 
				getFilter(i)->getFilterDescriptionItems(getLogicType());

			desc += " ";

			for (int j=0;j<matrixFilterDescriptionItems.GetSize()  - ((i== getFilterCount()-1)?1:0) ;j++)
			{
				MatrixFilterDescriptionItem* item = matrixFilterDescriptionItems.GetAt(j);
				desc += item->getText();
			}
		}

		desc += ".\r\n";
	}

	//now write out the sorts.
	if (getSortCount() > 0)
	{
		desc += "Sort ";
		desc += EntityName;

		desc += "'s by";
		
		for (int i=0;i<getSortCount();i++)
		{
			MatrixFilterDescriptionItems& matrixFilterDescriptionItems = 
				getSort(i)->getSortDescriptionItems();

			desc += " ";

			for (int j=0;j<matrixFilterDescriptionItems.GetSize() ;j++)
			{
				MatrixFilterDescriptionItem* item = matrixFilterDescriptionItems.GetAt(j);
				if ((!item->isPost())||(i < getSortCount()-1))
					desc += item->getText();
			}
		}
		desc += ".\r\n";
	}

	//now write out the columns.
	if (getColumnCount() > 0)
	{
		desc += "Show columns: ";
		
		for (int i=0;i<getColumnCount();i++)
		{
			MatrixFilterDescriptionItems& matrixFilterDescriptionItems = 
				getColumn(i)->getColumnDescriptionItems();

			desc += " ";

			for (int j=0;j<matrixFilterDescriptionItems.GetSize() ;j++)
			{
				MatrixFilterDescriptionItem* item = matrixFilterDescriptionItems.GetAt(j);
				if ((!item->isColumnCondition())&&((!item->isPost())||(i < getColumnCount()-1)))
					desc += item->getText();
			}
		}
		desc += ". ";
	}
	return desc;
}

void MatrixFilterSort::setFilter(const MatrixItemBase& filterItem, bool active)
{
	for (int i=0;i<_filter.GetSize();i++)
	{
		//found it in the list
		if (_filter.GetAt(i)->getToken() == filterItem.getToken())
		{
			//got one - no action 
			if (active == true)
				return; 
			else
			{
				delete _filter.GetAt(i);
				_filter.RemoveAt(i--);
			}
		}
	}

	//not found in the list - create one
	if (active == true)
		_filter.Add(filterItem.clone()); 

	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<_filter.GetUpperBound();i++)
		{
			if (_filter[i]->getName() > _filter[i+1]->getName())
			{
				changed = true;
				MatrixItemBase* temp = _filter.GetAt(i);
				_filter.SetAt(i,_filter.GetAt(i+1));
				_filter.SetAt(i+1, temp);
			}
		}
	}  
}

void MatrixFilterSort::clearSorts()
{
	for (int i=0;i<_sort.GetSize();i++)
		delete _sort.GetAt(i);
	_sort.RemoveAll();	
}

void MatrixFilterSort::setSort(const MatrixItemBase& sortItem, bool active, bool forceDirection)
{
	for (int i=0;i<_sort.GetSize();i++)
	{
		//found it in the list
		if (_sort.GetAt(i)->getToken() == sortItem.getToken())
		{
			//got one - no action 
			if (active == true)
				return; 
			else
			{
				delete _sort.GetAt(i);
				_sort.RemoveAt(i--);
			}
		}
	}

	//not found in the list - create one
	if (active == true)
	{
		MatrixItemBase* item = sortItem.clone();
		if (forceDirection)
			item->setSortAscending(TRUE);
		_sort.Add(item); 
	}

	//do not sort the sorts
}

void MatrixFilterSort::clearColumns()
{
	for (int i=0;i<_column.GetSize();i++)
		delete _column.GetAt(i);
	_column.RemoveAll();	
}


//we return the (new or existing) clone.
MatrixItemBase* MatrixFilterSort::setColumn(const MatrixItemBase& columnItem, bool active, bool forceDefaultWidth)
{
	for (int i=0;i<_column.GetSize();i++)
	{
		//found it in the list
		if (_column.GetAt(i)->getToken() == columnItem.getToken())
		{
			//already got one 
			if (active == true)
				// - no action 
				return _column.GetAt(i); 
			else
			{
				//remove
				delete _column.GetAt(i);
				_column.RemoveAt(i--);
			}
		}
	}

	//not found in the list - create one
	if (active == true)
	{
		MatrixItemBase* item = columnItem.clone();
		if (forceDefaultWidth)
			item->setColumnWidth();
		_column.Add(item); 

		return item;
	}

	//do not sort the columns

	return NULL;
}

int MatrixFilterSort::getFilterCount()
{
	return _filter.GetSize();
 }

MatrixItemBase* MatrixFilterSort::getFilter(int index)
{
	ASSERT((index >=0)&&(index <_filter.GetSize()));

	if ((index >=0)&&(index <_filter.GetSize()))
		return _filter.GetAt(index);

	return NULL;	
}

int MatrixFilterSort::getSortCount()
{
	return _sort.GetSize();
}



MatrixItemBase* MatrixFilterSort::getSort(int index)
{
	ASSERT((index >=0)&&(index <_sort.GetSize()));

	if ((index >=0)&&(index <_sort.GetSize()))
		return _sort.GetAt(index);

	return NULL;	
}

int MatrixFilterSort::getColumnCount()
{
	return _column.GetSize();
}

MatrixItemBase* MatrixFilterSort::getColumn(int index)
{
	ASSERT((index >=0)&&(index <_column.GetSize()));

	if ((index >=0)&&(index <_column.GetSize()))
		return _column.GetAt(index);

	return NULL;	
}

int MatrixFilterSort::getColumnIndex(MatrixItemToken token)
{
	for (int i=0;i<_column.GetSize();i++)
	{
		if (_column.GetAt(i)->getToken() == token)
			return i;
	}

	return -1;
}

CString MatrixFilterSort::getEntityName()
{
	if (_entityName.IsEmpty())
	{
		MatrixEntityBase* base = matrixManager()->createEntity(_entityToken);
        
		_entityName = base->getName();

		delete base;
	}

	return _entityName;
}

void MatrixFilterSort::editLogic(CWnd* parent)
{
 	LogicDlg dlg(parent, _logicType);

	if (dlg.DoModal() == IDOK)
	{
	}
}

bool MatrixFilterSort::moveSortUp(int index)
{
	if ((index > 0)&&
		(index < getSortCount()))
	{
		MatrixItemBase* temp = _sort.GetAt(index);
		_sort.SetAt(index,_sort.GetAt(index-1));
		_sort.SetAt(index-1, temp);
		return true;
	}
	return false;
}

bool MatrixFilterSort::moveSortDown(int index)
{
	if ((index >= 0)&&
		(index < getSortCount()-1))
	{
		//swap
		MatrixItemBase* temp = _sort.GetAt(index);
		_sort.SetAt(index,_sort.GetAt(index+1));
		_sort.SetAt(index+1, temp);
		return true;
	}
	return false;
}



bool MatrixFilterSort::deleteSort(int index)
{
	if ((index >= 0)&&
		(index < getSortCount()))
	{
		delete _sort.GetAt(index);
		_sort.RemoveAt(index);
		return true;
	}		   
	return false;
}

bool MatrixFilterSort::moveColumnUp(int index)
{
	if ((index > 0)&&
		(index < getColumnCount()))
	{
		MatrixItemBase* temp = _column.GetAt(index);
		_column.SetAt(index,_column.GetAt(index-1));
		_column.SetAt(index-1, temp);
		return true;
	}
	return false;
}

bool MatrixFilterSort::moveColumnDown(int index)
{
	if ((index >= 0)&&
		(index < getColumnCount()-1))
	{
		//swap
		MatrixItemBase* temp = _column.GetAt(index);
		_column.SetAt(index,_column.GetAt(index+1));
		_column.SetAt(index+1, temp);
		return true;
	}
	return false;
}

bool MatrixFilterSort::moveColumn(int index, int newIndex)
{      
	if ((newIndex < getColumnCount()) &&
		(index >= 0)&&
		(index < getColumnCount()))
	{
		MatrixItemBase* temp = _column.GetAt(index);
		_column.RemoveAt(index);
		_column.InsertAt(newIndex, temp);
		return true;
	}
	return false;
}

bool MatrixFilterSort::deleteColumn(int index)
{
	if ((index >= 0)&&
		(index < getColumnCount()))
	{
		delete _column.GetAt(index);
		_column.RemoveAt(index);
		return true;
	}		   
	return false;
}

bool MatrixFilterSort::deleteFilter(int index)
{
	if ((index >= 0)&&
		(index < getFilterCount()))
	{
		delete _filter.GetAt(index);
		_filter.RemoveAt(index);
		return true;
	}		   
	return false;
}

void MatrixFilterSort::setName(const CString& name)
{
	_name = name;
}

CString MatrixFilterSort::getName(bool display)
{
	return _name;
}

long MatrixFilterSort::getUniqueID()
{
	return _uniqueID;
}

LogicType MatrixFilterSort::getLogicType()
{
	return _logicType;
}

BOOL MatrixFilterSort::isAllInvoked()
{
	for (int i=0;i<_filter.GetSize();i++)
		if (!_filter.GetAt(i)->isInvoked())
			return FALSE;

	for (int i=0;i<_sort.GetSize();i++)
		if (!_sort.GetAt(i)->isInvoked())
			return FALSE;

	for (int i=0;i<_column.GetSize();i++)
		if (!_column.GetAt(i)->isInvoked())
			return FALSE;

	return TRUE;
}


void MatrixFilterSort::Serialize(CArchive& ar)
{   
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;

		//general
		ar << _entityToken;
		ar << _matrixFilterSectionToken;
		ar << _uniqueID;
		ar << _name;
		ar << _logicType;  
		
		//filters
		ar << _filter.GetSize();
		for (int i=0;i<_filter.GetSize();i++)
		{
			MatrixItemBase* filter = _filter.GetAt(i);
			ar << filter->getToken();
			filter->Serialize(ar);
		}
		
		//sorts
		ar << _sort.GetSize();
		for (int i=0;i<_sort.GetSize();i++)
		{
			MatrixItemBase* sort = _sort.GetAt(i);
			ar << sort->getToken();
			sort->Serialize(ar);
		}

		//_column
		ar << _column.GetSize();
		for (int i=0;i<_column.GetSize();i++)
		{
			MatrixItemBase* column = _column.GetAt(i);
			ar << column->getToken();
			column->Serialize(ar);
		}  		
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	


		//general
		int entityToken;
		ar >> entityToken;
		_entityToken = static_cast<MatrixEntityToken>(entityToken);

		int matrixFilterSectionToken;
		ar >> matrixFilterSectionToken;
		_matrixFilterSectionToken = static_cast<MatrixFilterSectionToken>(matrixFilterSectionToken);

		ar >> _uniqueID;

		ar >> _name;

		int logicType;
		ar >> logicType;  
		_logicType = static_cast<LogicType>(logicType);
		
		//filters
		int size; 
		ar >> size;
		for (int i=0;i<size;i++)
		{
			int token;
			ar >> token;
			MatrixItemToken matrixItemToken = static_cast<MatrixItemToken>(token);
			MatrixItemBase* filter = MatrixManagerColumnFactory::getInstance()->createColumnUsingFactory(matrixItemToken);
			filter->Serialize(ar);
			_filter.Add(filter);
		}
		
		//sorts
		ar >> size;
		for (int i=0;i<size;i++)
		{
			int token;
			ar >> token;
			MatrixItemToken matrixItemToken = static_cast<MatrixItemToken>(token);
			MatrixItemBase* sort = MatrixManagerColumnFactory::getInstance()->createColumnUsingFactory(matrixItemToken);
			sort->Serialize(ar);
			_sort.Add(sort);
		}

		//column
		ar >> size;
		for (int i=0;i<size;i++)
		{
			int token;
			ar >> token;
			MatrixItemToken matrixItemToken = static_cast<MatrixItemToken>(token);
			MatrixItemBase* column = MatrixManagerColumnFactory::getInstance()->createColumnUsingFactory(matrixItemToken);
			column->Serialize(ar);
			_column.Add(column);
		}
	}      
}


