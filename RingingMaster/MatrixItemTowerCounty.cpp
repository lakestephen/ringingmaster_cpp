#include "StdAfx.h"
#include "MatrixItemTowerCounty.h"
#include "RecordManager.h"
#include "RecordTower.h"
#include "RingingMaster.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerCounty, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerCounty););

MatrixItemTowerCounty::MatrixItemTowerCounty()
{
}

MatrixItemTowerCounty::~MatrixItemTowerCounty(void)
{
}

MatrixItemBase* MatrixItemTowerCounty::clone() const 
{
	return new MatrixItemTowerCounty(*this);
} 

MatrixItemToken MatrixItemTowerCounty::getToken() const
{
	return mit_tower_county;
}

CString MatrixItemTowerCounty::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
	{
		return recordTower->getCounty();
	}

	return "";
}

CString MatrixItemTowerCounty::getItemName() const
{
	return "County";
}

void MatrixItemTowerCounty::getAllItems(ListSelectItems &items)
{
	recordManager()->getAllCountys(items);
}
 
int MatrixItemTowerCounty::getDefaultWidth()
{
	return 60;
}