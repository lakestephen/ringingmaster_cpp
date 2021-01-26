#include "stdafx.h"
#include "MatrixItemTowerNight.h"
#include "RecordManager.h"
#include "resource.h"
#include "RecordTower.h"
#include "RingingMaster.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerNight, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerNight););

MatrixItemTowerNight::MatrixItemTowerNight()
{
}

MatrixItemTowerNight::~MatrixItemTowerNight(void)
{
}

MatrixItemBase* MatrixItemTowerNight::clone() const 
{
	return new MatrixItemTowerNight(*this);
} 

MatrixItemToken MatrixItemTowerNight::getToken() const
{
	return mit_tower_night;
}

CString MatrixItemTowerNight::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
	{
		return recordTower->getNightAsString(false);
	}

	return "";
}

CString MatrixItemTowerNight::getItemName() const
{
	return "Night";
}

void MatrixItemTowerNight::getAllItems(ListSelectItems &items)
{
	recordManager()->getAllNights(items);
}

int MatrixItemTowerNight::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	RecordTower* recordTower1 = dynamic_cast<RecordTower*>(translatedEntity1);
	RecordTower* recordTower2 = dynamic_cast<RecordTower*>(translatedEntity2);

	ASSERT(recordTower1);
	ASSERT(recordTower2);

	if ((recordTower1 != NULL)&&(recordTower2 != NULL))
	{
		return recordTower1->getNight() - recordTower2->getNight();
	}
	return 0;
}
 
int MatrixItemTowerNight::getDefaultWidth()
{
	return 55;
}
