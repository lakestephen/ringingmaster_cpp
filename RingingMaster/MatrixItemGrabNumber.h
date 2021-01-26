#pragma once

#include "MatrixItemIntBase.h"

class MatrixItemGrabNumber :
	public MatrixItemIntBase
{

public:
	CString getItemName() const;
	MatrixItemGrabNumber();
	virtual ~MatrixItemGrabNumber();
	
	MatrixItemBase* clone() const;

	MatrixItemToken getToken() const;

protected:
	int getInt(SaveableObject* translatedEntity);
};