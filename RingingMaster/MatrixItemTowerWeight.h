#pragma once
#include "matrixitembase.h"
#include "MatrixItemDoubleBase.h"

class MatrixItemTowerWeight:
	public MatrixItemDoubleBase
{

public:
	CString getItemName() const;
	MatrixItemTowerWeight();
	virtual ~MatrixItemTowerWeight();
	
	virtual MatrixItemBase* clone() const;

	virtual MatrixItemToken getToken() const;
	bool editFilterCondition(CWnd* parent);

protected:
	virtual double getDouble(SaveableObject* translatedEntity);
	int getDefaultWidth();
	CString getTextFromDouble(double doubleVal, SaveableObject* translatedEntity);

};

