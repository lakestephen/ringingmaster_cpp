#pragma once

#include "MatrixItemTextBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerComments :
	public MatrixItemTextBase
{
public:
	MatrixItemTowerComments(void);
public:
	~MatrixItemTowerComments(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;

protected:
	CString getText(SaveableObject* translatedEntity);

};
