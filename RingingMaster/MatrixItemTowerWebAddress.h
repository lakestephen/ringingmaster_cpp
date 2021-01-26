#pragma once

#include "MatrixItemTextBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerWebAddress :
	public MatrixItemTextBase
{
public:
	MatrixItemTowerWebAddress(void);
public:
	~MatrixItemTowerWebAddress(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;

protected:
	CString getText(SaveableObject* translatedEntity);

};
