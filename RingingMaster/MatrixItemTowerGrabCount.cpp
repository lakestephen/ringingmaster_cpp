#include "stdafx.h"
#include "MatrixItemTowerGrabCount.h"
#include "RecordManager.h"
#include "RecordTower.h"
#include "RingingMaster.h"
#include "resource.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerGrabCount, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerGrabCount););

MatrixItemTowerGrabCount::MatrixItemTowerGrabCount()
{
}

MatrixItemTowerGrabCount::~MatrixItemTowerGrabCount(void)
{
}

MatrixItemBase* MatrixItemTowerGrabCount::clone() const 
{
	return new MatrixItemTowerGrabCount(*this);
} 

MatrixItemToken MatrixItemTowerGrabCount::getToken() const
{
	return mit_tower_grabCount;
}

CString MatrixItemTowerGrabCount::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
	{
		return recordTower->getGrabCountAsString(false);
	}

	return "";
}

CString MatrixItemTowerGrabCount::getItemName() const
{
	return "Grab Count";
}

void MatrixItemTowerGrabCount::getAllItems(ListSelectItems &items)
{
	recordManager()->getAllGrabCounts(items);
}

int MatrixItemTowerGrabCount::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	RecordTower* recordTower1 = dynamic_cast<RecordTower*>(translatedEntity1);
	RecordTower* recordTower2 = dynamic_cast<RecordTower*>(translatedEntity2);

	ASSERT(recordTower1);
	ASSERT(recordTower2);

	if ((recordTower1 != NULL)&&(recordTower2 != NULL))
	{
		return recordTower1->getGrabCount() - recordTower2->getGrabCount();
	}
	return 0;
}
 
int MatrixItemTowerGrabCount::getDefaultWidth()
{
	return 22;
}
