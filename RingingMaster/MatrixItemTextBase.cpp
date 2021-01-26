// MatrixItemBase.cpp: implementation of the MatrixItemBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ringingmaster.h"

#include "MatrixItemTextBase.h"
#include "TextSelectDlg.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixItemTextBase::MatrixItemTextBase()
{
}

MatrixItemTextBase::MatrixItemTextBase(const MatrixItemTextBase& matrixItemTextBase):
MatrixItemBase(matrixItemTextBase),
_textSelectItem(matrixItemTextBase._textSelectItem)
{
}



MatrixItemTextBase::~MatrixItemTextBase()
{
}

bool MatrixItemTextBase::editFilterCondition(CWnd* parent)
{
 	TextSelectDlg dlg(parent, _textSelectItem,  "Filter on " + getName());

	bool ret = false;
	if (dlg.DoModal() == IDOK)
	{
		ret = true;
		_filterEditedAndReadyToUse = TRUE;
	}
					  
	return ret;
}



CString MatrixItemTextBase::getFilterDescriptionText()
{
	if (_filterEditedAndReadyToUse) 
	{

		CString type;
		switch (_textSelectItem._type)
		{
		case sst_startsWith:
			type = "starts with";
			break;
		case sst_endsWith:
			type = "ends with";
			break;
		case sst_contains:
			type = "contains";
			break;
		case sst_empty:
			return "is empty";
			break;
		case sst_not_empty:
			return "is not empty";
			break;
		default:
		case sst_exact:
			type = "is exactly";
			break;
		}

		CString out;
		out.Format("%s \'%s\'%s", type, _textSelectItem._string, (_textSelectItem._caseSensitive? " (case sensitive)":""));

		return out;	   
	}

	return MatrixItemBase::getFilterDescriptionText();
}

int MatrixItemTextBase::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	ASSERT(translatedEntity1);
	ASSERT(translatedEntity2);

	CString str1 = getText(translatedEntity1);
	CString str2 = getText(translatedEntity2);

	return str1.Compare(str2);
}

bool MatrixItemTextBase::doFilterShouldRemove(SaveableObject* translatedEntity)
{
	CString testString = getText(translatedEntity);

	bool keep = _textSelectItem.isValid(testString);

	return (!keep);
}

void MatrixItemTextBase::setTextSelectItem(const TextSelectItem& textSelectItem)
{
	_textSelectItem = textSelectItem;

	_filterEditedAndReadyToUse = TRUE;
}

void MatrixItemTextBase::Serialize(CArchive& ar)
{
	MatrixItemBase::Serialize(ar);

	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		_textSelectItem.Serialize(ar);
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		_textSelectItem.Serialize(ar);
	}
}
