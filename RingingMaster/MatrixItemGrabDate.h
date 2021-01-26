#pragma once

#include "MatrixItemDateTimeBase.h"


class MatrixItemGrabDate:
	public MatrixItemDateTimeBase
{

public:
	CString getItemName() const;
	MatrixItemGrabDate();
	virtual ~MatrixItemGrabDate();
	
	MatrixItemBase* clone() const;

	MatrixItemToken getToken() const;

protected:

	virtual DateTimeSelectFormatType getDateTimeSelectFormatType();
	
	COleDateTime getDateTime(SaveableObject* translatedEntity);	

};
