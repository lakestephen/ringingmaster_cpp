#include "StdAfx.h"
#include "MatrixItemTowerHome.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "RecordManager.h"
#include "RingingMaster.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerHome, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerHome););

MatrixItemTowerHome::MatrixItemTowerHome(void)
{
}

MatrixItemTowerHome::~MatrixItemTowerHome(void)
{
}

MatrixItemBase* MatrixItemTowerHome::clone() const 
{
	return new MatrixItemTowerHome(*this);
} 

MatrixItemToken MatrixItemTowerHome::getToken() const
{
	return mit_tower_home;
}

CString MatrixItemTowerHome::getItemName() const
{
	return "Home";
}

bool MatrixItemTowerHome::getOption(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return (recordManager()->getHomeTower() == recordTower);
	return false;
}

MatrixGridImages MatrixItemTowerHome::getImage(SaveableObject* translatedEntity)
{
	return getOption(translatedEntity)?mgi_pink_flag:mgi_none;
}

MatrixGridImages MatrixItemTowerHome::getHeaderImageIndex()
{
	return mgi_head_home;
}
