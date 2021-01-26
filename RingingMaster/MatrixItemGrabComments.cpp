#include "StdAfx.h"
#include "MatrixItemGrabComments.h"
#include "MatrixItemBase.h"
#include "RecordTower.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemGrabComments, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemGrabComments););

MatrixItemGrabComments::MatrixItemGrabComments(void)
{
}

MatrixItemGrabComments::~MatrixItemGrabComments(void)
{
}

MatrixItemBase* MatrixItemGrabComments::clone() const 
{
	return new MatrixItemGrabComments(*this);
} 

MatrixItemToken MatrixItemGrabComments::getToken() const
{
	return mit_grab_comments;
}

CString MatrixItemGrabComments::getItemName() const
{
	return "Comments";
}

CString MatrixItemGrabComments::getText(SaveableObject* translatedEntity)
{
	ASSERT(translatedEntity);
	RecordGrab* recordGrab = dynamic_cast<RecordGrab*>(translatedEntity);
	ASSERT(recordGrab);
	if (recordGrab)
		return recordGrab->getCommentAsSingleLine();

	return "";
}