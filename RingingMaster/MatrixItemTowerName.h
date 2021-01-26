#pragma once

#include "MatrixItemTextBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerName :
	public MatrixItemTextBase
{
public:
	MatrixItemTowerName(void);
public:
	~MatrixItemTowerName(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;

protected:
	CString getText(SaveableObject* translatedEntity);

};
