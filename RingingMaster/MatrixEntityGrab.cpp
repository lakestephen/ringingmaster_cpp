#include "StdAfx.h"
#include "MatrixEntityGrab.h"
#include "RecordManager.h"
#include "RecordConstants.h"
#include "RecordGrab.h"
#include "RecordTower.h"
#include "RingingMaster.h"
#include "SaveableObject.h"

const CString MatrixEntityGrab::Name =  "Grab";

MatrixEntityGrab::MatrixEntityGrab(void)
{
}

MatrixEntityGrab::~MatrixEntityGrab(void)
{
}

void MatrixEntityGrab::createColumns(MatrixItemBases& columns)
{
	CreateColumnHelper(columns, mit_grab_date);
	CreateColumnHelper(columns, mit_grab_bellssound);
	CreateColumnHelper(columns, mit_grab_bellsgo);
	CreateColumnHelper(columns, mit_grab_date);	
	CreateColumnHelper(columns, mit_grab_comments);		
}

bool MatrixEntityGrab::getEntityFromId(long id, SaveableObject*& entity) 
{
	entity = recordManager()->getGrab(id);

	return entity != NULL;
}

UINT MatrixEntityGrab::getContextMenuID(const SaveableObjects& originalEntitys, SaveableObject* selectedOriginalEntity)
{
	if (selectedOriginalEntity == 0)
		return IDR_GRAB_LIST;
	else
		return IDR_GRAB_LIST_OFF;
}					 

long MatrixEntityGrab::getIdFromEntity(SaveableObject* entity, long& id) //todo remove, and use the key in saveable object
{
	ASSERT(entity);
	RecordGrab* recordGrab = dynamic_cast<RecordGrab*>(entity);
	ASSERT(recordGrab);
	if (recordGrab)
	{
		id = recordGrab->getGrabIndex();
		return true;
	}
	else
	{
		id = 0;
		return false;
	}
}

void MatrixEntityGrab::resetIdList(CArray<long, long>& ids)
{
	ids.RemoveAll();
	
	for (int i=0;i<recordManager()->getAllGrabsCount();i++)
	{
		ids.Add(recordManager()->getAllGrab(i)->getGrabIndex());
	}
}

int MatrixEntityGrab::getUnfilteredIdCount() const
{
	return recordManager()->getAllGrabsCount();
}

void MatrixEntityGrab::createRelations(MatrixRelations& relations)
{
}

MatrixEntityToken MatrixEntityGrab::getMatrixEntityToken()
{
	return met_grab;
}

CString MatrixEntityGrab::getName(void)
{
	return Name;
}

void MatrixEntityGrab::doubleClick(SaveableObject* originalEntity, CWnd* parent)
{
	ASSERT(originalEntity);
	RecordGrab* recordGrab = dynamic_cast<RecordGrab*>(originalEntity);
	ASSERT(recordGrab);
	if (recordGrab)
	{
		recordManager()->editGrab(recordGrab);
	}
}



void MatrixEntityGrab::getStatusBarText(CString& pane1)
{
	RecordTower* tower = recordManager()->getHomeTower();
	if (tower)
		pane1 = tower->getDisplayName();
	else
		pane1.Empty();
}


MatrixItemToken MatrixEntityGrab::getDefaultSortToken()
{
	return mit_grab_bellsgo; //todo change
}