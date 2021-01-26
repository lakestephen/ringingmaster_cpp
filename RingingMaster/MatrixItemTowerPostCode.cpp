#include "StdAfx.h"
#include "MatrixItemTowerPostCode.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerPostCode, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerPostCode););

MatrixItemTowerPostCode::MatrixItemTowerPostCode(void)
{
}

MatrixItemTowerPostCode::~MatrixItemTowerPostCode(void)
{
}

MatrixItemBase* MatrixItemTowerPostCode::clone() const 
{
	return new MatrixItemTowerPostCode(*this);
} 

MatrixItemToken MatrixItemTowerPostCode::getToken() const
{
	return mit_tower_postcode;
}

CString MatrixItemTowerPostCode::getItemName() const
{
	return "Post Code";
}

CString MatrixItemTowerPostCode::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return recordTower->getPostCode();

	return "";
}