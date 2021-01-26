#pragma once

#include "MatrixItemSelectBase.h"


class MatrixItemTowerNote:
	public MatrixItemSelectBase
{

public:
	MatrixItemTowerNote();
	virtual ~MatrixItemTowerNote();
	
	MatrixItemBase* clone() const;

	MatrixItemToken getToken() const;

	CString getItemName() const;
	void getAllItems(ListSelectItems &items);

protected:

	CString getText(SaveableObject* translatedEntity);
	int getDefaultWidth();
	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
};
