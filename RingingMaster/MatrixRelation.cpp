#include "stdafx.h"
#include "RingingMaster.h"
#include "matrixrelation.h"
#include "MatrixEntityBase.h"
#include "MatrixManager.h"
#include "MatrixItemBase.h"




MatrixRelation::MatrixRelation(MatrixEntityToken entityType, TranslatorFunctionItem translatorFunctionItem, CString relationName) :
_entity(0), 
_fnTranslator(0), 
_relationName(relationName)
{
	//create an instance of the correct entity.
	_entity = matrixManager()->createEntity(entityType);

	ASSERT(_entity);
	if (_entity)
	{
		//deal with the entity
		_entity->createColumns(_columns);
		if (_relationName.IsEmpty())
			_relationName = _entity->getName();

		//apply the function translator and name to the columns
		for (int i=0;i<_columns.GetSize();i++)
		{
			_columns.GetAt(i)->setTranslatorFunction(translatorFunctionItem);
			_columns.GetAt(i)->setRelationName(_relationName);
		}
	}

	//get the translator  for internal use.
	_fnTranslator = MatrixManager::GetTranslatorFunction(translatorFunctionItem);

}

MatrixRelation::~MatrixRelation(void)
{
	delete _entity;

	for (int i=0;i<_columns.GetSize();i++)
		delete _columns.GetAt(i);
	_columns.RemoveAll();
}

int MatrixRelation::getColumnCount(void)
{
	return _columns.GetSize();
}

MatrixItemBase* MatrixRelation::getColumn(int index)
{
	ASSERT((index >=0)&&(index < _columns.GetSize()));
	if((index >=0)&&(index < _columns.GetSize()))
	{
		return _columns.GetAt(index);
	}

	return NULL;
}


UINT MatrixRelation::getContextMenuID(const SaveableObjects& originalEntitys, SaveableObject* selectedOriginalEntity)
{
	if ((_entity)&&(_fnTranslator))
	{
		SaveableObject* selectedTranslatedEntity = _fnTranslator(selectedOriginalEntity);
		if (selectedTranslatedEntity)
		{
			SaveableObjects translatedEntitys;
			for (int i=0;i<originalEntitys.GetSize();i++)
			{
				SaveableObject* translatedEntity = _fnTranslator(originalEntitys.GetAt(i));
				if (translatedEntity)
					translatedEntitys.Add(translatedEntity);
			}
			return _entity->getContextMenuID(translatedEntitys, selectedTranslatedEntity);
		}
	}
	return 0;
}

CString MatrixRelation::getRelationName()
{
	return _relationName;
}
