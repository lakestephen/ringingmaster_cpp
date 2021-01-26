#pragma once

#include "MatrixItemTextBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerGridRef :
	public MatrixItemTextBase
{
public:
	MatrixItemTowerGridRef(void);
public:
	~MatrixItemTowerGridRef(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;

protected:
	CString getText(SaveableObject* translatedEntity);

};
