#pragma once

#include "MatrixItemSelectBase.h"


class MatrixItemGrabBellsGo:
	public MatrixItemSelectBase
{

public:
	MatrixItemGrabBellsGo();
	virtual ~MatrixItemGrabBellsGo();
	
	MatrixItemBase* clone() const;

	MatrixItemToken getToken() const;

	CString getItemName() const;
	void getAllItems(ListSelectItems &items);

protected:

	CString getText(SaveableObject* translatedEntity);
	int getDefaultWidth();
	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
};
