#include "StdAfx.h"
#include "MatrixItemTowerName.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerName, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerName););

MatrixItemTowerName::MatrixItemTowerName(void)
{
}

MatrixItemTowerName::~MatrixItemTowerName(void)
{
}

MatrixItemBase* MatrixItemTowerName::clone() const 
{
	return new MatrixItemTowerName(*this);
} 

MatrixItemToken MatrixItemTowerName::getToken() const
{
	return mit_tower_name;
}

CString MatrixItemTowerName::getItemName() const
{
	return "Name";
}

CString MatrixItemTowerName::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return recordTower->getName();

	return "";
}