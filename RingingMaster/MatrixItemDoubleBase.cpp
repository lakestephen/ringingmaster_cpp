// MatrixItemBase.cpp: implementation of the MatrixItemBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MatrixItemDoubleBase.h"
#include "DoubleSelectDlg.h"
#include "resource.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixItemDoubleBase::MatrixItemDoubleBase() 
{
}
																	
MatrixItemDoubleBase::MatrixItemDoubleBase(const MatrixItemDoubleBase& matrixItemFloatBase):
MatrixItemBase(matrixItemFloatBase),
_doubleSelectItem(matrixItemFloatBase._doubleSelectItem)
{

}



MatrixItemDoubleBase::~MatrixItemDoubleBase()
{
}

bool MatrixItemDoubleBase::editFilterCondition(CWnd* parent)
{
	DoubleSelectDlg dlg(parent, _doubleSelectItem,  "Value:", "Filter on " + getName());
  
	bool ret = false;
	if (dlg.DoModal() == IDOK)
	{
		ret = true;
		_filterEditedAndReadyToUse = TRUE;
	}
					  
	return ret;
}      

int MatrixItemDoubleBase::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	ASSERT(translatedEntity1);
	ASSERT(translatedEntity2);

	double float1 = getDouble(translatedEntity1);
	double float2 = getDouble(translatedEntity2);

	double val = float1-float2;

	if (val > -0.0001 && val < 0.0001)
		return 0;

	return ((val > 0)?1:-1);
}

bool MatrixItemDoubleBase::doFilterShouldRemove(SaveableObject* translatedEntity)
{
	double floatVal = getDouble(translatedEntity);

	switch (_doubleSelectItem._type)
	{
	case dst_greater:
		return floatVal <= _doubleSelectItem._double1;
	case dst_less:
		return floatVal >= _doubleSelectItem._double1;
	case dst_between:
		return floatVal <= _doubleSelectItem._double1 || floatVal >= _doubleSelectItem._double2;
	case dst_exact:
		return floatVal != _doubleSelectItem._double1;
	}

	return true;
}

CString MatrixItemDoubleBase::getText(SaveableObject* translatedEntity)
{
	double floatVal = getDouble(translatedEntity);
	
	return getTextFromDouble(floatVal, translatedEntity);
}

CString MatrixItemDoubleBase::getTextFromDouble(double floatVal, SaveableObject* translatedEntity)
{
	CString str;
	str.Format("%.2f", floatVal);
	
	return str;
}

CString MatrixItemDoubleBase::getFilterDescriptionText()
{
	if (_filterEditedAndReadyToUse)
	{
		CString str("is '");
		
		switch (_doubleSelectItem._type)
		{
		case dst_greater:
			str += "greater than ";
			break;
		case dst_less:
			str += "less than ";
			break;
		case dst_between:
			str += "between ";
			break;
		case dst_exact:
			str += "exactly ";
			break;
		}
	
		str += getTextFromDouble(_doubleSelectItem._double1, NULL);

		if (_doubleSelectItem._type == dst_between)
		{
			str += " and ";
			str += getTextFromDouble(_doubleSelectItem._double2, NULL);
		}

		str += "'";

		return str;
	}

	return MatrixItemBase::getFilterDescriptionText();}

int MatrixItemDoubleBase::getDefaultWidth()
{
	return 50;
}

void MatrixItemDoubleBase::setDoubleSelectItem(const DoubleSelectItem& doubleSelectItem)
{
	_doubleSelectItem = doubleSelectItem;

	_filterEditedAndReadyToUse = TRUE;
}

void MatrixItemDoubleBase::Serialize(CArchive& ar)
{
	MatrixItemBase::Serialize(ar);

	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		_doubleSelectItem.Serialize(ar);
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		_doubleSelectItem.Serialize(ar);
	}
}

