#pragma once

enum MatrixFilterSectionToken;

class CombinedFilterItem  
{
public:
	CombinedFilterItem();
	virtual ~CombinedFilterItem();

	CString _name; 
	MatrixFilterSectionToken _filterType;
	long _filterId;

};

typedef CArray<CombinedFilterItem*, CombinedFilterItem*> CombinedFilterItems;

