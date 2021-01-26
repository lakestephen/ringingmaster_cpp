#pragma once

#include "MatrixItemSelectBase.h"


class MatrixItemTowerCountry:
	public MatrixItemSelectBase
{

public:
	MatrixItemTowerCountry();
	virtual ~MatrixItemTowerCountry();
	
	MatrixItemBase* clone() const;

	MatrixItemToken getToken() const;

	CString getItemName() const;
	void getAllItems(ListSelectItems &items);

protected:

	CString getText(SaveableObject* translatedEntity);
	int getDefaultWidth();
};
