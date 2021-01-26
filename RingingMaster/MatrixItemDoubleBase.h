#pragma once

#include "MatrixItemBase.h"

#include "DoubleSelectItem.h"


class MatrixItemDoubleBase  : public MatrixItemBase
{
public:
	MatrixItemDoubleBase();
	MatrixItemDoubleBase(const MatrixItemDoubleBase& matrixItemSelectBase);
	virtual ~MatrixItemDoubleBase();

	virtual void Serialize(CArchive& ar);   

	void setDoubleSelectItem(const DoubleSelectItem& floatSelectItem);

protected:
	bool editFilterCondition(CWnd* parent);

	CString getFilterDescriptionText();

	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
	
	virtual bool doFilterShouldRemove(SaveableObject* translatedEntity);

	virtual CString getText(SaveableObject* translatedEntity);
	virtual CString getTextFromDouble(double floatVal, SaveableObject* translatedEntity);
	
	int getDefaultWidth();

	virtual double getDouble(SaveableObject* translatedEntity) = 0;

	DoubleSelectItem _doubleSelectItem;
};
