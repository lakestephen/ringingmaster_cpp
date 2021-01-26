#include "StdAfx.h"
#include "MatrixItemTowerGridRef.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerGridRef, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerGridRef););

MatrixItemTowerGridRef::MatrixItemTowerGridRef(void)
{
}

MatrixItemTowerGridRef::~MatrixItemTowerGridRef(void)
{
}

MatrixItemBase* MatrixItemTowerGridRef::clone() const 
{
	return new MatrixItemTowerGridRef(*this);
} 

MatrixItemToken MatrixItemTowerGridRef::getToken() const
{
	return mit_tower_gridref;
}

CString MatrixItemTowerGridRef::getItemName() const
{
	return "Grid Reference";
}

CString MatrixItemTowerGridRef::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return recordTower->getMapRef().getDisplayGridRef();

	return "";
}