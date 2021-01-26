#pragma once

#include "MatrixItemOptionBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerHome :
	public MatrixItemOptionBase
{
public:
	MatrixItemTowerHome(void);
public:
	~MatrixItemTowerHome(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;

	MatrixGridImages getImage(SaveableObject* translatedEntity);
	MatrixGridImages getHeaderImageIndex();
protected:


	virtual bool getOption(SaveableObject* translatedEntity);

};
