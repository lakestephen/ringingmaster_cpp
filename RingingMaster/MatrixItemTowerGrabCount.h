#pragma once

#include "MatrixItemSelectBase.h"


class MatrixItemTowerGrabCount:
	public MatrixItemSelectBase
{

public:
	MatrixItemTowerGrabCount();
	virtual ~MatrixItemTowerGrabCount();
	
	MatrixItemBase* clone() const;

	MatrixItemToken getToken() const;

	CString getItemName() const;
	void getAllItems(ListSelectItems &items);

protected:

	CString getText(SaveableObject* translatedEntity);
	int getDefaultWidth();
	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
};
