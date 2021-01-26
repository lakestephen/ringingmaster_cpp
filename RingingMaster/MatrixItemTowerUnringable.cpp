#include "StdAfx.h"
#include "MatrixItemTowerUnringable.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "RecordManager.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerUnringable, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerUnringable););

MatrixItemTowerUnringable::MatrixItemTowerUnringable(void)
{
}

MatrixItemTowerUnringable::~MatrixItemTowerUnringable(void)
{
}

MatrixItemBase* MatrixItemTowerUnringable::clone() const 
{
	return new MatrixItemTowerUnringable(*this);
} 

MatrixItemToken MatrixItemTowerUnringable::getToken() const
{
	return mit_tower_unringable;
}

CString MatrixItemTowerUnringable::getItemName() const
{
	return "Unringable";
}

bool MatrixItemTowerUnringable::getOption(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return (recordTower->getUnringable() == TRUE) ;
	return false;
}




