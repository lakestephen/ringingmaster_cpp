#pragma once

#include "MatrixItemTextBase.h"
#include "MatrixConstants.h"


class MatrixItemBase;

class MatrixItemGrabComments :
	public MatrixItemTextBase
{
public:
	MatrixItemGrabComments(void);
public:
	~MatrixItemGrabComments(void);
	
	MatrixItemBase* clone() const;
	MatrixItemToken getToken() const;
	CString getItemName() const;

protected:
	CString getText(SaveableObject* translatedEntity);

};
