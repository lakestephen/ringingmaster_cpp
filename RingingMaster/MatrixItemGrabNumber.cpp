#include "stdafx.h"
#include "MatrixItemGrabNumber.h"
#include "RecordManager.h"
#include "resource.h"
#include "RecordGrab.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemGrabNumber, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemGrabNumber););

MatrixItemGrabNumber::MatrixItemGrabNumber()
{
}

MatrixItemGrabNumber::~MatrixItemGrabNumber(void)
{
}

MatrixItemBase* MatrixItemGrabNumber::clone() const 
{
	return new MatrixItemGrabNumber(*this);
} 

MatrixItemToken MatrixItemGrabNumber::getToken() const
{
	return mit_grab_number;
}

int MatrixItemGrabNumber::getInt(SaveableObject* translatedEntity)
{
	ASSERT(translatedEntity);
	RecordGrab* recordGrab = dynamic_cast<RecordGrab*>(translatedEntity);
	ASSERT(recordGrab);
	if (recordGrab)
		return recordGrab->getGrabIndex();

	return 0;
}

CString MatrixItemGrabNumber::getItemName() const
{
	return "Number";
}

