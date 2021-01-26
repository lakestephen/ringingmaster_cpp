#include "stdafx.h"
#include "RingingMaster.h"
#include "MatrixItemTowerCountry.h"
#include "RecordManager.h"
#include "RecordTower.h"
#include "resource.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerCountry, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerCountry););

MatrixItemTowerCountry::MatrixItemTowerCountry()
{
}

MatrixItemTowerCountry::~MatrixItemTowerCountry(void)
{
}

MatrixItemBase* MatrixItemTowerCountry::clone() const 
{
	return new MatrixItemTowerCountry(*this);
} 

MatrixItemToken MatrixItemTowerCountry::getToken() const
{
	return mit_tower_country;
}

CString MatrixItemTowerCountry::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
	{
		return recordTower->getCountryAsString(false);
	}

	return "";
}

CString MatrixItemTowerCountry::getItemName() const
{
	return "Country";
}

void MatrixItemTowerCountry::getAllItems(ListSelectItems &items)
{
	recordManager()->getAllCountrys(items);
}
 
int MatrixItemTowerCountry::getDefaultWidth()
{
	return 60;
}
