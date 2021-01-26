#pragma once

#include "MatrixItemBase.h"

#include "TextSelectItem.h"

class MatrixItemTextBase  : public MatrixItemBase
{
public:
	MatrixItemTextBase();
	MatrixItemTextBase(const MatrixItemTextBase& matrixItemTextBase);
	virtual ~MatrixItemTextBase();

	virtual void Serialize(CArchive& ar); 

	void setTextSelectItem(const TextSelectItem& textSelectItem);

protected:
	bool editFilterCondition(CWnd* parent);

	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
	
	virtual bool doFilterShouldRemove(SaveableObject* translatedEntity);


	virtual CString getFilterDescriptionText();

	TextSelectItem _textSelectItem;
};
