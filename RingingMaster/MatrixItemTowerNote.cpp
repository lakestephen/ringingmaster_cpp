#include "stdafx.h"
#include "MatrixItemTowerNote.h"
#include "RecordManager.h"
#include "resource.h"
#include "RecordTower.h"
#include "RingingMaster.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerNote, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerNote););

MatrixItemTowerNote::MatrixItemTowerNote()
{
}

MatrixItemTowerNote::~MatrixItemTowerNote(void)
{
}

MatrixItemBase* MatrixItemTowerNote::clone() const 
{
	return new MatrixItemTowerNote(*this);
} 

MatrixItemToken MatrixItemTowerNote::getToken() const
{
	return mit_tower_note;
}

CString MatrixItemTowerNote::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
	{
		return recordTower->getNoteAsString(false);
	}

	return "";
}

CString MatrixItemTowerNote::getItemName() const
{
	return "Note";
}

void MatrixItemTowerNote::getAllItems(ListSelectItems &items)
{
	recordManager()->getAllNotes(items);
}

int MatrixItemTowerNote::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	RecordTower* recordTower1 = dynamic_cast<RecordTower*>(translatedEntity1);
	RecordTower* recordTower2 = dynamic_cast<RecordTower*>(translatedEntity2);

	ASSERT(recordTower1);
	ASSERT(recordTower2);

	if ((recordTower1 != NULL)&&(recordTower2 != NULL))
	{
		return recordTower1->getNote() - recordTower2->getNote();
	}
	return 0;
}
 
int MatrixItemTowerNote::getDefaultWidth()
{
	return 22;
}
