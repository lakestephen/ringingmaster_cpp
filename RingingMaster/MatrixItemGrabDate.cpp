#include "stdafx.h"
#include "MatrixItemGrabDate.h"
#include "RecordGrab.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemGrabDate, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemGrabDate););


MatrixItemGrabDate::MatrixItemGrabDate()
{
	
}

MatrixItemGrabDate::~MatrixItemGrabDate(void)
{
}

DateTimeSelectFormatType MatrixItemGrabDate::getDateTimeSelectFormatType()
{
	return dtsft_date;
}

MatrixItemBase* MatrixItemGrabDate::clone() const 
{
	return new MatrixItemGrabDate(*this);
} 

MatrixItemToken MatrixItemGrabDate::getToken() const
{
	return mit_grab_date;
}

CString MatrixItemGrabDate::getItemName() const
{
	return "Date";
}

COleDateTime MatrixItemGrabDate::getDateTime(SaveableObject* translatedEntity)
{
	ASSERT(translatedEntity);
	RecordGrab* recordGrab = dynamic_cast<RecordGrab*>(translatedEntity);
	ASSERT(recordGrab);
	if (recordGrab)
	{
		return recordGrab->getDateOle();
	}	

	return COleDateTime();
}



