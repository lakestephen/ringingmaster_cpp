#pragma once

#include "MatrixItemOptionBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerGroundFloor :
	public MatrixItemOptionBase
{
public:
	MatrixItemTowerGroundFloor(void);
public:
	~MatrixItemTowerGroundFloor(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;
protected:

	virtual bool getOption(SaveableObject* translatedEntity);

};
