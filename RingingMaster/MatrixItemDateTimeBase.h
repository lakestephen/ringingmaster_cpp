#pragma once

#include "MatrixItemBase.h"

#include "DateTimeSelectItem.h"


class MatrixItemDateTimeBase  : public MatrixItemBase
{
public:
	MatrixItemDateTimeBase();
	MatrixItemDateTimeBase(const MatrixItemDateTimeBase& matrixItemSelectBase);
	virtual ~MatrixItemDateTimeBase();

	virtual void Serialize(CArchive& ar);

	void setDateTimeSelectItem(const DateTimeSelectItem& dateTimeSelectItem);
	virtual DateTimeSelectFormatType getDateTimeSelectFormatType();

protected:
	bool editFilterCondition(CWnd* parent);

	CString getDateTimeSelectFormatTypeString();
	void setDateTimeToFormat(COleDateTime& dateTime);

	int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2);
	
	virtual bool doFilterShouldRemove(SaveableObject* translatedEntity);

	CString getText(SaveableObject* translatedEntity);
	
	int getDefaultWidth();

	virtual COleDateTime getDateTime(SaveableObject* translatedEntity) = 0;

	virtual CString getFilterDescriptionText();

	DateTimeSelectItem _dateTimeSelectItem;
};
