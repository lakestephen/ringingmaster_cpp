// MatrixItemBase.cpp: implementation of the MatrixItemBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MatrixItemIntBase.h"
#include "IntSelectDlg.h"
#include "resource.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixItemIntBase::MatrixItemIntBase() 
{
}
																	
MatrixItemIntBase::MatrixItemIntBase(const MatrixItemIntBase& matrixItemIntBase):
MatrixItemBase(matrixItemIntBase),
_intSelectItem(matrixItemIntBase._intSelectItem)
{

}



MatrixItemIntBase::~MatrixItemIntBase()
{
}

bool MatrixItemIntBase::editFilterCondition(CWnd* parent)
{
	IntSelectDlg dlg(parent, _intSelectItem,  "Value:", "Filter on " + getName());

	bool ret = false;
	if (dlg.DoModal() == IDOK)
	{
		ret = true;
		_filterEditedAndReadyToUse = TRUE;
	}
					  
	return ret;
}      

int MatrixItemIntBase::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	ASSERT(translatedEntity1);
	ASSERT(translatedEntity2);

	int int1 = getInt(translatedEntity1);
	int int2 = getInt(translatedEntity2);

	return 	int1-int2;
}

bool MatrixItemIntBase::doFilterShouldRemove(SaveableObject* translatedEntity)
{
	int intVal = getInt(translatedEntity);

	switch (_intSelectItem._type)
	{
	case ist_greater:
		return intVal <= _intSelectItem._int1;
	case ist_less:
		return intVal >= _intSelectItem._int1;
	case ist_between:
		return intVal <= _intSelectItem._int1 || intVal >= _intSelectItem._int2;
	case ist_exact:
		return intVal != _intSelectItem._int1;
	}

	return true;
}

CString MatrixItemIntBase::getText(SaveableObject* translatedEntity)
{
	int intVal = getInt(translatedEntity);

	CString str;
	str.Format("%d", intVal);
	
	return str;
}

CString MatrixItemIntBase::getFilterDescriptionText()
{
	if (_filterEditedAndReadyToUse)
	{
		CString str("is '");
		
		switch (_intSelectItem._type)
		{
		case ist_greater:
			str += "greater than ";
			break;
		case ist_less:
			str += "less than ";
			break;
		case ist_between:
			str += "between ";
			break;
		case ist_exact:
			str += "exactly ";
			break;
		}
	
		CString temp;
		temp.Format("%d", _intSelectItem._int1);
		str += temp;

		if (_intSelectItem._type == ist_between)
		{
			CString temp;
			temp.Format(" and %d", _intSelectItem._int2);
			str += temp;
		}

		str += "'";

		return str;
	}

	return MatrixItemBase::getFilterDescriptionText();}

int MatrixItemIntBase::getDefaultWidth()
{
	return 40;
}

void MatrixItemIntBase::setIntSelectItem(const IntSelectItem& intSelectItem)
{
	_intSelectItem = intSelectItem;

	_filterEditedAndReadyToUse = TRUE;
}

void MatrixItemIntBase::Serialize(CArchive& ar)
{
	MatrixItemBase::Serialize(ar);

	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		_intSelectItem.Serialize(ar);
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		_intSelectItem.Serialize(ar);
	}
}

