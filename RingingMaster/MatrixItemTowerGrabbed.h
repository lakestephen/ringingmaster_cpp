#pragma once

#include "MatrixItemOptionBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemTowerGrabbed :
	public MatrixItemOptionBase	
{
public:
	MatrixItemTowerGrabbed(void);
public:
	~MatrixItemTowerGrabbed(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;

	int getDefaultWidth();
	MatrixGridImages getImage(SaveableObject* translatedEntity);
	MatrixGridImages getHeaderImageIndex();
protected:

	CString getText(SaveableObject* translatedEntity);
	virtual bool getOption(SaveableObject* translatedEntity);

};
