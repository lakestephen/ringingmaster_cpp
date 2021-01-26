#include "stdafx.h"
#include "MatrixItemGrabBellsSounds.h"
#include "RecordManager.h"
#include "RecordGrab.h"
#include "RingingMaster.h"
#include "resource.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemGrabBellsSounds, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemGrabBellsSounds););

MatrixItemGrabBellsSounds::MatrixItemGrabBellsSounds()
{
}

MatrixItemGrabBellsSounds::~MatrixItemGrabBellsSounds(void)
{
}

MatrixItemBase* MatrixItemGrabBellsSounds::clone() const 
{
	return new MatrixItemGrabBellsSounds(*this);
} 

MatrixItemToken MatrixItemGrabBellsSounds::getToken() const
{
	return mit_grab_bellssound;
}

CString MatrixItemGrabBellsSounds::getText(SaveableObject* translatedEntity)
{
	ASSERT(translatedEntity);
	RecordGrab* recordGrab = dynamic_cast<RecordGrab*>(translatedEntity);
	ASSERT(recordGrab);
	if (recordGrab)
	{
		return recordGrab->getBellsSoundAsString();
	}

	return "";
}

CString MatrixItemGrabBellsSounds::getItemName() const
{
	return "Bells Sound";
}

void MatrixItemGrabBellsSounds::getAllItems(ListSelectItems &items)
{
	recordManager()->getAllBellsSound(items);
}

int MatrixItemGrabBellsSounds::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	RecordGrab* recordGrab1 = dynamic_cast<RecordGrab*>(translatedEntity1);
	RecordGrab* recordGrab2 = dynamic_cast<RecordGrab*>(translatedEntity2);

	ASSERT(recordGrab1);
	ASSERT(recordGrab2);

	if ((recordGrab1 != NULL)&&(recordGrab2 != NULL))
	{
		return recordGrab1->getBellsSound() - recordGrab2->getBellsSound();
	}
	return 0;
}
 
int MatrixItemGrabBellsSounds::getDefaultWidth()
{
	return 55;
}
