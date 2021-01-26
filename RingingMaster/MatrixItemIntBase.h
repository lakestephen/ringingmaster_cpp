#pragma once

#include "MatrixItemBase.h"

#include "IntSelectItem.h"


class MatrixItemIntBase  : public MatrixItemBase
{
public:
	MatrixItemIntBase();
	MatrixItemIntBase(const MatrixItemIntBase& matrixItemSelectBase);
	virtual ~MatrixItemIntBase();

	virtual void Serialize(CArchive& ar);   

	void setIntSelectItem(const IntSelectItem& intSelectItem);

protected:
	bool editFilterCondition(CWnd* parent);

	CString getFilterDescriptionText();

	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
	
	virtual bool doFilterShouldRemove(SaveableObject* translatedEntity);

	virtual CString getText(SaveableObject* translatedEntity);
	
	int getDefaultWidth();

	virtual int getInt(SaveableObject* translatedEntity) = 0;

	IntSelectItem _intSelectItem;
};
