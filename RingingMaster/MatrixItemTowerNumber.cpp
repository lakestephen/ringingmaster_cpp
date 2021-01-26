#include "stdafx.h"
#include "MatrixItemTowerNumber.h"
#include "RecordManager.h"
#include "resource.h"
#include "RecordTower.h"
#include "RingingMaster.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerNumber, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerNumber););

MatrixItemTowerNumber::MatrixItemTowerNumber()
{
}

MatrixItemTowerNumber::~MatrixItemTowerNumber(void)
{
}

MatrixItemBase* MatrixItemTowerNumber::clone() const 
{
	return new MatrixItemTowerNumber(*this);
} 

MatrixItemToken MatrixItemTowerNumber::getToken() const
{
	return mit_tower_number;
}

CString MatrixItemTowerNumber::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
	{
		return recordTower->getNumberAsString();
	}

	return "";
}

CString MatrixItemTowerNumber::getItemName() const
{
	return "Number";
}

void MatrixItemTowerNumber::getAllItems(ListSelectItems &items)
{
	recordManager()->getAllNumbers(items);
}

int MatrixItemTowerNumber::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	RecordTower* recordTower1 = dynamic_cast<RecordTower*>(translatedEntity1);
	RecordTower* recordTower2 = dynamic_cast<RecordTower*>(translatedEntity2);

	ASSERT(recordTower1);
	ASSERT(recordTower2);

	if ((recordTower1 != NULL)&&(recordTower2 != NULL))
	{
		return recordTower1->getNumber() - recordTower2->getNumber();
	}
	return 0;
}
 
int MatrixItemTowerNumber::getDefaultWidth()
{
	return 22;
}
