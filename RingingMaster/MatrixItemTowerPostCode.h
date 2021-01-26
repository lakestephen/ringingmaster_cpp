#pragma once

#include "MatrixItemTextBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerPostCode :
	public MatrixItemTextBase
{
public:
	MatrixItemTowerPostCode(void);
public:
	~MatrixItemTowerPostCode(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;

protected:
	CString getText(SaveableObject* translatedEntity);

};
