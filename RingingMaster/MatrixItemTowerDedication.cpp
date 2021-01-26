#include "StdAfx.h"
#include "MatrixItemTowerDedication.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerDedication, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerDedication););

MatrixItemTowerDedication::MatrixItemTowerDedication(void)
{
}

MatrixItemTowerDedication::~MatrixItemTowerDedication(void)
{
}

MatrixItemBase* MatrixItemTowerDedication::clone() const 
{
	return new MatrixItemTowerDedication(*this);
} 

MatrixItemToken MatrixItemTowerDedication::getToken() const
{
	return mit_tower_dedication;
}

CString MatrixItemTowerDedication::getItemName() const
{
	return "Dedication";
}

CString MatrixItemTowerDedication::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return recordTower->getDedication();

	return "";
}