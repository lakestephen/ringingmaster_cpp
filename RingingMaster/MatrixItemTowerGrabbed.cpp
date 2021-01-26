#include "StdAfx.h"
#include "MatrixItemTowerGrabbed.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "RecordManager.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerGrabbed, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerGrabbed););

MatrixItemTowerGrabbed::MatrixItemTowerGrabbed(void)
{
}

MatrixItemTowerGrabbed::~MatrixItemTowerGrabbed(void)
{
}

MatrixItemBase* MatrixItemTowerGrabbed::clone() const 
{
	return new MatrixItemTowerGrabbed(*this);
} 

MatrixItemToken MatrixItemTowerGrabbed::getToken() const
{
	return mit_tower_grabbed;
}

CString MatrixItemTowerGrabbed::getItemName() const
{
	return "Grabbed";
}

bool MatrixItemTowerGrabbed::getOption(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return recordTower->getGrabCount() > 0;
	return false;
}

CString MatrixItemTowerGrabbed::getText(SaveableObject* translatedEntity)
{
	return "";
}

int MatrixItemTowerGrabbed::getDefaultWidth()
{
	return 18;
}

MatrixGridImages MatrixItemTowerGrabbed::getImage(SaveableObject* translatedEntity)
{
	return getOption(translatedEntity)?mgi_green_tick:mgi_none;
}

MatrixGridImages MatrixItemTowerGrabbed::getHeaderImageIndex()
{
	return mgi_head_tick;
}
