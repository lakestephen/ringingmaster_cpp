#include "StdAfx.h"
#include "MatrixItemTowerComments.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerComments, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerComments););

MatrixItemTowerComments::MatrixItemTowerComments(void)
{
}

MatrixItemTowerComments::~MatrixItemTowerComments(void)
{
}

MatrixItemBase* MatrixItemTowerComments::clone() const 
{
	return new MatrixItemTowerComments(*this);
} 

MatrixItemToken MatrixItemTowerComments::getToken() const
{
	return mit_tower_comments;
}

CString MatrixItemTowerComments::getItemName() const
{
	return "Comments";
}

CString MatrixItemTowerComments::getText(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
		return recordTower->getCommentAsSingleLine();

	return "";
}