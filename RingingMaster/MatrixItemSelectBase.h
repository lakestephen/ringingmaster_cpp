#pragma once

#include "MatrixItemBase.h"

#include "ListSelectItem.h"

class MatrixItemSelectBase  : public MatrixItemBase
{
public:
	MatrixItemSelectBase();
	MatrixItemSelectBase(const MatrixItemSelectBase& matrixItemSelectBase);
	virtual ~MatrixItemSelectBase();

	virtual void Serialize(CArchive& ar); 

protected:
	bool editFilterCondition(CWnd* parent);

	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
	
	virtual bool doFilterShouldRemove(SaveableObject* translatedEntity);

	virtual void getAllItems(ListSelectItems &items) = 0;
	virtual void getSelectedItems(const ListSelectItems &available, ListSelectItems &selected);
	virtual void setSelectedItems(ListSelectItems &items);
		
	virtual void destroySelectedItems();
	virtual CString getFilterDescriptionText();

	virtual CString getTextForFiltering(SaveableObject* translatedEntity);

	ListSelectItems _selectedItems;
};
