#include "StdAfx.h"
#include "MatrixItemTowerWebAddress.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerWebAddress, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerWebAddress););

MatrixItemTowerWebAddress::MatrixItemTowerWebAddress(void)
{
}

MatrixItemTowerWebAddress::~MatrixItemTowerWebAddress(void)
{
}

MatrixItemBase* MatrixItemTowerWebAddress::clone() const 
{
	return new MatrixItemTowerWebAddress(*this);
} 

MatrixItemToken MatrixItemTowerWebAddress::getToken() const
{
	return mit_tower_webaddress;
}

CString MatrixItemTowerWebAddress::getItemName() const
{
	return "Web Address";
}

CString MatrixItemTowerWebAddress::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return recordTower->getWebAddress();

	return "";
}