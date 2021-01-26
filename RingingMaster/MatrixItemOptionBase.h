#pragma once

#include "MatrixItemBase.h"

class MatrixItemOptionBase  : public MatrixItemBase
{
public:
	MatrixItemOptionBase();
	MatrixItemOptionBase(const MatrixItemOptionBase& matrixItemSelectBase);
	virtual ~MatrixItemOptionBase();

	virtual void Serialize(CArchive& ar); 

	void setOptionItem(bool optionItem);

protected:
	bool editFilterCondition(CWnd* parent);

	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
	
	virtual bool doFilterShouldRemove(SaveableObject* translatedEntity);

	CString getText(SaveableObject* translatedEntity);

	virtual CString getFilterDescriptionText();
	
	virtual int getDefaultWidth();

	virtual CString getTrueString();
	virtual CString getFalseString();

	virtual bool getOption(SaveableObject* translatedEntity) = 0;

	virtual MatrixGridImages getImage(SaveableObject* translatedEntity);

	BOOL _optionItem;
	CImageList* _imageList;
};
