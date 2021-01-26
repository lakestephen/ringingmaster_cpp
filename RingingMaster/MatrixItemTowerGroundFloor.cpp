#include "StdAfx.h"
#include "MatrixItemTowerGroundFloor.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "RecordManager.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerGroundFloor, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerGroundFloor););

MatrixItemTowerGroundFloor::MatrixItemTowerGroundFloor(void)
{
}

MatrixItemTowerGroundFloor::~MatrixItemTowerGroundFloor(void)
{
}

MatrixItemBase* MatrixItemTowerGroundFloor::clone() const 
{
	return new MatrixItemTowerGroundFloor(*this);
} 

MatrixItemToken MatrixItemTowerGroundFloor::getToken() const
{
	return mit_tower_groundfloor;
}

CString MatrixItemTowerGroundFloor::getItemName() const
{
	return "Ground Floor";
}

bool MatrixItemTowerGroundFloor::getOption(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return (recordTower->getGroundFloor() == TRUE) ;
	return false;
}




