#pragma once

#include "MatrixItemOptionBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerUnringable :
	public MatrixItemOptionBase
{
public:
	MatrixItemTowerUnringable(void);
public:
	~MatrixItemTowerUnringable(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;
protected:

	virtual bool getOption(SaveableObject* translatedEntity);

};
