
#pragma once

#include "MatrixConstants.h"


class MatrixEntityBase;



class MatrixRelation
{
public:
	MatrixRelation(MatrixEntityToken entityType, TranslatorFunctionItem translatorFunctionItem, CString relationName = CString(""));
	virtual ~MatrixRelation(void);

	int getColumnCount(void);
	MatrixItemBase* getColumn(int index);
	UINT getContextMenuID(const SaveableObjects& originalEntitys, SaveableObject* selectedOriginalEntity);
	CString getRelationName();
private:
	MatrixItemBases _columns;
	MatrixEntityBase* _entity;
	CString _relationName;
	MATRIX_ENTITY_TRANSLATOR* _fnTranslator;
	
};

