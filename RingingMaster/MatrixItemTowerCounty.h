#pragma once
#include "MatrixItemSelectBase.h"

class MatrixItemTowerCounty :
	public MatrixItemSelectBase
{

public:
	MatrixItemTowerCounty();
	virtual ~MatrixItemTowerCounty();
	
	MatrixItemBase* clone() const;

	MatrixItemToken getToken() const;

	CString getItemName() const;
	void getAllItems(ListSelectItems &items);

protected:

	CString getText(SaveableObject* translatedEntity);
	int getDefaultWidth();
};
