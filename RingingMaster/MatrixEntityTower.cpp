#include "StdAfx.h"
#include "RingingMaster.h"
#include "MatrixEntityTower.h"
#include "RecordManager.h"
#include "RecordTower.h"
#include "SaveableObject.h"

const CString MatrixEntityTower::Name =  "Tower";

MatrixEntityTower::MatrixEntityTower(void)
{
}

MatrixEntityTower::~MatrixEntityTower(void)
{
}

void MatrixEntityTower::createColumns(MatrixItemBases& columns)
{
	CreateColumnHelper(columns, mit_tower_home);
	CreateColumnHelper(columns, mit_tower_grabbed);
	CreateColumnHelper(columns, mit_tower_name);
	CreateColumnHelper(columns, mit_tower_county);
	CreateColumnHelper(columns, mit_tower_dedication);
	CreateColumnHelper(columns, mit_tower_number);
	CreateColumnHelper(columns, mit_tower_gridref);
	CreateColumnHelper(columns, mit_tower_postcode);
	CreateColumnHelper(columns, mit_tower_groundfloor);
	CreateColumnHelper(columns, mit_tower_weight);
	CreateColumnHelper(columns, mit_tower_note);
	CreateColumnHelper(columns, mit_tower_night);
	CreateColumnHelper(columns, mit_tower_unringable);
	CreateColumnHelper(columns, mit_tower_webaddress);
	CreateColumnHelper(columns, mit_tower_country);
	CreateColumnHelper(columns, mit_tower_grabCount);
	CreateColumnHelper(columns, mit_tower_comments);
}

bool MatrixEntityTower::getEntityFromId(long id, SaveableObject*& entity)
{
	entity = recordManager()->getTower(id);

	return entity != NULL;
}

UINT MatrixEntityTower::getContextMenuID(const SaveableObjects& originalEntitys, SaveableObject* selectedOriginalEntity)
{
	if (selectedOriginalEntity == 0)
		return IDR_TOWER_LIST_OFF;
	else
		return IDR_TOWER_LIST;
}					 

long MatrixEntityTower::getIdFromEntity(SaveableObject* entity, long& id)
{
	ASSERT(entity);
	RecordTower* recordTower = dynamic_cast<RecordTower*>(entity);
	ASSERT(recordTower);
	if (recordTower)
	{
		id = recordTower->getIndex();
		return true;
	}
	else
	{
		id = 0;
		return false;
	}
}

void MatrixEntityTower::resetIdList(CArray<long, long>& ids)
{
	ids.RemoveAll();
	
	for (int i=0;i<recordManager()->getAllTowersCount();i++)
	{
		ids.Add(recordManager()->getAllTower(i)->getIndex());
	}
}

int MatrixEntityTower::getUnfilteredIdCount() const
{
	return recordManager()->getAllTowersCount();
}

void MatrixEntityTower::createRelations(MatrixRelations& relations)
{
}

MatrixEntityToken MatrixEntityTower::getMatrixEntityToken()
{
	return met_tower;
}

CString MatrixEntityTower::getName(void)
{
	return Name;
}

void MatrixEntityTower::doubleClick(SaveableObject* originalEntity, CWnd* parent)
{
	ASSERT(originalEntity);
	RecordTower* recordTower = dynamic_cast<RecordTower*>(originalEntity);
	ASSERT(recordTower);
	if (recordTower)
	{
		recordManager()->editTower(recordTower);
	}
}



void MatrixEntityTower::getStatusBarText(CString& pane1)
{
	RecordTower* tower = recordManager()->getHomeTower();
	if (tower)
		pane1 = tower->getDisplayName();
	else
		pane1.Empty();
}


MatrixItemToken MatrixEntityTower::getDefaultSortToken()
{
	return mit_tower_name;
}