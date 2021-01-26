#include "stdafx.h"
#include "MatrixItemGrabBellsGo.h"
#include "RecordManager.h"
#include "RecordGrab.h"
#include "RingingMaster.h"
#include "resource.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemGrabBellsGo, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemGrabBellsGo););

MatrixItemGrabBellsGo::MatrixItemGrabBellsGo()
{
}

MatrixItemGrabBellsGo::~MatrixItemGrabBellsGo(void)
{
}

MatrixItemBase* MatrixItemGrabBellsGo::clone() const 
{
	return new MatrixItemGrabBellsGo(*this);
} 

MatrixItemToken MatrixItemGrabBellsGo::getToken() const
{
	return mit_grab_bellsgo;
}

CString MatrixItemGrabBellsGo::getText(SaveableObject* translatedEntity)
{
	ASSERT(translatedEntity);
	RecordGrab* recordGrab = dynamic_cast<RecordGrab*>(translatedEntity);
	ASSERT(recordGrab);
	if (recordGrab)
	{
		return recordGrab->getBellsGoAsString();
	}

	return "";
}

CString MatrixItemGrabBellsGo::getItemName() const
{
	return "Bells Go";
}

void MatrixItemGrabBellsGo::getAllItems(ListSelectItems &items)
{
	recordManager()->getAllBellsGo(items);
}

int MatrixItemGrabBellsGo::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	RecordGrab* recordGrab1 = dynamic_cast<RecordGrab*>(translatedEntity1);
	RecordGrab* recordGrab2 = dynamic_cast<RecordGrab*>(translatedEntity2);

	ASSERT(recordGrab1);
	ASSERT(recordGrab2);

	if ((recordGrab1 != NULL)&&(recordGrab2 != NULL))
	{
		return recordGrab1->getBellsGo() - recordGrab2->getBellsGo();
	}
	return 0;
}
 
int MatrixItemGrabBellsGo::getDefaultWidth()
{
	return 55;
}
